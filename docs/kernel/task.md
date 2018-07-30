# 任务

[[toc]]

提供任务的创建、删除、延迟、挂起、恢复等功能，以及锁定和解锁任务调度。支持任务按优先级高低的抢占调度及同优先级时间片轮转调度。  

## 概述

### 基本概念

从系统的角度看，任务是竞争系统资源的最小运行单元。任务可以使用或等待CPU、使用内存空间等系统资源，并独立于其它任务运行。  

Huawei LiteOS 的任务模块可以给用户提供多个任务，实现了任务之间的切换和通信，帮助用户管理业务程序流程。这样用户可以将更多的精力投入到业务功能的实现中。 

Huawei LiteOS 是一个支持多任务的操作系统。在 Huawei LiteOS 中，一个任务表示一个线程。 

Huawei LiteOS 中的任务是抢占式调度机制，同时支持时间片轮转调度方式。  

高优先级的任务可打断低优先级任务，低优先级任务必须在高优先级任务阻塞或结束后才能得到调度。  

Huawei LiteOS 的任务一共有 32 个优先级 (0-31)，最高优先级为 0，最低优先级为 31。  

### 任务相关概念

#### 任务状态

Huawei LiteOS 系统中的每一任务都有多种运行状态。系统初始化完成后，创建的任务就可以在系统中竞争一定的资源，由内核进行调度。  

任务状态通常分为以下四种：  

- **就绪（Ready）**：该任务在就绪列表中，只等待 CPU。  
- **运行（Running）**：该任务正在执行。  
- **阻塞（Blocked）**：该任务不在就绪列表中。包含任务被挂起、任务被延时、任务正在等待信号量、读写队列或者等待读写事件等。  
- **退出态（Dead）**：该任务运行结束，等待系统回收资源。  

**任务状态示意图**

![](./pic/task-status.png)

#### 任务状态迁移说明  

- **就绪态 → 运行态：**  

任务创建后进入就绪态，发生任务切换时，就绪列表中最高优先级的任务被执行，从而进入运行态，但此刻该任务依旧在就绪列表中。  

- **运行态 → 阻塞态：**  

正在运行的任务发生阻塞（挂起、延时、获取互斥锁、读消息、读信号量等待等）时，该任务会从就绪列表中删除，任务状态由运行态变成阻塞态，然后发生任务切换，运行就绪列表中剩余最高优先级任务。  

- **阻塞态 → 就绪态（阻塞态 → 运行态）：** 

阻塞的任务被恢复后（任务恢复、延时时间超时、读信号量超时或读到信号量等），此时被恢复的任务会被加入就绪列表，从而由阻塞态变成就绪态；此时如果被恢复任务的优先级高于正在运行任务的优先级，则会发生任务切换，将该任务由就绪态变成运行态。  

- **就绪态 → 阻塞态：**  

任务也有可能在就绪态时被阻塞（挂起），此时任务状态会由就绪态转变为阻塞态，该任务从就绪列表中删除，不会参与任务调度，直到该任务被恢复。  

- **运行态 → 就绪态：**

有更高优先级任务创建或者恢复后，会发生任务调度，此刻就绪列表中最高优先级任务变为运行态，那么原先运行的任务由运行态变为就绪态，依然在就绪列表中。  

- **运行态 → 退出态:**  

运行中的任务运行结束，内核自动将此任务删除，任务状态由运行态变为退出态。  

- **阻塞态 → 退出态:**  

阻塞的任务调用删除接口，任务状态由阻塞态变为退出态。 

#### 任务 ID  

任务 ID，在任务创建时通过参数返回给用户，作为任务的一个非常重要的标识。用户可以通过任务ID对指定任务进行任务挂起、任务恢复、查询任务名等操作。  

#### 任务优先级  

优先级表示任务执行的优先顺序。任务的优先级决定了在发生任务切换时即将要执行的任务。在就绪列表中的最高优先级的任务将得到执行。  

#### 任务入口函数  

每个新任务得到调度后将执行的函数。该函数由用户实现，在任务创建时，通过任务创建结构体指定。  

#### 任务控制块 TCB  

每一个任务都含有一个任务控制块（TCB）。TCB 包含了任务上下文栈指针（stack pointer）、任务状态、任务优先级、任务ID、任务名、任务栈大小等信息。TCB 可以反映出每个任务运行情况。   

#### 任务栈

每一个任务都拥有一个独立的栈空间，我们称为任务栈。栈空间里保存的信息包含局部变量、寄存器、函数参数、函数返回地址等。任务在任务切换时会将切出任务的上下文信息保存在自身的任务栈空间里面，以便任务恢复时还原现场，从而在任务恢复后在切出点继续开始执行。  

#### 任务上下文  

任务在运行过程中使用到的一些资源，如寄存器等，我们称为**任务上下文**。当这个任务挂起时，其他任务继续执行，在任务恢复后，如果没有把任务上下文保存下来，有可能任务切换会修改寄存器中的值，从而导致未知错误。  

因此，Huawei LiteOS 在任务挂起的时候会将本任务的任务上下文信息，保存在自己的任务栈里面，以便任务恢复后，从栈空间中恢复挂起时的上下文信息，从而继续执行被挂起时被打断的代码。  

#### 任务切换  

任务切换包含获取就绪列表中最高优先级任务、切出任务上下文保存、切入任务上下文恢复等动作。   

### 运作机制

Huawei LiteOS 任务管理模块提供任务创建、任务删除、任务延时、任务挂起和任务恢复、更改任务优先级、锁任务调度和解锁任务调度、根据任务控制块查询任务 ID、根据 ID 查询任务控制块信息功能。  

在任务模块初始化时，系统会先申请任务控制块需要的内存空间，如果系统可用的内存空间小于其所需要的内存空间，任务模块就会初始化失败。如果任务初始化成功，则系统对任务控制块内容进行初始化。  

用户创建任务时，系统会将任务栈进行初始化，预置上下文。此外，系统还会将“任务入口函数”地址放在相应位置。这样在任务第一次启动进入运行态时，将会执行“任务入口函数”。  

## 开发指导

### 使用场景

任务创建后，内核可以执行锁任务调度，解锁任务调度，挂起，恢复，延时等操作，同时也可以设置任务优先级，获取任务优先级。任务结束的时候，则进行当前任务自删除操作。  

### 功能

Huawei LiteOS 系统中的任务管理模块为用户提供下面几种功能。

| 功能分类          | 接口名               | 描述                                          |  
|------------------|----------------------|-----------------------------------------------|  
| 任务的创建和删除  | `LOS_TaskCreateOnly`  | 创建任务，并使该任务进入suspend状态，并不调度 |  
|                  | `LOS_TaskCreate`      | 创建任务，并使该任务进入ready状态，并调度     |  
|                  | `LOS_TaskDelete`      | 删除指定的任务                                |  
| 任务状态控制      | `LOS_TaskResume`      | 恢复挂起的任务                                |  
|                  | `LOS_TaskSuspend`     | 挂起指定的任务                                |  
|                  | `LOS_TaskDelay`       | 任务延时等待                                  |  
|                  | `LOS_TaskYield`       | 显式放权，调整指定优先级的任务调度顺序        |  
| 任务调度的控制    | `LOS_TaskLock`        | 锁任务调度                                    |  
|                  | `LOS_TaskUnlock`      | 解锁任务调度                                  |  
| 任务优先级的控制  | `LOS_CurTaskPriSet`   | 设置当前任务的优先级                          | 
|                  | `LOS_TaskPriSet`      | 设置指定任务的优先级                          |  
|                  | `LOS_TaskPriGet`      | 获取指定任务的优先级                          |  
| 任务信息获取      | `LOS_CurTaskIDGet`    | 获取当前任务的ID                              |  
|                  | `LOS_TaskInfoGet`     | 获取指定任务的信息                            |  
|                  | `LOS_TaskStatusGet`   | 获取指定任务的状态                            |  
|                  | `LOS_TaskNameGet`     | 获取指定任务的名称                            |  
|                  | `LOS_TaskInfoMonitor` | 监控所有任务，获取所有任务的信息              |  
|                  | `LOS_NextTaskIDGet`   | 获取即将被调度的任务的ID                      |  

### 开发流程

以创建任务为例，讲解开发流程。  

1. 在los_config.h中配置任务模块。

	配置 `LOSCFG_BASE_CORE_TSK_LIMIT` 系统支持最大任务数，这个可以根据需求自己配置。  
	配置 `LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE` 空闲（IDLE）任务栈大小，这个默认即可。  
	配置 `LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE` 默认任务栈大小，用户根据自己的需求进行配置，在用户创建任务时，可以进行针对性设置。  
	配置 `LOSCFG_BASE_CORE_TIMESLICE` 时间片开关为 YES。  
	配置 `LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT` 时间片，根据实际情况自己配置。  
	配置 `LOSCFG_BASE_CORE_TSK_MONITOR` 任务监测模块裁剪开关，可选择是否打开。  
	
2. 锁任务 `LOS_TaskLock` ，锁住任务，防止高优先级任务调度。  

3. 创建任务 `LOS_TaskCreate` 。  

4. 解锁任务 `LOS_TaskUnlock` ，让任务按照优先级进行调度。  

5. 延时任务 `LOS_TaskDelay` ，任务延时等待。  

6. 挂起指定的任务 `LOS_TaskSuspend` ，任务挂起等待恢复操作。  

7. 恢复挂起的任务 `LOS_TaskResume` 。  

### Task 状态

Huawei LiteOS 任务的状态由内核自动维护，对用户不可见，不需要用户去操作。  

## 注意事项  

- 创建新任务时，会对之前自删除任务的任务控制块和任务栈进行回收，非自删除任务的控制块和栈在任务删除的时候已经回收。  
- 任务名是指针没有分配空间，在设置任务名时，禁止将局部变量的地址赋值给任务名指针。  
- 若指定的任务栈大小为 0，则使用配置项 `LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE` 指定默认的任务栈大小。  
- 任务栈的大小按 8 字节大小对齐。确定任务栈大小的原则是，够用就行：多了浪费，少了任务栈溢出。  
- 挂起任务的时候若为当前任务且已锁任务，则不能被挂起。  
- 空闲（IDLE）任务及软件定时器任务不能被挂起或者删除。  
- 在中断处理函数中或者在锁任务的情况下，执行 `LOS_TaskDelay` 操作会失败。
- 锁任务调度，并不关中断，因此任务仍可被中断打断。
- 锁任务调度必须和解锁任务调度配合使用。
- 设置任务优先级的时候可能会发生任务调度。
- 除去空闲（IDLE）任务以外，系统可配置的任务资源个数是指：整个系统的任务资源总个数，而非用户能使用的任务资源个数。例如：系统软件定时器多占用一个任务资源数，那么系统可配置的任务资源就会减少一个。
- 为了防止系统出现问题，系统禁止对软件定时器任务优先级进行修改。
- `LOS_CurTaskPriSet` 和 `LOS_TaskPriSet` 接口不能在中断中使用。
- `LOS_TaskPriGet` 接口传入的 Task ID 对应的任务未创建或者超过最大任务数，统一返回 0xffff。
- 在删除任务时要保证任务申请的资源（如互斥锁、信号量等）已被释放。

## 编程实例

### 实例描述

下面的示例介绍任务的基本操作方法，包含任务创建、任务延时、任务锁与解锁调度、挂起和恢复、查询当前任务PID、根据PID查询任务信息等操作，阐述任务优先级调度的机制以及各接口的应用。  

1.  创建了2个任务:TaskHi和TaskLo。
2.  TaskHi为高优先级任务。
3.  TaskLo为低优先级任务。

#### 编程示例  

```c
static UINT32 g_uwTskHiID;    
static UINT32 g_uwTskLoID;   

#define TSK_PRIOR_HI 4  
#define TSK_PRIOR_LO 5  

static UINT32 Example_TaskHi(VOID)
{
    UINT32 uwRet = LOS_OK;

    dprintf("Enter TaskHi Handler.\r\n");

    /*延时5个Tick，延时后该任务会挂起，执行剩余任务中高优先级的任务(g_uwTskLoID任务)*/
    uwRet = LOS_TaskDelay(5);
    if (uwRet != LOS_OK)
    {
        dprintf("Delay Task Failed.\r\n");
        return LOS_NOK;
    }

    /*2个tick时间到了后，该任务恢复，继续执行*/
    dprintf("TaskHi LOS_TaskDelay Done.\r\n");

    /*挂起自身任务*/
    uwRet = LOS_TaskSuspend(g_uwTskHiID);
    if (uwRet != LOS_OK)
    {
        dprintf("Suspend TaskHi Failed.\r\n");
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_TASK,LOS_INSPECT_STU_ERROR);
        if (LOS_OK != uwRet)
        {
            dprintf("Set Inspect Status Err\n");
        }
        return LOS_NOK;
    }
    
    dprintf("TaskHi LOS_TaskResume Success.\r\n");
        
    uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_TASK,LOS_INSPECT_STU_SUCCESS);
    if (LOS_OK != uwRet)
    {
        dprintf("Set Inspect Status Err\n");
    }
    
    /*删除任务*/
    if(LOS_OK != LOS_TaskDelete(g_uwTskHiID))
    {
        dprintf("TaskHi delete failed .\n");
        return LOS_NOK;
    }
    
    return LOS_OK;
}

/*低优先级任务入口函数*/
static UINT32 Example_TaskLo(VOID)
{
    UINT32 uwRet;

    dprintf("Enter TaskLo Handler.\r\n");

    /*延时10个Tick，延时后该任务会挂起，执行剩余任务中就高优先级的任务(背景任务)*/
    uwRet = LOS_TaskDelay(10);
    if (uwRet != LOS_OK)
    {
        dprintf("Delay TaskLo Failed.\r\n");
        return LOS_NOK;
    }

    dprintf("TaskHi LOS_TaskSuspend Success.\r\n");

    /*恢复被挂起的任务g_uwTskHiID*/
    uwRet = LOS_TaskResume(g_uwTskHiID);
    if (uwRet != LOS_OK)
    {
        dprintf("Resume TaskHi Failed.\r\n");
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_TASK,LOS_INSPECT_STU_ERROR);
        if (LOS_OK != uwRet)  
        {
            dprintf("Set Inspect Status Err\n");
        }
        return LOS_NOK;
    }
    
    /*删除任务*/
    if(LOS_OK != LOS_TaskDelete(g_uwTskLoID))
    {
        dprintf("TaskLo delete failed .\n");
        
        return LOS_NOK;
    }
    
    return LOS_OK;  
```  

#### 结果验证

编译运行得到的结果为：

![](./pic/task-output.png)
