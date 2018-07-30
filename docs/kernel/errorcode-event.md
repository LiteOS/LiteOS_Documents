# 事件错误码

对事件存在失败的可能性操作，包括事件初始化，事件销毁，事件读写，时间清除。

| 序号 | 定义                                   | 实际值     | 描述                                                     | 参考解决方案             |
|------|----------------------------------------|------------|----------------------------------------------------------|--------------------------|
| 1    | `LOS_ERRNO_EVENT_SETBIT_INVALID`     | 0x02001c00 | 事件ID的第25个bit不能设置为1，因为该位已经作为错误码使用 | 事件ID的第25bit置为0     |
| 2    | `LOS_ERRNO_EVENT_READ_TIMEOUT`       | 0x02001c01 | 读超时                                                   | 增加等待时间或者重新读取 |
| 3    | `LOS_ERRNO_EVENT_EVENTMASK_INVALID`  | 0x02001c02 | 入参的事件ID是无效的                                     | 传入有效的事件ID参数     |
| 4    | `LOS_ERRNO_EVENT_READ_IN_INTERRUPT` | 0x02001c03 | 在中断中读取事件                                         | 启动新的任务来获取事件   |
| 5    | `LOS_ERRNO_EVENT_FLAGS_INVALID`      | 0x02001c04 | 读取事件的mode无效                                       | 传入有效的mode参数       |
| 6    | `LOS_ERRNO_EVENT_READ_IN_LOCK`      | 0x02001c05 | 任务锁住，不能读取事件                                   | 解锁任务，再读取事件     |
| 7    | `LOS_ERRNO_EVENT_PTR_NULL`           | 0x02001c06 | 传入的参数为空指针                                       | 传入非空入参             |

**错误码定义：** 错误码是一个32位的存储单元，31~24位表示错误等级，23~16位表示错误码标志，15~8位代表错误码所属模块，7~0位表示错误码序号，如下  
```c   
#define LOS_ERRNO_OS_ERROR(MID, ERRNO)  
(LOS_ERRTYPE_ERROR | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))
```  

::: tip 说明  
- `LOS_ERRTYPE_ERROR` ：Define critical OS errors  
- `LOS_ERRNO_OS_ID` ：OS error code flag  
- `MID` ：OS_MOUDLE_ID  
- `LOS_MOD_EVENT` ：Event module ID  
- `ERRNO` ：error ID number   

例如：  
```c   
#define LOS_ERRNO_EVENT_READ_IN_LOCK  LOS_ERRNO_OS_ERROR(LOS_MOD_EVENT, 0x05)  
```  
:::  
