# 信号量错误码

对可能导致信号量操作失败的情况，包括创建信号量、申请信号量、释放信号量、删除信号量等，均需要返回对应的错误码，以便快速定位错误原因。  

| 序号 | 定义                            | 实际数值   | 描述                         | 参考解决方案                             |
|------|---------------------------------|------------|------------------------------|-------------------------------------|
| 1    | `LOS_ERRNO_SEM_NO_MEMORY`     | 0x02000700 | 内存空间不足                 | 分配更大的内存分区                       |
| 2    | `LOS_ERRNO_SEM_INVALID`        | 0x02000701 | 非法传参                     | 改变传数为合法值                         |
| 3    | `LOS_ERRNO_SEM_PTR_NULL`      | 0x02000702 | 传入空指针                   | 传入合法指针                             |
| 4    | `LOS_ERRNO_SEM_ALL_BUSY`      | 0x02000703 | 信号量控制块不可用           | 释放资源信号量资源                       |
| 5    | `LOS_ERRNO_SEM_UNAVAILABLE`    | 0x02000704 | 定时时间非法                 | 传入正确的定时时间                       |
| 6    | `LOS_ERRNO_SEM_PEND_INTERR`   | 0x02000705 | 中断期间非法调用LOS_SemPend | 中断期间禁止调用LOS_SemPend             |
| 7    | `LOS_ERRNO_SEM_PEND_IN_LOCK` | 0x02000706 | 任务被锁，无法获得信号量     | 在任务被锁时，不能调用LOS_SemPend       |
| 8    | `LOS_ERRNO_SEM_TIMEOUT`        | 0x02000707 | 获取信号量时间超时           | 将时间设置在合理范围内                   |
| 9    | `LOS_ERRNO_SEM_OVERFLOW`       | 0x02000708 | 信号量允许pend次数超过最大值 | 传入合法的值                             |
| 10   | `LOS_ERRNO_SEM_PENDED`         | 0x02000709 | 等待信号量的任务队列不为空   | 唤醒所有等待该型号量的任务后删除该信号量 |

**错误码定义：** 错误码是一个32位的存储单元，31\~24位表示错误等级，23\~16位表示错误码标志，15\~8位代表错误码所属模块，7\~0位表示错误码序号，如下
```c
#define LOS_ERRNO_OS_NORMAL(MID,ERRNO)  \
(LOS_ERRTYPE_NORMAL | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))
```
::: tip 说明
- `LOS_ERRTYPE_NORMAL` ：Define the error level as critical
- `LOS_ERRNO_OS_ID` ：OS error code flag.
- `MID` ：OS_MOUDLE_ID
- `ERRNO` ：error ID number  

```c  
LOS_ERRNO_SEM_NO_MEMORY        LOS_ERRNO_OS_ERROR(LOS_MOD_SEM, 0x00))  
```
:::  
