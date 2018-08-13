
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

/**********************************README****************************************************
1, this demo will use 1 task,1 queue,1 softtimer,make sure these are configured correctly
2, the soft timer will run periodic to write message to the queue
3, the task_rcv will read the message from the queue,check the message,if right,it will print
   that is all!
********************************************************************************************/
/**************************************FILE FUNCTIONS****************************************/
static VOID timer_callback_entry(UINT32 arg);      //soft timer call back entry
static VOID *task_rcv_entry(UINT32 args);          //task_rcv entry
VOID tutor_easy_entry(VOID) ;                      //easy demo entry,called by user
/**************************************FILE DEFINES*********************************************/

/**************************************FILE DEFINES*****************************************/
// soft timer 
#define CN_VALID_DATA          100      //compare with this when receive a message
#define CN_STMR_CYCLE          2000     //unit:tick,usually it is 1ms 
// recv task needed
#define CN_TASK_RX_PRIOR        (8)      //receive task priority
#define CN_TASK_RX_STACKSIZE    0x400    //receive task stack size,if you do more deal,make it bigger
/**************************************FILE VARS********************************************/
// queue
static  UINT32 gs_queue_demo;           //queue, used to cache the message

/***************************************************************************************************
function     :this is the timer callback entry
parameters   :args,not used yet
instruction  :we will send message to the queue periodicly
***************************************************************************************************/
static VOID timer_callback_entry(UINT32 arg) {  
    UINT32 ret;
    const  UINT32  msg_snd = CN_VALID_DATA;    //we send the data fixed
    //copy function will copy the data to queue,timeout is zero for we could not wait in softtimer    
    ret = LOS_QueueWriteCopy(gs_queue_demo,(VOID *)&msg_snd, sizeof(msg_snd), 0); 
    if(ret != LOS_OK)
    {
        printf("send message failure,error:%x\n",ret);
    }
    return;
}
/***************************************************************************************************
function     :this is the task rcv entry
parameters   :
instruction  :we will read message from the queue, if no message, we will block;if any message received,
              we then check the data and print something 
***************************************************************************************************/
static VOID *task_rcv_entry(UINT32 args){
    UINT32 msg_rcv ;
    UINT32 ret  ;
    UINT32 uwlen  ;
    
    while (1) {
        uwlen = sizeof(msg_rcv);
        //we read data from the queue, and we will block here when no messages in queue
        ret = LOS_QueueReadCopy(gs_queue_demo, &msg_rcv, &uwlen, LOS_WAIT_FOREVER);
        if (ret != LOS_OK){
            printf("recv message failure,error:%x\n",ret); //system error here
            break;
        }
        else{
           //we check if the data is CN_VALID_DATA or not
            if ((msg_rcv == CN_VALID_DATA)&&(uwlen == sizeof(msg_rcv))) {
                printf("RCVMSG:   OK:%d\n",msg_rcv); //valid message here
                msg_rcv = 0; //clear the received message here.
            }
            else{
                printf("RCVMSG:ERROR:%d\n",msg_rcv); //some error happened for we receive bad data
            }
        }    
    }
}

/***************************************************************************************************
function     :this is the demo entry
parameters   :
instruction  :do all the initialize
***************************************************************************************************/
VOID tutor_easy_entry(VOID) 
{
    UINT32 ret;
    TSK_INIT_PARAM_S task_param;
    UINT32 id_queue;
    UINT32 id_task_rcv;
    UINT16 id_swtmr;

    //then we will create a queue here,depth is 1, and max message length is four bytes
    ret = LOS_QueueCreate("queue_demo", 1, &id_queue, 0, sizeof(UINT32));
    if (ret != LOS_OK)
    {
        printf("create queue error:%x\n",ret);
        goto ERROR_QUEUE;
    }
    printf("create queue_demo ok:id:%x!\n",id_queue);
    //then we create receive task here:task size is CN_TASK_RX_STACKSIZE,
    //task prior is CN_TASK_RX_PRIOR,and task entry is task_rcv_entry
    (VOID)memset((VOID *)(&task_param), 0, sizeof(TSK_INIT_PARAM_S));
    task_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_rcv_entry;
    task_param.uwStackSize = CN_TASK_RX_STACKSIZE;
    task_param.pcName = "task_rcv_entry";
    task_param.usTaskPrio = CN_TASK_RX_PRIOR;
    ret = LOS_TaskCreate(&id_task_rcv, &task_param);
    if (ret != LOS_OK)
    {
        printf("create task_rcv error:%x\n",ret);
        goto ERROR_TASKRCV;
    }
    printf("create task_rcv ok:id:%x!\n",id_task_rcv);

    //we also create a soft timer here:cycle:CN_STMR_CYCLE ticks,period and no args needed here
    //the timer should be start after create.if more info,please refer the LOS_SwtmrCreate
    ret = LOS_SwtmrCreate(CN_STMR_CYCLE, LOS_SWTMR_MODE_PERIOD, timer_callback_entry, &id_swtmr, \
                            0, OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE);
    if (LOS_OK != ret)
    {
        printf("create soft timer error:%x\n",ret);
        goto ERROR_SWTMR_CREATE;
    }
    printf("create soft timer:%d  OK\n",id_swtmr);
    ret = LOS_SwtmrStart(id_swtmr);  //you should start the timer(means enable)
    if (LOS_OK != ret)
    {
        printf("start soft timer error:%x\n",ret);
        goto ERROR_SWTMR_START;
    }
    printf("start soft timer:%d  OK\n",id_swtmr);
    printf("DEMO ENTRY START OK\n\r");
    gs_queue_demo = id_queue;         //do the queue communication
    return; //reach here means start the demo ok

ERROR_SWTMR_START:
    LOS_SwtmrDelete(id_swtmr);
ERROR_SWTMR_CREATE:
    LOS_TaskDelete(id_task_rcv);
ERROR_TASKRCV:
    LOS_QueueDelete(id_queue);
ERROR_QUEUE:
    printf("DEMO ENTRY START ERROR\n\r");
    return;//reach here means start the demo failed
}










