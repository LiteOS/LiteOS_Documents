# 设备接入OceanConnect集成开发流程

本章内容旨在帮助开发者在IoT设备上集成LiteOS
SDK端云互通组件，进行IoT应用开发和调测。LiteOS
SDK端云互通组件接入华为OceanConnect
IoT云平台默认采用的是以太网方式（即以太网口驱动+LwIP网络协议栈+LwM2M协议+LiteOS
SDK端云互通组件对接云平台）。

OceanConnect即华为IoT联接管理平台（IoT Connection Management
Platform）是面向运营商和企业/行业领域的统一开放云平台，支持SIM和非SIM场景的各种联接和联接管理。通过开放的APIs，向上集成各种行业应用，向下接入各种传感器、终端和网关，帮助运营商和企业/行业客户实现多种行业终端的快速接入，多种行业应用的快速集成。华为IoT联接管理平台提供安全可控的全联接管理，使能行业革新，构建IoT生态（本章中提到的IoT平台指OceanConnect）。

## 环境准备

在开发之前，需要提前获取如下信息：

* Huawei LiteOS及LiteOS SDK源代码

>   目前托管在GitHub，地址为<https://github.com/LiteOS/LiteOS>

* 开发者Portal的访问地址/账号/密码

>   需要向OceanConnect IoT平台申请。

* 设备对接地址/端口号

## 创建应用

通过创建应用，开发者可以根据自身应用的特征，选择不同的平台服务套件，降低应用开发难度。

* **步骤1** 登录OceanConnect IoT平台的开发者Portal。开发者Portal的访问地址、账号和密码需要向IoT平台服务商申请。

* **步骤2** 登录界面会跳出弹框，提示“当前账号没有应用！请先创建应用！”，点击“创建应用”。

**创建应用**

![](./pic/sdk/14.png)

* **步骤3** 在新弹出窗口中，配置应用信息，点击“确定”。

配置示例如下图，点击“确定”后，IoT平台会返回应用ID和应用密钥，请妥善保存应用密钥，以便于应用服务器接入平台时使用。如果遗忘密钥，需要通过“对接信息”-\>"重置密钥”进行重置。

**配置应用**

![](./pic/sdk/15.png)

![](./pic/sdk/6.gif)

如上配置仅为参考举例，具体配置请以现网需求为准。

**应用创建成功**

![](./pic/sdk/16.png)

----**结束**

## 开发Profile文件

Profile文件用来描述设备类型和设备服务能力。它定义了设备具备的服务能力，每个服务具备的属性、命令以及命令参数。

* **步骤1** 登录IoT平台的开发者Portal。开发者Portal的访问地址、账号和密码需要向IoT平台服务商申请。

* **步骤2** 选择“Profile开发”-\>“Profile在线开发”-\>“自定义产品”，点击右上角“+创建全新产品”。

IoT平台提供了Profile模板库，开发者可以根据自己需要，选择合适的模板直接使用。如果在模板库中未找到需要的Profile，再自己定义，示例如下。

**创建Profile文件**

![](./pic/sdk/17.png)

![](./pic/sdk/6.gif)

如上配置仅为参考举例，具体配置请以现网需求为准。

* **步骤3** 选择新创建的Profile文件，点击“新建服务”，配置设备的服务能力。

可参考“Profile开发”-\>“Profile在线开发”中的“产品模板”进行配置。例如新建一个名为LightControl的服务，包含一种属性light（灯的亮灭状态）和一种命令（设置灯亮on或者灭off）

**新建LightControl服务**

![](./pic/sdk/18.png)

* **步骤4** （可选）开发者Portal提供了Profile文件的导出功能。

选择“Profile开发”-\>“Profile在线开发”-\>新创建的Profile文件，点击右上角“导出该产品Profile”，可以对线上开发的Profile文件进行导出。

**导出Profile文件**

![](./pic/sdk/19.png)

----**结束**

## 开发编解码插件

IoT设备和IoT平台之间采用LwM2M协议通信，LwM2M消息的数据为应用层数据，应用层数据的格式由设备厂商自行定义。由于IoT设备对省电要求较高，所以应用层数据一般采用二进制格式。IoT平台在对应用层数据进行协议解析时，会转换成统一的json格式，以方便应用服务器使用。要实现二进制消息与json格式消息的转换，IoT平台需要使用编解码插件。

* **步骤1** 选择“插件开发”-\>“插件开发”-\>“开始设计”，点击右上角“+新建插件”。在弹出框中，选择Profile文件。

IoT平台提供了插件模板库，开发者可以根据自己需要，选择合适的模板直接使用。如果在模板库中未找到需要的插件，再自己定义。

**创建插件**

![](./pic/sdk/20.png)

* **步骤2** 点击“新增消息”，配置二进制码流和Profile属性/命令/命令响应的映射关系。

可参考“插件开发”-\>“插件开发”-\>“开始设计”中的“新手指导”和“插件模板”进行配置。

**开发插件（新建消息）**

![](./pic/sdk/21.png)

**开发插件（添加字段）**

![](./pic/sdk/22.png)

编解码插件的开发，即定义：

* **Profile** 文件定义的属性/响应在设备上报的二进制码流中的位置，以便于平台对设备上报数据和命令响应进行解码。

* **Profile** 文件定义的命令在平台下发的二进制码流中的位置，以便于平台对下发命令进行编码。

**二进制码流和Profile文件的映射关系**

![](./pic/sdk/23.png)

* **步骤3** 点击右上角“部署”。

点击部署后，需要先“保存”插件，之后才开始部署。部署需要等待时间小于60s。

**保存插件**

![](./pic/sdk/24.png)

**部署插件**

![](./pic/sdk/25.png)

* **步骤4** （可选）开发者Portal提供了编解码插件的下载功能。

选择“插件开发”-\>“插件开发”-\>新开发的编解码插件，点击右上角“下载”，可以对线上开发的编解码插件进行导出。

----**结束**

## 注册设备

应用服务器需要调用IoT平台的注册设备接口，在IoT平台添加设备。

IoT平台提供了应用模拟器，可以模拟应用服务器在IoT平台注册设备的场景。使用应用模拟器注册设备时，需要选择注册设备的Profile，以实现注册设备和Profile文件的关联。因此，通过应用模拟器注册设备，实际模拟了应用服务器在IoT平台**注册设备**和**修改设备信息**两个过程。本节基于应用模拟器进行操作。

* **步骤1** 选择“我的设备”-\>“注册设备”-\>“需要注册设备的Profile”，输入设备名称和验证码（verifyCode），点击“注册”。

注册设备后，IoT平台会返回设备ID和PSK码，请妥善保存。新增注册的设备状态为“未绑定（not
bound）”。

**注册设备**

![](./pic/sdk/26.png)

----**结束**

## 端云互通组件设备接入平台

设备接入IoT平台后，IoT平台才可以对设备进行管理。设备接入平台时，需要保证IoT平台已经有对应应用，并且已经在该应用下注册了此设备。

#### 5.6.1开发环境准备

- 集成开发工具：

	- MDK 5.18版本或者以上版本，从MDK官方网站下载。   

	- MDK依赖的pack包

>   说明：MDK工具需要license，请从MDK官方获取。

- LiteOS SDK端云互通组件代码：

  git clone https://github.com/LiteOS/LiteOS.git

- 硬件设备：野火STM32F429开发板，调试下载器（J-Link、ST-Link等）、网线、路由器。

#### 5.6.2开发板硬件介绍

本文档以野火STM32F429IG开发板为例进行介绍，板子的详细资料可以从<http://www.firebbs.cn/forum.php>下载。

**STM32F429IG_FIRE开发板外设**

![](./pic/sdk/27.png)

#### 5.6.3端云互通组件代码目录介绍

| 代码目录                                                                                                                                                                                                                                                                                                          | 功能                                     |
|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|------------------------------------------|
| [LiteOS](https://github.com/LiteOS/LiteOS)/[components](https://github.com/LiteOS/LiteOS/tree/master/components)/[connectivity](https://github.com/LiteOS/LiteOS/tree/master/components/connectivity)/[agent_tiny](https://github.com/LiteOS/LiteOS/tree/master/components/connectivity/agent_tiny)/[Lwm2m_client](https://github.com/LiteOS/LiteOS/tree/master/components/connectivity/agent_tiny/Lwm2m_client)/ | 端云互通组件实体                         |
| [LiteOS](https://github.com/LiteOS/LiteOS)/[components](https://github.com/LiteOS/LiteOS/tree/master/components)/[connectivity](https://github.com/LiteOS/LiteOS/tree/master/components/connectivity)/[agent_tiny](https://github.com/LiteOS/LiteOS/tree/master/components/connectivity/agent_tiny)/[examples](https://github.com/LiteOS/LiteOS/tree/master/components/connectivity/agent_tiny/examples)/     | 端云互通组件demo，包括用户需要实现的接口 |
| [LiteOS](https://github.com/LiteOS/LiteOS)/[components](https://github.com/LiteOS/LiteOS/tree/master/components)/[connectivity](https://github.com/LiteOS/LiteOS/tree/master/components/connectivity)/[agent_tiny](https://github.com/LiteOS/LiteOS/tree/master/components/connectivity/agent_tiny)/[osadapter](https://github.com/LiteOS/LiteOS/tree/master/components/connectivity/agent_tiny/osadapter)/    | 端云互通组件操作系统适配接口             |
| [LiteOS](https://github.com/LiteOS/LiteOS/tree/develop)/[components](https://github.com/LiteOS/LiteOS/tree/develop/components)/[connectivity](https://github.com/LiteOS/LiteOS/tree/develop/components/connectivity)/[at_frame](https://github.com/LiteOS/LiteOS/tree/develop/components/connectivity/at_frame)/                                                                                    | 串口AT框架                               |
| [LiteOS](https://github.com/LiteOS/LiteOS)/[components](https://github.com/LiteOS/LiteOS/tree/master/components)/[connectivity](https://github.com/LiteOS/LiteOS/tree/master/components/connectivity)/[lwm2m](https://github.com/LiteOS/LiteOS/tree/master/components/connectivity/lwm2m)/                                                                                                      | LwM2M协议                                |
| [LiteOS](https://github.com/LiteOS/LiteOS/tree/develop)/[components](https://github.com/LiteOS/LiteOS/tree/develop/components)/[fota](https://github.com/LiteOS/LiteOS/tree/develop/components/fota)/                                                                                                                                                                              | 固件在线升级支持                         |
| [LiteOS](https://github.com/LiteOS/LiteOS/tree/develop)/[components](https://github.com/LiteOS/LiteOS/tree/develop/components)/[security](https://github.com/LiteOS/LiteOS/tree/develop/components/security)/[mbedtls](https://github.com/LiteOS/LiteOS/tree/develop/components/security/mbedtls)                                                                                              | 安全协议TLS和DTLS                        |
| [LiteOS](https://github.com/LiteOS/LiteOS/tree/develop)/[components](https://github.com/LiteOS/LiteOS/tree/develop/components)/[net](https://github.com/LiteOS/LiteOS/tree/develop/components/net)/[lwip-2.0.3](https://github.com/LiteOS/LiteOS/tree/develop/components/net/lwip-2.0.3)                                                                                                     | TCPIP协议栈                              |
| [LiteOS](https://github.com/LiteOS/LiteOS/tree/develop)/[drivers](https://github.com/LiteOS/LiteOS/tree/develop/drivers)/[devices](https://github.com/LiteOS/LiteOS/tree/develop/drivers/devices)/[gprs](https://github.com/LiteOS/LiteOS/tree/develop/drivers/devices/gprs)                                                                                                            | GPRS设备适配文件                         |
| [LiteOS](https://github.com/LiteOS/LiteOS/tree/develop)/[drivers](https://github.com/LiteOS/LiteOS/tree/develop/drivers)/[devices](https://github.com/LiteOS/LiteOS/tree/develop/drivers/devices)/[wifi](https://github.com/LiteOS/LiteOS/tree/develop/drivers/devices/wifi)/                                                                                                           | WIFI设备适配文件                         |

#### 5.6.4设备接入开发步骤

**步骤1** 开发板的网口通过网线连接到路由器。

**步骤2** 设置本地IP。

在sys_init.c中修改device接入的局域网的IP地址值。（注：目前demo程序采用的是静态IP地址的方式，如果需要使用DHCP方式，请在main.c中顶部头文件包含之后定义USE_DHCP宏即可）

```
void net_init(void) 
{ 
    /* IP addresses initialization */ 
    IP_ADDRESS[0] = 192; 
    IP_ADDRESS[1] = 168; 
    IP_ADDRESS[2] = 0; 
    IP_ADDRESS[3] = 115; 
    NETMASK_ADDRESS[0] = 255; 
    NETMASK_ADDRESS[1] = 255; 
    NETMASK_ADDRESS[2] = 255; 
    NETMASK_ADDRESS[3] = 0; 
    GATEWAY_ADDRESS[0] = 192; 
    GATEWAY_ADDRESS[1] = 168; 
    GATEWAY_ADDRESS[2] = 0; 
    GATEWAY_ADDRESS[3] = 1;
}
```

**步骤3** 网口的mac地址修改。

在eth.c中将MAC_ADDR0\~MAC_ADDR5修改成真实的mac地址值保证不重复。

```
static int8_t eth_init(struct netif* netif) 
{ 
    HAL_StatusTypeDef hal_eth_init_status; 
    MACAddr[0] = 0x00; 
    MACAddr[1] = 0x80; 
    MACAddr[2] = 0xE1; 
    MACAddr[3] = 0x00; 
    MACAddr[4] = 0x00; 
    MACAddr[5] = 0x00;
} 
```

**步骤4** 设置云平台IP以及设备EP Name和PSK。

EP
Name就是在云平台上注册IMSI号，而PSK（预共享密钥）是用来加密传输的秘钥，agent_tiny_demo.c中示例如下：

```
#define DEFAULT_SERVER_IPV4 "139.159.209.89"
char * g_endpoint_name = "44440003"; 
#ifdef WITH_DTLS 
char *g_endpoint_name_s = "11110006"; 
unsigned char g_psk_value[16] = {0xef,0xe8,0x18,0x45,0xa3,0x53,0xc1,0x3c,0x0c,0x89,0x92,0xb3,0x1d,0x6b,0x6a,0x96};  
#endif
```

**步骤5** 编译并运行程序。

**步骤6** 查看设备状态。

登录IoT平台开发者Portal，选择“我的设备”，在设备列表中查看对应设备的状态。如果状态为“绑定（bound）”，则表示设备已经成功接入IoT平台。

**查看设备状态**

![](./pic/sdk/28.png)

----**结束**

#### 5.6.5端云互通组件设备数据上报

LiteOS SDK端云互通组件设备数据上报调测过程：

**步骤1** 在设备侧执行app_data_report函数，使设备上报数据。

修改agent_tiny_demo.c中的函数app_data_report如下：

```
struct Led_Light
{
uint8_t lightvalue;
…
};
extern get_led_lightvalue (void);
void app_data_report(void)
{
struct Led_Light light;
data_report_t report_data;
    int ret;
    int cnt = 0;
    report_data.buf = (uint8_t *)&light;
    report_data.callback = ack_callback;
    report_data.cookie = 0;
    report_data.len = sizeof(struct Led_Light);
    report_data.type = APP_DATA;
    while(1)
    {
        report_data.cookie = cnt;
        cnt++;
        ret = atiny_data_report(g_phandle, &report_data);
        printf("report ret:%d\n",ret);
        (void)LOS_TaskDelay(250*8);
}
}
```

**步骤2** 查看设备状态

登录IoT平台的开发者Portal，在“我的设备”界面的设备列表中，选择上报数据的设备，查看“历史数据”，验证设备数据上报的情况。

**使用LiteOS SDK端云互通组件的IoT设备数据上报业务流程**

![](./pic/sdk/29.png)

**查看数据上报结果**

![](./pic/sdk/30.png)

----**结束**

#### 5.6.6命令下发

命令下发一般分为两种形式：立即下发和缓存下发。

* **立即下发：** IoT平台立即发送收到的命令，如果设备不在线或者设备没收到指令则下发失败。立即下发适合对命令实时性有要求的场景，比如路灯开关灯，燃气表开关阀。使用立即下发时，应用服务器需要自己保证命令下发的时机。

**命令立即下发流程**

![](./pic/sdk/31.png)

* **缓存下发：** 平台收到命令后放入队列。在设备上线的时候，平台依次下发命令队列中的命令。缓存下发适合对命令实时性要求不高的场景，比如配置水表的参数。缓存下发平台根据设备的省电模式进行不同处理。

**命令缓存下发流程**

![](./pic/sdk/32.png)

应用服务器向IoT平台下发命令时，携带参数expireTime（简称TTL，表示最大缓存时间）。如果不带expireTime，则默认expireTime为48小时。

expireTime=0：命令立即下发。

expireTime\>0：命令缓存下发。

LiteOS SDK端云互通组件场景命令下发的调测过程：

**命令下发步骤如下**

**步骤1** 登录IoT平台的开发者Portal。开发者Portal的访问地址、账号和密码需要向IoT平台服务商申请。

**步骤2** 在“我的设备”界面的设备列表中，选择接收命令的设备，点击“命令下发（\</\>)”。在弹出界面中，配置下发给设备的命令参数。

**命令下发**

![](./pic/sdk/33.png)

**步骤3** 在“我的设备”界面的设备列表中，选择接收命令的设备-\>“历史命令”，查看“状态”栏的显示。

**命令下发状态**

![](./pic/sdk/34.png)

状态说明如下：

* **超期：** 表示命令在IoT平台缓存时间超期，未向设备下发。

* **成功：** 表示IoT平台已经将命令下发给设备，且收到设备上报的命令执行结果。

* **失败：** 表示编解码插件解析为空，或执行结果响应里面有“ERROR CODE”等。

* **超时：** 表示IoT平台等待ACK响应超时。

* **取消：** 表示应用侧已经取消命令下发。

* **等待：** 表示命令在IoT平台缓存，还未下发给设备。

* **已发送：** 表示IoT平台已经将命令下发给设备。

* **已送达：** 表示IoT平台已经将命令下发给设备，且收到设备返回的ACK消息。

**步骤4** 在设备侧执行atiny_cmd_ioctl函数（实际调用atiny_write_app_write回调函数处理下发命令），读取从IoT平台接收的命令信息。

```
struct Led_Light
int atiny_write_app_write(void* user_data, int len)
{
    int i;
		uint8_t cmd_data[len];
		memcpy(cmd_data, user_data, len);
		for(i=0;i<len;i++)
		{
			printf("########   %d",cmd_data[i]);//打印下发的命令数据，用户可以处理下
//发的命令，根据具体的命令控制硬件设备
		}
    (void)atiny_printf("write num19 object success\r\n");
    return ATINY_OK;
}
```

----**结束**

## （参考）端云互通组件无线接入开发指南

#### 5.7.1无线接入介绍

无线的接入方式包括WIFI、GSM、NB-IoT、Zigbee、蓝牙等，本文主要讲解WIFI和GSM（GPRS）的接入方式。对物联网开发者来说，WIFI或者GSM一般都是一个单独的模块，运行在MCU上的LiteOS
SDK端云互通组件需要使用WIFI或者GSM提供的网络服务时，需要通过串口AT指令就可以了，如图6-10所示，ESP8266是乐鑫的WIFI模组，SIM900A是SIMCom芯讯通推出的GSM/GPRS模组。

**Huawei LiteOS SDK端云互通组件无线接入方案示意图**

![](./pic/sdk/35.png)

AT 即Attention，AT指令集是从终端设备 (Terminal Equipment，TE)或者数据终端设备
(Data Terminal Equipment，DTE)向终端适配器(Terminal Adapter，
TA)或数据电路终端设备 (Data Circuit Terminal
Equipment，DCE)发送的。通过TA，TE发送AT指令来控制移动台(Mobile
Station，MS)的功能，与GSM
网络业务进行交互。用户可以通过AT指令进行呼叫、短信、电话本、数据业务、传真等方面的控制。

#### 5.7.2 AT框架介绍 

不论使用ESP8266还是SIM900A，都可以使用AT+UART方式接入，主要的差别在于具体的AT指令，但很多情况下都是类似的，LiteOS
SDK端云互通组件提供了一种AT框架，也可以称之为AT模板，方便用户移植不同串口通信模块（需要支持TCP/IP协议栈），AT框架的方案如下图所示。

在下图中， AT Socket是适配Atiny Socket接口，类似posix socket，AT
Send是调用at_cmd发送AT命令，AT Recv是AT Analyse
Task通过LiteOS消息队列Post消息到用户接收任务，AT Analyse
Task主要来解析来自串口的消息，包括用户数据和命令的响应，串口USART主要是在中断或者DMA模式下接收数据，AT
API Register是提供设备模块注册API函数。

**AT框架方案结构图**

![](./pic/sdk/36.png)

其中，深蓝色的部分是AT框架公共部分代码，用户不需要修改，浅蓝色的部分是设备相关代码，用户需要编写相应的设备代码，根据at_api_interface.h文件的定义，用户只要实现以下函数接口即可：

```
typedef struct { 
 	int32_t  (*init)(void);  /*初始化，初始化串口、IP网络等*/ 
 	int8_t (*get_localmac)(int8_t *mac);/*获取本地MAC*/ 
 	int8_t (*get_localip)(int8_t *ip, int8_t * gw, int8_t * mask);/*获取本地IP*/ 
 	/*建立TCP或者UDP连接*/ 
 	int32_t  (*connect)(const int8_t * host, const int8_t *port, int32_t proto); 
 	/*发送，当命令发送后，如果超过一定的时间没收到应答，要返回错误*/ 
 	int32_t  (*send)(int32_t id , const uint8_t  *buf, uint32_t len); 
    int32_t  (*recv_timeout)(int32_t id , int8_t  *buf, uint32_t len, int32_t timeout); 
 	int32_t  (*recv)(int32_t id , int8_t  *buf, uint32_t len); 
     
 	int32_t  (*close)(int32_t id);/*关闭连接*/ 
 	int32_t  (*recv_cb)(int32_t id);/*收到各种事件处理，暂不实现 */ 
 	int32_t  (*deinit)(void); 
}at_adaptor_api;
```

#### 5.7.3 移植WIFI模块-ESP8266

**步骤1** STM32F429开发板上连接ESP8266 串口wifi模块，如下图所示：

![](./pic/sdk/37.jpg)

**步骤2** 首先在设备文件LiteOS/drivers/devices/wifi/esp8266.c定义API结构体。

```
at_adaptor_api at_interface = {  
     .init = esp8266_init,     
     .get_localmac = esp8266_get_localmac, /*get local MAC*/ 
     .get_localip = esp8266_get_localip,/*get local IP*/ 
     /*build TCP or UDP connection*/ 
     .connect = esp8266_connect, 
     .send = esp8266_send, 
     .recv_timeout = esp8266_recv_timeout, 
     .recv = esp8266_recv, 
     .close = esp8266_close,/*close connection*/ 
     .recv_cb = esp8266_recv_cb,/* operation for events, not implements yet */ 
     .deinit = esp8266_deinit, 
};
```

**步骤3** 在LiteOS/targets/STM32F429IGTx_FIRE/Src/main.c文件中，代码如下:

```
#elif defined(WITH_AT_FRAMEWORK) && (defined(USE_ESP8266) || defined(USE_SIM900A)) 
     extern at_adaptor_api at_interface; 
     at_api_register(&at_interface); 
     agent_tiny_entry();
#endif
```

**步骤4** 确保打开了编译宏。

**全局宏包含WITH_AT_FRAMEWORK和USE_ESP8266**

![](./pic/sdk/38.png)

**步骤5** 在LiteOS/drivers/devices/wifi/esp8266.c实现具体设备API接口。

例如demo例程初始化如下：

```
int32_t esp8266_init() 
{ 
     at.init(); 
     at.oob_register(AT_DATAF_PREFIX, strlen(AT_DATAF_PREFIX), esp8266_data_handler); 
 #ifdef 	USE_USARTRX_DMA   HAL_UART_Receive_DMA(&at_usart,&at.recv_buf[at_user_conf.user_buf_len*0],at_user_conf.user_buf_len); 
#endif 
     esp8266_reset();   
     esp8266_echo_off(); 
     esp8266_choose_net_mode(STA); 
     while(AT_FAILED == esp8266_joinap(WIFI_SSID, WIFI_PASSWD)) 
     { 
         AT_LOG("connect ap failed, repeat..."); 
     }; 
     esp8266_set_mux_mode(at.mux_mode); 
     static int8_t ip[32]; 
     static int8_t gw[32]; 
     static int8_t mac[32]; 
     esp8266_get_localip(ip, gw, NULL); 
     esp8266_get_localmac(mac); 
     AT_LOG("get ip:%s, gw:%s mac:%s", ip, gw, mac); 
     return AT_OK; 
}
```

其它几个接口参考LiteOS/drivers/devices/wifi/esp8266.c即可，而ESP8266模块AT命令定义的宏在LiteOS/drivers/devices/wifi/esp8266.h，具体含义可以查看ESP8266官方手册,
具体含义可以查看ESP8266官方手册,另外用户需要在esp8266.h中修改自己连接的wifi的ssid和密码。

```
#define AT_CMD_RST    		"AT+RST" 
#define AT_CMD_ECHO_OFF 	"ATE0" 
#define AT_CMD_CWMODE  		"AT+CWMODE_CUR" 
#define AT_CMD_JOINAP  		"AT+CWJAP_CUR" 
#define AT_CMD_MUX 		"AT+CIPMUX" 
#define AT_CMD_CONN		"AT+CIPSTART" 
#define AT_CMD_SEND		"AT+CIPSEND" 
#define AT_CMD_CLOSE		"AT+CIPCLOSE" 
#define AT_CMD_CHECK_IP		"AT+CIPSTA_CUR?" 
#define AT_CMD_CHECK_MAC	"AT+CIPSTAMAC_CUR?"
```

#### 5.7.4 移植GSM模块-SIM900A

与ESP8266非常类似，只不过具体AT命令有稍微差异。

**步骤1** STM32F429开发板上连接SIM900A串口GSM模块，如下图所示

![](./pic/sdk/39.jpg)

**步骤2** 在设备文件LiteOS/drivers/devices/gprs/sim900a.c定义API结构体。

```
at_adaptor_api at_interface = { 
    .init = sim900a_ini, 
    /*TCP or UDP connect*/ 
    .connect = sim900a_connect, 
    /*send data, if no response, retrun error*/ 
    .send = sim900a_send, 
    .recv_timeout = sim900a_recv_timeout, 
    .recv = sim900a_recv, 
    .close = sim900a_close,/*close connect*/ 
    .recv_cb = sim900a_recv_cb,/*receive event handle, no available by now */ 
.deinit = sim900a_deinit, 
};
```

**步骤3** 在LiteOS/targets/STM32F429IGTx_FIRE/Src/main.c文件中，代码如下：

```
#elif defined(WITH_AT_FRAMEWORK) && (defined(USE_ESP8266) || defined(USE_SIM900A)) 
     extern at_adaptor_api at_interface; 
     at_api_register(&at_interface); 
     agent_tiny_entry(); 
#endif
```

**步骤4** 确保打开了编译宏

**全局宏包含WITH_AT_FRAMEWORK和USE_SIM900A**

![](./pic/sdk/40.png)

**步骤4** 在LiteOS/drivers/devices/gprs/sim900a.c实现具体设备API接口。

例如demo例程发送和接收函数如下：

```
int32_t  sim900a_recv_timeout(int32_t id, int8_t * buf, uint32_t len, int32_t timeout) 
{ 
uint32_t qlen = sizeof(QUEUE_BUFF); 
    QUEUE_BUFF  qbuf = {0, NULL}; 
    printf("****at.linkid[id].qid=%d***\n",at.linkid[id].qid); 
    int ret = LOS_QueueReadCopy(at.linkid[id].qid, &qbuf, &qlen, timeout); 
    AT_LOG("ret = %x, len = %d, id = %d", ret, qbuf.len, id); 
    if (qbuf.len){ 
        memcpy(buf, qbuf.addr, qbuf.len); 
        atiny_free(qbuf.addr); 
    } 
    return qbuf.len; 
} 
int32_t sim900a_send(int32_t id , const uint8_t  *buf, uint32_t len) 
{ 
    int32_t ret = -1; 
    char cmd[64] = {0}; 
    if (AT_MUXMODE_SINGLE == at.mux_mode) 
    { 
        snprintf(cmd, 64, "%s=%d", AT_CMD_SEND, len); 
    } 
    else 
    { 
        snprintf(cmd, 64, "%s=%d,%d", AT_CMD_SEND, id, len); 
    } 
    ret = at.write((int8_t *)cmd, "SEND OK", (int8_t*)buf, len); 
    return ret; 
}
```

而SIM900A模块AT命令定义的宏在LiteOS/drivers/devices/gprs/sim900a.h定义如下，具体含义可以查看SIM900A官方手册。

```
#define AT_CMD_AT    		"AT" 
#define AT_CMD_CPIN         "AT+CPIN?"//check sim card 
#define AT_CMD_COPS         "AT+COPS?"//check register network 
#define AT_CMD_CLOSE    	"AT+CIPCLOSE" 
#define AT_CMD_SHUT    		"AT+CIPSHUT" 
#define AT_CMD_ECHO_OFF 	"ATE0" 
#define AT_CMD_ECHO_ON  	"ATE1" 
#define AT_CMD_MUX 		"AT+CIPMUX" 
#define AT_CMD_CLASS        "AT+CGCLASS"//set MS type 
#define AT_CMD_PDP_CONT   	"AT+CGDCONT"//configure pdp context #define #defineAT_CMD_PDP_ATT    	"AT+CGATT"//pdp attach network 
#define AT_CMD_PDP_ACT		"AT+CGACT"//active pdp context 
#define AT_CMD_CSTT		"AT+CSTT"//start task 
#define AT_CMD_CIICR		"AT+CIICR"//start gprs connect 
#define AT_CMD_CIFSR		"AT+CIFSR"//get local ip 
#define AT_CMD_CIPHEAD		"AT+CIPHEAD" 
#define AT_CMD_CONN		"AT+CIPSTART" 
#define AT_CMD_SEND		"AT+CIPSEND" 
#define AT_CMD_CLOSE		"AT+CIPCLOSE"
```

### 注意事项

由于LiteOS
SDK端云互通组件的任务是发送和接收在同一个任务中，接收消息的接口不能一直是阻塞的，而必须使用带有超时机制的接收接口，即我们总是实现int32_t
(\*recv_timeout)(int32_t id , int8_t \*buf, uint32_t len, int32_t
timeout);这个接口，且接收超时时间目前是10秒（\#define BIND_TIMEOUT (10)）。

如果用户设计的应用发送消息和接收消息在不同的任务中，那么可以使用阻塞接口int32_t
(\*recv)(int32_t id , int8_t \*buf, uint32_t len)。

## （参考）设备模拟器接入平台

#### 5.8.1 设备模拟器接入平台

设备接入IoT平台后，IoT平台才可以对设备进行管理。

IoT平台提供了设备模拟器，可以模拟真实设备接入IoT平台的场景。本节基于设备模拟器进行操作。

**步骤1** 选择“模拟器”-\>“NB设备模拟器”-\>“绑定设备”，输入验证码，点击“确定”。

输入的验证码需要和注册设备时使用的验证码（verifyCode）一致。

**设备模拟器**

![](./pic/sdk/41.png)

**步骤1** 选择“我的设备”，在设备列表中查看对应设备的状态。如果状态为“绑定（bound）”，则表示设备已经成功接入IoT平台。

**查看设备状态**

![](./pic/sdk/42.png)

----**结束**

#### 5.8.2 设备模拟器数据上报

设备在收到平台下发命令或者资源订阅后，会上报命令响应或资源订阅消息，由IoT平台将设备上报的消息推送到应用服务器或订阅的地址。如果上报数据的南向设备是NB-IoT设备或者使用LiteOS
SDK端云互通组件集成的设备，IoT平台在将消息推送到应用服务器或订阅的地址之前，会先调用编解码插件对消息进行解析。

IoT平台提供了设备模拟器，可以模拟真实设备上报数据的场景。本节基于NB设备模拟器（NB设备模拟器也可以模拟LiteOS
SDK端云互通组件 的数据上报功能）进行操作。

**步骤1** 登录IoT平台的开发者Portal。开发者Portal的访问地址、账号和密码需要向IoT平台服务商申请。

**步骤2** 选择“模拟器”-\>“NB设备模拟器”，输入需要上报的码流，点击“发送”。

在“设备日志信息”-\>“数据发送”中，可以查看数据上报信息。

在“设备日志信息”-\>“数据接收”中，可以查看数据上报响应信息。

**模拟数据上报**

![](./pic/sdk/43.png)

**步骤3** 在“我的设备”界面的设备列表中，选择上报数据的设备，查看“历史数据”，验证编解码插件是否可以对上报数据进行解析。

**查看数据上报结果**

![](./pic/sdk/44.png)

以一款Led灯设备的编解码插件为例进行说明，该款设备包含一种服务LightControl（设置多种服务包含多种属性、多种命令类似）：

-   LightControl服务：包含light一种属性（灯亮或者灭）和一种命令（设置灯亮或者灭）。

>   使用设备模拟器中上报“01”的十六进制码流后，在“历史数据中”获得的编解码插件解码结果将会为：

>   LightControl：{ "light": 1 }

**步骤4** 在“我的设备”界面的设备列表中，选择上报数据的设备，查看“历史数据”，验证设备数据上报的情况。

“历史数据”中显示为经过编解码插件解析后的结果。

----**结束**

#### 5.8.3 应用模拟器命令下发

应用服务器需要调用IoT平台的命令下发接口，对设备下发控制指令。如果接收命令的设备是NB-IoT设备（或者集成LiteOS
SDK端云互通组件的南向设备），IoT平台收到应用服务器下发的命令后，会先调用编解码插件进行转换，再发送给设备。

IoT平台提供了应用模拟器，可以模拟应用服务器下发命令的场景。本节基于应用模拟器进行操作。

**步骤1** 在“我的设备”界面的设备列表中，选择接收命令的设备，点击“命令下发（\</\>)”。

在弹出界面中，配置下发给设备的命令参数。

**命令下发**

![](./pic/sdk/45.png)

**步骤2** 在“我的设备”界面的设备列表中，选择接收命令的设备-\>“历史命令”，查看“状态”栏的显示。

**命令下发状态**

![](./pic/sdk/46.png)

状态说明如下：

* **超期：** 表示命令在IoT平台缓存时间超期，未向设备下发。

* **成功：** 表示IoT平台已经将命令下发给设备，且收到设备上报的命令执行结果。

* **失败：** 表示编解码插件解析为空，或执行结果响应里面有“ERROR CODE”等。

* **超时：** 表示IoT平台等待ACK响应超时。

* **取消：** 表示应用侧已经取消命令下发。

* **等待：** 表示命令在IoT平台缓存，还未下发给设备。

* **已发送：** 表示IoT平台已经将命令下发给设备。

* **已送达：** 表示IoT平台已经将命令下发给设备，且收到设备返回的ACK消息。

**步骤3** 选择“模拟器”-\>“NB设备模拟器”-\>“设备日志信息”-\>“数据接收”，查看设备模拟器收到的命令信息。

**命令接收信息**

![](./pic/sdk/47.png)

----**结束**