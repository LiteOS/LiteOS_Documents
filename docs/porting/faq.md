# 移植中的常见问题

## 1. 代码编译都能通过，但是 OS 就是运行不起来？

为避免 OS 申请内存异常，需要编辑 `target_config.h`，将内存设置为当前芯片 SRAM 大小。

![](./pic/osconfig-borad-sram-size.png)

## 2. 使用 GCC 编译无法重定向 printf 到指定串口？

由于 Keil/IAR 内置 MicroLib，如果使用 GCC 编译器，重写 fputc 仍然无法打印到串口，必须添加 [syscalls.c](/porting/syscalls.c) 文件。 同时在 `usart.c` 中添加如下代码，将 printf 重定向到指定串口（可根据实际情况进行调整）：

```c
/* USER CODE BEGIN 0 */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END 0 */
```

```c
/* USER CODE BEGIN 1 */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
/* USER CODE END 1 */
```