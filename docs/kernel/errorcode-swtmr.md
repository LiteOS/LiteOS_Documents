### 软件定时器错误码

对软件定时器存在失败可能性的操作，包括创建、删除、暂停、重启定时器等等，均需要返回对应的错误码，以便快速定位错误原因。  

| 序号 | 定义                                      | 实际数值   | 描述                                         | 参考解决方案                                               |
|------|-------------------------------------------|------------|----------------------------------------------|------------------------------------------------------------|
| 1    | `LOS_ERRNO_SWTMR_PTR_NULL`              | 0x02000300 | 软件定时器回调函数为空                       | 定义软件定时器回调函数                                     |
| 2    | `LOS_ERRNO_SWTMR_INTERVAL_NOT_SUITED`  | 0x02000301 | 软件定时器间隔时间为0                        | 重新定义间隔时间                                           |
| 3    | `LOS_ERRNO_SWTMR_MODE_INVALID`          | 0x02000302 | 不正确的软件定时器模式                       | 确认软件定时器模式，范围为[0,2]                            |
| 4    | `LOS_ERRNO_SWTMR_RET_PTR_NULL`         | 0x02000303 | 软件定时器ID指针入参为NULL                   | 定义ID变量，传入指针                                       |
| 5    | `LOS_ERRNO_SWTMR_MAXSIZE`                | 0x02000304 | 软件定时器个数超过最大值                     | 重新定义软件定时器最大个数，或者等待一个软件定时器释放资源 |
| 6    | `LOS_ERRNO_SWTMR_ID_INVALID`            | 0x02000305 | 不正确的软件定时器ID入参                     | 确保入参合法                                               |
| 7    | `LOS_ERRNO_SWTMR_NOT_CREATED`           | 0x02000306 | 软件定时器未创建                             | 创建软件定时器                                             |
| 8    | `LOS_ERRNO_SWTMR_NO_MEMORY`             | 0x02000307 | 软件定时器链表创建内存不足                   | 申请一块足够大的内存供软件定时器使用                       |
| 9    | `LOS_ERRNO_SWTMR_MAXSIZE_INVALID`       | 0x02000308 | 不正确的软件定时器个数最大值                 | 重新定义该值                                               |
| 10   | `LOS_ERRNO_SWTMR_HWI_ACTIVE`            | 0x02000309 | 在中断中使用定时器                           | 修改源代码确保不在中断中使用                               |
| 11   | `LOS_ERRNO_SWTMR_HANDLER_POOL_NO_MEM` | 0x0200030a | membox内存不足                               | 扩大内存                                                   |
| 12   | `LOS_ERRNO_SWTMR_QUEUE_CREATE_FAILED`  | 0x0200030b | 软件定时器队列创建失败                       | 检查用以创建队列的内存是否足够                             |
| 13   | `LOS_ERRNO_SWTMR_TASK_CREATE_FAILED`   | 0x0200030c | 软件定时器任务创建失败                       | 检查用以创建软件定时器任务的内存是否足够并重新创建         |
| 14   | `LOS_ERRNO_SWTMR_NOT_STARTED`           | 0x0200030d | 未启动软件定时器                             | 启动软件定时器                                             |
| 15   | `LOS_ERRNO_SWTMR_STATUS_INVALID`        | 0x0200030e | 不正确的软件定时器状态                       | 检查确认软件定时器状态                                     |
| 16   | `LOS_ERRNO_SWTMR_SORTLIST_NULL`         | null       | 暂无                                         | 该错误码暂不使用                                           |
| 17   | `LOS_ERRNO_SWTMR_TICK_PTR_NULL`        | 0x02000310 | 用以获取软件定时器超时tick数的入参指针为NULL | 创建一个有效的变量                                         |

**错误码定义：**

错误码是一个 32 位的存储单元，31~24 位表示错误等级，23~16 位表示错误码标志，15~8 位代表错误码所属模块，7~0 位表示错误码序号，如下  

```c
#define LOS_ERRNO_OS_NORMAL(MID,ERRNO)  \
(LOS_ERRTYPE_NORMAL | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))
```
::: tip 说明
- `LOS_ERRTYPE_NORMAL` ：Define the error level as critical
- `LOS_ERRNO_OS_ID` ：OS error code flag.
- `MID` ：OS_MOUDLE_ID
- `ERRNO` ：error ID number    

例如：  
```c  
#define LOS_ERRNO_SWTMR_PTR_NULL  \
LOS_ERRNO_OS_ERROR(LOS_MOD_SWTMR, 0x00)  
```
:::
