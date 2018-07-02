# Task

## Overview

### Basic Concept

Task is the minimum running unit of competitive system resources from a system perspective. It can use or wait for CPU, use memory space, and can run independently of other tasks. 

Task modules of Huawei LiteOS provide a lot of tasks to help users manage business process procedures. It makes switches and communications between tasks come true. Through this, users can devote more energies to the achievement of business function. 

Huawei LiteOS is an operating system supported multi-task. In Huawei LiteOS, a task is same as a thread.

Task in Huawei LiteOS is preemptive scheduling mechanism, while supporting round-robin scheduling.

High-priority task can interrupt low-priority task, low-priority task can only be scheduled when the high-priority task blocked or completed.

A total of 32 priorities are defined, with priority 0 being the highest and 31 being the lowest.

### Related Concepts

#### Task States

A task in Huawei LiteOS switches between different states. After the operating system is initialized, a created task is allowed to contend for system resources according to the scheduling procedure regulated by Huawei LiteOS Kernel.

There are usually four task states:  

- **Ready**: The task is waiting for execution by a CPU.
- **Running**: The task is being executed.
- **Blocked**: The task is not on the list of ready tasks. For example, the task may be suspended, delayed, waiting for a semaphore, waiting to read from or write into a queue, or reading from or writing into a queue.
- **Dead**: The task execution is complete, and resources are waiting to be reclaimed.

**Task state schematic diagram**

<img :src="$withBase('/task-state.png')" alt="Task state schematic diagram">

#### The state transition process is as follows:

- **Ready → Running**  

A task enters Ready state once created. When a task switch occurs, the task with the highest priority is selected from ready tasks and enters Running state to be executed. Although the task is in Running state, it remains on the list of ready  tasks.

- **Running → Blocked**  

When a running task is blocked (for example, it is suspended, delayed, obtaining a mutex, reading a message, or waiting to read a semaphore), it will be deleted from the list of ready tasks and enters Blocked state. The state transition triggers a task switch where the task with the highest priority is selected from ready tasks.

- **Blocked → Ready (Blocked → Running)** 

After a blocked task is recovered (for example, if the task is resumed, the task successfully reads a semaphore, or if the delay period or semaphore read period expires), the task will be added to the list of ready tasks and enters ready state. If the recovered task takes precedence over the running task, a task switch will occur to send the resumed task into running state.

- **Ready → Blocked**  

If a ready task is blocked (suspended), it will be deleted from the list of ready tasks no longer participated in task scheduling and enter blocked state.

- **Running → Ready**

When a task is created or resumed with a higher priority than the running task, the created or resumed task enters running state and task scheduling will be occurred. Meanwhile, the original running task enters ready state but it remains on the list of ready tasks.

- **Running → Stopped**  

When a task running is stopped, the kernel automatically deletes the task. The status of the task will change from running to stopped.

- **Blocked → Stopped**  

If calling the delete API when the task is in blocked status, the task status will change from blocked to stopped.

#### Task ID 

You will receive a task ID after successfully creating a task. You may suspend, resume, or query a task using its ID. 

#### Task Priority 

Tasks are executed based on their priority. In the event of a task switch, the task with the highest priority will be selected from ready tasks.

#### Task Entry Function

Each task has a task entry function, which is defined by the task creation structure at the time of task creation and is executed after the task is scheduled. You can design task entry functions.

#### Task Control Block  

Each task has a task control block (TCB). A TCB contains task information such as context stack pointer (SP), state, priority, ID, name, and stack size. TCB can reflect running conditions of each task. 

#### Task Stack

Each task has a separate task stack. The task stack stores information such as local variables, registers, function parameters, and function return addresses. When a task switch occurs, the context information of the task that is replaced is saved to its task stack. When the task is resumed, its context information will be quickly retrieved from the task stack to help resume the task from where it was paused.

#### Task Context  

Resources (such as registers) used by a running task are collectively known as task context, just like registers. After a task is suspended, other running tasks might modify the context of the suspended task. If the original context of the suspended task is not saved, the suspended task uses the modified context once resumed, incurring unpredictable errors.

Therefore, Huawei LiteOS will save the task context information of this task in its own task stack. This function is to resume context information after the task is resumed. There by continuing to execute the interrupted code when the task is suspended.

#### Task Switch  

A task switch process involves a few activities, including selecting the ready task with the highest priority, saving the context of the task that will be replaced, and restoring the context of the task that is newly selected to be executed.

### Operation Mechanism

The task management module of Huawei LiteOS provides functions such as creating, deleting, delaying, suspending, and resuming tasks, changing task priorities, locking and unlocking task scheduling, querying task IDs according to TCB, and querying TCB information according to IDs. 

When a module is initialized before a task is created, the OS allocates memory space needed by the TCB of the task. If insufficient memory space is available, the task fails to be initialized. After the task is successfully initialized, the operating system initializes the TCB of the task.

While creating a task, the operating system initializes the task stack and resets the context. The operating system also places the task entry function in the correct position so that the function will be executed after the task is booted for the first time.

## Development Guidelines

### Usage Scenarios

After a task is created, Huawei LiteOS Kernel can perform operations such as unlocking task scheduling, scheduling/suspending/resuming/delaying a task, or assigning/acquiring a task priority. When the task ends, the task will be detached.  

### Functions

The task management module provides the following functions:

| Function Category            | API                 | Description                                                   |
|------------------------------|---------------------|---------------------------------------------------------------|
| Task creation and deletion   | `LOS_TaskCreateOnly`  | Creates a task and suspends the task without scheduling it                             |
|                              | `LOS_TaskCreate`      | Creates a task. The task enters Ready state and is scheduled                      |
|                              | `LOS_TaskDelete`      | Deletes a particular task                                                           |
| Task state control           | `LOS_TaskResume`      | Resumes the suspended task                                                         |
|                              | `LOS_TaskSuspend`     | Suspends a particular task                                           |
|                              | `LOS_TaskDelay`       | Delays the task                                                             |
|                              | `LOS_TaskYield`       | Defines decentralization, and adjusts the scheduling order of tasks with a particular priority |
| Task scheduling control      | `LOS_TaskLock`        | Locks task scheduling                                                       |
|                              | `LOS_TaskUnlock`      | Unlocks task scheduling                                                               |
| Task priority control        | `LOS_CurTaskPriSet`   | Assigns a priority to the current task                                            |
|                              | `LOS_TaskPriSet`      | Set the priority of a particular task                                                |
|                              | `LOS_TaskPriGet`      | Gets the priority of a particular task                                         |
| Task information acquisition | `LOS_CurTaskIDGet`    | Gets the ID of the current task                                                  |
|                              | `LOS_TaskInfoGet`     | Gets the information of the current task                                          |
|                              | `LOS_TaskStatusGet`   | Obtains the status of a specified task.                                        |
|                              | `LOS_TaskNameGet`     | Obtains the name of a specified task.                                                |
|                              | `LOS_TaskInfoMonitor` | Monitors all tasks and obtains task information.                                   |
|                              | `LOS_NextTaskIDGet`   | Obtains the ID of the task to be scheduled.                                                    |

### Development Process

Task creation is used as an example to explain the development process. 

1. Configure the task management module in the los_config.h file.

	`LOSCFG_BASE_CORE_TSK_LIMIT`: the maximum number of tasks allowed. You can config according to requirement.

	 `LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE`: IDLE task stack size. Retain the default value unless otherwise required. You can config according to requirement.

	 `LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE` : default task stack size. Specify the parameter value according to actual needs when users create tasks.
		
	 `LOSCFG_BASE_CORE_TIMESLICE` : a switch to enable or disable the Time Slice. Set it to YES.

	 `LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT`: time slice. You can config according to actual situations.

	 `LOSCFG_BASE_CORE_TSK_MONITOR`: a switch to enable or disable the task monitoring module.
	
2. Call the LOS_TaskLock API to lock task scheduling. Prohibits high-priority task scheduling.

3. Call the LOS_TaskCreate API to create a task.

4. Call the LOS_TaskUnlock API to unlock task scheduling. Schedules tasks in order of priority.

5. Schedules tasks in order of priority. Delays the task.延时任务 `LOS_TaskDelay` ，任务延时等待。  

6. Call the LOS_TaskSuspend API to suspend the task. Suspends the task.

7. Call the LOS_TaskResume API to resume the suspended task. Resumes the suspended task.


### Task State

The status of Huawei LiteOS tasks is automatically maintained by the kernel, which is invisible to users. Therefore, users do not need to perform this operation.

### Task Error Codes

An error code is returned when attempting to create, delete, suspend, resume, or delay a task fails. The error code gives some insights into the possible cause of the failure. 

| SN | Error Code                              | Error ID Number | Description                                                 | Recommended Solution                                                 |
|----|-----------------------------------------|-----------------|-------------------------------------------------------------|---------|
| 1  | LOS_ERRNO_TSK_NO_MEMORY                 | 0x02000200      | Insufficient memory                                         | Allocate a larger memory area                                        |
| 2  | LOS_ERRNO_TSK_PTR_NULL                  | 0x02000201      | Null task parameter                                         | Check task parameters                                                |
| 3  | LOS_ERRNO_TSK_STKSZ_NOT_ALIGN           | 0x02000202      | Task stack size not aligned                                 | Align the task stack size on the boundary                            |
| 4  | LOS_ERRNO_TSK_PRIOR_ERROR               | 0x02000203      | Incorrect task priority                                     | Check the task priority                                              |
| 5  | LOS_ERRNO_TSK_ENTRY_NULL                | 0x02000204      | Null task entry function                                    | Define a task entry function                                         |
| 6  | LOS_ERRNO_TSK_NAME_EMPTY                | 0x02000205      | Task name unspecified                                       | Specify the task name                                                |
| 7  | LOS_ERRNO_TSK_STKSZ_TOO_SMALL           | 0x02000206      | Too small task stack                                        | Expand the task stack                                                |
| 8  | LOS_ERRNO_TSK_ID_INVALID                | 0x02000207      | Invalid task ID                                             | Check task IDs                                                       |
| 9  | LOS_ERRNO_TSK_ALREADY_SUSPENDED         | 0x02000208      | Task already suspended                                      | Suspend the task after it is resumed                                 |
| 10 | LOS_ERRNO_TSK_NOT_SUSPENDED             | 0x02000209      | Task not suspended                                          | Suspend the task                                                     |
| 11 | LOS_ERRNO_TSK_NOT_CREATED               | 0x0200020a      | Task not created                                            | Create the task                                                      |
| 12 | LOS_ERRNO_TSK_OPERATE_SWTMR             | 0x02000222      | Cannot operate software timer tasks                         | Do not operate software timer tasks                                  |
| 13 | LOS_ERRNO_TSK_MSG_NONZERO               | 0x0200020c      | Task information not zero                                   | Do not use the error code                                            |
| 14 | LOS_ERRNO_TSK_DELAY_IN_INT              | 0x0300020d      | Attempt to delay the task while an interrupt is underway    | Delay the task after the interrupt is finished                       |
| 15 | LOS_ERRNO_TSK_DELAY_IN_LOCK             | 0x0200020e      | Attempt to delay the task while task scheduling is locked   | Delay the task after task scheduling is unlocked                     |
| 16 | LOS_ERRNO_TSK_YIELD_INVALID_TASK        | 0x0200020f      | Invalid task to be scheduled                                | Check the task                                                       |
| 17 | LOS_ERRNO_TSK_YIELD_NOT_ENOUGH_TASK     | 0x02000210      | No task or only one task available for scheduling           | Add more tasks                                                       |
| 18 | LOS_ERRNO_TSK_TCB_UNAVAILABLE           | 0x02000211      | No idle TCB                                                 | Add more TCBs                                                        |
| 19 | LOS_ERRNO_TSK_HOOK_NOT_MATCH            | 0x02000212      | Task hook function mismatch                                 | Do not use the error code                                            |
| 20 | LOS_ERRNO_TSK_HOOK_IS_FULL              | 0x02000213      | Maximum number of task hook functions is reached            | Do not use the error code                                            |
| 21 | LOS_ERRNO_TSK_OPERATE_IDLE              | 0x02000214      | Idle task                                                   | Check the task ID and do not attempt to operate the task with the ID |
| 22 | LOS_ERRNO_TSK_SUSPEND_LOCKED            | 0x03000215      | Attempt to suspend the task while task scheduling is locked | Suspend the task after task scheduling is unlocked                   |
| 23 | LOS_ERRNO_TSK_FREE_STACK_FAILED         | 0x02000217      | Failed to free task stack                                   | Do not use the error code                                            |
| 24 | LOS_ERRNO_TSK_STKAREA_TOO_SMALL         | 0x02000218      | Small task stack area                                       | Do not use the error code                                            |
| 25 | LOS_ERRNO_TSK_ACTIVE_FAILED             | 0x02000219      | Failed to trigger the task                                  | Create an idle task and trigger a task switch                        |
| 26 | LOS_ERRNO_TSK_CONFIG_TOO_MANY           | 0x0200021a      | Too many task configuration options                         | Do not use the error code                                            |
| 27 | LOS_ERRNO_TSK_CP_SAVE_AREA_NOT_ALIGN    | 0x0200021b      | None                                                        | Do not use the error code                                            |
| 28 | LOS_ERRNO_TSK_MSG_Q_TOO_MANY            | 0x0200021d      | None                                                        | Do not use the error code                                            |
| 29 | LOS_ERRNO_TSK_CP_SAVE_AREA_NULL         | 0x0200021e      | None                                                        | Do not use the error code                                            |
| 30 | LOS_ERRNO_TSK_SELF_DELETE_ERR           | 0x0200021f      | None                                                        | Do not use the error code                                            |
| 31 | LOS_ERRNO_TSK_STKSZ_TOO_LARGE           | 0x02000220      | Large task stack                                            | Reduce the task stack size                                           |
| 32 | LOS_ERRNO_TSK_SUSPEND_SWTMR_NOT_ALLOWED | 0x02000221      | Suspension of a software timer task not allowed             | Check the task ID. Do not attempt to suspend a software timer task.  |

**Error Code Definition** 

An error code is 32 bits in length, where:

- Bits 31–24: error severity
- Bits 23–16: error flag
- Bits 15–8: module that encounters the error
- Bits 7–0: error ID number

```c
 #define LOS_ERRNO_OS_NORMAL(MID,ERRNO)
 (LOS_ERRTYPE_NORMAL | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))

 // LOS_ERRTYPE_NORMAL ：Define the error level as critical
 // LOS_ERRNO_OS_ID ：OS error code flag.
 // MID：OS_MOUDLE_ID
 // ERRNO：error ID number
```

For example:

```c
LOS_ERRNO_TSK_NO_MEMORY  LOS_ERRNO_OS_FATAL(LOS_MOD_TSK, 0x00)
```

<img :src="$withBase('/icon_notice.png')">

0x16,0x1c and 0x0b error codes are not defined and therefore cannot be used.

### Platform Differences

None.  

## Precautions

- While a new task is being created, the task control blocks (TCBs) and task stacks of previously deleted tasks are reclaimed.

- A task name is a pointer and not allocated memory space. Do not set a task name to the address of a local variable when you set task name.

- If the task size is set to 0, the setting does not take effect. Instead, the default task size defined by the `LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE` parameter is applied.

- Task stack size is aligned with the base address on the boundary of 8 bytes. Follow the "nothing more and nothing less" principle while determining the task stack size.

- A running task cannot be suspended while current task scheduling is locked.

- Idle and software timer tasks must not be suspended or deleted.

- In the interrupt handler function or in the case of the lock task, the operation that calls the `LOS_TaskDelay` API will fail.

- Locking task scheduling does not disable interrupts. Tasks can still be interrupted while task scheduling is locked.

- Locked task scheduling and unlocked task scheduling must be used in coordination.

- Task scheduling may occur while a task priority is being set.

- The maximum number of tasks (excluding idle tasks) able to be set by operating system is not equal to the total number of tasks available to users. For example, when a task is created for software timers, the number of available tasks is decreased by 1.

- Do not change the priority of a software timer task by calling the `LOS_CurTaskPriSet` API or the `LOS_TaskPriSet` API. Otherwise, system problems may occur.

- The `LOS_CurTaskPriSet` or LOS_TaskPriSet API must not be used when interrupts are being processed.

- If the corresponding task ID that LOS_TaskPriGet interface into the task is not created or exceed the maximum number of tasks, unified return 0xffff.

- Resources such as a mutex or a semaphore allocated to a task must have been released when the task is being deleted.

## Programming Example

### Example Description

Two tasks will be created: TaskHi and TaskLo. TaskHi has a higher priority than
TaskLo. 

1.  Two tasks will be created: TaskHi and TaskLo.

2.  TaskHi has a higher priority.

3.  TaskLo has a lower priority.

#### Example Code

```c
static UINT32 g_uwTskHiID;    
static UINT32 g_uwTskLoID;   

#define TSK_PRIOR_HI 4  
#define TSK_PRIOR_LO 5  

static UINT32 Example_TaskHi(VOID)
{
    UINT32 uwRet = LOS_OK;

    dprintf("Enter TaskHi Handler.\r\n");

    /* task delay 5 ticks, task will be suspend */ 
    uwRet = LOS_TaskDelay(5);
    if (uwRet != LOS_OK)
    {
        dprintf("Delay Task Failed.\r\n");
        return LOS_NOK;
    }

    /* task resumed */
    dprintf("TaskHi LOS_TaskDelay Done.\r\n");

    /* suspend self */
    uwRet = LOS_TaskSuspend(g_uwTskHiID);
    if (uwRet != LOS_OK)
    {
        dprintf("Suspend TaskHi Failed.\r\n");
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_TASK,LOS_INSPECT_STU_ERROR);
        if (LOS_OK != uwRet)
        {
            dprintf("Set Inspect Status Err\n");
        }
        return LOS_NOK;
    }
    
    dprintf("TaskHi LOS_TaskResume Success.\r\n");
        
    uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_TASK,LOS_INSPECT_STU_SUCCESS);
    if (LOS_OK != uwRet)
    {
        dprintf("Set Inspect Status Err\n");
    }
    
    /* delete self */
    if(LOS_OK != LOS_TaskDelete(g_uwTskHiID))
    {
        dprintf("TaskHi delete failed .\n");
        return LOS_NOK;
    }
    
    return LOS_OK;
}

static UINT32 Example_TaskLo(VOID)
{
    UINT32 uwRet;

    dprintf("Enter TaskLo Handler.\r\n");

    /* task delay 10 ticks, task will be suspend */
    uwRet = LOS_TaskDelay(10);
    if (uwRet != LOS_OK)
    {
        dprintf("Delay TaskLo Failed.\r\n");
        return LOS_NOK;
    }

    dprintf("TaskHi LOS_TaskSuspend Success.\r\n");

    /* resumed task g_uwTskHiID */
    uwRet = LOS_TaskResume(g_uwTskHiID);
    if (uwRet != LOS_OK)
    {
        dprintf("Resume TaskHi Failed.\r\n");
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_TASK,LOS_INSPECT_STU_ERROR);
        if (LOS_OK != uwRet)  
        {
            dprintf("Set Inspect Status Err\n");
        }
        return LOS_NOK;
    }
    
    /* delete self */
    if(LOS_OK != LOS_TaskDelete(g_uwTskLoID))
    {
        dprintf("TaskLo delete failed .\n");
        
        return LOS_NOK;
    }
    
    return LOS_OK;  
```  

#### Verification

The verification result is as follows:

<img :src="$withBase('/task-sample.png')" alt="verification result">
