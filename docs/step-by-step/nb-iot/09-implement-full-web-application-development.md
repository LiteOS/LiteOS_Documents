# 步骤9：对接 OceanConnect IoT平台，实现后台 Web 服务全功能

## 9.1	Implementing access to Huawei IoT platform for the access-token

1)	Install request module

![](./image/nodejs-install-request-mod.png)

![](./image/nodejs-package-request.png)

2)	Create a directory named iotplatform, and ssl subdirectory of iotplatform

![](./image/nodejs-auth-ssl.png)

3)	Unzip client.crt and client.key in the ssl directory

![](./image/nodejs-client-key-files.png)

4)	Add conf.js for configuration to IoT platform

There are params come from IoT Developer Portal

![](./image/nodejs-code-default-conf.png)

![](./image/oceanconnect-application-details.png)

5)	Add auth.js file, which is used to connect IoT Platform and get accessToken.

![](./image/nodejs-code-auth-fetch-access-token.png)

When you run “npm start”, you can get the accesToken as followed:

![](./image/nodejs-auth-fetch-access-token.png)

6)	Add dm.js file, which is used to register device.

## 9.2	Implementing bind, unbind device to Huawei IoT platform

1)	Access register device and implementing bind device function

http://developer.huawei.com/ict/en/doc/site-oceanconnect-northbound_api_reference-en/index.html/en-us_topic_0103207034

You can look through this page above and build a request message.

![](./image/huawei-developer-oceanconnect-north-api-reg.png)

![](./image/nodejs-code-device-register.png)

2)	Access delete device and implementing unbind device function

http://developer.huawei.com/ict/en/doc/site-oceanconnect-northbound_api_reference-en/index.html/en-us_topic_0103207003

You can look through this page above and build a request message.

![](./image/huawei-developer-oceanconnect-north-api-delete.png)

![](./image/nodejs-code-device-delete.png)

## 9.3	Implementing query history data of device

http://developer.huawei.com/ict/en/doc/site-oceanconnect-northbound_api_reference-en/index.html/en-us_topic_0103207027

You can look through this page above and build a request message.

![](./image/huawei-developer-oceanconnect-north-api-query.png)

![](./image/nodejs-code-device-query.png)

When we register a new device, the IoT platform don’t know which type you have. So we have to access “modifying device information” openAPI to tell the platform which type you have.

![](./image/nodejs-code-define-device-info.png)

http://developer.huawei.com/ict/en/doc/site-oceanconnect-northbound_api_reference-en/index.html/en-us_topic_0103207033

You can look through this page above and build a request message.

![](./image/huawei-developer-oceanconnect-north-api-update.png)

![](./image/nodejs-code-device-update.png)

Finally, we finish bind/unbind/query router function.

1)	Implement and test device restful API for Front-end app

| Tables        | Are           | Cool  |
| ------------- |:-------------:| -----:|
| POST     | `http://url/devices/bind/:id` | Bind a device to IoT platform |
| DELETE      | `http://url/devices/unbind/:id`      |   Unbind a device to IoT platform |
| GET | `http://url/devices/:id`      |    Get a History data of the device |

-	Bind a new device

![](./image/nodejs-code-device-bind.png)

-	Unbind a device

![](./image/nodejs-code-device-unbind.png)

-	Get device history data

![](./image/nodejs-code-device-list.png)

2)	Using Postman to verity the functions

![](./image/postman-device-bind-post.png)

![](./image/postman-device-bind-result.png)

![](./image/postman-device-query-get.png)

![](./image/postman-device-query-result.png)

::: warning
If you create a new device, there is no data report of the device. So you’d better use the simulator in the developer portal. You could find it as followed:
![](./image/postman-device-query-no-data.png)
:::