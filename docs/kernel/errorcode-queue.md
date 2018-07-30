# 队列错误码

对队列存在失败可能性的操作，包括创建队列、删除队列等等，均需要返回对应的错误码，以便快速定位错误原因。  

| 序号 | 定义                                      | 实际数值   | 描述                                          | 参考解决方案                                                                                              |
|------|-------------------------------------------|------------|-----------------------------------------------|-----------------------------------------------------------------------------------------------------------|
| 1    | `LOS_ERRNO_QUEUE_MAXNUM_ZERO`           | 0x02000600 | 队列资源的最大数目配置为0                     | 配置要大于0的队列资源的最大数量。如果不使用队列模块，则将配置项设置为将队列资源的最大数量的剪裁设置为NO。 |
| 2    | `LOS_ERRNO_QUEUE_NO_MEMORY`             | 0x02000601 | 队列块内存无法初始化                          | 为队列块分配更大的内存分区，或减少队列资源的最大数量     |
| 3    | `LOS_ERRNO_QUEUE_CREATE_NO_MEMORY`     | 0x02000602 | 队列创建的内存未能被请求                      | 为队列分配更多的内存，或减少要创建的队列中的队列长度和节点的数目。                                        |
| 4    | `LOS_ERRNO_QUEUE_SIZE_TOO_BIG`         | 0x02000603 | 队列创建时消息长度超过上限                    | 更改创建队列中最大消息的大小至不超过上限                                                                  |
| 5    | `LOS_ERRNO_QUEUE_CB_UNAVAILABLE`        | 0x02000604 | 已超过创建的队列的数量的上限                  | 增加队列的配置资源数量                                                                                    |
| 6    | `LOS_ERRNO_QUEUE_NOT_FOUND`             | 0x02000605 | 无效的队列                                    | 确保队列ID是有效的                                                                                        |
| 7    | `LOS_ERRNO_QUEUE_PEND_IN_LOCK`         | 0x02000606 | 当任务被锁定时，禁止在队列中被阻塞            | 使用队列前解锁任务                                                                                        |
| 8    | `LOS_ERRNO_QUEUE_TIMEOUT`                | 0x02000607 | 等待处理队列的时间超时                        | 检查设置的超时时间是否合适                                                                                |
| 9    | `LOS_ERRNO_QUEUE_IN_TSKUSE`             | 0x02000608 | 阻塞任务的队列不能被删除                      | 使任务能够获得资源而不是在队列中被阻塞                                                                    |
| 10   | `LOS_ERRNO_QUEUE_WRITE_IN_INTERRUPT`   | 0x02000609 | 在中断处理程序中不能写队列                    | 将写队列设为非阻塞模式                                                                                    |
| 11   | `LOS_ERRNO_QUEUE_NOT_CREATE`            | 0x0200060a | 队列未创建                                    | 检查队列中传递的句柄是否有效                                                                              |
| 12   | `LOS_ERRNO_QUEUE_IN_TSKWRITE`           | 0x0200060b | 队列读写不同步                                | 同步队列的读写                                                                                            |
| 13   | `LOS_ERRNO_QUEUE_CREAT_PTR_NULL`       | 0x0200060c | 队列创建过程中传递的参数为空指针              | 确保传递的参数不为空指针                                                                                  |
| 14   | `LOS_ERRNO_QUEUE_PARA_ISZERO`           | 0x0200060d | 队列创建过程中传递的队列长度或消息节点大小为0 | 传入正确的队列长度和消息节点大小                                                                          |
| 15   | `LOS_ERRNO_QUEUE_INVALID`                | 0x0200060e | 读取队列、写入队列的handle无效                | 检查队列中传递的handle是否有效                                                                            |
| 16   | `LOS_ERRNO_QUEUE_READ_PTR_NULL`        | 0x0200060f | 队列读取过程中传递的指针为空                  | 检查指针中传递的是否为空                                                                                  |
| 17   | `LOS_ERRNO_QUEUE_READSIZE_ISZERO`       | 0x02000610 | 队列读取过程中传递的缓冲区大小为0             | 通过一个正确的缓冲区大小                                                                                  |
| 18   | `LOS_ERRNO_QUEUE_WRITE_PTR_NULL`       | 0x02000612 | 队列写入过程中传递的指针为空                  | 检查指针中传递的是否为空                                                                                  |
| 19   | `LOS_ERRNO_QUEUE_WRITESIZE_ISZERO`      | 0x02000613 | 队列写入过程中传递的缓冲区大小为0             | 通过一个正确的缓冲区大小                                                                                  |
| 20   | `LOS_ERRNO_QUEUE_WRITE_SIZE_TOO_BIG`  | 0x02000615 | 队列写入过程中传递的缓冲区大小比队列大小要大  | 减少缓冲区大小，或增大队列节点                                                                            |
| 21   | `LOS_ERRNO_QUEUE_ISFULL`                 | 0x02000616 | 在队列写入过程中没有可用的空闲节点            | 确保在队列写入之前，可以使用空闲的节点                                                                    |
| 22   | `LOS_ERRNO_QUEUE_PTR_NULL`              | 0x02000617 | 正在获取队列信息时传递的指针为空              | 检查指针中传递的是否为空                                                                                  |
| 23   | `LOS_ERRNO_QUEUE_READ_IN_INTERRUPT`    | 0x02000618 | 在中断处理程序中不能读队列                    | 将读队列设为非阻塞模式                                                                                    |
| 24   | `LOS_ERRNO_QUEUE_MAIL_HANDLE_INVALID`  | 0x02000619 | 正在释放队列的内存时传递的队列的handle无效    | 检查队列中传递的handle是否有效                                                                            |
| 25   | `LOS_ERRNO_QUEUE_MAIL_PTR_INVALID`     | 0x0200061a | 传入的消息内存池指针为空                      | 检查指针是否为空                                                                                          |
| 26   | `LOS_ERRNO_QUEUE_MAIL_FREE_ERROR`      | 0x0200061b | membox内存释放失败                            | 传入非空membox内存指针                                                                                    |
| 27   | `LOS_ERRNO_QUEUE_ISEMPTY`                | 0x0200061d | 队列已空                                      | 确保在读取队列时包含消息                                                                                  |
| 28   | `LOS_ERRNO_QUEUE_READ_SIZE_TOO_SMALL` | 0x0200061f | 读缓冲区大小小于队列大小                      | 增加缓冲区大小，或减小队列节点大小                                                                        |
