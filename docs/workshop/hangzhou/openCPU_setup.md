# openCPU开发环境安装指导

本文档旨在帮助开发者配置openCPU开发环境。文档中涉及的安装包都在开发环境工具包中给出。安装时请尽量安装到默认路径，否则可能需要开发者手动添加环境变量。
## 安装GCC Compiler
GCC Compiler安装包为gcc-arm-none-eabi-4_9-2015q2-20150609-win32.exe。打开安装程序，按照默认安装即可。

![1](./pic/opencpu_setup_1.png)

![2](./pic/opencpu_setup_2.png)

![3](./pic/opencpu_setup_3.png)

![4](./pic/opencpu_setup_4.png)

注意，在最后一步中，需要勾选add path to environment variable。否则需要手动将GCC Compiler安装路径添加到系统环境变量，如下图所示。

![5](./pic/opencpu_setup_5.png)

## 安装自动化构建工具Scons
Scons构建工具通过Python2.7编写，其它Python版本可能不兼容。因此需要先安装Python2.7。开发者可以通过开发环境工具包中的python-2.7.10.msi来安装。

![6](./pic/opencpu_setup_6.png)

![7](./pic/opencpu_setup_7.png)

![8](./pic/opencpu_setup_8.png)

注意上图，这里需要勾选添加路径。

![9](./pic/opencpu_setup_9.png)

![10](./pic/opencpu_setup_10.png)

至此完成了Python2.7的安装。但是，默认情况下Python脚本无法访问Windows系统API库，因此需要安装pywin32库。开发者可以通过开发环境工具包中的pywin32-220.win32-py2.7.exe来安装。

![11](./pic/opencpu_setup_11.png)

![12](./pic/opencpu_setup_12.png)

![13](./pic/opencpu_setup_13.png)

![14](./pic/opencpu_setup_14.png)

现在，开发者可以通过开发环境工具包中的scons-2.4.0-setup.exe来安装Scons，安装过程与pywin32类似。

![15](./pic/opencpu_setup_15.png)

![16](./pic/opencpu_setup_16.png)

![17](./pic/opencpu_setup_17.png)

![18](./pic/opencpu_setup_18.png)

## 安装Microsoft .NET Framework

开发者可通过开发环境工具包中NDP452-KB2901907-x86-x64-AllOS-ENU.exe安装。按照默认安装即可。

![19](./pic/opencpu_setup_19.png)

![20](./pic/opencpu_setup_20.png)

![21](./pic/opencpu_setup_21.png)

## 安装UEMonitor-3.22.0.14
通过开发环境工具包中提供的UEMonitor-3.22.0.14.msi进行安装。类似于串口调试工具，开发者可以通过该软件观察串口日志。

![22](./pic/opencpu_setup_22.png)

![23](./pic/opencpu_setup_23.png)

![24](./pic/opencpu_setup_24.png)

## 安装UEUpdaterUI-3.22.0.14

开发者可以通过该软件，从openCPU模组的主串口进行固件升级。安装包为UEUpdaterUI-3.22.0.14.msi。

![25](./pic/opencpu_setup_25.png)

![26](./pic/opencpu_setup_26.png)

## 安装UpdatePackage-3.22.0.14

通过UpdatePackage-3.22.0.14.msi进行安装。

![27](./pic/opencpu_setup_27.png)

![28](./pic/opencpu_setup_28.png)

## 验证开发环境

完成上述环境搭建步骤后，开发者可以通过开发环境工具包中的样例工程来验证开发环境是否搭建成功。比如，打开example目录下的OLED文件夹，打开其中的“命令提示符”，并输入命令```scons_new.bat```：

![33](./pic/opencpu_setup_33.png)

![30](./pic/opencpu_setup_30.png)

若环境搭建成功，则会输出：

![31](./pic/opencpu_setup_31.png)

并且在工程目录下，会生成new.fwpkg文件。该文件就是openCPU方案需要烧录到模组中的firmware package。

![32](./pic/opencpu_setup_32.png)

若得到了上面的结果，则说明环境搭建成功。

## 常见问题及解决方法

### 报 arm-none-eabi-gcc 错误

打开“命令提示符”，并输入命令```scons_new.bat```，出现```No such file or directory```的错误：

![34](./pic/opencpu_setup_34.png)

解决方案：
1. 查看系统环境变量是否正确配置。

2. 查找电脑中是否有文件与Compiler-GCC(GNU Tools ARM Embedded)重名。若有请将其删除。

### 路径有误
若开发者在安装过程中修改了默认的安装路径，需要在scons_new.bat中的路径做相应的修改。

![35](./pic/opencpu_setup_35.png)

### 路径中包含中文
若工程目录中含有中文，则编译过程中会出现下面的错误：

![36](./pic/opencpu_setup_36.png)

解决方案：

* 使用全英文路径。