# Event

[[toc]]

## Overview

### Basic Concept

Events are used for synchronization between tasks. A task or interrupt service routine can trigger an event (a synchronization signal) to another task through an event control block. One task is able to wait for several events to occur: whether while one event occurring or after several events occurred, both of these are sure to wake task up to do event handling.   

In a multi-task environment, tasks must be synchronized. In the one-to-many synchronization model, a task waits for multiple events. In the many-to-many synchronization model, multiple tasks wait for multiple events.  

Tasks trigger or wait for events through event control blocks. Events in Huawei LiteOS are used only for task synchronization, and not for data transport.  

Characteristics of events in Huawei LiteOS are as follows:  

- Events are not associated with tasks and are independent from each other. A 32-bit variable is used to indicate the type of the event in which a task is interested. Each bit indicates one event type with 0 indicating that the event does not occur and 1 indicating that the event occurs. There are 31 bits that indicate event types (bit 25 is reserved).  

- Events are used only for task synchronization, and not for data transport.

- Sending the same event type to a task for multiple times is equivalent to sending for only once.

- Multiple tasks are allowed to read or write the same event.

- Huawei LiteOS supports event reading and writing timeout.

#### Event control block

```c  
/**
 * @ingroup los_event
 * Event control structure
 */
typedef struct tagEvent
{
    UINT32 uwEventID;            /**bit that indicates an event type*/
    LOS_DL_LIST    stEventList;  /**linked list of  event reading tasks*/
} EVENT_CB_S, *PEVENT_CB_S;
```   

`uwEventID`: Indicates the type of the event in which a task is interested. Each bit indicates one event type with 0 indicating that the event does not occur and 1 indicating that the event occurs. There are 31 bits that indicate event types (bit 25 is reserved).  

#### Event reading mode

An event reading mode can be configured during event reading. Event reading modes are as follows:  

- `LOS_WAITMODE_AND`: Indicates that event of all event types specified by a mask need to be read. Event reading succeeds only when all events that are read occur.

- `LOS_WAITMODE_OR`: Indicates that an event of an event type specified by a mask needs to be read. Event reading succeeds when the event that is read occurs.

- `LOS_WAITMODE_CLR`: Indicates that after successful event reading, the event types or event type that is read is automatically cleared.

### Operation Mechanism

During event reading, one type or multiple types specified by uwEventMask are read. After event reading succeeds, the event type that is read is explicitly cleared if `LOS_WAITMODE_CLR` is configured in the event reading mode. The event type that is not cleared if `LOS_WAITMODE_CLR` is not configured. You can configure the event reading mode by passing in `LOS_WAITMODE_AND` to read all events of the event types specified by the event mask or by passing in `LOS_WAITMODE_OR` to read an event of an event type specified by the event mask.  

During event writing, a specified event type is written into an event. Multiple event types can be written concurrently. Event writing may trigger task scheduling.  

During event clearance, the bit that specifies the event type to be cleared is set to 0.  

Tasks woken up by events

![](./pic/event-wakeup.png)  

## Development Guidelines

### Usage Scenarios

Events are applicable in a variety of task synchronization scenarios and are partially similar to semaphore in purpose.  

### Functions

The event module provides the following functions:  

| Function Category       | API              | Description                                                                                                              |
|-------------------------|------------------|--------------------------------------------------------------------------------------------------------------------------|
| Event initialization    | LOS_EventInit    | Initializes an event control block                                                                                       |
| Event reading           | LOS_EventRead    | Reads an event within N ticks                                                                                            |
| Event writing           | LOS_EventWrite   | Writes an event                                                                                                          |
| Event clearance         | LOS_EventClear   | Clears an event                                                                                                          |
| Event mask verification | LOS_EventPoll    | Determines whether an event meets the expectations based on the passed-in event value, event mask, and verification mode |
| Event destroying        | LOS_EventDestroy | Destroys a specified event control block                                                                                 |

### Development Process

Typical process of using the event module:  

1. Call the `LOS_EventInit` API to initialize an event control block.  

2. Call the `LOS_EventInit` to write an event.  

3. Call the `LOS_EventRead` API to read an event.  

4. Call the `LOS_EventClear` API to clear an event.  


## Precautions

- Do not make calls to the `LOS_EventRead` and `LOS_EventWrite` APIs prior to the operating system being initialized. Otherwise, the operating system exhibits unexpected behavior.

- While an interrupt is underway, events can be written into an event control block but event reads are not allowed.

- Task blocking and event reading are not allowed while task scheduling is locked.

- The input parameter of `LOS_EventClear` is `~uwEvents` (reverse code of event type).

- Bit 25 of the event mask is merely used to distinguish whether the `LOS_EventRead` API returns an event or error code.  

## Programming Example

### Example Description

In the programming example, the `Example_TaskEntry` task is executed to create the `Example_Event` task. The `Example_Event` task is blocked from reading events. The `Example_TaskEntry` task writes an event in which the Example_Event task shows interest.

1. The `Example_TaskEntry` task is executed to create the `Example_Event` task. The `Example_Event` task takes a higher priority than the `Example_TaskEntry` task.

2. The `Example_Event` task is blocked from reading the event 0x00000001. After the `Example_Event` task is blocked, a task switch occurs to execute the task with a lower priority, namely, the `Example_TaskEntry` task.

3. The `Example_TaskEntry` task writes the event 0x00000001 toward the  `Example_Event` task. The `Example_Event` task is interested in the event 0x00000001 and is therefore woken up to process the event.

4. The `Example_Event` task is executed.

5. The `Example_TaskEntry` task is executed.

### Example Code

The order in which print-out is generated provides some clues into task switches that occur during event operations.  

The code is as follows:  

```c  
/* task pid */
static UINT32 g_TestTaskID;
/* event control struct */
static EVENT_CB_S  example_event;

/* wait event type */
#define event_wait 0x00000001

/* example task entry function */
VOID Example_Event(VOID)
{
    UINT32 uwEvent;
    UINT32 uwRet = LOS_OK;

    /*
     * timeout, WAITMODE to read event, timeout is 100 ticks,
     * if timeout, wake task directly
     */
    dprintf("Example_Event wait event 0x%x \n",event_wait);

    uwEvent = LOS_EventRead(&example_event, event_wait, LOS_WAITMODE_AND, 100);
    if(uwEvent == event_wait)
    {
        dprintf("Example_Event,read event :0x%x\n",uwEvent);
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_EVENT, LOS_INSPECT_STU_SUCCESS);
        if (LOS_OK != uwRet)
        {
            dprintf("Set Inspect Status Err\n");
        }
    }
    else
    {
        dprintf("Example_Event,read event timeout\n");
        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_EVENT, LOS_INSPECT_STU_ERROR);
        if (LOS_OK != uwRet)
        {
            dprintf("Set Inspect Status Err\n");
        }
    }
    return;
}

UINT32 Example_SndRcvEvent(VOID)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTask1;

    /* event init */
    uwRet = LOS_EventInit(&example_event);
    if(uwRet != LOS_OK)
    {
        dprintf("init event failed .\n");
        return LOS_NOK;
    }

    /* create task */
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_Event;
    stTask1.pcName       = "EventTsk1";
    stTask1.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    stTask1.usTaskPrio   = 5;
    uwRet = LOS_TaskCreate(&g_TestTaskID, &stTask1);
    if(uwRet != LOS_OK)
    {
        dprintf("task create failed .\n");
        return LOS_NOK;
    }

    /* write event */
    dprintf("Example_TaskEntry_Event write event .\n");
    uwRet = LOS_EventWrite(&example_event, event_wait);
    if(uwRet != LOS_OK)
    {
        dprintf("event write failed .\n");
        return LOS_NOK;
    }

    /* clear event flag */
    dprintf("EventMask:%d\n", example_event.uwEventID);
    LOS_EventClear(&example_event, ~example_event.uwEventID);
    dprintf("EventMask:%d\n", example_event.uwEventID);

    return LOS_OK;
}
```  

### Verification

The verification result is as follows:  

![](./pic/event-output.png)  

