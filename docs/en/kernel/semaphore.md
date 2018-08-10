# Semaphore

[[toc]]  

## Overview

### Basic Concept

A semaphore is a mechanism used for communication within a kernel, to achieve synchronization or mutual exclusion of critical resources between tasks.  

In a multi-task system, it is necessary to synchronize one task with another or prevent tasks battling for critical resources. Semaphores are a good choice to serve that purpose.  

Typically, a numerical value of a signal is used to correspond to the number of available resources. It means mutually exclusive resources remained that could be occupied. The meaning of its value is divided into two kinds of situations:  

- 0, it means the post operation that is not accumulated, and it is possible to block tasks on this signal.

- Positive number, it means there is one or several release operations which are posted.

The differences to use between semaphore for the purpose of synchronization and semaphore for the purpose of mutex are:  

- If a semaphore is used as a mutex, it is created with a full internal counter. Each time a task waits on critical resources, it is assigned the semaphore and the counter value is decreased by 1. When the counter value drops to 0, subsequent tasks are blocked from getting the semaphore.

- If a semaphore is used for task synchronization, it is created with an empty counter. When task 1 attempts to get the semaphore, it is blocked because the counter has reached the maximum value. Task 1 will enter Ready or Running state after task 2 releases the semaphore, thereby achieving task synchronization.

### Operation Mechanism

**Semaphore Control Block**

```c  
/**
 * @ingroup los_sem
 * Semaphore control structure.
 */
typedef struct
{
    UINT8           usSemStat;          /**whether to use flag bit*/
    UINT16          uwSemCount;         /**semaphore quantity index*/
    UINT32UINT16    usMaxSemCount;      /**Maximum number of semaphores*/
    UINT32          usSemID;            /**semaphore count*/
    LOS_DL_LIST     stSemList;          /**suspend the task blocked on the semaphore*/
}SEM_CB_S;
```  

**Semaphore Operation Principle**

During semaphore initialization, memory is allocated to N semaphores. N is configurable by users and limited by memory. All semaphores are initialized and added to the linked list of semaphores that are not in use.  

During semaphore creation, a semaphore is obtained from the linked list of semaphores that are not in use and the initial value of the semaphore is set.  

If the internal counter of a semaphore is more than 0 when the semaphore is pended, the counter value is decreased by 1 and the pending succeeds. If the counter value is 0, tasks are blocked from getting the semaphore and wait for other tasks to post the semaphore. The timeout interval of waiting on the semaphore can be configured. If a task is blocked from getting the semaphore, suspend the task to the tail of the queue of tasks waiting on the semaphore.  

If no tasks are waiting on a semaphore, the counter value is increased by 1 and the semaphore is posted. Otherwise, wake up the first task in the queue of tasks waiting on the semaphore.  

During semaphore deletion, the semaphore that is in use is set to be not in use and is added to the linked list of semaphores that are not in use.  

A semaphore allows multiple tasks to access the same resource at the same time but sets a limit on the number of the tasks. Tasks are not allowed to access the resource if the maximum number of the tasks that can access the resource is reached and need to wait for one task to release the semaphore.  

Working principle of semaphore  

![](./pic/semaphore-operation.png)  

## Development Guidelines

### Usage Scenarios

Semaphores find their use in locking resources, counting resources, and maintaining synchronization between threads or between threads and interrupts.  

### Functions

The semaphore module provides the following functions:  

| Function Category               | API                   | Description                |
|---------------------------------|-----------------------|----------------------------|
| Semaphore creation and deletion | `LOS_SemCreate`       | Creates a semaphore        |
|                                 | `LOS_BinarySemCreate` | Creates a binary semaphore |
|                                 | `LOS_SemDelete`       | Deletes a semaphore        |
| Semaphore pend and post         | `LOS_SemPend`         | Pends on a semaphore       |
|                                 | `LOS_SemPost`         | Posts a semaphore          |

### Development Process

Semaphore typical development process:  

1. Call the `LOS_SemCreate` API to create a semaphore.

2. Call the `LOS_SemPend` to pend on a semaphore.

 Huawei LiteOS takes actions depending on the semaphore pend mode.

 - **Non-blocking mode**: If the maximum number of tasks allowed by the semaphore is not reached, the request for the semaphore is fulfilled. Otherwise, the request for the semaphore is rejected.

 - **Permanent blocking mode**: The requesting task waits endlessly for a semaphore and the task enters Blocked state in the meantime. If the maximum number of tasks allowed by the semaphore is not reached, the request for the semaphore is fulfilled. Otherwise, the operating system blocks the requesting task until a task releases the semaphore. It then selects the ready task with the highest priority to be executed.  

 - **Temporary blocking mode**: the requesting task waits for a specified period of time for a semaphore and enters Blocked state in the meantime. If the maximum number of tasks allowed by the semaphore is not reached, the request for the semaphore is fulfilled. Otherwise, the operating system blocks the requesting task until a task releases the semaphore or the timeout period elapses. It then selects the ready task with the highest priority to be executed.  

3. Call the `LOS_SemPost` API to post a semaphore.

 - If there are tasks blocked from acquiring the semaphore, the operating system wakes up the first blocked task. The woken-up task then enters Ready state and is scheduled.

 - If there are no tasks blocked from acquiring the semaphore, the operating system posts the semaphore.

4. Call the `LOS_SemDelete` API to delete a semaphore.

## Precautions

- As interrupts cannot be blocked, permanent blocking and temporary blocking are not allowed for interrupts during the request for a semaphore.  

## Programming Example

### Example Description

In the programming example, the following activities will happen:  

1. The `Example_TaskEntry` task is executed to create a semaphore. Task scheduling is locked. Two tasks `Example_SemTask1` and `Example_SemTask2` are created, where `Example_SemTask2` takes a higher priority than `Example_SemTask1`. Then, task scheduling is unlocked. `Example_TaskEntry` releases the semaphore.

2. `Example_SemTask2` is granted the semaphore, scheduled, and sent to sleep mode for 20 ticks. While `Example_SemTask2` is delayed, `Example_SemTask1` is woken up.

3. `Example_SemTask1` pends on the semaphore and is willing to wait the semaphore for 10 ticks to become free. At the time when `Example_SemTask1` requests the semaphore, the semaphore is held by `Example_SemTask2` and consequently `Example_SemTask1` is suspended. After the 10-tick wait period elapses, the semaphore is still out of the reach of `Example_SemTask1`, and `Example_SemTask1` is woken up, attempting to wait permanently for the semaphore. The wait for semaphore switches `Example_SemTask1` to suspended state.

4. After 20 ticks, `Example_SemTask2` is woken up and releases the semaphore. `Example_SemTask1` is scheduled, granted the semaphore, and finally releases it.

5. 40 ticks after `Example_SemTask1` is finished, Example_TaskEntry is woken up, deletes the semaphore and then the two tasks.

### Example Code

Prerequisites  

- The `LOSCFG_BASE_IPC_SEM` parameter in the `los_config.h` file is set to YES.

- The `LOSCFG_BASE_IPC_SEM_LIMIT` parameter in the `los_config.h` file is set to the maximum number (for example, 1024) of semaphores that the operating system allows.

The code is as follows:  

```c
/* task pid */
static UINT32 g_TestTaskID01, g_TestTaskID02;
/* sem id */
static UINT32 g_usSemID;

static VOID Example_SemTask1(VOID)
{
    UINT32 uwRet;

    dprintf("Example_SemTask1 try get sem g_usSemID ,timeout 10 ticks.\n");
    /* get sem, timeout is 10 ticks */
    uwRet = LOS_SemPend(g_usSemID, 10);

    /* get sem ok */
    if (LOS_OK == uwRet)
    {
        LOS_SemPost(g_usSemID);
        return;
    }
    /* timeout, get sem fail */
    if (LOS_ERRNO_SEM_TIMEOUT == uwRet)
    {
        dprintf("Example_SemTask1 timeout and try get sem g_usSemID wait forever.\n");
        /* get sem wait forever, LOS_SemPend return until has been get mux */
        uwRet = LOS_SemPend(g_usSemID, LOS_WAIT_FOREVER);
        if (LOS_OK == uwRet)
        {
            dprintf("Example_SemTask1 wait_forever and got sem g_usSemID success.\n");
            LOS_SemPost(g_usSemID);
            uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_SEM, LOS_INSPECT_STU_SUCCESS);
            if (LOS_OK != uwRet)
            {
                dprintf("Set Inspect Status Err\n");
            }
            return;
        }
    }
    return;
}

static VOID Example_SemTask2(VOID)
{
    UINT32 uwRet;
    dprintf("Example_SemTask2 try get sem g_usSemID wait forever.\n");
    /* wait forever get sem */
    uwRet = LOS_SemPend(g_usSemID, LOS_WAIT_FOREVER);

    if(LOS_OK == uwRet)
    {
        dprintf("Example_SemTask2 get sem g_usSemID and then delay 20ticks .\n");
    }

    /* task delay 20 ticks */
    LOS_TaskDelay(20);

    dprintf("Example_SemTask2 post sem g_usSemID .\n");
    /* release sem */
    LOS_SemPost(g_usSemID);

    return;
}

UINT32 Example_Semphore(VOID)
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S stTask1;
    TSK_INIT_PARAM_S stTask2;

   /* create sem */
    LOS_SemCreate(0, &g_usSemID);

    /* lock task schedue */
    LOS_TaskLock();

    /* create task1 */
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SemTask1;
    stTask1.pcName       = "MutexTsk1";
    stTask1.uwStackSize  = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTask1.usTaskPrio   = TASK_PRIO_TEST;
    uwRet = LOS_TaskCreate(&g_TestTaskID01, &stTask1);
    if (uwRet != LOS_OK)
    {
        dprintf("task1 create failed .\n");
        return LOS_NOK;
    }

    /* create task2 */
    memset(&stTask2, 0, sizeof(TSK_INIT_PARAM_S));
    stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SemTask2;
    stTask2.pcName       = "MutexTsk2";
    stTask2.uwStackSize  = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTask2.usTaskPrio   = (TASK_PRIO_TEST - 1);
    uwRet = LOS_TaskCreate(&g_TestTaskID02, &stTask2);
    if (uwRet != LOS_OK)
    {
        dprintf("task2 create failed .\n");

        /* delete task 1 */
        if (LOS_OK != LOS_TaskDelete(g_TestTaskID01))
        {
            dprintf("task1 delete failed .\n");
        }

        return LOS_NOK;
    }

    /* unlock task schedue */
    LOS_TaskUnlock();

    uwRet = LOS_SemPost(g_usSemID);

    /* task delay 40 ticks */
    LOS_TaskDelay(40);

    /* delete sem */
    LOS_SemDelete(g_usSemID);

    return uwRet;
}
```

### Verification

The verification result is as follows:  

![](./pic/semaphore-output.png)  
