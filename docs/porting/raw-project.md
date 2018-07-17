# 准备“裸机”工程

要想学好 RTOS，首先需要在自己的实验平台，即开发板上进行移植，移植通常分为 **操作系统移植** 和 **驱动程序移植**，这里我们只介绍 Huawei LiteOS 内核移植。由于驱动程序移植需要依赖具体的外设，大家可以在[场景教程](../tutorials/temp-hum.md)中进行学习。

::: tip 提示
Huawei LiteOS SDK 组件部分移植也会在相关的场景教学中进行介绍，
:::

## 硬件准备

### 开发板（支持 ARM Cortex-M 内核的各类开发板）

Huawei LiteOS 目前已经适配了数十款基于 ARM Cortex 内核的开发板，包括市面上常见的 STM32F0、STM32F1、STM32F3、STM32F4、STM32F7 全系列，NXP i.MAX RT10XX 系列等多种芯片。开发者可以根据自己手头上的开发板参考对应的移植指导，完成 Huawei LiteOS 内核移植工作。

在进行 OS 移植之前，需要先准备好以下工具，查阅开发板手册，了解板上的主控 MCU 规格以及外设等。

### 下载器/仿真器

嵌入式程序下载有多种方法：USB、串口、JTAG、SWD 等。

- **USB/串口 下载**

[pic]

::: tip USB转串口驱动
下载路径：

安装说明：
:::

- **SWD/JTAG 下载**

[pic]

::: tip ST-Link/J-Link 驱动
下载路径：

安装说明：
:::

## 软件准备

### STM32 IDE 工具介绍

[pic]

1. MDK-ARM

介绍

::: tip 提示
软件支持包下载
:::

2. IAR for ARM

介绍

3. GCC (SW4STM32、makefile)

介绍

::: tip 提示
软件支持包下载
:::

## LiteOS 源码获取

LiteOS 开源代码路径：https://github.com/LiteOS/LiteOS

从 developer 分支获取最新源代码

### 移植相关目录树说明

```c
├─ arch                     // ddd
│  └─ arm
│     ├─ arm-m
│     └─ common
├─ components
│  └─ cmsis
├─ examples
│  ├─ api
│  └─ include
├─ kernel
│  ├─ base
│  ├─ extended
│  └─ include
└─ targets
   └─ Standard_STM32F103RB_NUCLEO
```



