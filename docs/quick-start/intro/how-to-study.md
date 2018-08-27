# 如何学习并使用 Huawei LiteOS

## 提前了解
- Huawei LiteOS 目前已经支持 ARM Cortex-M0、M3、M4、M7；Cortex-A7、A17、A53，ARM 9、11，X86，DSP，RISC-V 等多个芯片架构，请根据需要选择合适的芯片架构；
- Huawei LiteOS 目前已经适配了 50+ 包括 ST、NXP、MIDMOTION、SILICON、ATMEL、GD 等主流厂商的开发板，`kernel` 的 CPU 文件夹目录下有目前开源项目支持的CPU 类型，对于已完成移植的开发板，可以直接使用，不需要重新移植；
- 开发前，请选择合适的 IDE（目前支持的IDE主要有Keil、IAR、GCC），今年还将会推出自研 IDE 工具；
- Huawei LiteOS 是轻量级操作系统，推荐硬件资源为 RAM 8KB 以上（ LiteOS内核最小占用约 6KB ），Flash 32K 以上；
- 支持 CMSIS 标准接口；
- 获取 [Huawei LiteOS源码](https://github.com/LITEOS/LiteOS)；

## 学习推荐路径

### 面向物联网开发小白

> 对嵌入式应用开发一无所知，零基础的开发者

可以先体验[快速上手（简单）](/quick-start/demo/a-simple-blinky-demo.md)，然后依次完成[LiteOS 内核详解]()，[LiteOS SDK详解]()和[LiteOS 移植手册]()的学习，最后选择适合自己的[教程]()进阶。

### 面向嵌入式开发专家

> 已经掌握一种或多种 RTOS，具备独立开发能力

由于您已经熟悉 RTOS，可以花几分钟先了解[快速上手（复杂）]()，然后直接到[LiteOS 内核实战]()掌握 LiteOS 内核关键知识，然后通过学习[开发板场景化案例实战]()掌握 LiteOS SDK 使用。