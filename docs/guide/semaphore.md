# 信号量

## 概述

### 基本概念

信号量（Semaphore）是一种实现任务间通信的机制，实现任务之间同步或临界资源的互斥访问。常用于协助一组相互竞争的任务来访问临界资源。  

在多任务系统中，各任务之间需要同步或互斥实现临界资源的保护，信号量功能可以为用户提供这方面的支持。 

通常一个信号量的计数值用于对应有效的资源数，表示剩下的可被占用的互斥资源数。其值的含义分两种情况：   

- 0，表示没有积累下来的Post操作，且有可能有在此信号量上阻塞的任务。  

- 正值，表示有一个或多个Post下来的释放操作。  

以同步为目的的信号量和以互斥为目的的信号量在使用有如下不同：  

- 用作互斥时，信号量创建后记数是满的，在需要使用临界资源时，先取信号量，使其变空，这样其他任务需要使用临界资源时就会因为无法取到信号量而阻塞，从而保证了临界资源的安全。  

- 用作同步时，信号量在创建后被置为空，任务1取信号量而阻塞，任务2在某种条件发生后，释放信号量，于是任务1得以进入READY或RUNNING态，从而达到了两个任务间的同步。  

### 运作机制

**信号量控制块**  
```c
/**
 * @ingroup los_sem
 * Semaphore control structure.
 */
typedef struct
{
    UINT16           usSemStat;          /**是否使用标志位*/
    UINT16          uwSemCount;         /**信号量索引号*/
    UINT16          usMaxSemCount;      /**信号量最大数*/
    UINT16          usSemID;            /**信号量计数*/
    LOS_DL_LIST     stSemList;          /**挂接阻塞于该信号量的任务*/
}SEM_CB_S;  
```  

**信号量运作原理**

信号量初始化，为配置的N个信号量申请内存（N值可以由用户自行配置，受内存限制），并把所有的信号量初始化成未使用，并加入到未使用链表中供系统使用。  

信号量创建，从未使用的信号量链表中获取一个信号量资源，并设定初值。  

信号量申请，若其计数器值大于0，则直接减1返回成功。否则任务阻塞，等待其它任务释放该信号量，等待的超时时间可设定。当任务被一个信号量阻塞时，将该任务挂到信号量等待任务队列的队尾。  

信号量释放，若没有任务等待该信号量，则直接将计数器加1返回。否则唤醒该信号量等待任务队列上的第一个任务。  

信号量删除，将正在使用的信号量置为未使用信号量，并挂回到未使用链表。  

信号量允许多个任务在同一时刻访问同一资源，但会限制同一时刻访问此资源的最大任务数目。访问同一资源的任务数达到该资源的最大数量时，会阻塞其他试图获取该资源的任务，直到有任务释放该信号量。  

信号量运作示意图  

## 开发指导

### 使用场景

信号量是一种非常灵活的同步方式，可以运用在多种场合中，实现锁、同步、资源计数等功能，也能方便的用于任务与任务，中断与任务的同步中。  

### 功能

Huawei LiteOS 系统中的信号量模块为用户提供下面几种功能。

| 功能分类           | 接口名               | 描述             |
|--------------------|----------------------|------------------|
| 信号量的创建和删除 | LOS_SemCreate       | 创建信号量       |
|                    | LOS_BinarySemCreate | 创建二进制信号量 |
|                    | LOS_SemDelete       | 删除指定的信号量 |
| 信号量的申请和释放 | LOS_SemPend         | 申请指定的信号量 |
|                    | LOS_SemPost         | 释放指定的信号量 |

### 开发流程

信号量的开发典型流程：

1.  创建信号量 `LOS_SemCreate`。  

2.  申请信号量 `LOS_SemPend`。  

	信号量有三种申请模式：无阻塞模式、永久阻塞模式、定时阻塞模式

	- 无阻塞模式：任务需要申请信号量，若当前信号量的任务数没有到信号量设定的上限，则申请成功。否则，立即返回申请失败

	- 永久阻塞模式：任务需要申请信号量，若当前信号量的任务数没有到信号量设定的上限，则申请成功。否则，该任务进入阻塞态，系统切换到就绪任务中优先级最高者继续执行。任务进入阻塞态后，直到有其他任务释放该信号量，阻塞任务才会重新得以执行

	- 定时阻塞模式：任务需要申请信号量，若当前信号量的任务数没有到信号量设定的上限，则申请成功。否则，该任务进入阻塞态，系统切换到就绪任务中优先级最高者继续执行。任务进入阻塞态后，指定时间超时前有其他任务释放该信号量，或者用户指定时间超时后，阻塞任务才会重新得以执行

3.  释放信号量 `LOS_SemPost`。

	- 如果有任务阻塞于指定信号量，则唤醒该信号量阻塞队列上的第一个任务。该任务进入就绪态，并进行调度

	- 如果没有任务阻塞于指定信号量，释放信号量成功

4.  删除信号量 `LOS_SemDelete`。

### 信号量错误码

对可能导致信号量操作失败的情况，包括创建信号量、申请信号量、释放信号量、删除信号量等，均需要返回对应的错误码，以便快速定位错误原因。  

| 序号 | 定义                            | 实际数值   | 描述                         | 参考解决方案                             |
|------|---------------------------------|------------|------------------------------|------------------------------------------|
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
LOS_ERRTYPE_NORMAL ：Define the error level as critical
LOS_ERRNO_OS_ID ：OS error code flag.
MID：OS_MOUDLE_ID
ERRNO：error ID number  
```

例如：  
```  
LOS_ERRNO_SEM_NO_MEMORY        LOS_ERRNO_OS_ERROR(LOS_MOD_SEM, 0x00))  
```  

## 注意事项

- 由于中断不能被阻塞，因此在申请信号量时，阻塞模式不能在中断中使用。

## 编程实例

### 实例描述

本实例实现如下功能：  

1.  测试任务 `Example_TaskEntry` 创建一个信号量，锁任务调度，创建两个任务 `Example_SemTask1`、`Example_SemTask2`, `Example_SemTask2`优先级高于 `Example_SemTask1`，两个任务中申请同一信号量，解锁任务调度后两任务阻塞，测试任务 `Example_TaskEntry` 释放信号量。

2.  `Example_SemTask2` 得到信号量，被调度，然后任务休眠 20Tick，`Example_SemTask2` 延迟，`Example_SemTask1` 被唤醒。

3.  `Example_SemTask1` 定时阻塞模式申请信号量，等待时间为10Tick，因信号量仍被 `Example_SemTask2` 持有，`Example_SemTask1` 挂起，10Tick 后仍未得到信号量，`Example_SemTask1` 被唤醒，试图以永久阻塞模式申请信号量，`Example_SemTask1` 挂起。

4.  20Tick 后 `Example_SemTask2` 唤醒， 释放信号量后，`Example_SemTask1` 得到信号量被调度运行，最后释放信号量。

5.  `Example_SemTask1` 执行完，40Tick 后任务 `Example_TaskEntry` 被唤醒。

### 编程示例

前提条件：  

在 `los_config.h` 中，将 `LOSCFG_BASE_IPC_SEM` 配置为 YES。  

配置用户定义的 `LOSCFG_BASE_IPC_SEM_LIMIT` 最大的信号量数，如 1024。  

代码实现如下：  
```c 
/*测试任务优先级*/
#define TASK_PRIO_TEST  5

/*任务PID*/
static UINT32 g_TestTaskID01,g_TestTaskID02;
/*信号量结构体ID*/
static UINT32 g_usSemID;


static VOID Example_SemTask1(void)
{
    UINT32 uwRet;

    dprintf("Example_SemTask1 try get sem g_usSemID ,timeout 10 ticks.\n");
    /*定时阻塞模式申请信号量，定时时间为10Tick*/
    uwRet = LOS_SemPend(g_usSemID, 10);

    /*申请到信号量*/
    if(LOS_OK == uwRet)
    {
        LOS_SemPost(g_usSemID);
        return;
    }
    /*定时时间到，未申请到信号量*/
    if(LOS_ERRNO_SEM_TIMEOUT == uwRet)
    {
        dprintf("Example_SemTask1 timeout and try get sem g_usSemID wait forever.\n");
        /*永久阻塞模式申请信号量,获取不到时程序阻塞，不会返回*/
        uwRet = LOS_SemPend(g_usSemID, LOS_WAIT_FOREVER);
        if(LOS_OK == uwRet)
        {
            dprintf("Example_SemTask1 wait_forever and got sem g_usSemID success.\n");
            LOS_SemPost(g_usSemID);
            uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_SEM,LOS_INSPECT_STU_SUCCESS);
            if (LOS_OK != uwRet)  
            {
                dprintf("Set Inspect Status Err\n");
            }
            return;
        }
    }
    return;
}

static VOID Example_SemTask2(void)
{
    UINT32 uwRet;
    dprintf("Example_SemTask2 try get sem g_usSemID wait forever.\n");
    /*永久阻塞模式申请信号量*/
    uwRet = LOS_SemPend(g_usSemID, LOS_WAIT_FOREVER);

    if(LOS_OK == uwRet)
    {
        dprintf("Example_SemTask2 get sem g_usSemID and then delay 20ticks .\n");
    }

    /*任务休眠20 Tick*/
    LOS_TaskDelay(20);

    dprintf("Example_SemTask2 post sem g_usSemID .\n");
    /*释放信号量*/
    LOS_SemPost(g_usSemID);

    return;
}

UINT32 Example_Semphore(VOID)
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S stTask1;
    TSK_INIT_PARAM_S stTask2;

   /*创建信号量*/
    LOS_SemCreate(0,&g_usSemID);

    /*锁任务调度*/
    LOS_TaskLock();

    /*创建任务1*/
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SemTask1;
    stTask1.pcName       = "MutexTsk1";
    stTask1.uwStackSize  = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTask1.usTaskPrio   = TASK_PRIO_TEST;
    uwRet = LOS_TaskCreate(&g_TestTaskID01, &stTask1);
    if(uwRet != LOS_OK)
    {
        dprintf("task1 create failed .\n");
        return LOS_NOK;
    }

    /*创建任务2*/
    memset(&stTask2, 0, sizeof(TSK_INIT_PARAM_S));
    stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SemTask2;
    stTask2.pcName       = "MutexTsk2";
    stTask2.uwStackSize  = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTask2.usTaskPrio   = (TASK_PRIO_TEST - 1);
    uwRet = LOS_TaskCreate(&g_TestTaskID02, &stTask2);
    if(uwRet != LOS_OK)
    {
        dprintf("task2 create failed .\n");
        
        /*删除任务1*/
        if(LOS_OK != LOS_TaskDelete(g_TestTaskID01))
        {
            dprintf("task1 delete failed .\n");
        }
        
        return LOS_NOK;
    }

    /*解锁任务调度*/
    LOS_TaskUnlock();

    uwRet = LOS_SemPost(g_usSemID);

    /*任务休眠40 Tick*/
    LOS_TaskDelay(40);

    /*删除信号量*/
    LOS_SemDelete(g_usSemID);
    
    return uwRet;
}  
```  