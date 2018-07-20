# Huawei LiteOS 源码说明

## 下载 Huawei LiteOS 源码

LiteOS 开源代码路径：[https://github.com/LiteOS/LiteOS](https://github.com/LiteOS/LiteOS)

从 developer 分支获取最新源代码

```bash
git pull 
```

## 源码目录说明

```c
├─ arch                                   // 与 cpu 相关
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