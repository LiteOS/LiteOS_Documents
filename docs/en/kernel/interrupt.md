# Interrupt

[[toc]]  

## Overview

### Basic Concept

When a condition that needs immediate attention occurs, the CPU suspends current activities and switches to deal with the condition.  

The CPU runs faster than external peripherals. When external peripherals are able to fulfill an activity alone, the CPU takes care of other activities.  

When the CPU must be involved in fulfilling an activity, the interrupt mechanism enables an external peripheral to emit an interrupt signal to alert the CPU of the high-priority condition requiring the interruption of current activities. The CPU does not need to keep waiting for peripheral states, thereby improving CPU efficiency and accelerating system response.  

The interrupt mechanism supports:  

- Initialize

- Create

- Lock or unlock

- Restore

- Enable

- Disable

::: tip Note
The interrupt mechanism of Huawei LiteOS is based on interrupt.
:::  

### Introduce of Interrupt

The following three types of hardware are involved in the interrupt mechanism:  

- **Device**:  the interrupt source. When a device requests the help of the CPU, it emits an interrupt signal to the interrupt controller.

- **Interrupt controller**:  a type of peripheral that sends an interrupt request to the CPU after receiving an interrupt signal from the interrupt pins of other peripherals. On the interrupt controller, you can prioritize, enable, or disable interrupt sources, as well as specify an interrupt trigger mode on each interrupt source. Common interrupt controllers include the Vector Interrupt Controller (VIC) and General Interrupt Controller (GIC, typically used in ARM Cortex-A7).

- **CPU**:  executes an interrupt handler at the request of an interrupt source.

### Terminology Associated With Interrupt

- **Interrupt ID**: a unique identifier contained in all interrupt requests from a particular interrupt source.

- **Interrupt request (IRQ)**: an electrical pulse signal sent to alert the CPU of an urgent condition. The CPU suspends current activities and deals with the condition that needs immediate attention.

- **Interrupt priority**: the priority of an interrupt source. Interrupt priority is determined based on importance and urgency. Huawei LiteOS supports the interrupt priority and interrupt nesting of the interrupt controller. Interrupt management does not limit the priority and nesting.

- **Interrupt handler**: When an external peripheral generates an interrupt request, the CPU executes an interrupt handler to switch from current activities to the event that needs immediate attention.

- **Interrupt trigger**: set to 1 when an interrupt source emits an interrupt signal.

- **Interrupt trigger type**: the way in which an interrupt signal is sent to the interrupt controller. Typically, an interrupt signal is either level-triggered or edge-triggered.

- **Interrupt vector**: starting address of interrupt service routine.

- **Interrupt vector table**: a table where interrupt vectors are stored based on interrupt ID.  

## Development Guidelines

### Usage Scenarios

When an interrupt request is generated, the CPU responds by suspending current activities and calling the user-defined interrupt handler to deal with the condition that needs immediate attention.  

### Functions

The interrupt module provides the following functions:  

| **API**        | **Description**                                                              |
|----------------|------------------------------------------------------------------------------|
| `LOS_HwiCreate`  | Creates a hardware interrupt to register the corresponding interrupt handler |
| `LOS_IntUnLock`  | Unlocks an interrupt                                                         |
| `LOS_IntRestore` | Restores an interrupt                                                        |
| `LOS_IntLock`    | Locks an interrupt                                                           |
| `LOS_HwiDelete`  | Deletes a hard interrupt                                                     |

### Development Process

1. Configure the following parameters:

  - `LOSCFG_PLATFORM_HWI`: a switch to enable or disable the hardware interrupt module. Set to YES.

  - `LOSCFG_PLATFORM_HWI_LIMIT`: the maximum allowed number of hardware interrupts.

2. Call the `LOS_HwiInit` API to initialize the interrupt mechanism.

3. Call the `LOS_HwiCreate` API to create an interrupt. Enabel the specified interrupt as required.

4. Call the `LOS_HwiDelete` API to delete an interrupt.  


## Precautions

- The register address of the `LosHwiInit` operation and the maximum allowed number of interrupts vary depending on hardware specifications.

- Avoid long-running interrupt handlers because they have negative impact on CPU's response to interrupts.

- The function leading to schedule cannot be performed after breading off.

- The input parameter of the `LOS_IntRestore()` API must be the PRIMASK that is saved by the `LOS_IntLock()` API before locking the interrupt.

- In Cortex-A7, interrupts 0–31 are for internal use and it is not advisable to request or create them.

## Programming Example

### Example Description

The programming example will cover the following functions:  

1. Initialize an hardware interrupt

2. Register an interrupt

3. Trigger an interrupt

4. View the output


**Prerequisite:**

- The `LOSCFG_PLATFORM_HWI` parameter in the los_config.h file is set to **YES**.

- The `OS_HWI_MAX_USED_NUM` parameter in the los_config.h file is set to the maximum number of hardware interrupts the operating system allows.

::: tip Note  
The current interrupt test code provides a basic framework. The interrupt hardware initialization code can be implemented by calling the `Example_Exti0_Init()` function based on the hardware of the development board.  
:::  

### Example Code

The code is as follows:

```c  
static void Example_Exti0_Init()
{
    /*add your IRQ init code here*/

    return;

}

static VOID User_IRQHandler(void)
{
    printf("\r\n User IRQ test\r\n");
    return;

}

UINT32 Example_Interrupt(VOID)
{
    UINTPTR uvIntSave;
    uvIntSave = LOS_IntLock();
    
    Example_Exti0_Init();
    
    LOS_HwiCreate(6, 0, 0, User_IRQHandler, 0); //create interrupt
    
    LOS_IntRestore(uvIntSave);
    
    return LOS_OK;
}  
```  

### Verification

![](./pic/interrupt-output.png)  
