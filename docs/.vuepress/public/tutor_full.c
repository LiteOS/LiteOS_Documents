
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

/**************************************FILE FUNCTIONS****************************************/
static VOID   report_msg(UINT8 *buf,UINT32 len);      //report message to the upper,like the cloud server
static VOID  *task_sample_entry(UINT32 args);         //DATA SAMPLE TASK ENTRY,args is the task no
static VOID   debug_print(const CHAR *fmt,...);       //DO THE PRINT, MULTI TASK SYNC HERE
VOID tutor_full_entry(VOID) ;                         //the demo entry,do all the initialize
/**************************************FILE DEFINES*********************************************/
#define CN_DEV_ID       0x101010   //for the imitate,may be you should use globle vars for change
#define CN_INVALID_ID   0X1234      //all the handle if failed we will set it to invalid as this
//this is defines for the rcv task
#define CN_RCV_CACHELEN         30   //UNIT:message item
#define CN_RCV_CACHETIMEOUT     5000 //UNIT:TICK
#define CN_RCV_LOOPCYCLE        10   //UNIT:TICK
//defines for general task stacksize and prior
#define CN_GENERALTASK_PRIOR        8    //task prior if not specified
#define CN_GENERALTASK_STACKSIZE    0x400//task stacksize if not specified
//defines the event used for the alarm task,cmdtask,eventtask
#define CN_EVENT_IO_OPEN   (1<<0)
#define CN_EVENT_IO_CLOSE  (1<<1)
#define CN_EVENT_ALARM     (1<<2)
#define CN_EVENT_INTERESTING    (CN_EVENT_IO_OPEN|CN_EVENT_IO_CLOSE|CN_EVENT_ALARM) //YOU COULD ADD MORE
//defines for the cmd buf
#define CN_CMD_BUFLEN          64
#define CN_CMD_WAIT_TIMEOUT    2000   //UNIT:TICKS

/**************************************FILE DATA STRUCTURE**********************************/
//the data receive task use this structure to cache the message
struct sample_msg_item
{
    LOS_DL_LIST   list;
    UINT8        *buf;
};

//all the sample demo use the same logic, so we define it format here,you may change it yourself as you wish
struct sample_params
{
    CHAR               *name;
    UINT32              prior;
    UINT32              stacksize; 
    TSK_ENTRY_FUNC      entry;
    UINT32              cycle;
    UINT32              id_task;
    UINT16              id_timer;
};
//we imitate the report data message:taskno id_task ticknow loop_times;you may rewrite it yourself 
struct sample_data{//YOU COULD IMPLEMENT YOUR MESSAGE PROTOCOL HERE
    UINT32 devid;
    UINT32 taskno;
    UINT32 id_task;
    UINT32 loop_times;
    UINT64 ticknow;
};
#define CN_SAMPLE_DATALEN     (sizeof(struct sample_data))  

/**************************************FILE VARS*********************************************/
static UINT32 gs_sample_queue  = CN_INVALID_ID;    //queue, used to cache the message
static UINT32 gs_sync_cmdtask  = CN_INVALID_ID;    //this sync is used for the cmdtask, which will wait the sem
static UINT32 gs_mux_debugprint= CN_INVALID_ID;    //debug mux lock
static UINT32 gs_mux_report    = CN_INVALID_ID;    //the report function will use this mux for multi task reasons
static EVENT_CB_S  gst_comn_event;                 //the event,ipc between tasks without parameters
static UINT8  gs_cmd_buf[CN_CMD_BUFLEN];           //the cmd buffer,write by the isr of the hardware and read by cmd task
static UINT32 gs_cmd_buf_datalen = 0;              //how many data in cmd buffer
//define the sample process
static  struct sample_params gst_sample_params[]={ 
    {
        "CSQ_SAMPLE",
        10,
        0x400,
        task_sample_entry,
        10000, 
        CN_INVALID_ID,    
        CN_INVALID_ID,
    },
    {
        "PWR_SAMPLE",
        10,
        0x400,
        task_sample_entry,
        5000, 
        CN_INVALID_ID,    
        CN_INVALID_ID,
    },
    {
        "DHT_SAMPLE",
        10,
        0x400,
        task_sample_entry,
        1000,
        CN_INVALID_ID,    
        CN_INVALID_ID,
    },
};
#define CN_SAMPLE_TASKNUM      (sizeof(gst_sample_params)/sizeof(struct sample_params))

/************************FUNCTION IMPLEMENT*********************************************/
/***************************************************************************************************
function     :do printf for multi task
parameters   :same as printf
instruction  :here we deal the print,because we use multi task, they may has the same prior
              to avoid print conflict, we use a mutex here to do multi task sync
***************************************************************************************************/
static void debug_print(const char *fmt,...)
{
    UINT32 ret;
    char s[64]; //maybe  enough here,if you need more, change it,take care of the task stack size!
    va_list arg_lst;
    ret = LOS_MuxPend(gs_mux_debugprint, LOS_WAIT_FOREVER);
    if (ret == LOS_OK) {
        va_start(arg_lst, fmt);	
        vsprintf(s,fmt,arg_lst);
        va_end(arg_lst);	
        puts(s);
        LOS_MuxPost(gs_mux_debugprint);
    }
    return;
}
/***************************************************************************************************
function     :report data to the cloud
parameters   :buf, the address of the message to report
              len, the message length
instruction  :here we use print instead for we has no access to the cloud,you could implement yourself
***************************************************************************************************/
static VOID report_msg(UINT8 *buf,UINT32 len){
    UINT32 ret;
    //take care of the multi task called, if your method support multi task, you could remove the mux here
    ret = LOS_MuxPend(gs_mux_report, LOS_WAIT_FOREVER);  
    if (ret == LOS_OK) {
        debug_print("REPORT MESSAGELEN:%d\r\n",len);  //we use the print to do the imitate
        LOS_MuxPost(gs_mux_report);
    }
    return;
}  

/***************************************************************************************************
function     :alarm monitor task
parameters   :args,not used yet
instruction  :we has no alarm monitor sensor, so we will write a event at about 5000 ticks instead
***************************************************************************************************/
static VOID *task_alaram_entry(UINT32 args){

    UINT32 event;
    UINT32 ret;
    while (1) {
        LOS_TaskDelay(5000);  //5000 ticks
        event = CN_EVENT_ALARM;
        ret = LOS_EventWrite(&gst_comn_event, event);
        if (ret != LOS_OK) {
            debug_print("TASK_ALARM:WRITE ALARM EVENT:%x ERROR\r\n",event);
        }
        else{
            debug_print("TASK_ALARM:WRITE ALARM EVENT:%x OK\r\n",event);
        }
    }
}
/***************************************************************************************************
function     :cmd monitor task
parameters   :args,not used yet
instruction  :actually we will monitor the cloud host if any messages, here we use the data toggle here
              and the data simulated here using intlock between the task and isr
***************************************************************************************************/
static VOID *task_cmd_entry(UINT32 args){

    UINT32 event;
    UINT32 ret;
    UINT8  cmd_data[CN_CMD_BUFLEN];
    event = CN_EVENT_IO_OPEN;
    while (1) {
        ret = LOS_SemPend(gs_sync_cmdtask, CN_CMD_WAIT_TIMEOUT); //this will  be trigglerd by the hardware isr
        LOS_IntLock(); //this sync with the isr
        if (gs_cmd_buf_datalen > 0) {
            memcpy(cmd_data, gs_cmd_buf,gs_cmd_buf_datalen);
            gs_cmd_buf_datalen = 0;  //clear it
            ret = LOS_OK;
        }
        else{
            gs_cmd_buf_datalen = CN_CMD_BUFLEN;
        }
        LOS_IntUnLock();
        //do the data analyze here, we just toggle the event instead
        event = (event==CN_EVENT_IO_OPEN)?CN_EVENT_IO_CLOSE:CN_EVENT_IO_OPEN;
        ret = LOS_EventWrite(&gst_comn_event, event);
        if (ret != LOS_OK) {
            debug_print("TASK_CMD:WRITE CMD EVENT:%x ERROR\r\n",event);
        }
        else{
            debug_print("TASK_CMD:WRITE CMD EVENT:%x OK\r\n",event);
        }
    }
}
/***************************************************************************************************
function     :event process task
parameters   :args,not used yet
instruction  :we read event,check its type and do corresponding,we use print here instead
***************************************************************************************************/
static VOID *task_event_entry(UINT32 args){

    UINT32 event;
    UINT32 event_interesting;
    const CHAR   *alarm_msg = "HELLO, ALARM";
    
    event_interesting = CN_EVENT_INTERESTING;
    while (1) {
        event = LOS_EventRead(&gst_comn_event, event_interesting, \
                                  LOS_WAITMODE_OR | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);
        if (event & CN_EVENT_ALARM) {//THIS IS A ALARM EVENT,DO THE REPORT HERE
            report_msg((UINT8 *)alarm_msg, strlen(alarm_msg));
        }
        if (event & CN_EVENT_IO_OPEN) {//THIS IS A IO OPEN EVENT
            debug_print("IO EVENT:OPEN\r\n"); //we use print imitating the hardware
        }
        if (event & CN_EVENT_IO_CLOSE) {//THIS IS A IO CLOSE EVENT
           debug_print("IO EVENT:CLOSE\r\n");//we  use print imitationg the hardware
        }
    }
}

/***************************************************************************************************
function     :event process task
parameters   :args,not used yet
instruction  :we read event,check its type and do corresponding,we use print here instead
***************************************************************************************************/
static VOID *task_sample_entry(UINT32 args)
{
    UINT32 id_task;
    UINT32 ret;
    UINT8 *buf;              //malloc from the heap, and free by the receive task
    UINT32 snd_len;           //how many data has been write to the queue
    UINT32 loop_times = 0;
    struct sample_data  *smpdata;

    id_task = gst_sample_params[args].id_task;
    while(1){
        loop_times++;
        LOS_TaskSuspend(id_task); //wait until the soft timer active it
        snd_len = CN_SAMPLE_DATALEN;
        buf = LOS_MemAlloc(m_aucSysMem0,snd_len);
        if(NULL != buf){
            //fill the buf with data
            memset(buf,CN_SAMPLE_DATALEN,snd_len);
            smpdata = (struct sample_data *)buf;
            smpdata->devid = CN_DEV_ID;
            smpdata->taskno = args;
            smpdata->id_task = id_task;
            smpdata->loop_times = loop_times;
            smpdata->ticknow = LOS_TickCountGet();
            ret= LOS_QueueWrite(gs_sample_queue,buf,snd_len,LOS_WAIT_FOREVER);
            if(ret != LOS_OK ){
                debug_print("TASK %d WRITE DATA ERROR:%x\r\n",id_task,ret);
                LOS_MemFree(m_aucSysMem0,buf);
            }
            else{
                debug_print("TASK %d WRITE DATA:%d Bytes\r\n",id_task,snd_len);
            }   
        }
    }
}
/***************************************************************************************************
function     :timer call back entry
parameters   :args,pass the id_task which will be resumed
instruction  :all the trigger timer use the same entry,specify them by the args
***************************************************************************************************/
static VOID timer_callback_entry(UINT32 arg) {  
    UINT32 ret;
    UINT32 id_task;
    id_task = arg; //maybe you should check whether it is valid
    ret = LOS_TaskResume(id_task);
    if (ret != LOS_OK) {
        debug_print("RESUME TASK:%d ERROR:%x\r\n\r",id_task,ret);
    }
    return;
}
/***************************************************************************************************
function     :task receive entry
parameters   :args,not used yet
instruction  :we will read message from the queue, and make it to a list
              added to the cache queue, if cached length is over or time is up,we will pack all the
              message received and send it to the cloud
***************************************************************************************************/
static VOID *task_rcv_entry(UINT32 args)
{
    UINT32  ret ;
    UINT32  len ;
    UINT8  *msg_buf ;  //usedto pack all the message
    UINT32 *msg_rcv ;  //storage the pointer read from the queue
    UINT32  msg_num ;
    UINT32  msg_no ;
    UINT64  last_reporttick;
    UINT64  cur_tick ;  
    LOS_DL_LIST   msg_lst_head;     //message cached list head
    LOS_DL_LIST  *item,*next;       //used for the interate the message list
    struct sample_msg_item  *itemcache;

    //initialize the message cached list
    msg_num = 0;
    msg_buf = NULL;
    msg_rcv = NULL;
    last_reporttick = 0;
    LOS_ListInit(&msg_lst_head);
    while (1) {
        //we read data from the queue, and we will block here when no messages in queue
        msg_rcv = NULL;
        len = sizeof(msg_rcv);
        ret = LOS_QueueRead(gs_sample_queue, &msg_rcv, len, CN_RCV_LOOPCYCLE);
        if (ret == LOS_OK){
            //we get a message here, so pack it with list and cached it
            itemcache = LOS_MemAlloc(m_aucSysMem0,sizeof(struct sample_msg_item));
            if (NULL != itemcache) {
                memset(itemcache,0,sizeof(struct sample_msg_item));
                itemcache->buf = (UINT8 *)msg_rcv;
                LOS_ListAdd(&msg_lst_head, &itemcache->list);
                msg_num++;
            }
        }
        cur_tick = LOS_TickCountGet(); //anyway, we will check here if timeout
        if (((cur_tick - last_reporttick) > CN_RCV_CACHETIMEOUT) || (msg_num > CN_RCV_CACHELEN)) {
            //pack all the data and send here
            if(msg_num == 0){
                continue; //no message get here, do another loop
            }
            msg_buf = LOS_MemAlloc(m_aucSysMem0,CN_SAMPLE_DATALEN*msg_num);
            if (NULL != msg_buf) {
                msg_no = 0;
                LOS_DL_LIST_FOR_EACH_SAFE(item,next,&msg_lst_head){
                    itemcache = (struct sample_msg_item *)item;
                    memcpy(msg_buf + (msg_no * CN_SAMPLE_DATALEN), itemcache->buf,CN_SAMPLE_DATALEN);
                    //remove the item and free the memory(INCLUDE THE LIST AND BUF IN THE LIST)
                    LOS_ListDelete(&itemcache->list);
                    LOS_MemFree(m_aucSysMem0, itemcache->buf);
                    LOS_MemFree(m_aucSysMem0, itemcache);
                }
                //now send all the data once
                report_msg(msg_buf,msg_num * CN_SAMPLE_DATALEN);
                last_reporttick =  LOS_TickCountGet();
                //free the buf we get
                LOS_MemFree(m_aucSysMem0, msg_buf);
                //NOW CLEAR THE MSGNUM
                msg_num = 0;
            }
            else{
                debug_print("NO MEMORY HERE,CHECK THE HEAP\r\n\r");
            }
        }    
    }
}

/***************************************************************************************************
function     :this is the demo entry
parameters   :
instruction  :we will create one task(task_rcv),one queue(queue_demo), and one soft timer
              the soft timer send data to queue each 2000 ticks (tick default 1ms)
              the receive task receive data from the queue,if no messsage it will block here
***************************************************************************************************/
VOID tutor_full_entry(VOID) 
{
    UINT32 ret;
    TSK_INIT_PARAM_S task_param;
    UINT32 id_queue;
    UINT32 id_task_rcv;
    UINT32 id_task_sample;
    UINT16 id_swtmr;
    UINT32 id_task_cmd;
    UINT32 id_task_alarm;
    UINT32 id_task_event;
    UINT32 timer_ticks_cycle;
    UINT32 sample_task_no;

    ret = LOS_EventInit(&gst_comn_event);
    if (ret!= LOS_OK) {
        goto ERROR_EVENT;
    }
    
    ret = LOS_MuxCreate(&gs_mux_debugprint);//create the debug  mutex sync
    if(ret != LOS_OK){
        goto ERROR_DBGMUX;
    }
    ret = LOS_MuxCreate(&gs_mux_report);    //create the report mutex sync,
    if(ret != LOS_OK){
        goto ERROR_REPORTMUX;
    }
    //create the cmd task trigger semphore
    ret = LOS_SemCreate(0,&gs_sync_cmdtask); //initialize it not active,it is actived by the irq or something else
    if(ret != LOS_OK){
        goto ERROR_CMDTASKSYNC;
    }
 
    //then we will create a queue here,depth is 8, and max message length is four bytes(storage the pointer)
    ret = LOS_QueueCreate("queue_demo", 8, &id_queue, 0, sizeof(UINT32));
    if (ret != LOS_OK){
        debug_print("create queue error:%x\r\n",ret);
        goto ERROR_QUEUE;
    }
    debug_print("create queue_demo ok:id:%x!\r\n",id_queue);
    //then we create receive task here:task size is CN_GENERALTASK_STACKSIZE,
    //task prior is CN_GENERALTASK_PRIOR,and task entry is task_rcv
    (VOID)memset((VOID *)(&task_param), 0, sizeof(TSK_INIT_PARAM_S));
    task_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_rcv_entry;
    task_param.uwStackSize = CN_GENERALTASK_STACKSIZE;
    task_param.pcName = "task_rcv";
    task_param.usTaskPrio = CN_GENERALTASK_PRIOR;
    ret = LOS_TaskCreate(&id_task_rcv, &task_param);
    if (ret != LOS_OK){
        debug_print("create task_rcv error:%x\r\n",ret);
        goto ERROR_TASKRCV;
    }
    debug_print("create task_rcv ok:id:%x!\r\n",id_task_rcv);

    //create the alarm task
    (VOID)memset((VOID *)(&task_param), 0, sizeof(TSK_INIT_PARAM_S));
    task_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_alaram_entry;
    task_param.uwStackSize = CN_GENERALTASK_STACKSIZE;
    task_param.pcName = "task_alarm";
    task_param.usTaskPrio = CN_GENERALTASK_PRIOR;
    ret = LOS_TaskCreate(&id_task_rcv, &task_param);
    if (ret != LOS_OK){
        debug_print("create task_alarm error:%x\r\n",ret);
        goto ERROR_TASKALARM;
    }
    debug_print("create task_alarm ok:id:%x!\r\n", id_task_alarm);
    //create the cmd task
    (VOID)memset((VOID *)(&task_param), 0, sizeof(TSK_INIT_PARAM_S));
    task_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_cmd_entry;
    task_param.uwStackSize = CN_GENERALTASK_STACKSIZE;
    task_param.pcName = "task_cmd";
    task_param.usTaskPrio = CN_GENERALTASK_PRIOR;
    ret = LOS_TaskCreate(&id_task_rcv, &task_param);
    if (ret != LOS_OK){
        debug_print("create task_cmd error:%x\r\n",ret);
        goto ERROR_TASKCMD;
    }
    debug_print("create task_cmd ok:id:%x!\r\n", id_task_alarm);
    //create the event task
    (VOID)memset((VOID *)(&task_param), 0, sizeof(TSK_INIT_PARAM_S));
    task_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_event_entry;
    task_param.uwStackSize = CN_GENERALTASK_STACKSIZE;
    task_param.pcName = "task_event";
    task_param.usTaskPrio = CN_GENERALTASK_PRIOR;
    ret = LOS_TaskCreate(&id_task_rcv, &task_param);
    if (ret != LOS_OK){
        debug_print("create task_event error:%x\r\n",ret);
        goto ERROR_TASKEVENT;
    }
    debug_print("create task_event ok:id:%x!\r\n", id_task_alarm);
    //we also create a soft timer here:cycle:CN_STMR_CYCLE ticks,period and no args needed here
    //the timer should be start after create.if more info,please refer the LOS_SwtmrCreate
    for (sample_task_no= 0;sample_task_no < CN_SAMPLE_TASKNUM;sample_task_no++) {
        (VOID)memset((VOID *)(&task_param), 0, sizeof(TSK_INIT_PARAM_S));
        task_param.pfnTaskEntry = gst_sample_params[sample_task_no].entry;
        task_param.uwStackSize = gst_sample_params[sample_task_no].stacksize;
        task_param.pcName = gst_sample_params[sample_task_no].name;
        task_param.usTaskPrio = gst_sample_params[sample_task_no].prior;
        task_param.uwArg = sample_task_no;
        ret = LOS_TaskCreate(&id_task_sample, &task_param);
        if (ret != LOS_OK)
        {
            break; //maybe not support so many task or memory not enough
        }
        gst_sample_params[sample_task_no].id_task = id_task_sample;
        //now create a soft timer here,args use the id_task
        timer_ticks_cycle = LOS_MS2Tick(gst_sample_params[sample_task_no].cycle);
        ret = LOS_SwtmrCreate(timer_ticks_cycle, LOS_SWTMR_MODE_PERIOD, timer_callback_entry, &id_swtmr, \
                        id_task_sample, OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE);
        if (LOS_OK == ret)
        {
            gst_sample_params[sample_task_no].id_timer = id_swtmr;
            ret = LOS_SwtmrStart(id_swtmr);  //you should start the timer(means enable)
            if (LOS_OK == ret) {
                debug_print("id_task:%d TASKNAME:%-16s id_timer:%d\r\n",\
                            gst_sample_params[sample_task_no].id_task,\
                            gst_sample_params[sample_task_no].name,\
                            gst_sample_params[sample_task_no].id_timer);
            }
            else{
                break;//start the timer failed, stop the demo
            }
        }
    }
    if (ret != LOS_OK) { //check if we are success or not,if not,do quit process
        goto ERROR_SAMPE_TASK;
    }

    debug_print("DEMO ENTRY START OK\r\n\r");
    gs_sample_queue = id_queue;         //do the queue communication
    return;                          //reach here means start the demo ok
    
ERROR_SAMPE_TASK:
    for(sample_task_no = 0;sample_task_no < CN_SAMPLE_TASKNUM;sample_task_no++){
        if(gst_sample_params[sample_task_no].id_timer  != CN_INVALID_ID){
            LOS_SwtmrDelete(gst_sample_params[sample_task_no].id_timer);
        }
        if(gst_sample_params[sample_task_no].id_task  != CN_INVALID_ID){
            LOS_TaskDelete(gst_sample_params[sample_task_no].id_task);
        }
    }
    LOS_TaskDelete(id_task_event);
ERROR_TASKEVENT:
    LOS_TaskDelete(id_task_cmd);
ERROR_TASKCMD:
    LOS_TaskDelete(id_task_alarm);
ERROR_TASKALARM:
    LOS_TaskDelete(id_task_rcv);
ERROR_TASKRCV:
    LOS_QueueDelete(id_queue);
ERROR_QUEUE:
    LOS_SemDelete(gs_sync_cmdtask);
    gs_sync_cmdtask =CN_INVALID_ID;
ERROR_CMDTASKSYNC:
    LOS_MuxDelete(gs_mux_report);
    gs_mux_report =CN_INVALID_ID;
ERROR_REPORTMUX:
    LOS_MuxDelete(gs_mux_debugprint);
    gs_mux_debugprint = CN_INVALID_ID;
ERROR_DBGMUX:
ERROR_EVENT:    
    return;//reach here means start the demo failed
}






