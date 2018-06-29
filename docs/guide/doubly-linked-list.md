# 双向链表

## 概述

### 基本概念

双向链表是指含有往前和往后两个方向的链表，即每个结点中除存放下一个节点指针外，还增加一个指向其前一个节点的指针。其头指针head是唯一确定的。  

从双向链表中的任意一个结点开始，都可以很方便地访问它的前驱结点和后继结点，这种数据结构形式使得双向链表在查找时更加方便，特别是大量数据的遍历。由于双向链表具有对称性，能方便地完成各种插入、删除等操作，但需要注意前后方向的操作。  

## 开发指导

### 功能

Huawei LiteOS系统中的双向链表模块为用户提供下面几个接口。

| 功能分类             | 接口名              | 描述                     |
|----------------------|---------------------|--------------------------|
| 初始化链表           | `LOS_ListInit`       | 对链表进行初始化         |
| 增加节点             | `LOS_ListAdd`        | 将新节点添加到链表中。   |
| 在链表尾端插入节点   | `LOS_ListTailInsert` | 将节点插入到双向链表尾端 |
| 删除节点             | `LOS_ListDelete`     | 将指定的节点从链表中删除 |
| 判断双向链表是否为空 | `LOS_ListEmpty`      | 判断链表是否为空         |
| 删除节点并初始化链表 | `LOS_ListDelInit`    | 将指定的节点从链表中删除，并使用该节点初始化链表  |

### 开发流程

双向链表的典型开发流程：

1.  调用 `LOS_ListInit` 初始双向链表。

2.  调用 `LOS_ListAdd` 向链表中增加节点。

3.  调用 `LOS_ListTailInsert` 向链表尾部插入节点。

4.  调用 `LOS_ListDelete` 删除指定节点。

5.  调用 `LOS_ListEmpty` 判断链表是否为空。

6.  调用 `LOS_ListDelInit` 删除指定节点并以此节点初始化链表。

## 注意事项

- 需要注意节点指针前后方向的操作。

## 编程实例

### 实例描述

使用双向链表，首先要申请内存，删除节点的时候要注意释放掉内存。  

本实例实现如下功能： 

1.  调用函数进行初始化双向链表。  

2.  增加节点。  

3.  删除节点。  

4.  测试操作是否成功。  

### 编程示例

代码实现如下：  
```c
#include "stdio.h"
#include "los_list.h"

static UINT32 DLlist_sample(VOID)
{
    LOS_DL_LIST DLlist = {NULL,NULL};
    LOS_DL_LIST DLlistNode01 = {NULL,NULL};
    LOS_DL_LIST DLlistNode02 = {NULL,NULL};
    LOS_DL_LIST DLlistNode03 = {NULL,NULL};

    PRINTK("Initial head\n");
    LOS_ListInit(&DLlist);

    LOS_ListAdd(&DLlist,&DLlistNode01);
    if (DLlistNode01.pstNext == &DLlist && DLlistNode01.pstPrev == &DLlist)
    {
        PRINTK("Add DLlistNode01 success \n");
    }

    LOS_ListTailInsert(&DLlist,&DLlistNode02);
    if (DLlistNode02.pstNext == &DLlist && DLlistNode02.pstPrev == &DLlistNode01)
    {
        PRINTK("Tail insert DLlistNode02 success \n");
    }

    LOS_ListHeadInsert(&DLlistNode02,&DLlistNode03);
    if (DLlistNode03.pstNext == &DLlist && DLlistNode03.pstPrev == &DLlistNode02)
    {
        PRINTK("Head insert DLlistNode03 success \n");
    }

    LOS_ListDelInit(&DLlistNode03);
    LOS_ListDelete(&DLlistNode01);
    LOS_ListDelete(&DLlistNode02);

    if (LOS_ListEmpty(&DLlist))
    {
        PRINTK("Delete success \n");
    }

    return LOS_OK;
}
```

编译运行得到的结果为：   
```  
Initial head 
Add DLlistNode01 success 
Tail insert DLlistNode02 success 
Head insert DLlistNode03 success 
Delete success  
```  