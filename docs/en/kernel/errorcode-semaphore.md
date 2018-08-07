# Semaphore Error Codes

Error codes are returned if errors occur during semaphore operations, such as creating, pending, posting, and deleting semaphores, to facilitate fault locating.  

| No. | Definition                 | Error Code | Description                                                           | Solution                                                                             |
|-----|----------------------------|------------|-----------------------------------------------------------------------|--------------------------------------------------------------------------------------|
| 1   | `LOS_ERRNO_SEM_NO_MEMORY`    | 0x02000700 | The memory space is insufficient.                                     | Allocate a larger memory space to the semaphore.                                     |
| 2   | `LOS_ERRNO_SEM_INVALID`      | 0x02000701 | The passed-in parameter is invalid.                                   | Modify the parameter to a valid one.                                                 |
| 3   | `LOS_ERRNO_SEM_PTR_NULL`     | 0x02000702 | The passed-in pointer is null.                                        | Pass in a non-null pointer.                                                          |
| 4   | `LOS_ERRNO_SEM_ALL_BUSY`     | 0x02000703 | The semaphore control block is unavailable.                           | Post semaphore resources.                                                            |
| 5   | `LOS_ERRNO_SEM_UNAVAILABLE`  | 0x02000704 | The scheduled time is invalid.                                        | Modify the scheduled time to a correct one.                                          |
| 6   | `LOS_ERRNO_SEM_PEND_INTERR`  | 0x02000705 | When the CPU is processing interrupts, the LOS_SemPend API is called. | Do not call the LOS_SemPend API when the CPU is processing interrupts.               |
| 7   | `LOS_ERRNO_SEM_PEND_IN_LOCK` | 0x02000706 | The task is locked and fails to obtain the semaphore.                 | Do not call the LOS_SemPend API when the task is locked.                             |
| 8   | `LOS_ERRNO_SEM_TIMEOUT`      | 0x02000707 | The time for obtaining semaphores expires.                            | Set the time to a proper range.                                                      |
| 9   | `LOS_ERRNO_SEM_OVERFLOW`     | 0x02000708 | The number of allowed semaphore pendings exceeds the maximum value.   | Pass in a valid value.                                                               |
| 10  | `LOS_ERRNO_SEM_PENDED`       | 0x02000709 | The task queue waiting for the semaphore is not empty.                | Wake up all tasks that are waiting for the semaphore, and then delete the semaphore. |

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

For example  
```c
LOS_ERRNO_SEM_NO_MEMORY        LOS_ERRNO_OS_ERROR(LOS_MOD_SEM, 0x00))
```  
:::  
