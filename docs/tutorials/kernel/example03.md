# 范例03（优先级抢占实验）

嵌入式实时操作系统最大特点就是任务优先级管理，高优先级任务被中断触发后，可以打断正在运行中的低优先级任务，在第一时间类完成“最紧急”的任务。

## 功能说明

我们在[范例02](example02.md)基础上，调整任务优先级，可以观察到高优先级任务会一直抢占 cpu，导致低优先级任务无法执行。

```c
#define TASK_PRIO_HI        4
#define TASK_PRIO_LO        5
```

::: tip 注意
Huawei LiteOS 任务优先级最高为 0，最低为 31。这个与 FreeRTOS 正好相反。
:::

## 代码讲解

### 1. 使用 `LOS_TaskCreate` 创建两个任务，任务优先级分别为 4 和 5；

```c{8,19}
UINT32 Example03_Entry(VOID) {
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S stInitParam = {0};
    
    printf("Example03_Entry\r\n");

    stInitParam.pfnTaskEntry = Example_TaskHi;
    stInitParam.usTaskPrio = TASK_PRIO_HI;
    stInitParam.pcName = "TaskHi";
    stInitParam.uwStackSize = TASK_STK_SIZE;
    stInitParam.uwArg = (UINT32)pcTextForTaskHi;
    uwRet = LOS_TaskCreate(&s_uwTskHiID, &stInitParam);
    if (uwRet != LOS_OK) {
        printf("Example_TaskHi create Failed!\r\n");
        return LOS_NOK;
    }

    stInitParam.pfnTaskEntry = Example_TaskLo;
    stInitParam.usTaskPrio = TASK_PRIO_LO;
    stInitParam.pcName = "TaskLo";    
    stInitParam.uwStackSize = TASK_STK_SIZE;
    stInitParam.uwArg = (UINT32)pcTextForTaskLo;
    uwRet = LOS_TaskCreate(&s_uwTskLoID, &stInitParam);
    if (uwRet != LOS_OK) {
        printf("Example_TaskLo create Failed!\r\n");
        return LOS_NOK;
    }

    return uwRet;
}
```

### 2. `Task1` 和 `Task2` 周期性打印字符串

```c
static VOID * Example_TaskHi(UINT32 uwArg) {
    UINT32 i;
    for (;;) {
        printf("%s\r\n", (const CHAR *)uwArg);
        for (i = 0; i < TASK_LOOP_COUNT; i++) {
            // 占用CPU耗时运行
        }
    }
}

static VOID * Example_TaskLo(UINT32 uwArg) {
    volatile UINT32 i;
    for (;;) {
        printf("%s\r\n", (const CHAR *)uwArg);
        for (i = 0; i < TASK_LOOP_COUNT; i++) {
            // 占用CPU耗时运行
        }
    }
}
```

## 效果演示

![](./pic/result03.png)