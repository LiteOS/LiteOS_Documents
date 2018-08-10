
# LiteOS SDK端云互通组件API

## 数据结构

#### 平台下发命令枚举类型

```
typedef enum  
{  
    ATINY_GET_MANUFACTURER,                 /*获取厂商名字*/ 
    ATINY_GET_DEV_TYPE，                    /*获取设备类型，由厂商定义和使用*/ 
    ATINY_GET_MODEL_NUMBER,                 /*获取设备模型，由厂商定义和使用*/ 
    ATINY_GET_SERIAL_NUMBER,                /*获取设备序列号*/ 
    ATINY_DO_DEV_REBOOT,                    /*下发设备复位命令*/ 
 
    ATINY_GET_BATERRY_LEVEL,                /*获取电池剩余电量*/ 
    ATINY_GET_TOTAL_MEMORY,                 /*获取内存总量*/ 
    ATINY_GET_MEMORY_FREE,                  /*获取空闲内存*/ 
    ATINY_GET_DEV_ERR, ,                    /*获取设备状态，比如内存耗尽、电池不足、信号强度低等*/ 
 
    ATINY_DO_FACTORY_RESET,                 /*厂商复位*/ 
    ATINY_GET_FIRMWARE_VER,                 /*设备版本号*/ 
 
    ATINY_GET_NETWORK_BEARER,               /*网络通信承载类型，比如GSM、WCDMA、TD-SCDMA等*/ 
    ATINY_GET_SIGNAL_STRENGTH,              /*网络信号强度*/ 
    ATINY_GET_CELL_ID,                      /*网络小区ID*/ 
    ATINY_GET_LINK_QUALITY,                 /*网络链路质量*/ 
 
    ATINY_WRITE_APP_DATA,                   /*业务数据下发命令字*/ 
    ATINY_UPDATE_PSK,                       /*更新psk命令字*/ 
} atiny_cmd_e;
```

#### 关键事件枚举类型

该枚举类型用于LiteOS SDK端云互通组件把自身状态通知用户  

```
typedef enum  
{  
    ATINY_REG_OK,              /*设备注册成功*/ 
    ATINY_REG_FAIL,            /*设备注册失败*/ 
    ATINY_DATA_SUBSCRIBLE,     /*数据开始订阅，设备侧允许上报数据 */ 
    ATINY_DATA_UNSUBSCRIBLE,   /*数据取消订阅 ，设备侧停止上报数据*/ 
} atiny_event_e;
```

#### LwM2M协议参数结构体

```
typedef struct  
{  
    int   life_time;             /*LwM2M协议生命周期，默认50000,*/  
    unsigned int  storing_cnt;   /* L2M2M缓存数据报文个数*/  
} atiny_server_param_t;
```

#### 安全及服务器参数结构体

```
typedef struct  
{  
    bool  is_bootstrap;      /*是否bootstrap服务器*/ 
    char* server_ip;         /*服务器ip，字符串表示，支持ipv4和ipv6*/ 
    char* server_port;       /*服务器端口号*/ 
    char* psk_Id;            /*预置共享密钥ID*/ 
    char* psk;               /*预置共享密钥*/ 
    unsigned short psk_len;  /*预置共享密钥长度*/ 
} atiny_security_param_t;

```

#### 上报数据的枚举类型

用户上报数据的数据类型，用户根据自身应用扩展
  
```
typedef enum  
{  
    BATERRY_LEVEL，  /*电池电量*/ 
    APP_DATA         /*用户数据*/ 
} atiny_report_type_e;

```

#### 服务器参数结构体

```
typedef struct  
{  
    atiny_server_param_t   server_params;  
    atiny_security_param_t security_params[2];  /*支持一个IOT服务器，一个bootstrap服务器*/ 
} atiny_param_t;
```

#### 终端设备参数结构体

```
typedef struct  
{  
    char* endpoint_name;  /*北向申请产生的epname，每个设备对应唯一一个epname*/ 
} atiny_device_info_t;
```
#### 数据上报数据结构

以下枚举值，表述了用户上报的数据，最终的反馈类型，比如数据发送成功，数据发送但未得到确认，具体定义如下：  

```
typedef enum  
{  
    NOT_SENT = 0,        /*待上报的数据未发送*/ 
    SENT_WAIT_RESPONSE,  /*待上报的数据已发送，等待响应*/ 
    SENT_FAIL,           /*待上报的数据发送失败*/ 
    SENT_TIME_OUT,       /*待上报的数据已发送，等待响应超时*/ 
    SENT_SUCCESS,        /*待上报的数据发送成功*/ 
    SENT_GET_RST,        /*待上报的数据已发送，但对端响应RST报文*/ 
    SEND_PENDING,        /*待上报的数据等待发送*/ 
} data_send_status_e;  
 
用户使用以下数据结构上报数据： 
typedef struct _data_report_t  
{  
    atiny_report_type_e type;    /*数据上报类型，比如业务数据，电池剩余电量等 */  
    int cookie;                  /*数据cookie,用以在ack回调中，区分不同的数据*/  
    int len;                     /*数据长度，不应大于MAX_REPORT_DATA_LEN*/  
    uint8_t* buf;                /*数据缓冲区首地址*/  
    atiny_ack_callback callback; /*ack回调，其入参取值data_send_status_e类型 */  
} data_report_t;
```

## API接口

**接口列表**

LiteOS SDK端云互通组件为用户提供以下几类接口：

| 接口分类 | 接口名                                                                   | 描述                                                                                                                                                                                                                                               |
|----------|--------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 依赖接口 | int atiny_cmd_ioctl (atiny_cmd_e cmd, char\* arg, int len);              | LiteOS SDK端云互通组件申明和调用，由开发者实现。该接口是LwM2M标准对象向设备下发命令的统一入口，包含的参数有：                                                                                                                                      |
|          | void atiny_event_notify(atiny_event_e event, const char\* arg, int len); | LiteOS SDK端云互通组件申明和调用，由开发者实现。LiteOS SDK端云互通组件把注册过程的关键状态，以及运行过程的关键事件通知用户，便于用户根据自身的应用场景灵活地做可靠性处理。该接口包含的参数有：                                                     |
| 对外接口 | int atiny_init(atiny_param_t\* atiny_params, void\*\* phandle);          | LiteOS SDK端云互通组件的初始化接口，由LiteOS SDK端云互通组件实现，设备调用。该接口包含的参数有：                                                                                                                                                   |
|          | int atiny_bind(atiny_device_info_t\* device_info, void\* phandle);       | LiteOS SDK端云互通组件的主函数体，由LiteOS SDK端云互通组件实现，设备调用，调用成功后，不会返回。该接口是LiteOS SDK端云互通组件主循环体，实现了LwM2M协议处理，注册状态机，重传队列，订阅上报。该接口包含的参数有：                                  |
|          | void atiny_deinit(void\* phandle);                                       | LiteOS SDK端云互通组件的去初始化接口，由LiteOS SDK端云互通组件实现，设备调用。该接口为阻塞式接口，调用该接口时，会直到agent tiny主任务退出，资源释放完毕，该接口才会退出。 该接口的参数phandle为atiny_init调用获取到的LiteOS SDK端云互通组件句柄。 |
|          | int atiny_data_report(void\* phandle, data_report_t\* report_data);      | LiteOS SDK端云互通组件数据上报接口，由LiteOS SDK端云互通组件实现，设备调用，设备应用数据使用该接口上报。该接口为阻塞接口，不允许在中断中使用。该接口包含的参数有：                                                                                 |

-   参数cmd为具体命令字，比如下发业务数据，下发复位，升级命令等。

-   参数arg和参数len为具体命令对应的参数及参数长度。

为了避免死锁，该接口中禁止调用LiteOS SDK端云互通组件对外接口。

-   参数event为具体事件类型，比如设备注册，去注册，数据订阅和去订阅等。

-   参数arg和参数len为具体事件的参数和参数长度。

为了避免死锁，该接口中禁止调用LiteOS SDK端云互通组件对外接口。

-   参数atiny_params请参考atiny_param_t数据结构说明。

-   参数phandle为出参，返回表征当前创建的LiteOS SDK端云互通组件的句柄。

-   参数device_info请参考数据结构atiny_device_info_t说明。

-   参数phandle为atiny_init调用获取到的LiteOS SDK端云互通组件句柄。

-   参数phandle为atiny_init调用获取到的Agent Tiny句柄。

-   参数report_data请参考数据结构data_report_t说明。

**错误码**

LiteOS SDK端云互通组件对外接口和依赖接口，统一使用以下错误码：

| 序号 | 定义                       | 实际数值 | 描述                           | 参考解决方案                                                       |
|------|----------------------------|----------|--------------------------------|--------------------------------------------------------------------|
| 1    | ATINY_OK                   | 0        | 正常返回码                     |                                                                    |
| 2    | ATINY_ARG_INVALID          | \-1      | 非法参数                       | 确保入参合法                                                       |
| 3    | ATINY_BUF_OVERFLOW         | \-2      | 缓冲区溢出                     | 确保缓冲区充足                                                     |
| 4    | ATINY_MSG_CONGEST          | \-3      | 消息拥塞                       | 暂缓数据上报                                                       |
| 5    | ATINY_MALLOC_FAILED        | \-4      | 内存申请失败                   | 检查内存是否有泄漏                                                 |
| 6    | ATINY_RESOURCE_NOT_FOUND   | \-5      | 数据上报类型非法               | 检查数据类型是否正确                                               |
| 7    | ATINY_RESOURCE_NOT_ENOUGH  | \-6      | 系统资源不足                   | 检查系统资源，比如信号量，套接字个数等，是否配置过少，或是否有泄漏 |
| 8    | ATINY_CLIENT_UNREGISTERED  | \-7      | LiteOS SDK端云互通组件注册失败 | 检查psk，服务器信息等是否正确                                      |
| 9    | ATINY_SOCKET_CREATE_FAILED | \-8      | 网络套接字创建失败             | 检查网络配置参数是否正确                                           |

## atiny_cmd_ioctl实现示例接口

atiny_cmd_ioctl是LiteOS
SDK端云互通组件定义的一个通用可扩展的接口，其命令字如atiny_cmd_e所定义，用户根据自身需求进行选择性实现，也可以根据自身需求进行扩展。常用的接口定义如下表所示，每一个接口都和atiny_cmd_e的枚举值一一对应：

| 回调接口函数                                              | 描述[w1]                                                                                                             |
|-----------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------|
| int atiny_get_manufacturer(char\* manufacturer,int len)   | 获取厂商名字，参数manufacturer指向的内存由LiteOS SDK端云互通组件分配，户填充自身的厂商名字，长度不能超过参数len。    |
| int atiny_get_dev_type(char \* dev_type,int len)          | 获取设备类型，参数dev_type指向的内存由LiteOS SDK端云互通组件分配，户填充自身的设备类型，长度不能超过参数len。        |
| int atiny_get_model_number((char \* model_numer, int len) | 获取设备模型号，参数model_numer指向的内存由LiteOS SDK端云互通组件分配，户填充自身的设备模型号，长度不能超过参数len。 |
| int atiny_get_serial_number(char\* num,int len)           | 获取设备序列号，参数numer指向的内存由LiteOS SDK端云互通组件分配，户填充自身的设备序列号，长度不能超过参数len。       |
| int atiny_get_dev_err(int\* arg，int len)                 | 获取设备状态，比如内存耗尽、电池不足、信号强度低等，参数arg由LiteOS SDK端云互通组件分配，用户填充，长度不能超过len。 |
| int atiny_do_dev_reboot(void)                             | 设备复位。                                                                                                           |
| int atiny_do_factory_reset(void)                          | 厂商复位。                                                                                                           |
| int atiny_get_baterry_level(int\* voltage)                | 获取电池剩余电量。                                                                                                   |
| int atiny_get_memory_free(int\* size)                     | 获取空闲内存大小。                                                                                                   |
| int atiny_get_total_memory(int\* size)                    | 获取总共内存大小。                                                                                                   |
| int atiny_get_signal_strength(int\* singal_strength)      | 获取信号强度。                                                                                                       |
| int atiny_get_cell_id(long\* cell_id)                     | 获取小区ID。                                                                                                         |
| int atiny_get_link_quality(int\* quality)                 | 获取信道质量。                                                                                                       |
| int atiny_write_app_write(void\* user_data, int len)      | 业务数据下发。                                                                                                       |
| int atiny_update_psk(char\* psk_id, int len)              | 预置共享密钥更新。                                                                                                   |