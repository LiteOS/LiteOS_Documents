# 步骤9：对接 OceanConnect IoT平台，实现后台 Web 服务全功能

## 9.1	Implementing access to Huawei IoT platform for the access-token

1)	Install request module

![](./pic/nodejs-install-request-mod.png)

![](./pic/nodejs-package-request.png)

2)	Create a directory named iotplatform, and ssl subdirectory of iotplatform

![](./pic/nodejs-auth-ssl.png)

3)	Unzip client.crt and client.key in the ssl directory

![](./pic/nodejs-client-key-files.png)

4)	Add conf.js for configuration to IoT platform

There are params come from IoT Developer Portal

![](./pic/nodejs-code-default-conf.png)

![](./pic/oceanconnect-application-details.png)

5)	Add auth.js file, which is used to connect IoT Platform and get accessToken.

![](./pic/nodejs-code-auth-fetch-access-token.png)

When you run “npm start”, you can get the accesToken as followed:

![](./pic/nodejs-auth-fetch-access-token.png)

6)	Add dm.js file, which is used to register device.

## 9.2	Implementing bind, unbind device to Huawei IoT platform

1)	Access register device and implementing bind device function

http://developer.huawei.com/ict/en/doc/site-oceanconnect-northbound_api_reference-en/index.html/en-us_topic_0103207034

You can look through this page above and build a request message.

![](./pic/huawei-developer-oceanconnect-north-api-reg.png)

![](./pic/nodejs-code-device-register.png)

2)	Access delete device and implementing unbind device function

http://developer.huawei.com/ict/en/doc/site-oceanconnect-northbound_api_reference-en/index.html/en-us_topic_0103207003

You can look through this page above and build a request message.

![](./pic/huawei-developer-oceanconnect-north-api-delete.png)

![](./pic/nodejs-code-device-delete.png)

## 9.3	Implementing query history data of device

http://developer.huawei.com/ict/en/doc/site-oceanconnect-northbound_api_reference-en/index.html/en-us_topic_0103207027

You can look through this page above and build a request message.

![](./pic/huawei-developer-oceanconnect-north-api-query.png)

![](./pic/nodejs-code-device-query.png)

When we register a new device, the IoT platform don’t know which type you have. So we have to access “modifying device information” openAPI to tell the platform which type you have.

![](./pic/nodejs-code-define-device-info.png)

http://developer.huawei.com/ict/en/doc/site-oceanconnect-northbound_api_reference-en/index.html/en-us_topic_0103207033

You can look through this page above and build a request message.

![](./pic/huawei-developer-oceanconnect-north-api-update.png)

![](./pic/nodejs-code-device-update.png)

Finally, we finish bind/unbind/query router function.

1)	Implement and test device restful API for Front-end app

| Tables        | Are           | Cool  |
| ------------- |:-------------:| -----:|
| POST     | `http://url/devices/bind/:id` | Bind a device to IoT platform |
| DELETE      | `http://url/devices/unbind/:id`      |   Unbind a device to IoT platform |
| GET | `http://url/devices/:id`      |    Get a History data of the device |

-	Bind a new device

![](./pic/nodejs-code-device-bind.png)

-	Unbind a device

![](./pic/nodejs-code-device-unbind.png)

-	Get device history data

![](./pic/nodejs-code-device-list.png)

2)	Using Postman to verity the functions

![](./pic/postman-device-bind-post.png)

![](./pic/postman-device-bind-result.png)

![](./pic/postman-device-query-get.png)

![](./pic/postman-device-query-result.png)

::: warning
If you create a new device, there is no data report of the device. So you’d better use the simulator in the developer portal. You could find it as followed:
![](./pic/postman-device-query-no-data.png)
:::