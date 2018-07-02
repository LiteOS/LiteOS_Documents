# 事件

## 概述

### 基本概念

事件是一种实现任务间通信的机制，可用于实现任务间的同步，但事件通信只能是事件类型的通信，无数据传输。一个任务可以等待多个事件的发生：可以是任意一个事件发生时唤醒任务进行事件处理；也可以是几个事件都发生后才唤醒任务进行事件处理。事件集合用32位无符号整型变量来表示，每一位代表一个事件。  

多任务环境下，任务之间往往需要同步操作，一个等待即是一个同步。事件可以提供一对多、多对多的同步操作。一对多同步模型：一个任务等待多个事件的触发；多对多同步模型：多个任务等待多个事件的触发。  

任务可以通过创建事件控制块来实现对事件的触发和等待操作。Huawei LiteOS的事件仅用于任务间的同步，不提供数据传输功能。  

Huawei LiteOS 提供的事件具有如下特点： 

- 事件不与任务相关联，事件相互独立，一个32位的变量，用于标识该任务发生的事件类型，其中每一位表示一种事件类型（0表示该事件类型未发生、1表示该事件类型已经发生），一共31种事件类型（第25位保留）。

- 事件仅用于任务间的同步，不提供数据传输功能。

- 多次向任务发送同一事件类型，等效于只发送一次。

- 允许多个任务对同一事件进行读写操作。

- 支持事件读写超时机制。

#### 事件控制块

```c
/**
 * @ingroup los_event
 * Event control structure
 */
typedef struct tagEvent
{
    UINT32 uwEventID;            /**标识发生的事件类型位*/
    LOS_DL_LIST    stEventList;  /**读取事件任务链表*/
} EVENT_CB_S, *PEVENT_CB_S;
```

`uwEventID`：用于标识该任务发生的事件类型，其中每一位表示一种事件类型（0表示该事件类型未发生、1表示该事件类型已经发生），一共31种事件类型，第25位系统保留。  

#### 事件读取模式

在读事件时，可以选择读取模式。读取模式如下：  

- 所有事件（`LOS_WAITMODE_AND`）：读取掩码中所有事件类型，只有读取的所有事件类型都发生了，才能读取成功。

- 任一事件（`LOS_WAITMODE_OR`）： 读取掩码中任一事件类型，读取的事件中任意一种事件类型发生了，就可以读取成功。

- 清除事件（`LOS_WAITMODE_CLR`）：LOS_WAITMODE_AND| LOS_WAITMODE_CLR或 LOS_WAITMODE_OR| LOS_WAITMODE_CLR 时表示读取成功后，对应事件类型位会自动清除。

### 运作机制

读事件时，可以根据入参事件掩码类型uwEventMask读取事件的单个或者多个事件类型。事件读取成功后，如果设置LOS_WAITMODE_CLR会清除已读取到的事件类型，反之不会清除已读到的事件类型，需显式清除。可以通过入参选择读取模式，读取事件掩码类型中所有事件还是读取事件掩码类型中任意事件。  

写事件时，对指定事件写入指定的事件类型，可以一次同时写多个事件类型。写事件会触发任务调度。  

清除事件时，根据入参事件和待清除的事件类型，对事件对应位进行清0操作。  

事件唤醒任务示意图

## 开发指导

### 使用场景

事件可应用于多种任务同步场合，能够一定程度替代信号量。  

### 功能

Huawei LiteOS系统中的事件模块为用户提供下面几个接口。

| 功能分类     | 接口名            | 描述                                                                     |
|--------------|-------------------|--------------------------------------------------------------------------|
| 事件初始化   | `LOS_EventInit`    | 初始化一个事件控制块                                                     |
| 读事件       | `LOS_EventRead`    | 读取指定事件类型，超时时间为相对时间：单位为Tick                         |
| 写事件       | `LOS_EventWrite`   | 写指定的事件类型                                                         |
| 清除事件     | `LOS_EventClear`   | 清除指定的事件类型                                                       |
| 校验事件掩码 | `LOS_EventPoll`    | 根据用户传入的事件值、事件掩码及校验模式，返回用户传入的事件是否符合预期 |
| 销毁事件     | `LOS_EventDestroy` | 销毁指定的事件控制块                                                     |

### 开发流程

使用事件模块的典型流程如下：  

1.  调用事件初始化 `LOS_EventInit` 接口，初始化事件等待队列。  

2.  写事件 `LOS_EventWrite`，配置事件掩码类型。  

3.  读事件 `LOS_EventRead`，可以选择读取模式。  

4.  清除事件 `LOS_EventClear`，清除指定的事件类型。

### Event错误码

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
LOS_ERRTYPE_ERROR：Define critical OS errors
LOS_ERRNO_OS_ID：OS error code flag
MID：OS_MOUDLE_ID
LOS_MOD_EVENT：Event module ID
ERRNO：error ID number  
```  

例如：  
```c 
#define LOS_ERRNO_EVENT_READ_IN_LOCK  LOS_ERRNO_OS_ERROR(LOS_MOD_EVENT, 0x05)  
```  

## 注意事项

- 在系统初始化之前不能调用读写事件接口。如果调用，则系统运行会不正常。

- 在中断中，可以对事件对象进行写操作，但不能读操作。

- 在锁任务调度状态下，禁止任务阻塞与读事件。

- `LOS_EventClear` 入参值是：要清除的指定事件类型的反码（~uwEvents）。

- 事件掩码的第25位不能使用，原因是为了区别 `LOS_EventRead` 接口返回的是事件还是错误码。

## 编程实例

### 实例描述

示例中，任务 `Example_TaskEntry` 创建一个任务 `Example_Event`，`Example_Event` 读事件阻塞，Example_TaskEntry向该任务写事件。 

1.  在任务 `Example_TaskEntry` 创建任务 `Example_Event`，其中任务 `Example_Event` 优先级高于 `Example_TaskEntry`。  

2.  在任务 `Example_Event` 中读事件 0x00000001，阻塞，发生任务切换，执行任务 `Example_TaskEntry`。  

3.  在任务 `Example_TaskEntry` 向任务 `Example_Event` 写事件 0x00000001，发生任务切换，执行任务 `Example_Event`。  

4.  `Example_Event` 得以执行，直到任务结束。  

5.  `Example_TaskEntry` 得以执行，直到任务结束。

### 编程示例

可以通过打印的先后顺序理解事件操作时伴随的任务切换。  

代码实现如下：  
```c
/*任务PID*/
static UINT32 g_TestTaskID;

/*事件控制结构体*/
static EVENT_CB_S  example_event;

/*等待的事件类型*/
#define event_wait 0x00000001

/*用例任务入口函数*/
VOID Example_Event(VOID)
{
    UINT32 uwEvent;
    UINT32 uwRet = LOS_OK;

    /*超时 等待方式读事件,超时时间为100 Tick
    若100 Tick 后未读取到指定事件，读事件超时，任务直接唤醒*/
    printf("Example_Event wait event 0x%x \r\n",event_wait);

    uwEvent = LOS_EventRead(&example_event, event_wait, LOS_WAITMODE_AND, 100);
    if(uwEvent == event_wait)
    {
        printf("Example_Event,read event :0x%x\r\n",uwEvent);
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_EVENT,LOS_INSPECT_STU_SUCCESS);
        if (LOS_OK != uwRet)  
        {
            printf("Set Inspect Status Err\r\n");
        }
    }
    else
    {
        printf("Example_Event,read event timeout\r\n");
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_EVENT,LOS_INSPECT_STU_ERROR);
        if (LOS_OK != uwRet)  
        {
            printf("Set Inspect Status Err\r\n");
        }
    }
    return;
}

UINT32 Example_SndRcvEvent(VOID)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTask1;

    /*事件初始化*/
    uwRet = LOS_EventInit(&example_event);
    if(uwRet != LOS_OK)
    {
        printf("init event failed .\r\n");
        return LOS_NOK;
    }

    /*创建任务*/
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_Event;
    stTask1.pcName       = "EventTsk1";
    stTask1.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask1.usTaskPrio   = 5;
    uwRet = LOS_TaskCreate(&g_TestTaskID, &stTask1);
    if(uwRet != LOS_OK)
    {
        printf("task create failed .\r\n");
        return LOS_NOK;
    }

    /*写用例任务等待的事件类型*/
    printf("Example_TaskEntry_Event write event .\r\n");

    uwRet = LOS_EventWrite(&example_event, event_wait);
    if(uwRet != LOS_OK)
    {
        printf("event write failed .\r\n");
        return LOS_NOK;
    }

    /*清标志位*/
    printf("EventMask:%d\r\n",example_event.uwEventID);
    LOS_EventClear(&example_event, ~example_event.uwEventID);
    printf("EventMask:%d\r\n",example_event.uwEventID);

    return LOS_OK;
}  
```

### 结果验证

编译运行得到的结果为：  