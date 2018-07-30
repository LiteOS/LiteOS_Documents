# 任务错误码

对任务存在失败可能性的操作，包括创建任务、删除任务、挂起任务、恢复任务、延时任务等等，均需要返回对应的错误码，以便快速定位错误原因。 

| 序号 | 定义                                          | 实际数值   | 描述                                   | 参考解决方案                               |
|------|-----------------------------------------------|------------|----------------------------------------|--------------------------------------------|
| 1    | `LOS_ERRNO_TSK_NO_MEMORY`                   | 0x03000200 | 内存空间不足                           | 分配更大的内存分区                         |
| 2    | `LOS_ERRNO_TSK_PTR_NULL`                    | 0x02000201 | 任务参数为空                           | 检查任务参数                               |
| 3    | `LOS_ERRNO_TSK_STKSZ_NOT_ALIGN`            | 0x02000202 | 任务栈大小未对齐                       | 对齐任务栈                                 |
| 4    | `LOS_ERRNO_TSK_PRIOR_ERROR`                 | 0x02000203 | 不正确的任务优先级                     | 检查任务优先级                             |
| 5    | `LOS_ERRNO_TSK_ENTRY_NULL`                  | 0x02000204 | 任务入口函数为空                       | 定义任务入口函数                           |
| 6    | `LOS_ERRNO_TSK_NAME_EMPTY`                  | 0x02000205 | 任务名为空                             | 设置任务名                                 |
| 7    | `LOS_ERRNO_TSK_STKSZ_TOO_SMALL`            | 0x02000206 | 任务栈太小                             | 扩大任务栈                                 |
| 8    | `LOS_ERRNO_TSK_ID_INVALID`                  | 0x02000207 | 无效的任务ID                           | 检查任务ID                                 |
| 9    | `LOS_ERRNO_TSK_ALREADY_SUSPENDED`           | 0x02000208 | 任务已经被挂起                         | 等待这个任务被恢复后，再去尝试挂起这个任务 |
| 10   | `LOS_ERRNO_TSK_NOT_SUSPENDED`               | 0x02000209 | 任务未被挂起                           | 挂起这个任务                               |
| 11   | `LOS_ERRNO_TSK_NOT_CREATED`                 | 0x0200020a | 任务未被创建                           | 创建这个任务                               |
| 12   | `LOS_ERRNO_TSK_OPERATE_SWTMR`               | 0x02000222 | 不允许操作软件定时器任务               | 用户不要试图去操作软件定时器任务的设置     |
| 13   | `LOS_ERRNO_TSK_MSG_NONZERO`                 | 0x0200020c | 任务信息非零                           | 暂不使用该错误码                           |
| 14   | `LOS_ERRNO_TSK_DELAY_IN_INT`               | 0x0300020d | 中断期间，进行任务延时                 | 等待退出中断后再进行延时操作               |
| 15   | `LOS_ERRNO_TSK_DELAY_IN_LOCK`              | 0x0200020e | 任务被锁的状态下，进行延时             | 等待解锁任务之后再进行延时操作             |
| 16   | `LOS_ERRNO_TSK_YIELD_INVALID_TASK`         | 0x0200020f | 将被排入行程的任务是无效的             | 检查这个任务                               |
| 17   | `LOS_ERRNO_TSK_YIELD_NOT_ENOUGH_TASK`     | 0x02000210 | 没有或者仅有一个可用任务能进行行程安排 | 增加任务数                                 |
| 18   | `LOS_ERRNO_TSK_TCB_UNAVAILABLE`             | 0x02000211 | 没有空闲的任务控制块可用               | 增加任务控制块数量                         |
| 19   | `LOS_ERRNO_TSK_HOOK_NOT_MATCH`             | 0x02000212 | 任务的钩子函数不匹配                   | 暂不使用该错误码                           |
| 20   | `LOS_ERRNO_TSK_HOOK_IS_FULL`               | 0x02000213 | 任务的钩子函数数量超过界限             | 暂不使用该错误码                           |
| 21   | `LOS_ERRNO_TSK_OPERATE_IDLE`                | 0x02000214 | 这是个IDLE任务                         | 检查任务ID，不要试图操作IDLE任务           |
| 22   | `LOS_ERRNO_TSK_SUSPEND_LOCKED`              | 0x03000215 | 将被挂起的任务处于被锁状态             | 等待任务解锁后再尝试挂起任务               |
| 23   | `LOS_ERRNO_TSK_FREE_STACK_FAILED`          | 0x02000217 | 任务栈free失败                         | 该错误码暂不使用                           |
| 24   | `LOS_ERRNO_TSK_STKAREA_TOO_SMALL`          | 0x02000218 | 任务栈区域太小                         | 该错误码暂不使用                           |
| 25   | `LOS_ERRNO_TSK_ACTIVE_FAILED`               | 0x03000219 | 任务触发失败                           | 创建一个IDLE任务后执行任务转换             |
| 26   | `LOS_ERRNO_TSK_CONFIG_TOO_MANY`            | 0x0200021a | 过多的任务配置项                       | 该错误码暂不使用                           |
| 27   | `LOS_ERRNO_TSK_CP_SAVE_AREA_NOT_ALIGN`   | 0x0200021b | 暂无                                   | 该错误码暂不使用                           |
| 28   | `LOS_ERRNO_TSK_MSG_Q_TOO_MANY`            | 0x0200021d | 暂无                                   | 该错误码暂不使用                           |
| 29   | `LOS_ERRNO_TSK_CP_SAVE_AREA_NULL`         | 0x0200021e | 暂无                                   | 该错误码暂不使用                           |
| 30   | `LOS_ERRNO_TSK_SELF_DELETE_ERR`            | 0x0200021f | 暂无                                   | 该错误码暂不使用                           |
| 31   | `LOS_ERRNO_TSK_STKSZ_TOO_LARGE`            | 0x02000220 | 任务栈大小设置过大                     | 减小任务栈大小                             |
| 32   | `LOS_ERRNO_TSK_SUSPEND_SWTMR_NOT_ALLOWED` | 0x02000221 | 不允许挂起软件定时器任务               | 检查任务ID, 不要试图挂起软件定时器任务     |

**错误码定义：** 

错误码是一个32位的存储单元

- 31~24位表示错误等级
- 23~16位表示错误码标志
- 15~8位代表错误码所属模块
- 7~0位表示错误码序号，如下

```c
 #define LOS_ERRNO_OS_NORMAL(MID,ERRNO)
 (LOS_ERRTYPE_NORMAL | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | (ERRNO))
```

::: tip 说明
 - `LOS_ERRTYPE_NORMAL` ：Define the error level as critical  
 - `LOS_ERRNO_OS_ID` ：OS error code flag.  
 - `MID` ：OS_MOUDLE_ID  
 - `ERRNO` ：error ID number  

例如：   
```c
LOS_ERRNO_TSK_NO_MEMORY  LOS_ERRNO_OS_FATAL(LOS_MOD_TSK, 0x00)
```  
:::  

::: warning 注意
错误码序号 0x16、0x1c、0x0b，未被定义，不可用。
:::
