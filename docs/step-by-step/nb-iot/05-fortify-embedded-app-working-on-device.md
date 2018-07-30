# 步骤5：增强嵌入式应用功能，实现 NB-IoT OneButton 按键功能开发

## 5.1	Configure UART3 which connect NB-IoT module in STM32CubeMX, and output a refresh project base on pre-project.

![](./pic/cube-stm32f103vtx-usart3.png)

![](./pic/cube-stm32f103vtx-config-usart3.png)

![](./pic/cube-stm32f103vtx-config-usart3-nvic.png)

Output a refresh project.

## 5.2	According to the steps, write a piece of code, and then complete the last works.

1)	create some variable which save the receive message from NB-IoT module.

![](./pic/sw4stm32-code-recv-buffer.png)

2)	Using HAL_UART_Receive_IT function receive message.

![](./pic/sw4stm32-code-receive-it.png)

3)	receive messages and print them to UART1

![](./pic/sw4stm32-code-while.png)

4)	Rewrite HAL_GPIO_EXTI_Callback, when Button is triggered, send a AT commands to UART3, print them to UART1 at the same time.

![](./pic/sw4stm32-code-hal-exti-callback.png)

5)	Write a new function HAL_UART_RxCpltCallback, which can receive data from UART3 and print them to the UART1

![](./pic/sw4stm32-code-hal-uart-rxcpltcallback.png)

When you see the QCOM print the contain, it means worked.

![](./pic/oceanconnect-real-device-history-data-with-evb.png)
