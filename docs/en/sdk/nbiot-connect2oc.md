# Introduction

Although there are various NB-IoT module vendors, such as Lierda, Quectel, and u-blox, NB-IoT chip vendors are few. NB-IoT chips can be connected to OceanConnect, and this document will introduce three types of NB-IoT chips. If NB-IoT modules has a reference manual, use the reference manual; or else, you could use this document as a reference.

If NB-IoT chips, such as HiSilicon chips, support OceanConnect, you could just follow their introduction. If NB-IoT chips support the standard LWM2M, you could add the object 19. If NB-IoT chips support the UDP, you could use Huawei LiteOS that well implements the LWM2M or CoAP and adapts to OceanConnect. The following figure shows what you need to do with three types of NB-IoT chips.

Figure 1-1 What you need to do with three types of NB-IoT chips

![](./figures/en-us_image_0150623379.png)

The following chapter takes three examples.

## Examples ##

<h2 id="NB-IoT-chips-support-oceanconnect.md">NB-IoT Chips Support OceanConnect</h2>

This example refers to the Quectel AT command reference manual.

All HiSilicon chips support OceanConnect and may have the same process and instructions. Therefore, you could connect the NB-IoT chips to OceanConnect in the following way: Obtain the IMEI number, and use it as the verification code to create a device on OceanConnect.

```

AT+CGSN=1

+CGSN:XXXXXXXXX
OK
//Query the IMEI number. If only "ERROR" is returned, customers need to write the IMEI number. This only happens in early versions. After obtaining the IMEI number, you could use it as the verification code to register a device with the specified profile.

```

Set the band. When obtaining the SIM card from a device with NB-IoT modules, you need to check the band of the SIM card and the corresponding module. For example, if the band of an SIM card is 20, and the module supports bands 5, 6, 8, and 20, set the band corresponding to the module to 20. However, you need to disable the UE function.

```

AT+CFUN=0            //Disable the UE function.

OK

AT+NBAND=20  //Set the band corresponding to the module to 20.

OK

AT+CFUN=1   //Enable the UE function.

OK
AT+NCONFIG=AUTOCONNECT,TRUE  //Set the UE to the automatic mode. When the module powers on, it connects to the network and is activated.
OK

AT+NRB                               //Reset the UE, and keep the setting.
```

When the module powers on, it is automatically connected to the network. You could execute AT commands to check whether the module has been connected to the network.

```


AT+CGATT?                     //Check whether the UE is connected to the network.

+CGATT:1                   //1: connected; 0: disconnected, it takes a few seconds to connect.


OK

```

After the module is connected to the network, you could use the network functions, for example, querying the server IP address with the domain name. If the AT+NCDP command does not support the domain name, you could use DNS to get one. HiSillicon chips support the domain name. Therefore, you do not need to query the IP address. Next, you need to set the CDP server properly.

```

AT+NCDP=device.hw-oc.com,5683 //Set the CDP server with the domain name or the fixed IP address (if any).

OK

AT+NNMI=1                        //Set the reporting function to the automatic mode, which indicates that if any message reaches the module, the module automatically reports.
OK

+QLWEVTIND:0                  //Register data with the server successfully.
+QLWEVTIND:3                  //The object 19 has been found on OceanConnect. When the module reports this message, the data is sent to OceanConnect.

```

After you connect to the IoT server, you could report data to OceanConnect or receive commands from OceanConnect.

```

AT+NMGS=3,313233   //The format is AT+NMGS = len,hexdata (ascii format). 31: represents the hex byte 0x31. 3: You have sent 3 bytes including 0x31, 0x32, and 0x33.

OK

```

If receiving data from OceanConnect, the module reports the data to the AT interface.

```

+NNMI:2,4F4E   //The format is +NNMI:len,hexdata(asci format), which means that the server sends 2 bytes including 0x4F and 0x4E.
OK

```

Remember that Boudica 120 \(and earlier versions\) uses CoAP while Boudica 150 uses LWM2M. You could refer to NB-IoT DRIVER from LiteOS. LiteOS supports the AT framework, which simplifies the processing of AT commands.

<h2 id="NB-IoT-chips-support-the-standard-lwm2m.md">NB-IoT Chips Support the Standard LWM2M</h2>

If NB-IoT chips support the standard LWM2M, you could add the object 19. The module can use the object 19 to report data to OceanConnect, and OceanConnect sends commands to the object 19. The following uses MT2625\(MTK\) as an example:

```

AT+CGPADDR=1        //Check the IP address allocated for the default PDN.
+CGPADDR: 1,10.52.241.198
OK
//Set the IP address and port for OceanConnect.
//If you need to connect to another IoT platform, execute the AT+QLWDEL command before setting the IP address and port.
AT+QLWSERV="xx.xxx.xx.xx",5683
OK
AT+QLWCONF="867724030023557"//Set the IMEI number.
OK
AT+QLWADDOBJ=19,0,1,"0"//Add an LWM2M object 19/0/0.
OK
AT+QLWADDOBJ=19,1,1,"0"//Add an LWM2M object 19/1/0.
OK
AT+QLWOPEN=0 //Register the LWM2M objects 19/0/0 and 19/1/0 with OceanConnect in direct push mode.
OK
CONNECT OK //Registered successfully.
+QLWOBSERVE: 0,19,0,0 //Receive the request for observing the object 19/0/0.
AT+QLWCFG="dataformat",1,1 //Configure the hex string mode for sending and receiving data.
OK
//Send the NON message to OceanConnect, including 10 hex bytes (0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, and 0xA)
AT+QLWDATASEND=19,0,0,10,0102030405060708090A ,0x0000
OK
+QLWDATARECV: 19,1,0,4,AAAA0000 //Receive data from OceanConnect.
//Send the CON message to OceanConnect, and wait for an ACK response from OceanConnect.
AT+QLWDATASEND=19,0,0,10,0102030405060708090A ,0x0100
OK
SEND OK //The message has been successfully sent.
+QLWDATARECV: 19,1,0,4,AAAA0000 //Receive data from OceanConnect.
AT+QLWDATASTATUS?//Query the sending status of the CON message.
+QLWDATASTATUS: 4 //The message has been successfully sent.
OK
AT+QLWCLOSE //Deregister from OceanConnect.
OK
CLOSE OK //Deregistered successfully.
AT+QLWDEL //Delete the LWM2M context.
OK

```

Some NB-IoT chips do not support IoT setting with the domain name. If only the domain name has been provided for the server, you could decode the domain name with DNS based on the UDP.

<h2 id="nb-chips-support-the-udp.md">NB-IoT Chips Support the UDP</h2>

For NB chips supporting the UDP, to connect to NB-IoT modules, MCUs must implement LWM2M, and Huawei LiteOS can implement it and adapt to OceanConnect. Therefore, you can use Huawei LiteOS. To obtain more details about Huawei LiteOS, visit  [https://liteos.github.io/en/kernel/](https://liteos.github.io/en/kernel/).

and how to connect Huawei IoT platform,please visit [https://liteos.github.io/en/sdk/device-cloud-interconnect.html#overview](https://liteos.github.io/en/sdk/device-cloud-interconnect.html#overview).
