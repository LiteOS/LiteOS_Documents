# 步骤10：自制 HTTPS/SSL 证书，实现平台数据订阅/发布功能

In step9, we have finished almost all the code. We can create a device, bind to the IoT platform and query history data. But when device send data to the platform, how does the web app know the button status has been changed?
In this step, we will learn subscribe API.

## 10.1	Make HTTPS/SSL certification files using OpenSSL tools

Installing OpenSSL for you OS

1)	Create private key file

`openssl genrsa -out server.key 1024`

![](./image/openssl-genrsa.png)

2)	Generating CSR certificate signature through private key

`openssl req -config ./cnf/openssl.cnf -new -key server.key -out server.csr`

![](./image/openssl-gencsr.png)

3)	Generating certificate files through private keys and certificate signatures

`openssl x509 -req -in server.csr -signkey server.key -out server.crt`

![](./image/openssl-gencrt.png)

4)	Write a demo https server and verifying certificate files
Copy server.key and server.crt to one temporary directory, copy the a demo https server, and run it(node app.js).

![](./image/nodejs-code-server-ssl-demo.png)

![](./image/nodejs-ssl-server-page.png)

## 10.2	Make localhost server through NAT, let IoT platform can push data.

![](./image/ngrok-nat-page.png)

## 10.3	Implementing Subscribe to IoT platform data

1)	Download nginx stable version in http://nginx.org/ and install it for you OS.

![](./image/nginx-download-page.png)

Copy server.crt and server.crt to nginx’sconf directory.

![](./image/nginx-ssl-conf.png)

Write these lines to nginx.conf

![](./image/nginx-ssl-conf-edit.png)

![](./image/nginx-start.png)

![](./image/mongodb-verify-page.png)

2)	Setting up the NAT address for your server

![](./image/nginx-nat-conf.png)

![](./image/nginx-ssl-verify-page.png)

3)	Add /callback router in devces.js

![](./image/nodejs-code-callback.png)

4)	Login Huawei IoT Developer Portal, and go to Application Subscription Page

![](./image/oceanconnect-application-subscribe.png)

Fill the callback url in the blank, and then system will check it.

![](./image/oceanconnect-deviceDataChanged-callback-page.png)

![](./image/oceanconnect-deviceDataChanged-verify-page.png)

5)	When a new data will be sent to IoT platform, it will notify to web service as followed. You also could push message to Front-end with WebSockets.

![](./image/nodejs-device-callback.png)

That it is.