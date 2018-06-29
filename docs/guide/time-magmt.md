# 时间管理

## 概述

### 基本概念

时间管理以系统时钟为基础。时间管理提供给应用程序所有和时间有关的服务。  

系统时钟是由定时/计数器产生的输出脉冲触发中断而产生的，一般定义为整数或长整数。输出脉冲的周期叫做一个“时钟滴答”。系统时钟也称为时标或者Tick。一个Tick的时长可以静态配置。  

用户是以秒、毫秒为单位计时，而芯片CPU的计时是以Tick为单位的，当用户需要对系统操作时，例如任务挂起、延时等，输入秒为单位的数值，此时需要时间管理模块对二者进行转换。  

Tick与秒之间的对应关系可以配置。  

Huawei LiteOS的时间管理模块提供时间转换、统计、延迟功能以满足用户对时间相关需求的实现。  

### 相关概念

- Cycle  

系统最小的计时单位。Cycle的时长由系统主频决定，系统主频就是每秒钟的Cycle数。  

- Tick  

Tick是操作系统的基本时间单位，对应的时长由系统主频及每秒Tick数决定，由用户配置。  

## 开发指导

### 使用场景

用户需要了解当前系统运行的时间以及Tick与秒、毫秒之间的转换关系等。  

### 功能

Huawei LiteOS系统中的时间管理主要提供以下两种功能：  

- 时间转换：根据主频实现CPU Tick数到毫秒、微秒的转换。  

- 时间统计：获取系统Tick数。  

| 功能分类 | 接口名               | 描述                |
|----------|----------------------|---------------------|
| 时间转换 | `LOS_MS2Tick`         | 毫秒转换成Tick      |
|          | `LOS_Tick2MS`         | Tick转化为毫秒      |
| 时间统计 | `LOS_CyclePerTickGet` | 每个Tick多少Cycle数 |
|          | `LOS_TickCountGet`    | 获取当前的Tick数    |

### 开发流程

时间管理的典型开发流程：

1.  确认配置项 `LOSCFG_BASE_CORE_TICK_HW_TIME` 为 `YES` 开启状态。

	在 `los_config.h` 中配置每秒的 Tick 数 `LOSCFG_BASE_CORE_TICK_PER_SECOND`；

2.  调用时钟转换接口。

3.  获取系统 `Tick` 数完成时间统计。

	通过 `LOS_TickCountGet` 获取全局 `g_ullTickCount`。

## 注意事项

- 获取系统Tick数需要在系统时钟使能之后。

- 时间管理不是单独的功能模块，依赖于 `los_config.h` 中的 `OS_SYS_CLOCK` 和 `LOSCFG_BASE_CORE_TICK_PER_SECOND` 两个配置选项。

- 系统的 Tick 数在关中断的情况下不进行计数，故系统 Tick 数不能作为准确时间计算。

## 编程实例

### 实例描述

在下面的例子中，介绍了时间管理的基本方法，包括：  

1.  时间转换：将毫秒数转换为Tick数，或将Tick数转换为毫秒数。  

2.  时间统计和时间延迟：统计每秒的Cycle数、Tick数和延迟后的Tick数。  

### 编程示例

前提条件：

- 配好LOSCFG\_BASE\_CORE\_TICK\_PER\_SECOND每秒的Tick数。

- 配好OS\_SYS\_CLOCK 系统时钟，单位: Hz。

时间转换：  
```c
VOID Example_TransformTime(VOID)
{  
    UINT32 uwMs;
    UINT32 uwTick;

    uwTick = LOS_MS2Tick(10000);//10000 ms数转换为Tick数
     printf("uwTick = %d \n",uwTick);
    uwMs= LOS_Tick2MS(100);//100 Tick数转换为ms数
        printf("uwMs = %d \n",uwMs);
}   
```  

时间统计和时间延迟：  
```c
UINT32 Example_GetTick(VOID)
{
    UINT32 uwRet = LOS_OK;
    UINT32 uwcyclePerTick;
    UINT64 uwTickCount1,uwTickCount2;

    uwcyclePerTick  = LOS_CyclePerTickGet();//每个Tick多少Cycle数
    if(0 != uwcyclePerTick)
    {
        dprintf("LOS_CyclePerTickGet = %d \n", uwcyclePerTick);
    }

    uwTickCount1 = LOS_TickCountGet();//获取Tick数
    if(0 != uwTickCount1)
    {
        dprintf("LOS_TickCountGet = %d \n", (UINT32)uwTickCount1);
    }
    LOS_TaskDelay(200);//延迟200 Tick
    uwTickCount2 = LOS_TickCountGet();
    if(0 != uwTickCount2)
    {
        dprintf("LOS_TickCountGet after delay = %d \n", (UINT32)uwTickCount2);
    }
        
    if((uwTickCount2 - uwTickCount1) >= 200)
    {
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_SYSTIC,LOS_INSPECT_STU_SUCCESS);
        if (LOS_OK != uwRet)  
        {
            dprintf("Set Inspect Status Err\n");
        }
        return LOS_OK;   
    }
    else
    {
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_SYSTIC,LOS_INSPECT_STU_ERROR);
        if (LOS_OK != uwRet)  
        {
            dprintf("Set Inspect Status Err\n");
        }
        return LOS_NOK; 
    }
}  
```

### 结果验证    
编译运行得到的结果为：
