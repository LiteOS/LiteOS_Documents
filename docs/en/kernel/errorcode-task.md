# Task Error Codes

An error code is returned when attempting to create, delete, suspend, resume, or delay a task fails. The error code gives some insights into the possible cause of the failure.  

| SN | Error Code                              | Error ID Number | Description                                                 | Recommended Solution                                                 |
|----|-----------------------------------------|-----------------|-------------------------------------------------------------|---------|
| 1  | `LOS_ERRNO_TSK_NO_MEMORY`                 | 0x02000200      | Insufficient memory                                         | Allocate a larger memory area                                        |
| 2  | `LOS_ERRNO_TSK_PTR_NULL`                  | 0x02000201      | Null task parameter                                         | Check task parameters                                                |
| 3  | `LOS_ERRNO_TSK_STKSZ_NOT_ALIGN`           | 0x02000202      | Task stack size not aligned                                 | Align the task stack size on the boundary                            |
| 4  | `LOS_ERRNO_TSK_PRIOR_ERROR`               | 0x02000203      | Incorrect task priority                                     | Check the task priority                                              |
| 5  | `LOS_ERRNO_TSK_ENTRY_NULL`                | 0x02000204      | Null task entry function                                    | Define a task entry function                                         |
| 6  | `LOS_ERRNO_TSK_NAME_EMPTY`                | 0x02000205      | Task name unspecified                                       | Specify the task name                                                |
| 7  | `LOS_ERRNO_TSK_STKSZ_TOO_SMALL`           | 0x02000206      | Too small task stack                                        | Expand the task stack                                                |
| 8  | `LOS_ERRNO_TSK_ID_INVALID`                | 0x02000207      | Invalid task ID                                             | Check task IDs                                                       |
| 9  | `LOS_ERRNO_TSK_ALREADY_SUSPENDED`         | 0x02000208      | Task already suspended                                      | Suspend the task after it is resumed                                 |
| 10 | `LOS_ERRNO_TSK_NOT_SUSPENDED`             | 0x02000209      | Task not suspended                                          | Suspend the task                                                     |
| 11 | `LOS_ERRNO_TSK_NOT_CREATED`               | 0x0200020a      | Task not created                                            | Create the task                                                      |
| 12 | `LOS_ERRNO_TSK_OPERATE_SWTMR`             | 0x02000222      | Cannot operate software timer tasks                         | Do not operate software timer tasks                                  |
| 13 | `LOS_ERRNO_TSK_MSG_NONZERO`               | 0x0200020c      | Task information not zero                                   | Do not use the error code                                            |
| 14 | `LOS_ERRNO_TSK_DELAY_IN_INT`              | 0x0300020d      | Attempt to delay the task while an interrupt is underway    | Delay the task after the interrupt is finished                       |
| 15 | `LOS_ERRNO_TSK_DELAY_IN_LOCK`             | 0x0200020e      | Attempt to delay the task while task scheduling is locked   | Delay the task after task scheduling is unlocked                     |
| 16 | `LOS_ERRNO_TSK_YIELD_INVALID_TASK`        | 0x0200020f      | Invalid task to be scheduled                                | Check the task                                                       |
| 17 | `LOS_ERRNO_TSK_YIELD_NOT_ENOUGH_TASK`     | 0x02000210      | No task or only one task available for scheduling           | Add more tasks                                                       |
| 18 | `LOS_ERRNO_TSK_TCB_UNAVAILABLE`           | 0x02000211      | No idle TCB                                                 | Add more TCBs                                                        |
| 19 | `LOS_ERRNO_TSK_HOOK_NOT_MATCH`            | 0x02000212      | Task hook function mismatch                                 | Do not use the error code                                            |
| 20 | `LOS_ERRNO_TSK_HOOK_IS_FULL`              | 0x02000213      | Maximum number of task hook functions is reached            | Do not use the error code                                            |
| 21 | `LOS_ERRNO_TSK_OPERATE_IDLE`              | 0x02000214      | Idle task                                                   | Check the task ID and do not attempt to operate the task with the ID |
| 22 | `LOS_ERRNO_TSK_SUSPEND_LOCKED`            | 0x03000215      | Attempt to suspend the task while task scheduling is locked | Suspend the task after task scheduling is unlocked                   |
| 23 | `LOS_ERRNO_TSK_FREE_STACK_FAILED`         | 0x02000217      | Failed to free task stack                                   | Do not use the error code                                            |
| 24 | `LOS_ERRNO_TSK_STKAREA_TOO_SMALL`         | 0x02000218      | Small task stack area                                       | Do not use the error code                                            |
| 25 | `LOS_ERRNO_TSK_ACTIVE_FAILED`             | 0x02000219      | Failed to trigger the task                                  | Create an idle task and trigger a task switch                        |
| 26 | `LOS_ERRNO_TSK_CONFIG_TOO_MANY`           | 0x0200021a      | Too many task configuration options                         | Do not use the error code                                            |
| 27 | `LOS_ERRNO_TSK_CP_SAVE_AREA_NOT_ALIGN`    | 0x0200021b      | None                                                        | Do not use the error code                                            |
| 28 | `LOS_ERRNO_TSK_MSG_Q_TOO_MANY`            | 0x0200021d      | None                                                        | Do not use the error code                                            |
| 29 | `LOS_ERRNO_TSK_CP_SAVE_AREA_NULL`         | 0x0200021e      | None                                                        | Do not use the error code                                            |
| 30 | `LOS_ERRNO_TSK_SELF_DELETE_ERR`           | 0x0200021f      | None                                                        | Do not use the error code                                            |
| 31 | `LOS_ERRNO_TSK_STKSZ_TOO_LARGE`           | 0x02000220      | Large task stack                                            | Reduce the task stack size                                           |
| 32 | `LOS_ERRNO_TSK_SUSPEND_SWTMR_NOT_ALLOWED` | 0x02000221      | Suspension of a software timer task not allowed             | Check the task ID. Do not attempt to suspend a software timer task.  |

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

::: tip 举例  

```c
LOS_ERRNO_TSK_NO_MEMORY  LOS_ERRNO_OS_FATAL(LOS_MOD_TSK, 0x00)
```  
:::

::: warning 注意
0x16,0x1c and 0x0b error codes are not defined and therefore cannot be used.  
:::  
