# 实验程序设计

## 实验目的

通过写简单的 Huawei LiteOS 应用代码，测试 Huawei LiteOS 的移植是否成功。如果大家对 LiteOS 不太了解，可以直接将本实验代码复制粘贴到自己的移植工程中。

如果需要全面验证，可以导入 example 全局验证代码。

## 实验设计

创建 3 个 Task（任务）和 1 个 Queue（队列）：

- task_start: 负责创建 task_send, task_recv 和 queue_t1
- task_send: 控制 LED0 每 500ms 闪烁一次，向 queue_t1 写入数值 100，表明数据发送
- task_recv: 读取 queue_t，发现读取数值为 100 时，控制 LED1 闪烁一次，表明接收到数据，同时在 UART1 打印 "hello_world"

## 实验工程

### 修改 main.c

```c

```

## 引入 example 工程

