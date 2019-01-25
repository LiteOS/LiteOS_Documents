# 术语


### LiteOS SDK 相关术语 ###

<a name="table19262791"></a>
<table><thead align="left"><tr id="row49671007"><th class="cellrowborder" valign="top" width="5%" id="mcps1.1.4.1.1"><p id="p63928619"><a name="p63928619"></a><a name="p63928619"></a>序号</p>
</th>
<th class="cellrowborder" valign="top" width="13%" id="mcps1.1.4.1.2"><p id="p10835611"><a name="p10835611"></a><a name="p10835611"></a>术语名称</p>
</th>
<th class="cellrowborder" valign="top" width="82%" id="mcps1.1.4.1.3"><p id="p5269271"><a name="p5269271"></a><a name="p5269271"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="row24157832"><td class="cellrowborder" valign="top" width="5%" headers="mcps1.1.4.1.1 "><p id="p10627414"><a name="p10627414"></a><a name="p10627414"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.1.4.1.2 "><p id="p55514216"><a name="p55514216"></a><a name="p55514216"></a>LiteOS SDK</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.4.1.3 "><p id="p357671"><a name="p357671"></a><a name="p357671"></a>LiteOS SDK是Huawei LiteOS软件开发工具包（Software Development Kit），包括端云互通组件、FOTA、JS引擎、传感框架等内容。</p>
</td>
</tr>
<tr id="row3219042"><td class="cellrowborder" valign="top" width="5%" headers="mcps1.1.4.1.1 "><p id="p59415887"><a name="p59415887"></a><a name="p59415887"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.1.4.1.2 "><p id="p47957532"><a name="p47957532"></a><a name="p47957532"></a>南向设备</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.4.1.3 "><p id="p59354865"><a name="p59354865"></a><a name="p59354865"></a>用于采集数据的嵌入式设备，比如STM32开发板，或者温湿度采集的传感器等。</p>
</td>
</tr>
<tr id="row64431743"><td class="cellrowborder" valign="top" width="5%" headers="mcps1.1.4.1.1 "><p id="p51588660"><a name="p51588660"></a><a name="p51588660"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.1.4.1.2 "><p id="p17931909"><a name="p17931909"></a><a name="p17931909"></a>北向应用</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.4.1.3 "><p id="p43198556"><a name="p43198556"></a><a name="p43198556"></a>用于查看IoT云平台上南向设备上报数据或者给南向设备下发控制命令的手机APP或者PC端的应用程序。</p>
</td>
</tr>
<tr id="row53242684"><td class="cellrowborder" valign="top" width="5%" headers="mcps1.1.4.1.1 "><p id="p17690132"><a name="p17690132"></a><a name="p17690132"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.1.4.1.2 "><p id="p23614564"><a name="p23614564"></a><a name="p23614564"></a>设备profile</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.4.1.3 "><p id="p33731541"><a name="p33731541"></a><a name="p33731541"></a>用于描述南向设备具有的能力以及上报的数据的格式的一组json格式文件，这些文件需要上传到IoT云平台上。</p>
</td>
</tr>
<tr id="row35148417"><td class="cellrowborder" valign="top" width="5%" headers="mcps1.1.4.1.1 "><p id="p28449542"><a name="p28449542"></a><a name="p28449542"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.1.4.1.2 "><p id="p22711551"><a name="p22711551"></a><a name="p22711551"></a>编解码插件</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.4.1.3 "><p id="p27696341"><a name="p27696341"></a><a name="p27696341"></a>用于将南向设备上报的私有数据解析成设备profile描述的并且IoT云平台能够识别和存储的数据，以及将北向应用下发的命令编码成南向设备能够识别的数据格式的一组函数组成的jar文件。简而言之就是南向设备和云平台之间的一个数据转换的程序。</p>
</td>
</tr>
<tr id="row47940478"><td class="cellrowborder" valign="top" width="5%" headers="mcps1.1.4.1.1 "><p id="p57973481"><a name="p57973481"></a><a name="p57973481"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.1.4.1.2 "><p id="p65340389"><a name="p65340389"></a><a name="p65340389"></a>AT指令</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.4.1.3 "><p id="p58080153"><a name="p58080153"></a><a name="p58080153"></a>AT指令集是从终端设备（Terminal Equipment，TE)或数据终端设备（Data Terminal Equipment，DTE)向终端适配器(Terminal Adapter，TA)或数据电路终端设备(Data Circuit Terminal Equipment，DCE)发送的。本文通过AT指令操作WIFI或者GSM模组。</p>
</td>
</tr>
<tr id="row52959336"><td class="cellrowborder" valign="top" width="5%" headers="mcps1.1.4.1.1 "><p id="p61847844"><a name="p61847844"></a><a name="p61847844"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.1.4.1.2 "><p id="p43619501"><a name="p43619501"></a><a name="p43619501"></a>端云互通组件</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.4.1.3 "><p id="p43518698"><a name="p43518698"></a><a name="p43518698"></a>端云互通组件是华为物联网解决方案中，资源受限终端对接到 IoT云平台的重要组件。</p>
</td>
</tr>
<tr id="row56123968"><td class="cellrowborder" valign="top" width="5%" headers="mcps1.1.4.1.1 "><p id="p49747559"><a name="p49747559"></a><a name="p49747559"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.1.4.1.2 "><p id="p3020470"><a name="p3020470"></a><a name="p3020470"></a>OceanConnect</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.4.1.3 "><p id="p43331542"><a name="p43331542"></a><a name="p43331542"></a>华为IoT联接管理平台（IoT Connection Management Platform）是面向运营商和企业/行业领域的统一开放云平台，支持SIM和非SIM场景的各种联接和联接管理。</p>
</td>
</tr>
<tr id="row1447492653213"><td class="cellrowborder" valign="top" width="5%" headers="mcps1.1.4.1.1 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.1.4.1.2 "><p id="p17474152611323"><a name="p17474152611323"></a><a name="p17474152611323"></a>OTA</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.4.1.3 "><p id="p1874234319329"><a name="p1874234319329"></a><a name="p1874234319329"></a>SOTA是指Over the Air Technology，空中下载技术，是通过移动通信（GSM、NB-IoT等）的空中接口对通信模组及应用进行远程管理的技术，OTA包括SOTA升级和FOTA升级。</p>
</td>
</tr>
<tr id="row19515184114239"><td class="cellrowborder" valign="top" width="5%" headers="mcps1.1.4.1.1 "><p id="p13516341142319"><a name="p13516341142319"></a><a name="p13516341142319"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.1.4.1.2 "><p id="p1551624111237"><a name="p1551624111237"></a><a name="p1551624111237"></a>SOTA</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.4.1.3 "><p id="p1451618416237"><a name="p1451618416237"></a><a name="p1451618416237"></a>SOTA是指Software Over The Air，通过华为自研的PCP升级协议，加上内置LwM2M协议的NB-IoT模组，实现对第三方MCU的升级；</p>
</td>
</tr>
<tr id="row1925014518235"><td class="cellrowborder" valign="top" width="5%" headers="mcps1.1.4.1.1 "><p id="p225004532320"><a name="p225004532320"></a><a name="p225004532320"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="13%" headers="mcps1.1.4.1.2 "><p id="p18250184514233"><a name="p18250184514233"></a><a name="p18250184514233"></a>FOTA</p>
</td>
<td class="cellrowborder" valign="top" width="82%" headers="mcps1.1.4.1.3 "><p id="p525084502314"><a name="p525084502314"></a><a name="p525084502314"></a>FOTA是指Fireware Over The Air， 通过NB-IoT模组内置LwM2M协议的5号对象，实现对模组本身的升级。</p>
</td>
</tr>
</tbody>
</table>

>![](./public_sys-resources/icon-note.gif) **说明：**   
>编解码插件和设备profile是一对一的关系，即一个设备profile只能对应一个编解码插件。APP和编解码插件的关系是一对多的关系，即一个APP下面可以同时关联多个编解码插件。  