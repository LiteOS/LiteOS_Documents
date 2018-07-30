# 验证实验设计

目前 Huawei LiteOS 内置了 **内核测试代码**，我们可以直接拿这套代码进行验证。

## 添加 example 测试代码

![](./image/raw-project-group-example.png)

也不要忘记头文件路径导入

![](./image/raw-project-example-path.png)

## 修改 main.c 文件

```c
/* USER CODE BEGIN Includes */
#include "los_base.h"
#include "los_inspect_entry.h"
/* USER CODE END Includes */

/* USER CODE BEGIN 2 */
if (LOS_OK != LOS_KernelInit())
{
    return LOS_NOK;
}

LOS_Inspect_Entry();
LOS_Start();
/* USER CODE END 2 */
```

::: danger 危险
请务按照上面示例的方式添加测试代码。因为只有在 USER CODE 注释包裹内添加，下一次再使用 STM32Cube 导出的代码才不会被覆盖。
:::