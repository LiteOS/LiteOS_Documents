# LiteOS代码结构导读

## LiteOS代码入口

LiteOS入口在工程对应的main.c中，基本流程如下：

```c  
int main(void)

{

    UINT32 uwRet = LOS_OK;

    HardWare_Init();            //硬件初始化

    uwRet = LOS_KernelInit();   //初始化LiteOS内核
    
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    LOS_Inspect_Entry();        //初始化内核例程

    LOS_Start();                //进行task调度，LiteOS开始正常工作
}  
```  

## LiteOS的代码目录结构说明  

关于代码树中各个目录存放的源代码的相关内容简介如下：  

```  
.
├─ arch
│  ├─ arm
│     ├─ arm-m      ------  M核中断、调度、tick相关代码
│     ├─ common     ------  arm核公用的cmsis core接口
├─ components
│  ├─ cmsis         ------  LiteOS提供的cmsis os接口实现
│  ├─ connectivity
│     ├─ agent_tiny ------  agent_tiny端云互通组件，包括公共头文件、示例代码、客户端实现代码、操作系统适配层代码
│     ├─ lwm2m      ------  lwm2m协议实现
│  ├─ net
│     ├─ lwip_port  ------  lwip驱动及OS适配代码
│     ├─ lwip-2.0.3 ------  lwip协议实现
│  ├─ security
│     ├─ mbedtls/mbedtls_port  ------ MBEDTLS的OS适配代码
│     ├─ mbedtls/mbedtl-2.6.0  ------ MBEDTLS协议实现
├─ doc              ------    此目录存放的是LiteOS的使用文档和API说明等文档
├─ examples
│  ├─ API           ------    供开发者测试LiteOS内核的demo示例，此目录存放的是内核功能测试用的相关用例的代码
│  ├─ include       ------    API功能头文件存放目录
├─ kernel
│  ├─ base
│     ├─ core       ------    LiteOS基础内核代码，包括队列、task调度、软timer、时间片计算等功能
│     ├─ OM         ------    与错误处理相关的文件
│     ├─ include    ------    LiteOS内核内部使用的头文件
│     ├─ ipc        ------    LiteOS中task间通讯的相关接口，包括事件、信号量、消息队列、互斥锁等
│     ├─ mem        ------    LiteOS中的内核内存管理的相关代码
│     ├─ misc       ------    内存对齐功能以及毫秒级休眠sleep功能
│  ├─ include       ------    LiteOS开源内核头文件
│  ├─ extended
│     ├─ tickless   ------    低功耗框架代码
├─ targets
   ├─ Standard_FRDM_KW41Z          ------  FRDM（ARM Cortex M0+）板端工程代码（含原厂芯片驱动）
   ├─ Standard_STM32F103RB_NUCLEO  ------  F103（ARM Cortex M3）板端工程代码（含原厂芯片驱动）
   ├─ Cloud_STM32F429IGTx_FIRE     ------  F429（ARM Cortex M4）板端工程代码（含原厂芯片驱动）
   └─ Standard_STM32F746ZG_NUCLEO  ------  F746（ARM Cortex M7）板端工程代码（含原厂芯片驱动）

```  
