# 互斥锁错误码

对互斥锁存在失败的可能性操作，包括互斥锁创建，互斥锁删除，互斥锁申请，互斥锁释放。  

| 序号 | 定义                             | 实际数值   | 描述                                       | 参考解决方案                         |
|------|----------------------------------|------------|--------------------------------------------|--------------------------------------|
| 1    | `LOS_ERRNO_MUX_NO_MEMORY`      | 0x02001d00 | 内存请求失败                               | 减少互斥锁限制数量的上限             |
| 2    | `LOS_ERRNO_MUX_INVALID`         | 0x02001d01 | 互斥锁不可用                               | 传入有效的互斥锁的ID                 |
| 3    | `LOS_ERRNO_MUX_PTR_NULL`       | 0x02001d02 | 入参为空                                   | 确保入参可用                         |
| 4    | `LOS_ERRNO_MUX_ALL_BUSY`       | 0x02001d03 | 没有互斥锁可用                             | 增加互斥锁限制数量的上限             |
| 5    | `LOS_ERRNO_MUX_UNAVAILABLE`     | 0x02001d04 | 锁失败，因为锁被其他线程使用               | 等待其他线程解锁或者设置等待时间     |
| 6    | `LOS_ERRNO_MUX_PEND_INTERR`    | 0x02001d05 | 在中断中使用互斥锁                         | 在中断中禁止调用此接口               |
| 7    | `LOS_ERRNO_MUX_PEND_IN_LOCK`  | 0x02001d06 | 任务调度没有使能，线程等待另一个线程释放锁 | 设置PEND为非阻塞模式或者使能任务调度 |
| 8    | `LOS_ERRNO_MUX_TIMEOUT`         | 0x02001d07 | 互斥锁PEND超时                             | 增加等待时间或者设置一直等待模式     |
| 9    | `LOS_ERRNO_MUX_OVERFLOW`        | 0x02001d08 | 暂未使用，待扩展                           | 无                                   |
| 10   | `LOS_ERRNO_MUX_PENDED`          | 0x02001d09 | 删除正在使用的锁                           | 等待解锁再删除锁                     |
| 11   | `LOS_ERRNO_MUX_GET_COUNT_ERR` | 0x02001d0a | 暂未使用，待扩展                           | 无                                   |
| 12   | `LOS_ERRNO_MUX_REG_ERROR`      | 0x02001d0b | 暂未使用，待扩展                           | 无                                   |

**错误码定义：** 错误码是一个 32 位的存储单元，31~24 位表示错误等级，23~16 位表示错误码标志，15~8 位代表错误码所属模块，7~0 位表示错误码序号，如下  
```c
#define LOS_ERRNO_OS_ERROR(MID, ERRNO)  \
(LOS_ERRTYPE_ERROR | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))
```
::: tip 注意
- LOS_ERRTYPE_ERROR：Define critical OS errors
- LOS_ERRNO_OS_ID：OS error code flag
- LOS_MOD_MUX：Mutex module ID
- MID：OS_MOUDLE_ID
- ERRNO：error ID number    

例如：  
```  
LOS_ERRNO_MUX_TIMEOUT       LOS_ERRNO_OS_ERROR(LOS_MOD_MUX, 0x07)   
```  
:::  
