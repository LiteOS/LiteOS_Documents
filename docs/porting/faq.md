# 移植中的常见问题

## 代码编译都能通过，但是 OS 就是运行不起来？

为避免 OS 申请内存异常，需要编辑 `target_config.h`，将内存设置为当前芯片 SRAM 大小。

![](./pic/osconfig-borad-sram-size.png)