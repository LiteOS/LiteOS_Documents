
/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "los_swtmr.h"
#include "los_queue.h"
#include "los_task.h"
#include "los_memory.h"
#include "los_list.h"
#include "los_mux.h"
#include "los_sem.h"
#include "los_hwi.h"

/**********************************README****************************************************
1, this demo will use 7 task,1 queue,2 mux,1 sem,3 softtimer,make sure these are configured correctly
2, task_cmd intend to receive command from the cloud host, and send corresponding event to task_event
3, task_alarm intend to monitor the alarm sensor, and send event to the task_event
4, task_event handle all the event do the corresponding actions(print instead)
5, task_sample(more than one) sample the data periodicly(triggered by the soft timer) and send to the sample queue
6, task_rcv read the sample data from the sample queue and cache them in a list,when enough
   message get, we will pack all the message and do report
7, report here we only use print instead
   that is all!
********************************************************************************************/

/**************************************FILE DEFINES*********************************************/
#define LOC_DEVICEID       0x0001   //for the imitate,may be you should use globle vars for change
#define INVALID_TASKID   (0xFFFFFFFF)      //all the handle if failed we will set it to invalid as this
#define INVALID_TIMERID  (0xFFFF)
//this is defines for the rcv task
#define Q_MAX_MSGNUM         30   //UNIT:message item
#define Q_CACHE_TIMEOUT     5000 //UNIT:TICK
#define RCV_LOOPCYCLE        10   //UNIT:TICK
//defines for general task stacksize and prior
#define DEFAULT_TASK_PRIOR        8    //task prior if not specified
#define DEFAULT_TASK_STACKSIZE    0x1000//task stacksize if not specified
//defines the event used for the alarm task,cmdtask,eventtask
#define EVENT_IO_OPEN   (1<<0)
#define EVENT_IO_CLOSE  (1<<1)
#define EVENT_ALARM     (1<<2)
//defines for the cmd buf
#define CLOUD_CMD_LENGTH       256
#define CMD_WAIT_TIMEOUT    2000   //UNIT:TICKS

#define NOT_USED(arg)          (arg = arg;)

/**************************************FILE DATA STRUCTURE**********************************/
//the data receive task use this structure to cache the message
typedef struct _sample_msg_item
{
    LOS_DL_LIST   list;
    UINT8        *buf;
}tSampleMsgItem;

//all the sample demo use the same logic, so we define it format here,you may change it yourself as you wish
typedef struct _sample_params
{
    CHAR               *name;
    UINT16              prior;
    UINT32              stacksize; 
    TSK_ENTRY_FUNC      entry;
    UINT32              cycle;
    UINT32              taskId;
    UINT16              timerId;
}tSampleParams;

typedef struct sample_data
{
    UINT16 device_id;
    UINT16 data_type;
    UINT32 loop_times;
    UINT64 tick_now;
}tSampleData;
#define SAMPLE_DATA_LEN     (sizeof(tSampleData))  

static VOID *Task_Sample(UINT32 args);

/**************************************FILE VARS*********************************************/
UINT32 g_SampleQueueId;    //queue, used to cache the message
UINT32 g_RecvTaskId;
UINT32 g_EventTaskId;
static UINT32 g_CloudCmdSemId;    //this sync is used for the cmdtask, which will wait the sem
static UINT32 g_DebugPrintMuxId;    //debug mux lock
static UINT32 g_DataReportMuxId;    //the report function will use this mux for multi task reasons
static EVENT_CB_S  g_SampleEvent;                 //the event,ipc between tasks without parameters
static UINT8  g_CmdBuffer[CLOUD_CMD_LENGTH];           //the cmd buffer,write by the isr of the hardware and read by cmd task
static UINT32 g_CmdBufferLength;              //how many data in cmd buffer
//define the sample process
static  tSampleParams g_SampleParams[] = 
{ 
    {
        "CSQ_SAMPLE",
        DEFAULT_TASK_PRIOR + 1,
        DEFAULT_TASK_STACKSIZE,
        Task_Sample,
        10000, 
        INVALID_TASKID,    
        INVALID_TIMERID
    },
    {
        "PWR_SAMPLE",
        DEFAULT_TASK_PRIOR + 1,
        DEFAULT_TASK_STACKSIZE,
        Task_Sample,
        5000, 
        INVALID_TASKID,    
        INVALID_TIMERID
    },
    {
        "DHT_SAMPLE",
        DEFAULT_TASK_PRIOR,
        DEFAULT_TASK_STACKSIZE,
        Task_Sample,
        1000,
        INVALID_TASKID,    
        INVALID_TIMERID
    },
};
#define SAMPLE_TASKNUM      (sizeof(g_SampleParams)/sizeof(tSampleParams))

/************************FUNCTION IMPLEMENT*********************************************/
/***************************************************************************************************
function     :do printf for multi task
parameters   :same as printf
instruction  :here we deal the print,because we use multi task, they may has the same prior
              to avoid print conflict, we use a mutex here to do multi task sync
***************************************************************************************************/
static void Debug_Print(const CHAR *fmt,...)
{
    UINT32 ulRet = LOS_OK;
    CHAR s[64] = {0}; //maybe  enough here,if you need more, change it,take care of the task stack size!
    va_list arg_lst = {0};
    
    ulRet = LOS_MuxPend(g_DebugPrintMuxId, LOS_WAIT_FOREVER);
    if (LOS_OK == ulRet) 
    {
        va_start(arg_lst, fmt);	
        vsprintf(s,fmt,arg_lst);
        va_end(arg_lst);	
        puts(s);
        LOS_MuxPost(g_DebugPrintMuxId);
    }
    return;
}
/***************************************************************************************************
function     :report data to the cloud
parameters   :buf, the address of the message to report
              len, the message length
instruction  :here we use print instead for we has no access to the cloud,you could implement yourself
***************************************************************************************************/
static VOID Data_Report(UINT8 *buf, UINT32 len)
{
    UINT32 ulRet;
    //take care of the multi task called, if your method support multi task, you could remove the mux here
    ulRet = LOS_MuxPend(g_DataReportMuxId, LOS_WAIT_FOREVER);  
    if (LOS_OK == ulRet) 
    {
        Debug_Print("REPORT MESSAGELEN:%d\r\n",len);  //we use the print to do the imitate
        LOS_MuxPost(g_DataReportMuxId);
    }
    return;
}  

/***************************************************************************************************
function     :alarm monitor task
parameters   :args,not used yet
instruction  :we has no alarm monitor sensor, so we will write a event at about 5000 ticks instead
***************************************************************************************************/
static VOID *Task_SmokyAlarm(UINT32 ulArg)
{
    UINT32 ulRet = LOS_OK;
    UINT32 ulEvent = EVENT_ALARM;
    
    while (1) 
    {
        LOS_TaskDelay(5000);  //5000 ticks
        ulRet = LOS_EventWrite(&g_SampleEvent, ulEvent);
        if (LOS_OK != ulRet) 
        {
            Debug_Print("TASK_ALARM:WRITE ALARM EVENT:%x ERROR\r\n",ulEvent);
        }
        else
        {
            Debug_Print("TASK_ALARM:WRITE ALARM EVENT:%x OK\r\n",ulEvent);
        }
    }
}
/***************************************************************************************************
function     :cmd monitor task
parameters   :args,not used yet
instruction  :actually we will monitor the cloud host if any messages, here we use the data toggle here
              and the data simulated here using intlock between the task and isr
***************************************************************************************************/
static VOID *Task_CloudCmd(UINT32 args)
{
    UINT32 ulRet = LOS_OK;
    UINT32 ulEvent = EVENT_IO_OPEN;
    UINT8  cmd_data[CLOUD_CMD_LENGTH];
    while (1) {
        ulRet = LOS_SemPend(g_CloudCmdSemId, CMD_WAIT_TIMEOUT); //this will  be trigglerd by the hardware isr
        LOS_IntLock(); //this sync with the isr
        if (g_CmdBufferLength > 0) 
        {
            memcpy(cmd_data, g_CmdBuffer,g_CmdBufferLength);
            g_CmdBufferLength = 0;  //clear it
            ulRet = LOS_OK;
        }
        else
        {
            g_CmdBufferLength = CLOUD_CMD_LENGTH;
        }
        LOS_IntUnLock();
        
        //do the data analyze here, we just toggle the ulEvent instead
        ulEvent = (ulEvent == EVENT_IO_OPEN) ? EVENT_IO_CLOSE : EVENT_IO_OPEN;
        ulRet = LOS_EventWrite(&g_SampleEvent, ulEvent);
        if (LOS_OK != ulRet) 
        {
            Debug_Print("TASK_CMD:WRITE CMD EVENT:%x ERROR\r\n",ulEvent);
        }
        else
        {
            Debug_Print("TASK_CMD:WRITE CMD EVENT:%x OK\r\n",ulEvent);
        }
    }
}

/***************************************************************************************************
function     :event process task
parameters   :args,not used yet
instruction  :we read event,check its type and do corresponding,we use print here instead
***************************************************************************************************/
static VOID *Task_Sample(UINT32 ulTaskNo)
{
    UINT32 ulRet = LOS_OK;
    UINT32 ulTaskId = INVALID_TASKID;
    tSampleData *pData = NULL;
    UINT32 ulLoopTimes = NULL;

    ulTaskId = g_SampleParams[ulTaskNo].taskId;
    while(1)
    {
        ulLoopTimes++;
        LOS_TaskSuspend(ulTaskId); //wait until the soft timer active it

        
        
        pData = (tSampleData*)LOS_MemAlloc(m_aucSysMem0, SAMPLE_DATA_LEN);
        if (NULL != pData)
        {
            pData->device_id = LOC_DEVICEID;
            pData->data_type = ulTaskNo;
            pData->loop_times = ulLoopTimes;
            pData->tick_now = LOS_TickCountGet();
            ulRet= LOS_QueueWrite(g_SampleQueueId, pData, SAMPLE_DATA_LEN, LOS_WAIT_FOREVER);
            if (LOS_OK != ulRet)
            {
                Debug_Print("TASK %d WRITE DATA ERROR:%x\r\n", ulTaskId, ulRet);
                LOS_MemFree(m_aucSysMem0, pData);
            }
            else
            {
                Debug_Print("TASK %d WRITE DATA:%d Bytes\r\n",ulTaskId, SAMPLE_DATA_LEN);
            }   
        }
    }
}
/***************************************************************************************************
function     :task receive entry
parameters   :args,not used yet
instruction  :we will read message from the queue, and make it to a list
              added to the cache queue, if cached length is over or time is up,we will pack all the
              message received and send it to the cloud
***************************************************************************************************/
static VOID *Task_RecvQ(UINT32 args)
{
    UINT32  ulRet = LOS_OK;
    UINT8  *pMsgBuffer = NULL;  //usedto pack all the message
    UINT32 *pData = NULL;  //storage the pointer read from the queue
    UINT32  ulDataNum = NULL;
    UINT32  ulMsgNo = NULL;
    UINT64  last_reporttick = NULL;
    LOS_DL_LIST   msg_lst_head;     //message cached list head
    LOS_DL_LIST  *item,*next;       //used for the interate the message list
    tSampleMsgItem  *itemcache;

    //initialize the message cached list
    LOS_ListInit(&msg_lst_head);
    
    while (1) {
        //we read data from the queue, and we will block here when no messages in queue
        pData = NULL;
        ulRet = LOS_QueueRead(g_SampleQueueId, &pData, sizeof(pData), RCV_LOOPCYCLE);
        if (ulRet == LOS_OK && NULL != pData){
            //we get a message here, so pack it with list and cached it
            itemcache = LOS_MemAlloc(m_aucSysMem0,sizeof(tSampleMsgItem));
            if (NULL != itemcache) {
                memset(itemcache,0,sizeof(tSampleMsgItem));
                itemcache->buf = (UINT8 *)pData;
                LOS_ListAdd(&msg_lst_head, &itemcache->list);
                ulDataNum++;
            }
        }

        if (((LOS_TickCountGet() - last_reporttick) > Q_CACHE_TIMEOUT) 
            || (ulDataNum > Q_MAX_MSGNUM)) 
        {
            //pack all the data and send here
            if (ulDataNum == 0){
                continue; //no message get here, do another loop
            }
            pMsgBuffer = LOS_MemAlloc(m_aucSysMem0, SAMPLE_DATA_LEN * ulDataNum);
            if (NULL != pMsgBuffer) 
            {
                ulMsgNo = 0;
                LOS_DL_LIST_FOR_EACH_SAFE(item, next, &msg_lst_head)
                {
                    itemcache = (tSampleMsgItem*)item;
                    memcpy(pMsgBuffer + (ulMsgNo * SAMPLE_DATA_LEN), itemcache->buf, SAMPLE_DATA_LEN);
                    //remove the item and free the memory(INCLUDE THE LIST AND BUF IN THE LIST)
                    LOS_ListDelete(&itemcache->list);
                    LOS_MemFree(m_aucSysMem0, itemcache->buf);
                    LOS_MemFree(m_aucSysMem0, itemcache);
                }
                
                Data_Report(pMsgBuffer, ulDataNum * SAMPLE_DATA_LEN);

                LOS_MemFree(m_aucSysMem0, pMsgBuffer);
                ulDataNum = 0;
                last_reporttick = LOS_TickCountGet();
            }
            else
            {
                Debug_Print("NO MEMORY HERE,CHECK THE HEAP\r\n\r");
            }
        }    
    }
}
/***************************************************************************************************
function     :event process task
parameters   :args,not used yet
instruction  :we read event,check its type and do corresponding,we use print here instead
***************************************************************************************************/
static VOID *Task_EventsHandle(UINT32 args){

    UINT32 ulEvent;
    const CHAR   *alarm_msg = "HELLO, ALARM";
    
    while (1) {
        ulEvent = LOS_EventRead(
                    &g_SampleEvent, 
                    EVENT_IO_OPEN|EVENT_IO_CLOSE|EVENT_ALARM, 
                    LOS_WAITMODE_OR | LOS_WAITMODE_CLR, 
                    LOS_WAIT_FOREVER
                );
        if (ulEvent & EVENT_ALARM) 
        {
            Data_Report((UINT8 *)alarm_msg, strlen(alarm_msg));
        }
        if (ulEvent & EVENT_IO_OPEN) 
        {
            Debug_Print("IO EVENT:OPEN\r\n"); //we use print imitating the hardware
        }
        if (ulEvent & EVENT_IO_CLOSE) 
        {
            Debug_Print("IO EVENT:CLOSE\r\n");//we  use print imitationg the hardware
        }
    }
}
/***************************************************************************************************
function     :timer call back entry
parameters   :args,pass the id_task which will be resumed
instruction  :all the trigger timer use the same entry,specify them by the args
***************************************************************************************************/
static VOID Timer_Callback(UINT32 ulTaskId)
{  
    UINT32 ulRet = LOS_OK;
    
    ulRet = LOS_TaskResume(ulTaskId);
    if (ulRet != LOS_OK) 
    {
        Debug_Print("RESUME TASK:%d ERROR:%x\r\n", ulTaskId, ulRet);
    }
    
    return;
}

/***************************************************************************************************
function     :this is the demo entry
parameters   :
instruction  :we will create one task(task_rcv),one queue(queue_demo), and one soft timer
              the soft timer send data to queue each 2000 ticks (tick default 1ms)
              the receive task receive data from the queue,if no messsage it will block here
***************************************************************************************************/
UINT32 Full_Demo(VOID) 
{
    UINT32 ulRet;
    TSK_INIT_PARAM_S stTaskParam = {0};
    UINT32 ulTaskId = INVALID_TASKID;
    UINT16 ulSwTmrId = INVALID_TIMERID;
    UINT32 ulTaskNo = NULL;

    ulRet = LOS_EventInit(&g_SampleEvent);
    if (LOS_OK != ulRet) 
    {
        Debug_Print("create Event error.:%x\r\n", ulRet);
        return LOS_NOK;
    }
    
    ulRet = LOS_MuxCreate(&g_DebugPrintMuxId);
    if (LOS_OK != ulRet)
    {
        Debug_Print("create Mutex error.:%x\r\n", ulRet);
        return LOS_NOK;
    }
    ulRet = LOS_MuxCreate(&g_DataReportMuxId);
    if (LOS_OK != ulRet)
    {
        Debug_Print("create Mutex error.:%x\r\n", ulRet);
        return LOS_NOK;

    }
    //create the cmd task trigger semphore
    ulRet = LOS_SemCreate(0, &g_CloudCmdSemId); //initialize it not active,it is actived by the irq or something else
    if (LOS_OK != ulRet)
    {
        Debug_Print("create Semaphore error.:%x\r\n", ulRet);
        return LOS_NOK;
    }
 
    //then we will create a queue here,depth is 8, and max message length is four bytes(storage the pointer)
    ulRet = LOS_QueueCreate("queue_demo", 8, &g_SampleQueueId, 0, sizeof(UINT32));
    if (LOS_OK != ulRet)
    {
        Debug_Print("create queue error:%x\r\n",ulRet);
        return LOS_NOK;
    }

    //then we create receive task here:task size is DEFAULT_TASK_STACKSIZE,
    //task prior is DEFAULT_TASK_PRIOR,and task entry is task_rcv
    stTaskParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Task_RecvQ;
    stTaskParam.uwStackSize = DEFAULT_TASK_STACKSIZE;
    stTaskParam.pcName = "Task_RecvQ";
    stTaskParam.usTaskPrio = DEFAULT_TASK_PRIOR;

    ulRet = LOS_TaskCreate(&g_RecvTaskId, &stTaskParam);
    if (LOS_OK != ulRet)
    {
        Debug_Print("create Task_RecvQ error:%x\r\n",ulRet);
        return LOS_NOK;
    }

    stTaskParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Task_SmokyAlarm;
    stTaskParam.uwStackSize = DEFAULT_TASK_STACKSIZE;
    stTaskParam.pcName = "Task_SmokyAlarm";
    stTaskParam.usTaskPrio = DEFAULT_TASK_PRIOR;
    
    ulRet = LOS_TaskCreate(&g_EventTaskId, &stTaskParam);
    if (LOS_OK != ulRet)
    {
        Debug_Print("create Task_SmokyAlarm error:%x\r\n",ulRet);
        return LOS_NOK;
    }

    stTaskParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Task_CloudCmd;
    stTaskParam.uwStackSize = DEFAULT_TASK_STACKSIZE;
    stTaskParam.pcName = "Task_CloudCmd";
    stTaskParam.usTaskPrio = DEFAULT_TASK_PRIOR;
    ulRet = LOS_TaskCreate(&ulTaskId, &stTaskParam);
    if (LOS_OK != ulRet)
    {
        Debug_Print("create Task_CloudCmd error:%x\r\n",ulRet);
        return LOS_NOK;
    }

    stTaskParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Task_EventsHandle;
    stTaskParam.uwStackSize = DEFAULT_TASK_STACKSIZE;
    stTaskParam.pcName = "Task_EventsHandle";
    stTaskParam.usTaskPrio = DEFAULT_TASK_PRIOR;
    ulRet = LOS_TaskCreate(&ulTaskId, &stTaskParam);
    if (LOS_OK != ulRet)
    {
        Debug_Print("create Task_EventsHandle error:%x\r\n",ulRet);
        return LOS_NOK;
    }

    for (ulTaskNo = 0; ulTaskNo < SAMPLE_TASKNUM; ulTaskNo++) 
    {
        (VOID)memset((VOID *)(&stTaskParam), 0, sizeof(TSK_INIT_PARAM_S));
        stTaskParam.pfnTaskEntry = g_SampleParams[ulTaskNo].entry;
        stTaskParam.uwStackSize = g_SampleParams[ulTaskNo].stacksize;
        stTaskParam.pcName = g_SampleParams[ulTaskNo].name;
        stTaskParam.usTaskPrio = g_SampleParams[ulTaskNo].prior;
        stTaskParam.uwArg = ulTaskNo;
        ulRet = LOS_TaskCreate(&ulTaskId, &stTaskParam);
        if (LOS_OK != ulRet)
        {
            Debug_Print("create %s error:%x\r\n", stTaskParam.pcName, ulRet);
            return LOS_NOK;
        }
        g_SampleParams[ulTaskNo].taskId = ulTaskId;
        
        //now create a soft timer here,args use the id_task
        ulRet = LOS_SwtmrCreate(
                    LOS_MS2Tick(g_SampleParams[ulTaskNo].cycle), 
                    LOS_SWTMR_MODE_PERIOD, 
                    Timer_Callback, 
                    &ulSwTmrId, 
                    ulTaskId
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
                    , OS_SWTMR_ROUSES_ALLOW, 
                    OS_SWTMR_ALIGN_SENSITIVE
#endif
                );
        if (LOS_OK != ulRet)
        {
            Debug_Print("create Swtmr error:%x\r\n", ulRet);
            return LOS_NOK;
        }

        g_SampleParams[ulTaskNo].timerId = ulSwTmrId;
        ulRet = LOS_SwtmrStart(ulSwTmrId);  //you should start the timer(means enable)
        if (LOS_OK == ulRet) {
            Debug_Print("TaskId:%d TaskName:%-16s TimerId:%d\r\n",
                        g_SampleParams[ulTaskNo].taskId,
                        g_SampleParams[ulTaskNo].name,
                        g_SampleParams[ulTaskNo].timerId);
        }
    }
    
    return LOS_OK;
}






