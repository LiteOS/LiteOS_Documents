# Doubly-Linked List

[[toc]]  

## Overview

### Basic Concept

A doubly linked list is a linked data structure that consists of a set of sequentially linked records called nodes. Each node in a doubly linked list contains two pointers that reference to the previous and to the next node in the sequence of nodes. The head of the doubly linked list is deterministic and immediately accessible.  

Any node of a doubly linked list, once obtained, can be used to begin a new traversal of the list in either direction (towards the beginning or end) from the given node. This allows a lot of data to be quickly traversed. Because of the symmetric nature of a doubly linked list, nodes can easily be inserted into or removed from the list.  

## Development Guidelines

### Functions

The doubly linked list module provides the following functions:  

| Function Category                     | API                | Description                                                                               |
|---------------------------------------|--------------------|-------------------------------------------------------------------------------------------|
| List initialization                   | `LOS_InitList`       | Initializes a doubly linked list                                                          |
| Node insertion                        | `LOS_ListAdd`        | Inserts a node to a doubly linked list                                                    |
|                                       | `LOS_ListTailInsert` | Inserts a node to the tail of a doubly linked list                                        |
| Node deletion                         | `LOS_ListDelete`     | Deletes a node from a doubly linked list                                                  |
| List status determination             | `LOS_ListEmpty`      | Determines whether a doubly linked list is empty                                          |
| Node deletion and list initialization | `LOS_ListDelInit`    | Deletes a node from a doubly linked list Uses the node to initialize a doubly linked list |

### Development Process

Doubly linked list development process:  

1. Call the `LOS_InitList` API to initialize a doubly linked list.

2. Call the `LOS_ListAdd` API to insert a node into the list.

3. Call the `LOS_ListTailInsert` API to insert a node into the tail of the list.

4. Call the `LOS_ListDelete` API to delete a node from the list.

5. Call the `LOS_ListEmpty` API to determine whether the doubly linked list is empty.

6. Call the `LOS_ListDelInit` API to delete a node and use the node to initialize the doubly linked list.

## Precautions

- Need to pay attention to the operation of the node pointer in the front and back direction.

## Programming Example

### Example Description

Before using a doubly linked list, ensure that sufficient memory space is available to store the list. After deleting a node from the list, do not forget to free up the memory occupied by the node.  

In the programming example, the following steps will be performed:  

1. Initialize a doubly linked list.

2. Insert a node into the list.

3. Delete a node from the list.

4. Check whether the insertion and deletion was successful.

### Example Code

The code is as follows:

```c
#include "stdio.h"
#include "los_list.h"

static UINT32 DLlist_sample(VOID)
{
    LOS_DL_LIST DLlist = {NULL,NULL};
    LOS_DL_LIST DLlistNode01 = {NULL,NULL};
    LOS_DL_LIST DLlistNode02 = {NULL,NULL};
    LOS_DL_LIST DLlistNode03 = {NULL,NULL};

    printf("Initial head.\r\n");
    LOS_ListInit(&DLlist);

    LOS_ListAdd(&DLlist,&DLlistNode01);
    if (DLlistNode01.pstNext == &DLlist && DLlistNode01.pstPrev == &DLlist)
    {
        printf("Add DLlistNode01 success.\r\n");
    }

    LOS_ListTailInsert(&DLlist,&DLlistNode02);
    if (DLlistNode02.pstNext == &DLlist && DLlistNode02.pstPrev == &DLlistNode01)
    {
        printf("Tail insert DLlistNode02 success.\r\n");
    }

    LOS_ListHeadInsert(&DLlistNode02,&DLlistNode03);
    if (DLlistNode03.pstNext == &DLlist && DLlistNode03.pstPrev == &DLlistNode02)
    {
        printf("Head insert DLlistNode03 success.\r\n");
    }

    LOS_ListDelInit(&DLlistNode03);
    LOS_ListDelete(&DLlistNode01);
    LOS_ListDelete(&DLlistNode02);

    if (LOS_ListEmpty(&DLlist))
    {
        printf("Delete success.\r\n");
    }

    return LOS_OK;
}
```   

### Verification

The verification result is as follows:  

```c
Initial head 
Add DLlistNode01 success 
Tail insert DLlistNode02 success 
Head insert DLlistNode03 success 
Delete success  
```    
