# LiteOS技术资料编写和部署说明

## 项目背景

Huawei LiteOS 技术社区一直都聚焦开源代码的维护和更新，相关配套资料却没能及时跟上。为了帮助大家快速掌握 Huawei LiteOS，我们新建此项目。

优美且强大的 vuepress 工具让我们可以专注编写 Huawei LiteOS 技术文档。我们利用该工具快速构建一套 GitHub Page 静态技术资料网站。

该技术网站会发布在：[https://liteos.github.io](https://liteos.github.io)，大家如果发现任何问题可以随时提交 ISSUE，我们会尽快修改。当然我们更欢迎大家与我们一起来完善社区资料。:+1:

## 使用说明

> vuepress 工具依赖 Node.js 8.0 以上版本。需提前[下载](https://npm.taobao.org/mirrors/node/)并安装好。

操作步骤如下：

```bash
# 安装对应平台的Node.js，版本大于8.0（Windows，Mac，Linux）
node -v

# 切换到LiteOS_Documents根目录，执行 npm install，安装配套组件
npm install

# 执行 npm run dev 开始写作，可以打开本地浏览器实时查看编辑结果，默认地址为 http://localhost:8080
npm run dev

# 执行 npm run build 构建静态网站，将 .vuepress/dist 目录拷贝到 xxx.github.io 项目中，即可以完成静态网站部署（默认我们会定期同步到 liteos.github.io 中）
npm run build

# 我们为了方便您本地进行演示，还集成了一套本地的 HTTP Server，执行 npm run start，可以启动 http 服务器进行本地发布。
npm run start

```
## 小建议

建议大家使用 [Visual Studio Code](https://code.visualstudio.com/) 进行开发，其内置了 git 和 命令行终端，可以让你在一个统一的环境下进行编写，无需安装其他软件。而且经过我们长期使用，在该环境中运行 vuepress 稳定性较高。

> 注意 Windows 环境下需要提前安装好 [Git for windows](https://git-scm.com/downloads) 工具，VSC 会自动识别并将该工具集成到内部。其他 OS（Mac 和 Linux）都内置了 git 工具，可以不必安装。