# Time Management

[[toc]]  

## Overview

### Basic Concept

Time management provides time services to applications and uses system time as the reference time.  

System time is generated when an output pulse of a timer/counter triggers an interrupt, it is an integral number or long integral number of ticks. The interval between consecutive output pulses is a tick. The tick length is statically configured.  

User time is measured in seconds or milliseconds, whereas CPU time is measured in ticks. When a user initiates an operation to the operating system, such as suspending or delaying a task, the time management module converts user time between seconds/milliseconds and ticks.  

The rule for conversion between ticks and seconds is user configurable.  

The time management module of Huawei LiteOS provides time conversion, measurement, and deferral to satisfy what users need.  

### Related Concepts

- Cycle

Cycle is the minimal time unit of the operating system. The system clock speed is represented in the form of cycles per second.  

- Tick

A tick is the basic time unit used in OS. The tick length is user configurable. Typically, it is determined by the system clock speed and represented in the form of ticks per second.  

## Development Guidelines

### Usage Scenarios

Read the topic when you want to learn more about system time and conversion between ticks and seconds/milliseconds.  

### Functions

The time management module of Huawei LiteOS provides the following functions:  

- Time conversion: converts the CPU runtime from ticks to milliseconds or microseconds

- Time measurement: measures the system runtime in ticks

| Function Category | API                   | Description                                 |
|-------------------|-----------------------|---------------------------------------------|
| Time conversion   | `LOS_MS2Tick`         | Converts milliseconds into ticks (reserved) |
|                   | `LOS_Tick2MS`         | Converts ticks into milliseconds (reserved) |
| Time measurement  | `LOS_CyclePerTickGet` | Counts the number of cycles per tick        |
|                   | `LOS_TickCountGet`    | Measures the runtime in ticks               |

### Development Process

Time management typical development process:  

1. Set the `LOSCFG_BASE_CORE_TICK_HW_TIME` parameter in the `los_config.h` file to YES.

   Set the `LOSCFG_BASE_CORE_TICK_PER_SECOND` parameter in the `los_config.h` file to a valid number of ticks per second.

2. Call the clock conversion API.

3. Gets the system runtime that is measured in ticks

   Calls the `LOS_TickCountGet` API to get the global `g_ullTickCount`.

## Precautions

- The system runtime (measured in ticks) can be acquired only after the system clock is enabled.

- The time management module works only after the `OS_SYS_CLOCK` in `los_config.h` is enabled and the `LOSCFG_BASE_CORE_TICK_PER_SECOND` of the Tick module is specified.

- When measured in ticks, system runtime is not accurate, because it is not measured while interrupts are disabled.

## Programming Example

### Example Description

The programming example will cover the following functions:  

1. Time conversion: from milliseconds to ticks, or conversely

2. Time measurement and deferral: measures the number of cycles per second, the number of ticks for which the system is running, and the number of ticks for which the deferral lasts

### Example Code

Prerequisites  

- The `LOSCFG_BASE_CORE_TICK_PER_SECOND` in the `los_config.h` file is set to a valid number of ticks per second.

- The `OS_SYS_CLOCK` (unit: Hz) is set.

Time conversion:  

```c
VOID Example_TransformTime(VOID) 
 { 
     UINT32 uwMs; 
     UINT32 uwTick; 

     uwTick = LOS_MS2Tick(10000);//Convert 10000 ms into ticks 
      printf("uwTick = %d \n",uwTick); 
     uwMs= LOS_Tick2MS(100);//Convert 100 ticks into ms 
         printf("uwMs = %d \n",uwMs); 
 }
```

Time measurement and deferral:    
```c  
UINT32 Example_GetTick(VOID)
{
    UINT32 uwRet = LOS_OK;
    UINT32 uwcyclePerTick;
    UINT64 uwTickCount1,uwTickCount2;

    uwcyclePerTick  = LOS_CyclePerTickGet();
    if (0 != uwcyclePerTick)
    {
        dprintf("LOS_CyclePerTickGet = %d \n", uwcyclePerTick);
    }

    uwTickCount1 = LOS_TickCountGet();
    if (0 != uwTickCount1)
    {
        dprintf("LOS_TickCountGet = %d \n", (UINT32)uwTickCount1);
    }
    LOS_TaskDelay(200);
    uwTickCount2 = LOS_TickCountGet();
    if (0 != uwTickCount2)
    {
        dprintf("LOS_TickCountGet after delay = %d \n", (UINT32)uwTickCount2);
    }

    if ((uwTickCount2 - uwTickCount1) >= 200)
    {
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_SYSTIC, LOS_INSPECT_STU_SUCCESS);
        if (LOS_OK != uwRet)
        {
            dprintf("Set Inspect Status Err\n");
        }
        return LOS_OK;
    }
    else
    {
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_SYSTIC, LOS_INSPECT_STU_ERROR);
        if (LOS_OK != uwRet)
        {
            dprintf("Set Inspect Status Err\n");
        }
        return LOS_NOK;
    }
}
```  


### Verification

The verification result is as follows:  

![](./pic/timemgr-output.png)  
