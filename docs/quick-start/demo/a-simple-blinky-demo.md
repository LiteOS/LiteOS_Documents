# 一个简单的点灯演示工程（简单）

## 业务流程

**周期性采集或控制** 在物联网领域是一个十分常见的功能。通常的做法是利用 MCU 中内置的硬件定时器，周期产生定时器中断，并在中断处理函数中使能采集或控制条件。然后在后台进程中触发调用对应的采集任务。但这一方案有以下两个明显缺陷：

1. 如果周期采集任务种类较多，会浪费大量硬件定时器；
2. 当多个采集任务先后触发，后台程序只能根据触发条件执行，无法保证任务按顺序处理。

利用 LiteOS 软件定时器和消息队列读写机制，可以更为方便的解决上述问题。

### 资源使用

| 任务 | 消息队列 | 软件定时器 |
| ---- | -------- | ---------- |
| 1    | 1        | 2          |

### 简要说明

1. 根据资源使用情况，确认系统的配置文件，相应资源是否配备
2. 创建 2 个软件定时器，配置为周期模式，Timer1 每 1000ms 向队列中写入控灯1命令字 100，Timer2 每 500ms 向队列中写入控灯2命令字 200
3. 消息队列接收任务，从队列中读取命令字，检查是否为期望的数据并输出相关信息（控制灯进行闪烁）；如果没有数据，该任务会阻塞。

## 代码解读

[源码浏览](/blinky_demo.c)

### 1. 创建全局变量

```c
static UINT32 g_QueueId;           /* queue id */
static UINT32 g_RecvTaskId;        /* task id */
static UINT16 g_TimerLED1;         /* timer1 id */
static UINT16 g_TimerLED2;         /* timer2 id */
```

### 2. 函数入口 `UINT32 Blinky_Demo(void)`

```c
UINT32 Blinky_Demo(VOID) 
{
    UINT32 ulRet = LOS_OK;
    TSK_INIT_PARAM_S stTask = {0};

    /* Create the queue. */
    ulRet = LOS_QueueCreate(
              "MsgQueue",       /* queue name */
              QUEUE_LENGTH,     /* queue length */
              &g_QueueId,       /* queue id */
              0,                /* flags, not used */
              sizeof(UINT32)    /* message size */
              );
    if (ulRet != LOS_OK)
    {
      printf("create the queue error:%x\r\n", ulRet);
      return ulRet;
    }
    
    stTask.pfnTaskEntry = (TSK_ENTRY_FUNC)Task_RecvQ;
    stTask.uwStackSize = RECV_TASK_STACK_SIZE;
    stTask.pcName = "RecvTask";
    stTask.usTaskPrio = RECV_TASK_PRIOR;

    /* Create the task. */
    ulRet = LOS_TaskCreate(
              &g_RecvTaskId,    /* receive task id */
              &stTask           /* task params */
              );
    if (ulRet != LOS_OK)
    {
        printf("create task_rcv error:%x\r\n",ulRet);
        return ulRet;
    }

    /* Create the software timer1. */
    ulRet = Timers_Init(1000, g_TimerLED1, SND_CMD1_MSG);
    if (ulRet != LOS_OK)
    {
        printf("create timer error:%x\r\n", ulRet);
        return ulRet;
    }

    ulRet = Timers_Init(500, g_TimerLED2, SND_CMD2_MSG);
    if (ulRet != LOS_OK)
    {
        printf("create timer error:%x\r\n", ulRet);
        return ulRet;
    }

    return LOS_OK;
}
```
### 3. 定时器回调函数 `static UINT32 Timer_Init(UINT32 ulInterval, UINT16 ulTimerID, UINT32 ulSendMsg)`

```c
static VOID Timer_Callback(UINT32 ulArg)
{  
    UINT32 ulRet = LOS_OK;
    
    ulRet = LOS_QueueWriteCopy(
                g_QueueId,
                (VOID *)&ulArg,         /* send to msg */
                sizeof(UINT32),
                0
                ); 
    if(ulRet != LOS_OK)
    {
        printf("send message failure,error:%x\r\n",ulRet);
    }
    return;
}
```

### 4. 队列收消息任务处理函数 `static VOID Queue_RecvTask(UINT32 ulArg)`

```c
static VOID Task_RecvQ(UINT32 ulArg)
{
    UINT32 ulRecvMsg = 0;
    UINT32 ulRet = LOS_OK;
    UINT32 ulLength = 0;
    
    while (1) {
        ulLength = sizeof(ulRecvMsg);
        ulRet = LOS_QueueReadCopy(g_QueueId, &ulRecvMsg, &ulLength, LOS_WAIT_FOREVER);
        if (LOS_OK == ulRet)
        {
            switch(ulRecvMsg)
            {
                case SND_CMD1_MSG:
                    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                    break;
                case SND_CMD2_MSG:
                    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                    break;
                default:
                    break;
            }
          
            printf("RCVMSG:   OK:%d\r\n", ulRecvMsg); //valid message here
            ulRecvMsg = 0; //clear the received message here.
        }
    }
}
```

### 5. 定期器初始化函数 `static UINT32 Timer_Init(UINT32 ulInterval, UINT16 ulTimerID, UINT32 ulSendMsg)`

```c
static UINT32 Timers_Init(UINT32 ulInterval, UINT16 ulTimerID, UINT32 ulSendMsg)
{
    UINT32 ulRet = LOS_OK;

    ulRet = LOS_SwtmrCreate(
              ulInterval,             /* interval for ulInterval tick */
              LOS_SWTMR_MODE_PERIOD,  /* peridic software timer */ 
              Timer_Callback,         /* software timer callback function */
              &ulTimerID,             /* software timer id */
              ulSendMsg               /* sendmsg */
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
              , OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE
#endif
            );
    if (LOS_OK == ulRet)
    {
        ulRet = LOS_SwtmrStart(ulTimerID);
        if (LOS_OK != ulRet)
        {
            printf("start software timer error:%x\r\n",ulRet);
            return ulRet;
        }
    }

    return ulRet;
}
```

### 6. 最后在 `main.c` 中添加 demo 入口函数
```c
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
UINT32 Blinky_Demo(VOID);
/* USER CODE END PFP */
```
```c{7}
  /* USER CODE BEGIN 2 */  
  if (LOS_OK != LOS_KernelInit())
  {
      return LOS_NOK;
  }
  
  Blinky_Demo();

  LOS_Start();
  /* USER CODE END 2 */
```

## 代码验证

创建 `blinky_demo.c` 源文件，合入到编译工程

![](./pic/project_demo.png)

编译通过后，运行程序可以看到 LED1 和 LED2 间隔闪烁，同时在串口打印出如下信息。

![](./pic/blinky_demo.png)

