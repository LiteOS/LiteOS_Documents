# Software Timer Error Codes

Error codes are returned if errors occur during software timer operations, such as creating, deleting, suspending, or restarting a software timer, to facilitate fault locating.  

| No. | Definition                          | Error Code | Description                                                                                  | Solution                                                                                           |
|-----|-------------------------------------|------------|----------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------|
| 1   | `LOS_ERRNO_SWTMR_PTR_NULL`            | 0x02000300 | The callback function of the software timer is null.                                         | Define the callback function of the software timer.                                                |
| 2   | `LOS_ERRNO_SWTMR_INTERVAL_NOT_SUITED` | 0x02000301 | The timer length of the software timer is 0.                                                 | Redefine the timer length.                                                                         |
| 3   | `LOS_ERRNO_SWTMR_MODE_INVALID`        | 0x02000302 | The mode of the software timer is incorrect.                                                 | Modify the mode of the software timer. Range: [0, 2].                                              |
| 4   | `LOS_ERRNO_SWTMR_RET_PTR_NULL`        | 0x02000303 | The passed-in pointer to the software timer ID is null.                                      | Pass in a non-null pointer.                                                                        |
| 5   | `LOS_ERRNO_SWTMR_MAXSIZE`             | 0x02000304 | The number of software timers exceeds the maximum value.                                     | Redefine the maximum number of software timers, or wait until a software timer releases resources. |
| 6   | `LOS_ERRNO_SWTMR_ID_INVALID`          | 0x02000305 | The passed-in software timer ID is incorrect.                                                | Pass in a correct software timer ID.                                                               |
| 7   | `LOS_ERRNO_SWTMR_NOT_CREATED`         | 0x02000306 | No software timer is created.                                                                | Create a software timer.                                                                           |
| 8   | `LOS_ERRNO_SWTMR_NO_MEMORY`           | 0x02000307 | The memory space is insufficient for creating the linked list of a software timer.           | Apply for a larger memory space for the software timer.                                            |
| 9   | `LOS_ERRNO_SWTMR_MAXSIZE_INVALID`     | 0x02000308 | The maximum number of software timers is incorrect.                                          | Redefine the maximum number of software timers.                                                    |
| 10  | `LOS_ERRNO_SWTMR_HWI_ACTIVE`          | 0x02000309 | A timer is used when the CPU is processing interrupts.                                       | Modify the source code to ensure that no timer is used when the CPU is processing interrupts.      |
| 11  | `LOS_ERRNO_SWTMR_HANDLER_POOL_NO_MEM` | 0x0200030a | The memory space allocated to the membox is insufficient.                                    | Expand the memory space.                                                                           |
| 12  | `LOS_ERRNO_SWTMR_QUEUE_CREATE_FAILED` | 0x0200030b | The software timer queue fails to be created.                                                | Check whether the memory space is sufficient for creating the queue.                               |
| 13  | `LOS_ERRNO_SWTMR_TASK_CREATE_FAILED`  | 0x0200030c | The software timer task fails to be created.                                                 | Allocate sufficient memory space for creating the software timer task.                             |
| 14  | `LOS_ERRNO_SWTMR_NOT_STARTED`         | 0x0200030d | The software timer is not started.                                                           | Start the software timer.                                                                          |
| 15  | `LOS_ERRNO_SWTMR_STATUS_INVALID`      | 0x0200030e | The software timer status is incorrect.                                                      | Check the software timer status.                                                                   |
| 16  | `LOS_ERRNO_SWTMR_SORTLIST_NULL`       | Null       | The error code is not in use.                                                                | N/A                                                                                                |
| 17  | `LOS_ERRNO_SWTMR_TICK_PTR_NULL`       | 0x02000310 | The passed-in pointer used for obtaining the number of software timer timeout ticks is null. | Pass in a non-null pointer.                                                                        |

An error code is a 32-bit storage unit. Bit 24 to bit 31 indicate an error level; bit 16 to bit 23 indicate an error code flag; bit 8 to bit 15 indicate the ID of the module that reports the error code; bit 0 to bit 7 indicate an error code. The following is the example of an error code:  

```c
#define LOS_ERRNO_OS_NORMAL(MID,ERRNO)  \
(LOS_ERRTYPE_NORMAL | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))
```  
::: tip Note  
- `LOS_ERRTYPE_NORMAL`: Define the error level as critical
- `LOS_ERRNO_OS_ID`: OS error code flag.
- `MID`: OS_MOUDLE_ID
- `ERRNO`: error ID number

For example:  
```c  
#define LOS_ERRNO_SWTMR_PTR_NULL \ LOS_ERRNO_OS_ERROR(LOS_MOD_SWTMR, 0x00)
```  
:::  
