
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

#include "stm32f1xx_hal.h"
#include "gpio.h"

#include "los_swtmr.h"
#include "los_queue.h"
#include "los_task.h"

/**********************************README****************************************************
1, this demo will use 1 task,1 queue,2 softtimer,make sure these are configured correctly
2, the soft timer will run periodic to write message to the queue
3, the task_rcv will read the message from the queue,check the message,if right,it will print
   that is all!
********************************************************************************************/

/**************************************MACRO DEFINES*****************************************/
#define QUEUE_LENGTH             2
#define SND_CMD1_MSG             100
#define SND_CMD2_MSG             200
#define RECV_TASK_PRIOR          8          //receive task priority
#define RECV_TASK_STACK_SIZE     0x400      //receive task stack size,if you do more deal,make it bigger

/**************************************GLOBAL VARS********************************************/
UINT32 g_QueueID;
UINT32 g_RecvTaskID;
UINT16 g_TimerID1;
UINT16 g_TimerID2;

/***************************************************************************************************
function     :this is the timer callback entry
parameters   :args,not used yet
instruction  :we will send message to the queue periodicly
***************************************************************************************************/
static VOID Timer_Callback(UINT32 ulArg)
{  
    UINT32 ulRet = LOS_OK;
    
    ulRet = LOS_QueueWriteCopy(
                g_QueueID,
                (VOID *)&ulArg,         /* send to msg */
                sizeof(UINT32),
                0
                ); 
    if(ulRet != LOS_OK)
    {
        printf("send message failure,error:%x\r\n",ulRet);
    }
    return;
}

static UINT32 Timers_Init(UINT32 ulInterval, UINT16 ulTimerID, UINT32 ulSendMsg)
{
    UINT32 ulRet = LOS_OK;

    ulRet = LOS_SwtmrCreate(
              ulInterval,             /* interval for ulInterval tick */
              LOS_SWTMR_MODE_PERIOD,  /* peridic software timer */ 
              Timer_Callback,         /* software timer callback function */
              &ulTimerID,             /* software timer id */
              ulSendMsg               /* sendmsg */
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
              , OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE
#endif
            );
    if (LOS_OK == ulRet)
    {
        ulRet = LOS_SwtmrStart(ulTimerID);
        if (LOS_OK != ulRet)
        {
            printf("start software timer error:%x\r\n",ulRet);
            return ulRet;
        }
    }

    return ulRet;
}

static VOID Task_RecvQ(UINT32 ulArg)
{
    UINT32 ulRecvMsg = 0;
    UINT32 ulRet = LOS_OK;
    UINT32 ulLength = 0;
    
    while (1) {
        ulLength = sizeof(ulRecvMsg);
        ulRet = LOS_QueueReadCopy(g_QueueID, &ulRecvMsg, &ulLength, LOS_WAIT_FOREVER);
        if (LOS_OK == ulRet)
        {
            switch(ulRecvMsg)
            {
                case SND_CMD1_MSG:
                    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                    break;
                case SND_CMD2_MSG:
                    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                    break;
                default:
                    break;
            }
          
            printf("RCVMSG:   OK:%d\r\n", ulRecvMsg); //valid message here
            ulRecvMsg = 0; //clear the received message here.
        }
    }
}

UINT32 Blinky_Demo(VOID) 
{
    UINT32 ulRet = LOS_OK;
    TSK_INIT_PARAM_S stTask = {0};

    /* Create the queue. */
    ulRet = LOS_QueueCreate(
              "MsgQueue",       /* queue name */
              QUEUE_LENGTH,     /* queue length */
              &g_QueueID,       /* queue id */
              0,                /* flags, not used */
              sizeof(UINT32)    /* message size */
              );
    if (ulRet != LOS_OK)
    {
      printf("create the queue error:%x\r\n", ulRet);
      return ulRet;
    }
    
    stTask.pfnTaskEntry = (TSK_ENTRY_FUNC)Task_RecvQ;
    stTask.uwStackSize = RECV_TASK_STACK_SIZE;
    stTask.pcName = "RecvTask";
    stTask.usTaskPrio = RECV_TASK_PRIOR;

    /* Create the task. */
    ulRet = LOS_TaskCreate(
              &g_RecvTaskID,    /* receive task id */
              &stTask           /* task params */
              );
    if (ulRet != LOS_OK)
    {
        printf("create task_rcv error:%x\r\n",ulRet);
        return ulRet;
    }

    /* Create the software timer1. */
    ulRet = Timers_Init(1000, g_TimerID1, SND_CMD1_MSG);
    if (ulRet != LOS_OK)
    {
        printf("create timer error:%x\r\n", ulRet);
        return ulRet;
    }

    ulRet = Timers_Init(500, g_TimerID2, SND_CMD2_MSG);
    if (ulRet != LOS_OK)
    {
        printf("create timer error:%x\r\n", ulRet);
        return ulRet;
    }

    return LOS_OK;
}




