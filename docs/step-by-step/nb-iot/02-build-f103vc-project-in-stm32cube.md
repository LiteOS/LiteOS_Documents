# 步骤2：学习 STM32Cube 工具创建裸机工程，实现按键控制功能

After this step, you could code a One Button embedded application with C language. Don’t worry about program language. As we using two Powerful Advanced Helper of ST, you code no more 20 lines code.

## 2.1	Install STM32CubeMX at first, which help us create a Project

![stm32cubemx](./pic/stm32cubemx.png)

Open STM32CubeMX, you first download the STM32F103VCT6 HAL library that ST provided.

![stm32cubemx-package](./pic/cube-install-packages.png)

Expands the STM32F1 MCU Packages, and download the latest package. Wait a minute, you get all the resource.

![](./pic/cube-install-stm32f1.png)

Next, we could new a project, input “STM32F103VC” in search bar, and select STM32F103VCTx item. Double click this item or click the start project button.

![](./pic/cube-stm32f103vtx.png)

![](./pic/cube-stm32f103vtx-main.png)

Please take a closer look at the configuration item. Open the Datasheet of the development board. We will enable the three Peripherals, including LED0, KEY0, USART1

![](./pic/terabits-datasheet.png)

1.	Left click PA12, select “GPIO_Output” item, and then right click it, named “LED0”

![](./pic/terabits-cubemx-gpio-out.png)

2.	Left PA5, select “GPIO_Input” item, and then right click it, name “KEY0”

![](./pic/terabits-cubemx-gpio-exti.png)

3.	Expand the left navigator bar, the find the USART1, set the mode is “Asynchronous”.

![](./pic/terabits-cubemx-usart1-async.png)

The tools will be enable the USART1.

![](./pic/cube-stm32f103vtx-usart1.png)

4.	Change to the configuration page, follow me set some params.

Click USART1 item, change Baud Rate equals 9600 Bits/s and enable interrupt item in NVIC settings

![](./pic/cube-stm32f103vtx-config.png)

![](./pic/cube-stm32f103vtx-config-usart.png)

![](./pic/cube-stm32f103vtx-config-exti.png)

Click GPIO item, LED0 is not set, KEY0 change to Pull-In.

![](./pic/cube-stm32f103vtx-config-gpio.png)

![](./pic/cube-stm32f103vtx-config-nvic.png)

5.	That’s all. You select a directory save the project.

![](./pic/cube-stm32f103vtx-settings.png)

![](./pic/cube-stm32f103vtx-settings-sw4stm32.png)

![](./pic/cube-stm32f103vtx-settings-pair-of-files.png)

Click “Generate source code” button, it automated output the One Button project.

![](./pic/cube-stm32f103vtx-generate-code.png)

![](./pic/cube-stm32f103vtx-generate-code-snap.png)

![](./pic/cube-stm32f103vtx-generate-code-finish.png)

SW4STM32 IDE could be automated opened, the project is prapared. 

![](./pic/sw4stm32-project.png)

::: tip
If you open the SW4STM32 at first, you may be wait a long time, because it will download the cross-compile tools and libraries. There should be no error, when you click the Build Project.
:::

![](./pic/sw4stm32-build-porject.png)

2.2	According to the steps, write a piece of code, and then complete the last works.

Open a `main.c` find `/* USER CODE BEGIN 2 */`, write a line code in function `main()`, which enable LED0 is turn off when the main board was power on.

![](./pic/sw4stm32-code-led0-set.png)

Find `/* USER CODE BEGIN 4 */`, write a new function, named `HAL_GPIO_EXTI_Callback`, when you click the KEY0, the function will be call. 
`HAL_GPIO_TogglePin()` can control the LED0, and HAL_UART_Transmit() can send a message to PC windows

![](./pic/sw4stm32-code-gpio_exti_callback.png)

When you see the QCOM print the contain, it means worked. 

![](./pic/qcom-usart-print.png)