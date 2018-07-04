# NB-IoT OneButton 一键式报警开发指导 <Badge text="beta" type="warn"/><Badge text="0.1"/>

## 概述

通过这个手把手教程，我们将帮助您利用华为的NB-IOT解决方案从嵌入式设备开发到Web应用程序开发。只需 10 步，您就能基于 NB-IoT 开发板和 Node.js Web 应用开发出一套 NB-IoT 一键式报警产品。

完成这套教程后，您可以实现以下功能：

当您按下报警按键，会点亮报警LED灯，同时将按键状态（开/关）通过 NB-IoT 网络上报到火警中心。在这个场景中，我们将使用 NB-IoT 开发板模拟开发火警按钮，以及开发配套的后台 Web应用模拟消防中心。

![一键式报警场景](./image/scene.png)

请系好安全带，我们马上就要进入 NB-IoT 的世界了。

## 你将学到什么

- 首先基于 NB-IoT 开发板，我们将使用一系列开发工具，比如 STM32CubeMX，Keil/Sw4STM32，QCom，
- 快速理解并掌握华为 IoT 全套开发流程
- 最后，我们还会手把手教会您基于 Node.js 和 MongoDB 开发出后台 Web 服务器

## 你要准备什么

提前下好如下工具和驱动，为了减少篇幅，我们不会过多涉及安装，如果您遇到安装问题，请上百度/谷歌进行搜索，或者发[邮件](mailto:Jeffrey.fu@huawei.com)给我

-	NB-IoT evaluation kit
-	ST-Link v2 in-circuit debugger/programmer
-	CH340 USB to TTL dervier
-	STM32CubeMX 4.25.0 (java 1.7 above)
-	SW4STM32 2.4
-	Node.js 8.11.1
- MongoDB 3.6
-	Postman 6.0.1
-	Nginx 1.14.0
-	OpenSSL 1.1.0

## 开发板选择

目前我们已经有了 4 块 NB-IoT 开发板，开发者可以根据手头上的开发板选择对应的教程。

| Tables        | Are           | Cool  |
| ------------- |:-------------:| -----:|
| Terabits NB-IoT     | right-aligned | $1600 |
| YiQi NB-IoT      | centered      |   $12 |
| IoTClub NB-IoT | are neat      |    $1 |