# Event Error Codes

Error codes are returned if errors occur during event operations, such as event initialization, event destroying, event reading, event writing, and event clearance, to facilitate fault locating.  

| No. | Definition                        | Error Code | Description                                                                          | Solution                        |
|-----|-----------------------------------|------------|--------------------------------------------------------------------------------------|---------------------------------|
| 1   | `LOS_ERRNO_EVENT_SETBIT_INVALID`  | 0x02001c00 | Bit 25 of the event ID must not be set to 1 because it is reserved as an error code. | Set bit 25 of the event ID to 0.|
| 2   | `LOS_ERRNO_EVENT_READ_TIMEOUT`    | 0x02001c01 | Event reading times out.                                                             | Increase the permitted wait time. Alternatively, re-read the event. |
| 3   | `LOS_ERRNO_EVENT_EVENTMASK_INVALID` | 0x02001c02 | The passed-in event ID is invalid.                                                 | Pass in a valid event ID.       |
| 4   | `LOS_ERRNO_EVENT_READ_IN_INTERRUPT` | 0x02001c03 | The event is being read when an interrupt is being processed.                      | Let a new task read the event.  |
| 5   | `LOS_ERRNO_EVENT_FLAGS_INVALID`   | 0x02001c04 | The mode of event reading is invalid.                                                | Pass in a valid mode.           |
| 6   | `LOS_ERRNO_EVENT_READ_IN_LOCK`    | 0x02001c05 | The task is locked and fails to read the event.                                      | Unlock the task, and then let the task read the event.     |
| 7   | `LOS_ERRNO_EVENT_PTR_NULL`        | 0x02001c06 | The passed-in pointer is null.                                                       | Pass in a non-null pointer.     |

An error code is a 32-bit storage unit. Bit 24 to bit 31 indicate an error level; bit 16 to bit 23 indicate an error code flag; bit 8 to bit 15 indicate the ID of the module that reports the error code; bit 0 to bit 7 indicate an error code. The following is the example of an error code:  

```c  
#define LOS_ERRNO_OS_ERROR(MID, ERRNO)  \
(LOS_ERRTYPE_ERROR | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))
```  

::: tip Note
- `LOS_ERRTYPE_ERROR`: Define critical OS errors
- `LOS_ERRNO_OS_ID`: OS error code flag
- `MID`: OS_MOUDLE_ID
- `LOS_MOD_EVENT`: Event module ID
- `ERRNO`: error ID number

For example:  
```c  
#define LOS_ERRNO_EVENT_READ_IN_LOCK LOS_ERRNO_OS_ERROR(LOS_MOD_EVENT, 0x05)
```   
:::  
