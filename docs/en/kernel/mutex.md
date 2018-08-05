# Mutex

[[toc]]  

## Overview

### Basic Concept

A mutual exclusion (mutex) is a special binary semaphore designed to grant a task exclusive use of common resources.  

At a given point in time, a mutex is either locked or unlocked. When a task acquires a mutex, the mutex is locked and the task has exclusive ownership of the mutex. When the task releases the mutex, the mutex is unlocked and the task loses exclusive ownership of the mutex. While a task has exclusive ownership of a mutex, other tasks are unable to acquire or release the mutex.  

In a multi-task environment, it is common to see tasks competing for the same common resource. A mutex can avoid the task conflict problem without the trouble of priority inversion experienced with semaphores.  

Mutex of Huawei LiteOS has characters as below:  

- Solve the problem of priority inversion by using inheritance algorithm.

### Operation Mechanism

**Mutex Operation Principle**

In a multi-task environment, multiple tasks may battle for the same common resource. If the common resource is not shareable, it must be used exclusively by a particular task.  

When a task accesses a non-shareable common resource, the mutex is locked. Other tasks are blocked from accessing the resource until the task releases the mutex. In this way, only one task accesses the non-shareable common resource at a given point in time, which ensures the integrity of the non-shareable common resources.  

Working principle of a mutex  

![](./pic/mutex-operation.png)  

## Development Guidelines

### Usage Scenarios

A mutex is a good choice for preventing tasks from accessing the same shared resource at the same time.  

### Functions

The mutex module provides the following functions:  

| Function Category           | API           | Description      |
|-----------------------------|---------------|------------------|
| Mutex creation and deletion | LOS_MuxCreate | Creates a mutex  |
|                             | LOS_MuxDelete | Deletes a mutex  |
| Mutex request and release   | LOS_MuxPend   | Pends on a mutex |
|                             | LOS_MuxPost   | Releases a mutex |

### Development Process

Mutex typical development process:  

1. Call the `LOS_MuxCreate` API to create a mutex.

2. Call the `LOS_MuxPend` API to pend on a mutex. Takes actions depending on the mutex pend mode.

  - Non-blocking mode: If no task has acquired the mutex or the task that has acquired the mutex is the same as the requesting task, the operating system grants the mutex to the requesting task.

  - Permanent blocking mode: the requesting task waits endlessly for a mutex and enters Blocked state in the meantime. If the mutex has not been acquired by any task, the operating system grants the mutex to the requesting task. Otherwise, the operating system blocks the requesting task until the mutex is released. While the requesting task is blocked, the operating system selects the task with the highest priority among ready tasks to be executed.

  - Temporary blocking mode: the requesting task waits for a specified period of time for a mutex and enters Blocked state in the meantime. If the mutex has not been acquired by any task, the operating system grants the mutex to the requesting task. Otherwise, the operating system blocks the requesting task until the mutex is released or the timeout period elapses. It then selects the ready task with the highest priority to be executed.

3. Call the `LOS_MuxPost` to release a mutex.

  - If there are tasks blocked from acquiring the mutex, the operating system wakes up the first blocked task. The woken-up task then enters Ready state and is scheduled.

  - If there are no tasks blocked from acquiring the mutex, the operating system releases the mutex.

4. Call the `LOS_MuxDelete` API to delete a mutex.

## Precautions

- Tasks are unable to lock the same mutex. If a task attempts to lock a mutex that has been locked by another task, the task will be blocked from locking the mutex until the mutex is unlocked.

- Do not use any mutex for interrupt service routines.

- Release a mutex immediately when the mutex is no longer in use. Otherwise, tasks will be blocked for a long time, slowing down task scheduling.

- Do not change the priority of a task by calling APIs such as `LOS_TaskPriSet` while the task has full ownership of a mutex.

## Programming Example

### Example Description

In the programming example, the following activities will happen:  

1. The `Example_TaskEntry` task is executed to create a mutex. Task scheduling is locked. Two tasks `Example_MutexTask1` and `Example_MutexTask2` are created, where `Example_MutexTask2` takes a higher priority than `Example_MutexTask1`. Then, task scheduling is unlocked.  

2. `Example_MutexTask2` is scheduled, granted a mutex, and then sent to sleep mode for 100 ticks. While `Example_MutexTask2` is suspended, Example_Mu`texTask1 is woken up.  

3. `Example_MutexTask1` pends on the mutex and is willing to wait the mutex for 10 ticks to become free. At the time when `Example_MutexTask1` requests the mutex, the mutex is held by `Example_MutexTask2` and consequently  `Example_MutexTask1` is suspended. After the 10-tick wait period elapses, the mutex is still out of the reach of Example_MutexTask1, and `Example_MutexTask1` is woken up, attempting to wait permanently for the mutex. The wait for the mutex switches `Example_MutexTask1` to suspended state.

4. After 100 ticks, `Example_MutexTask2` is woken up and releases the mutex. `Example_MutexTask1` is scheduled, granted the mutex, and finally releases it.

5. 300 ticks after `Example_MutexTask1` is finished, Example_TaskEntry is executed to delete the mutex.

### Example Code

Prerequisites

- The `LOSCFG_BASE_IPC_MUX` parameter in the `los_config.h` file is set to YES.

- The `LOSCFG_BASE_IPC_MUX_LIMIT` parameter in the `los_config.h` file is set to the maximum number of mutexes that the operating system allows.

The code is as follows:  

```c  
/* mux handle id */
static UINT32 g_Testmux01;

/* task pid */
static UINT32 g_TestTaskID01;
static UINT32 g_TestTaskID02;

static VOID Example_MutexTask1(VOID)
{
    UINT32 uwRet;

    dprintf("task1 try to get mutex, wait 10 Tick.\n");
    /* get mux */
    uwRet = LOS_MuxPend(g_Testmux01, 10);

    if (uwRet == LOS_OK)
    {
        dprintf("task1 get mutex g_Testmux01.\n");
        /* release mux */
        LOS_MuxPost(g_Testmux01);
        return;
    }
    else if (uwRet == LOS_ERRNO_MUX_TIMEOUT)
    {
        dprintf("task1 timeout and try to get  mutex, wait forever.\n");
        /* LOS_WAIT_FOREVER type get mux, LOS_MuxPend return until has been get mux */
        uwRet = LOS_MuxPend(g_Testmux01, LOS_WAIT_FOREVER);
        if (uwRet == LOS_OK)
        {
            dprintf("task1 wait forever,got mutex g_Testmux01 success.\n");
            /* release mux */
            LOS_MuxPost(g_Testmux01);
            uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_MUTEX, LOS_INSPECT_STU_SUCCESS);
            if (LOS_OK != uwRet)
            {
                dprintf("Set Inspect Status Err\n");
            }
            return;
        }
    }
    return;
}

static VOID Example_MutexTask2(VOID)
{
    UINT32 uwRet;

    dprintf("task2 try to get mutex, wait forever.\n");
    /* get mux */
    uwRet = LOS_MuxPend(g_Testmux01, LOS_WAIT_FOREVER);
    if(uwRet != LOS_OK)
    {
        dprintf("task2 LOS_MuxPend failed .\n");
        return;
    }

    dprintf("task2 get mutex g_Testmux01 and suspend 100 Tick.\n");

    /* task delay 100 ticks */
    LOS_TaskDelay(100);

    dprintf("task2 resumed and post the g_Testmux01\n");
    /* release mux */
    LOS_MuxPost(g_Testmux01);
    return;
}

UINT32 Example_MutexLock(VOID)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTask1;
    TSK_INIT_PARAM_S stTask2;

    /* create mux */
    LOS_MuxCreate(&g_Testmux01);

    /* lock task schedue */
    LOS_TaskLock();

    /* create task1 */
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_MutexTask1;
    stTask1.pcName       = "MutexTsk1";
    stTask1.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask1.usTaskPrio   = 5;
    uwRet = LOS_TaskCreate(&g_TestTaskID01, &stTask1);
    if (uwRet != LOS_OK)
    {
        dprintf("task1 create failed .\n");
        return LOS_NOK;
    }

    /* create task2 */
    memset(&stTask2, 0, sizeof(TSK_INIT_PARAM_S));
    stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_MutexTask2;
    stTask2.pcName       = "MutexTsk2";
    stTask2.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask2.usTaskPrio   = 4;
    uwRet = LOS_TaskCreate(&g_TestTaskID02, &stTask2);
    if (uwRet != LOS_OK)
    {
        dprintf("task2 create failed .\n");
        return LOS_NOK;
    }

    /* unlock task schedue */
    LOS_TaskUnlock();
    /* task delay 300 ticks */
    LOS_TaskDelay(300);

    /* delete mux */
    LOS_MuxDelete(g_Testmux01);

    return LOS_OK;
}  
```  

### Verification

The verification result is as follows:  

![](./pic/mutex-output.png)  
