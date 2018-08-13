# OS_CONFIG 系统配置

## 与移植相关的配置宏定义

```c
#define    OS_SYS_CLOCK     (SystemCoreClock)   // 配置CPU主频，需要用户更改
#define    LOSCFG_PLATFORM_HWI          NO      // 配置OS是否接管中断
#define    LOSCFG_PLATFORM_HWI_LIMIT    32      // 配置外部中断数目
#define    LOSCFG_BASE_CORE_TSK_LIMIT	   3      // 配置支持的最大任务数量
#define    LOSCFG_BASE_IPC_SEM_LIMIT     2	    // 配置支持的最大信号量数量
#define    LOSCFG_BASE_IPC_MUX_LIMIT     2      // 配置支持的最大互斥锁数量
#define    LOSCFG_BASE_IPC_QUEUE_LIMIT   2      // 配置支持的最大队列数量
#define    LOSCFG_BASE_CORE_SWTMR_LIMIT  2      // 配置支持的最大软件定时器数量
#define    BOARD_SRAM_START_ADDR     0x20000000 // 配置目标芯片RAM起始地址
#define    BOARD_SRAM_SIZE_KB        8	        // 配置目标芯片RAM大小，单位KB
#define    CMSIS_OS_VER              2          // 配置CMSIS OS版本，支持V1和V2
```

### 注意：

用户根据自身硬件情况修改上述配置，特别是CPU主频和RAM尺寸要根据情况修改；

本教程要求OS不接管中断，接管中断的方式需要自行修改sct文件，所以推荐初级用户先不要使能接管中断的方式；

最大任务数、信号量数、互斥锁数、队列数、软件定时器数建议用户根据实际需要配置合理的数量，数量太多会占用额外的RAM资源；