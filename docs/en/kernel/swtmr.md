# Software Timers

[[toc]]  

## Overview

### Basic Concept

A software timer is a timer simulated by software, and works based on system tick interrupts. When a predefined number of ticks elapse, a software timer triggers a user-defined callback function. The timer length is an integral number of ticks.  

Only a limited number of hardware timers can be used due to hardware constraints. Software timers can fulfill the demand for more timers, allowing you to create more timing services.  

The software timer module supports the following functions:  

- Statically disable a software timer by macro

- Create a software timer

- Start a software timer

- Stop a software timer

- Delete a software timer

- Measure the number of ticks that must elapse prior to expiry of a software timer

### Operation Mechanism

Software timers are system resources and are allocated continuous memory at the initialization of the timer module. The maximum number of software timers supported by the operating system is defined by `LOSCFG_BASE_CORE_SWTMR_LIMIT` in the `los_config.h` file.  

Software timers are placed in a queue and a triggered in the first in first out order. The software timers with a short life cycle are placed at the beginning of queue so that they will be triggered earlier than those with a longer life cycle. 

The software timer length is measured in ticks. When a software timer is actuated, Huawei LiteOS determines the timer expiry time based on the current system time (in ticks) and timer length (in ticks) and adds the timer control structure to the global timing list.  

When a tick interrupt occurs, the tick interrupt handler scans the global timing list for expired timers. If such a timer is found, the timer is recorded.  

After the tick interrupt handler finishes processing, the software timer task (a task exclusively used for software timers) is assigned the highest priority and then woken up to call the Timer_Callback function (callback function that handles software timer expiry) of the expired timer.  

**Software Timer States**

- `OS_SWTMR_STATUS_UNUSED` 

While the timer module is being initialized, the operating system initializes all timer resources in the system to `OS_SWTMR_STATUS_UNUSED` state.  

- `OS_SWTMR_STATUS_CREATED`

If the `LOS_SwtmrCreate` API is called in `OS_SWTMR_STATUS_UNUSED` state or if the `LOS_SwtmrStop` API is called after timer start-up, the timer switches to `OS_SWTMR_STATUS_CREATED` state.  

- `OS_SWTMR_STATUS_TICKING`

If the `LOS_SwtmrStart` API is called after the timer is created, the timer switches to `OS_SWTMR_STATUS_TICKING` state.  

**Software Timer Modes**

Depending on timer mode, software timers are classified into two types:  

- One-shot timer: The timer triggers the timer event only once after it is started. Then, the timer is automatically deleted.  

- Periodic timer: The timer triggers the timer event periodically until the timer is manually stopped.  

## Development Guidelines

### Usage Scenarios

- If you want to trigger a timer event only once, create a one-shot timer and define a Timer_Callback function for the timer. When the timer expires, the Timer_Callback function will be executed.

- If you want to trigger a timer event periodically, create a periodic timer and define a Timer_Callback function for the timer. When the timer expires, the Timer_Callback function will be executed.

### Functions

The software timer module provides the following functions. For details about the APIs, see the API reference.  

| Function Category                                    | API                | Description                                                                       |
|------------------------------------------------------|--------------------|-----------------------------------------------------------------------------------|
| Timer creation and deletion                          | `LOS_SwtmrCreate`  | Creates a software timer                                                          |
|                                                      | `LOS_SwtmrDelete`  | Deletes a software timer                                                          |
| Timer start and stop                                 | `LOS_SwtmrStart`   | Starts a software timer                                                           |
|                                                      | `LOS_SwtmrStop`    | Stops a software timer                                                            |
| Measurement of remaining ticks prior to timer expiry | `LOS_SwtmrTimeGet` | Measures the number of ticks that must elapse prior to expiry of a software timer |

### Development Process

Software timer typical development process:  

1. Set software timer.

 - Set `LOSCFG_BASE_CORE_SWTMR` and `LOSCFG_BASE_IPC_QUEUE` to YES.

 - Set `LOSCFG_BASE_CORE_SWTMR_LIMIT` to the maximum number of software timers supported by the operating system.

 - Set `OS_SWTMR_HANDLE_QUEUE_SIZE` to the maximum size of the software timer queue.

2. Call the `LOS_SwtmrCreate` API to create a software timer.

 - Creates a software timer that has a user-defined timer length, Timer_Callback function, and trigger mode; returns the software timer handler after successful creation.

 - Returns the function execution result (successful or failed).

3. Call the `LOS_SwtmrStart` API to start a software timer.

4. Call the `LOS_SwtmrTimeGet` API to get left number of Ticks of software timer.

5. Call the `LOS_SwtmrStop` API to stop a software timer.

6. Call the `LOS_SwtmrDelete` API to delete a software timer.


## Precautions

- Limit the number of operations contained in the callback function of a software timer. Do not use the API or perform any operation that may suspend or block tasks.

- Software timers are placed in a queue. A task is used exclusively to convey software timer information. The priority of a task in a software timer is set to 0, which is not allowed to be modified.

- The maximum number of software timer resources is not equal to the total number of software timer resources available to users. When a software timer occupies a software timer resource, the number of available software timer resources is decreased by 1.

- After the callback function of a one-shot software timer is executed, the software timer is automatically deleted and the resources allocated to the timer are reclaimed.

## Programming Example

### Example Description

In the programming example, the following steps will be performed:  

1. Create, delete, start, stop or restart a software timer.

2. Use a one-shot software timer and a periodical software timer.

### Example Code

Prerequisites  

- The `LOSCFG_BASE_CORE_SWTMR` parameter in the `los_config.h` file is set to YES.

- The `LOSCFG_BASE_CORE_SWTMR_LIMIT` parameter in the `los_config.h` file is set to the maximum number of software timers supported by the operating system.

- The `OS_SWTMR_HANDLE_QUEUE_SIZE` parameter in the `los_config.h` file is set to the maximum size of the software timer queue.

The code is as follows:  

```c  
static VOID Timer1_Callback(UINT32 arg)
{
    UINT32 tick_last1;

    g_timercount1++;
    tick_last1 = (UINT32)LOS_TickCountGet();
    dprintf("g_timercount1=%d\n", g_timercount1);
    dprintf("tick_last1=%d\n", tick_last1);
}

static VOID Timer2_Callback(UINT32 arg)
{
    UINT32 uwRet = LOS_OK;
    UINT32 tick_last2;

    tick_last2 = (UINT32)LOS_TickCountGet();
    g_timercount2++;
    dprintf("g_timercount2=%d\n", g_timercount2);
    dprintf("tick_last2=%d\n", tick_last2);
    uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_TIMER, LOS_INSPECT_STU_SUCCESS);
    if (LOS_OK != uwRet)
    {
        dprintf("Set Inspect Status Err\n");
    }
}

UINT32 Example_swTimer(VOID)
{
    UINT16 id1;
    UINT16 id2;// timer id
    UINT32 uwRet = LOS_OK;

#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
    uwRet = LOS_SwtmrCreate(1000, LOS_SWTMR_MODE_ONCE, Timer1_Callback, &id1, 1, OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE);
#else
    uwRet = LOS_SwtmrCreate(1000, LOS_SWTMR_MODE_ONCE, Timer1_Callback, &id1, 1);
#endif
    if (LOS_OK != uwRet)
    {
        dprintf("create Timer1 failed\n");
    }
    else
    {
        dprintf("create Timer1 success\n");
    }

#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
    uwRet = LOS_SwtmrCreate(100, LOS_SWTMR_MODE_PERIOD, Timer2_Callback, &id2, 1, OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE);
#else
    uwRet = LOS_SwtmrCreate(100, LOS_SWTMR_MODE_PERIOD, Timer2_Callback, &id2, 1);
#endif
    if(LOS_OK != uwRet)
    {
        dprintf("create Timer2 failed\n");
    }
    else
    {
        dprintf("create Timer2 success\n");
    }

    uwRet = LOS_SwtmrStart(id1);
    if (LOS_OK != uwRet)
    {
        dprintf("start Timer1 failed\n");
    }
    else
    {
        dprintf("start Timer1 sucess\n");
    }

    (VOID)LOS_TaskDelay(200);

    uwRet = LOS_SwtmrStop(id1);
    if (LOS_OK != uwRet)
    {
        dprintf("stop Timer1 failed\n");
    }
    else
    {
        dprintf("stop Timer1 sucess\n");
    }

    uwRet = LOS_SwtmrStart(id1);
    if (LOS_OK != uwRet)
    {
        dprintf("start Timer1 failed\n");
    }

    (VOID)LOS_TaskDelay(1000);

    /*the timer that mode is once, kernel will delete it automatically when timer is timeout */
    uwRet = LOS_SwtmrDelete(id1);
    if (LOS_OK != uwRet)
    {
        dprintf("delete Timer1 failed\n");
    }
    else
    {
        dprintf("delete Timer1 sucess\n");
    }

    uwRet = LOS_SwtmrStart(id2);
    if (LOS_OK != uwRet)
    {
        dprintf("start Timer2 failed\n");
    }
    else
    {
        dprintf("start Timer2 success\n");
    }

    (VOID)LOS_TaskDelay(1000);

    uwRet = LOS_SwtmrStop(id2);
    if (LOS_OK != uwRet)
    {
        dprintf("stop Timer2 failed\n");
    }

    uwRet = LOS_SwtmrDelete(id2);
    if (LOS_OK != uwRet)
    {
        dprintf("delete Timer2 failed\n");
    }

    return LOS_OK;
}  
```  

### Verification

The verification result is as follows:  

![](./pic/swtmr-output.png)  
