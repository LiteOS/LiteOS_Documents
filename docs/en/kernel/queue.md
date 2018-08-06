# Queue

[[toc]]  

## Overview

### Basic Concept

A queue, also known as message queue, stores messages (also known as data) to be communicated between tasks. The length of message received by a queue is user
defined. A queue receives messages of user-defined length from tasks or interrupts and determines whether to store a transferred message based on the interface through which the message is sent. A task reads messages from a queue. If the queue is empty, the task is suspended. When a new message is stored in the queue, the suspended task is woken up and processes the message.  

A queue allows for asynchronous processing of messages, through which a message can be placed in a queue but left not processed immediately, and messages can be buffered.  

The following features characterize queues:   

- Messages in a queue are processed in the first in first out order. A message can be read and written asynchronously.

- Reading data from a queue and writing data into a queue support the timeout mechanism.

- The sender and the receiver agree on the type of message to be exchanged. The message length is variable, but cannot exceed the maximum message unit length.

- A task can choose any queue to send or receive messages.

- Multiple tasks can choose the same queue to send or receive messages.

- If a queue is allocated a dynamic memory block, the memory block can be reclaimed using the LOS_FreeMem API when the queue is no longer in use.

### Operation Mechanism

#### Queue Control Block

```c  
/**
  * @ingroup los_queue
  * Queue information block structure
  */
typedef struct tagQueueCB
{
    UINT8       *pucQueue;      /**< pointer to the queue */
    UINT16      usQueueState;   /**< queue state */
    UINT16      usQueueLen;     /**< number of messages in the queue */
    UINT16      usQueueSize;    /**< message node size */
    UNIT16      usQueueID;      /**< queue ID */
    UINT16      usQueueHead;    /**< message head node position (array subscript)*/
    UINT16      usQueueTail;    /**< message tail node position (array superscript)*/
    UINT16      usReadWritableCnt[2];      /**< Number of writable or readable messages in a queue. 0 indicates readable, and 1 indicates writable.*/ 
    LOS_DL_LIST stReadWriteableList[2];    /**< Read/write blocking queue. 0 indicates the read blocking queue, and 1 indicates the write blocking queue.*/
    LOS_DL_LIST stMemList;      /**< MailBox module usage */
} QUEUE_CB_S;  
```    

Each queue control block contains the element of queue state that indicates the usage of this queue:  

- `OS_QUEUE_UNUSED`: The queue is not in use.

- `OS_QUEUE_INUSED`: The queue is in use.

### Working Principles

During queue creation, memory is allocated to the queue based on the queue length and message node size and the queue ID is returned.  

A message head node position (Head) and a message tail node position (Tail) are used in a queue control block to indicate the message storage in a queue. Head indicates the start position of an occupied message, and Tail indicates the start position of a vacant message. When a queue is first created, both Head and Tail point to the start position of the queue.  

Data is written into the vacant message unit after the occupied message unit tail. If Tail points to the queue tail, the data is written into the start of the queue. The `usWritableCnt` parameter specifies whether the queue is fully occupied. Data cannot be written to a fully occupied queue (the `usWritableCnt` parameter value is **0**).  

Data is read from the head of the occupied message units. If Head points to the queue tail, the data that is first written into the start of the queue is read. The `usReadableCnt` parameter specifies whether data is available for reading. A task of reading data from a vacant queue (the `usReadableCnt` parameter value is **0**) will be suspended.  

During queue deletion, locate the queue that has a specified ID, set the queue state to be not in use, free up the memory allocated to the queue, and initialize the queue control head.  

Read/write from/into a queue  

![](./pic/queue-operation.png)  

## Development Guidelines

### Functions

The message processing module of Huawei LiteOS provides the following functions:  

| Function Category             | API                  | Description                                                                                                            |
|-------------------------------|----------------------|------------------------------------------------------------------------------------------------------------------------|
| Queue creation                | `LOS_QueueCreate`    | Creates a queue.                                                                                                       |
| Queue reading                 | `LOS_QueueRead`      | Reads data from a queue; data copy is not supported. The buff stores addresses of message units.                       |
| Queue writing                 | `LOS_QueueWrite`     | Writes data into a queue; data copy is not supported. The data written into a message unit is the buff address.        |
| Queue reading                 | `LOS_QueueReadCopy`  | Reads data from a particular queue; data copy is supported. The buff stores data retrieved from message units.         |
| Queue writing                 | `LOS_QueueWriteCopy` | Writes data into a particular queue; data copy is supported. The data written into a message unit is the buff address. |
| Queue writing                 | `LOS_QueueWriteHead` | Write data into the head of a particular queue.                                                                        |
| Queue deletion                | `LOS_QueueDelete`    | Deletes a queue.                                                                                                       |
| Queue information acquisition | `LOS_QueueInfoGet`   | Gets information about a queue.                                                                                        |

### Development Process

Typical process of using the queue module:  

1. Call the `LOS_QueueCreate` API to create a queue. Creates a queue and returns a queue ID.

2. Call the `LOS_QueueWrite` API to write data into a queue.

3. Call the `LOS_QueueRead` API to read data from a queue.

4. Call the `LOS_QueueInfoGet` API to get information about the queue.

5. Call the `LOS_QueueDelete` to delete a queue.


## Precautions

- The maximum number of queues is not equal to the total number of queues available to users. When a queue is allocated to accommodate software timers, the number of available queues is decreased by 1.

- The queue name that is passed into the `LOS_QueueCreate` API is reserved for future use.

- The input parameter uwTimeOut of queue APIs must be set to relative time.

- The `LOS_QueueReadCopy` API must be used together with the `LOS_QueueWriteCopy` API, and the `LOS_QueueRead` and `LOS_QueueWrite` APIs must be used together.

- The `LOS_QueueWrite` and `LOS_QueueRead` APIs are called to operate data addresses. Ensure that the memory that is pointed to by the pointer obtained by calling the `LOS_QueueRead` API is not modified or released during the queue reading. Otherwise, unexpected results may be caused.

## Programming Example

### Example Description

Two tasks are created in the programming example. Task 1 calls the send_Entry API to send messages. Task 2 calls the recv_Entry API to receive messages.  

1. Call the `LOS_TaskCreate` API to create tasks 1 and 2.

2. Call the `LOS_QueueCreate` API to create a queue.

3. Call the `send_Entry` API to enable task 1 to send a message.

4. Call the `rev_Entry` API to enable task 2 to send a message.

5. Call the `LOS_QueueDelete` API to delete the queue.

### Example Code

```c  
#include "los_base.h"
#include "los_task.h"
#include "los_swtmr.h"
#include "los_hwi.h"
#include "los_queue.h"
#include "los_event.h"
#include "los_typedef.h"
#include "los_api_msgqueue.h"
#include "los_inspect_entry.h"

static UINT32 g_uwQueue;

static CHAR abuf[] = "test is message x";

/* task1 send data */
static VOID *send_Entry(UINT32 uwParam)
{
    UINT32 i = 0,uwRet = 0;
    UINT32 uwlen = sizeof(abuf);

    while (i < API_MSG_NUM)
    {
        abuf[uwlen -2] = '0' + i;
        i++;

        /* write abuf data to queue */
        uwRet = LOS_QueueWrite(g_uwQueue, abuf, uwlen, 0);
        if(uwRet != LOS_OK)
        {
            dprintf("send message failure,error:%x\n",uwRet);
        }

        LOS_TaskDelay(5);
    }

    return NULL;
}

/* task2 recv data */
static VOID *recv_Entry(UINT32 uwParam)
{
    UINT32 uwReadbuf;
    UINT32 uwRet = LOS_OK;
    UINT32 uwMsgCount = 0;

    while (1)
    {
        /* read data from queue to uwReadbuf */
        uwRet = LOS_QueueRead(g_uwQueue, &uwReadbuf, 24, 0);
        if (uwRet != LOS_OK)
        {
            dprintf("recv message failure,error:%x\n",uwRet);
            break;
        }
        else
        {
            dprintf("recv message:%s\n", (CHAR *)uwReadbuf);
            uwMsgCount++;
        }

        (VOID)LOS_TaskDelay(5);
    }

    /* delete queue */
    while (LOS_OK != LOS_QueueDelete(g_uwQueue))
    {
        (VOID)LOS_TaskDelay(1);
    }

    dprintf("delete the queue success!\n");

    if (API_MSG_NUM == uwMsgCount)
    {
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_MSG, LOS_INSPECT_STU_SUCCESS);
        if (LOS_OK != uwRet)
        {
            dprintf("Set Inspect Status Err\n");
        }
    }
    else
    {
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_MSG, LOS_INSPECT_STU_ERROR);
        if (LOS_OK != uwRet)
        {
            dprintf("Set Inspect Status Err\n");
        }
    }

    return NULL;
}

UINT32 Example_MsgQueue(VOID)
{
    UINT32 uwRet = 0;
    UINT32 uwTask1, uwTask2;
    TSK_INIT_PARAM_S stInitParam1;

    /* create task1 */
    stInitParam1.pfnTaskEntry = send_Entry;
    stInitParam1.usTaskPrio = 9;
    stInitParam1.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stInitParam1.pcName = "sendQueue";
    LOS_TaskLock();//lock task schedue
    uwRet = LOS_TaskCreate(&uwTask1, &stInitParam1);
    if (uwRet != LOS_OK)
    {
        dprintf("create task1 failed!,error:%x\n",uwRet);
        return uwRet;
    }

    /*create task2*/
    stInitParam1.pfnTaskEntry = recv_Entry;
    uwRet = LOS_TaskCreate(&uwTask2, &stInitParam1);
    if (uwRet != LOS_OK)
    {
        dprintf("create task2 failed!,error:%x\n",uwRet);
        return uwRet;
    }

    /* create queue */
    uwRet = LOS_QueueCreate("queue", 5, &g_uwQueue, 0, 24);
    if (uwRet != LOS_OK)
    {
        dprintf("create queue failure!,error:%x\n",uwRet);
    }

    dprintf("create the queue success!\n");
    LOS_TaskUnlock();//unlock task schedue

    return LOS_OK;
}  
```

### Verification

![](./pic/queue-output.png)  
