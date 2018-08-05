
# Mutex Error Code

Error codes are returned if errors occur during mutex operations, such as mutex creation, mutex deletion, mutex pending, and mutex posting, to facilitate fault locating.

| No. | Definition                    | Error Code | Description                                            | Solution                                                        |
|-----|-------------------------------|------------|--------------------------------------------------------|-----------------------------------------------------------------|
| 1   | `LOS_ERRNO_MUX_NO_MEMORY`     | 0x02001d00 | The request for memory is rejected.                    | Lower the upper limit on the number of mutexes.                 |
| 2   | `LOS_ERRNO_MUX_INVALID`       | 0x02001d01 | The mutex is not usable.                               | Pass in a valid mutex ID.                                       |
| 3   | `LOS_ERRNO_MUX_PTR_NULL`      | 0x02001d02 | The input parameter is null.                           | Pass in a non-null parameter.                                   |
| 4   | `LOS_ERRNO_MUX_ALL_BUSY`      | 0x02001d03 | No mutexes are available.                              | Raise the upper limit on the number of mutexes.                 |
| 5   | `LOS_ERRNO_MUX_UNAVAILABLE`   | 0x02001d04 | The mutex fails to be locked because it is locked by another thread.          | Wait for another thread to release the mutex. Alternatively, set a timeout period. |
| 6   | `LOS_ERRNO_MUX_PEND_INTERR`   | 0x02001d05 | Mutex pend occurs when an interrupt is being processed. | Do not call this API when an interrupt is being processed.     |
| 7   | `LOS_ERRNO_MUX_PEND_IN_LOCK`  | 0x02001d06 | Task scheduling is not enabled, and the thread is waiting for another thread to release the mutex. | Set the mutex pend mode to the non-blocking mode. Alternatively, enable task scheduling.       |
| 8   | `LOS_ERRNO_MUX_TIMEOUT`       | 0x02001d07 | Mutex pend times out.   | Increase the wait time. Alternatively, set the mutex pend mode to the permanent blocking mode. |
| 9   | `LOS_ERRNO_MUX_OVERFLOW`      | 0x02001d08 | The error code is not in use.                           | N/A                                                            |
| 10  | `LOS_ERRNO_MUX_PENDED`        | 0x02001d09 | The mutex being deleted is locked.                      | Delete the mutex after it is released.                         |
| 11  | `LOS_ERRNO_MUX_GET_COUNT_ERR` | 0x02001d0a | The error code is not in use.                           | N/A                                                            |
| 12  | `LOS_ERRNO_MUX_REG_ERROR`     | 0x02001d0b | The error code is not in use.                           | N/A                                                            |

An error code is a 32-bit storage unit. Bit 24 to bit 31 indicate an error level; bit 16 to bit 23 indicate an error code flag; bit 8 to bit 15 indicate the ID of the module that reports the error code; bit 0 to bit 7 indicate an error code. The following is the example of an error code:  

```c  
#define LOS_ERRNO_OS_ERROR(MID, ERRNO)  \
(LOS_ERRTYPE_ERROR | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))
```  

::: tip Note
- `LOS_ERRTYPE_ERROR`: Define critical OS errors
- `LOS_ERRNO_OS_ID`: OS error code flag
- `MID`: OS_MOUDLE_ID
- `LOS_MOD_MUX`: Mutex module ID
- `ERRNO`: error ID number  

For example: 
```c  
LOS_ERRNO_MUX_TIMEOUT LOS_ERRNO_OS_ERROR(LOS_MOD_MUX, 0x07)
```  
:::
