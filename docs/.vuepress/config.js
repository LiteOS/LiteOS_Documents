module.exports = {
  locales: {
    "/": {
      lang: "zh-CN",
      title: "Huawei LiteOS | 中文网",
      description: "轻量级物联网操作系统，物的智能化使能平台"
    },
    "/en/": {
      lang: "en-US",
      title: "Huawei LiteOS | English Site",
      description: "A Lightweight IoT Operating System that Makes Everything Around Us Smart"
    }
  },
  themeConfig: {
    repo: "LiteOS/LiteOS_Documents",
    repoLabel: "Github",
    docsDir: "doc",
    editLinks: true,
    locales: {
      "/": {
        selectText: "语言",
        label: "简体中文",
        editLinkText: "在 GitHub 上编辑此页",
        lastUpdated: "上次更新",
        algolia: {},
        nav: [{
            text: "学习",
            items: [{
                text: "入门",
                link: "/quick-start/"
              },
              {
                text: "内核",
                link: "/kernel/"
              },
              {
                text: "组件",
                link: "/components/"
              },
              {
                text: "移植",
                link: "/porting/"
              },
              {
                text: "教程",
                link: "/tutorials/"
              }
            ]
          },
          {
            text: "生态",
            items: [{
                text: "手把手教程",
                items: [{
                  text: "NB-IoT@LiteOS 应用开发",
                  link: "/step-by-step/nb-iot/"
                },{
                  text: "ESP8266@LiteOS 应用开发",
                  link: "/step-by-step/esp8266/"
                }]
              },
              {
                text: "对接第三方 IoT 平台",
                items: [{
                  text: "中移动 OneNet 云平台",
                  link: "/connect-to-cloud/onenet/"
                },{
                  text: "阿里云 IoT 平台",
                  link: "/connect-to-cloud/aliyun/"
                }]
              },
              {
                text: "高级应用专题",
                items: [{
                  text: "NB-IoT OpenCPU 开发",
                  link: "/opencpu/"
                }, {
                  text: "钛云物联 All-in-Java 开发",
                  link: "/all-in-java/"
                }, {
                  text: "MapleJS 智能家开发",
                  link: "/maplejs/"
                }]
              }
            ]
          },
          {
            text: "视频",
            items: [{
                text: "Huawei LiteOS 源码深度剖析",
                link: "/video/source-code-analysis/"
              }, {
                text: "朱老师 NB476 开发板学习",
                link: "/video/connect-to-cloud/"
              }]
          },
          {
            text: "活动",
            items: [
              {
                text: "2018 华为开发者大赛",
                link: "http://developer.huawei.com/ict/cn/hdc/"
              }
            ]
          }
        ],
        sidebar: {
          "/quick-start/": [{
              title: "入门",
              collapsable: false,
              children: [
                "",
                "./terminology",
                "./why-use-the-rtos",
                "./why-select-huawei-liteos"
              ]
            },
            {
              title: "快速上手",
              collapsable: true,
              children: [
                "./a-simple-blink-demo",
                "./a-comprehensive-smoke-demo"
              ]
            }
          ],
          "/kernel/": [{
              title: "内核",
              collapsable: false,
              children: [""]
            },
            {
              title: "基础",
              collapsable: true,
              children: [
                "./task",
                "./memory",
                "./interrupt",
                "./queue",
                "./event",
                "./mutex",
                "./semaphore",
                "./time-magmt",
                "./swtmr",
                "./list"
              ]
            },
            {
              title: "扩展",
              collapsable: true,
              children: ["./low-power"]
            },
            {
              title: "调试指南",
              collapsable: true,
              children: ["./faq"]
            }
          ],
          "/components/": [{
            title: "组件",
            collapsable: false,
            children: [
              "",
              "./oceanconnect-agenttiny",
              './fota',
              './at-adaptor-framework',
              './maplejs',
              './sensors-framework'
            ]
          }],
          "/porting/": [{
              title: "准备工作",
              collapsable: false,
              children: [
                "", 
                "./evb-and-tools", 
                "./raw-project-with-stm32cube",
                //"./raw-project-with-others",
                "./liteos-source-code",
              ]
            },
            {
              title: "OS 移植",
              collapsable: false,
              children: [
                "./stm32", 
                //"./nxp", 
                //"./gd32", 
                "./faq"]
            },
            {
              title: "移植后验证",
              collapsable: false,
              children: [
                "./inspect-example", 
                "./inspect-results"]
            },
            {
              title: "附1：开发工具安装手册",
              collapsable: false,
              children: [
                "./keil", 
                "./iar", 
                "./gcc-sw4stm32"]
            },
            {
              title: "附2：OS_CONFIG 系统配置详解",
              collapsable: false,
              children: ["./os-config"]
            }
          ],
          "/tutorials/": [{
              title: "教程",
              collapsable: false,
              children: [""]
            },
            {
              title: "内核例程",
              collapsable: true,
              children: [
                "./example01",
                "./example02",
                "./example03",
                "./example04",
                "./example05",
                "./example06",
                "./example07",
                "./example08",
                "./example09",
                "./example10"
              ]
            },
            {
              title: "综合例程",
              collapsable: true,
              children: ["./temp-hum", "./smoke", "./gps", "./light", "./wifi"]
            }
          ],
          "/case-study/": [{
            title: "案例",
            collapsable: false,
            children: [
              "",
              "./meter",
              "./goods",
              "./ofo",
              "./smoke",
              "./track"
            ]
          }],
          "/step-by-step/nb-iot/": [{
              title: "手把手教程",
              collapsable: false,
              children: [""]
            },
            {
              title: "钛比 NB-IoT 开发板",
              collapsable: true,
              children: [
                "./01-inspect-terabits-f103vc-board",
                "./02-build-f103vc-project-in-stm32cube",
                "./03-design-profile-and-codec-plugin",
                "./04-using-at-commands-connect-oceanconnect",
                "./05-fortify-embedded-app-working-on-device",
                "./06-install-nodejs-and-mongodb-server",
                "./07-build-a-restful-server-with-device-management",
                "./08-design-backend-services-api-for-frontend-app",
                "./09-implement-full-web-application-development",
                "./10-make-certification-subscribe-to-platform"
              ]
            },
            {
              title: "YiQi EVB 01/02 开发板",
              collapsable: true,
              children: [
                "./01-inspect-yiqi-evb01-l152cb-board",
                "./02-build-l152cb-project-in-stm32cube",
                "./03-design-profile-and-codec-plugin",
                "./04-using-at-commands-connect-oceanconnect",
                "./05-fortify-embedded-app-working-on-device",
                "./06-install-nodejs-and-mongodb-server",
                "./07-build-a-restful-server-with-device-management",
                "./08-design-backend-services-api-for-frontend-app",
                "./09-implement-full-web-application-development",
                "./10-make-certification-subscribe-to-platform"
              ]
            },
            {
              title: "IoTClub M1 开发板",
              collapsable: true,
              children: ["./iotclub-nbiot-overview"]
            },
            {
              title: "朱老师 NB476 开发板",
              collapsable: true,
              children: ["./nb476-nbiot-overview"]
            }
          ],
          "/": [""]
        }
      },
      "/en/": {
        selectText: "Languages",
        label: "English",
        editLinkText: "Edit this page on GitHub",
        algolia: {},
        nav: [{
            text: "Getting Started",
            link: "/en/getting-started"
          },
          {
            text: "Develop Guide",
            link: "/en/kernel/"
          }
        ],
        sidebar: {
          "/en/kernel/": [{
              title: "Guide",
              collapsable: false,
              children: ["./"]
            },
            {
              title: "Kernel",
              collapsable: false,
              children: [
                "./task",
                "./memory",
                "./interrupt",
                "./queue",
                "./event",
                "./mutex",
                "./semaphore",
                "./time-magmt",
                "./swtmr",
                "./list"
              ]
            },
            {
              title: "External",
              collapsable: false,
              children: ["./low-power"]
            }
          ],

          "/": [""]
        }
      }
    }
  }
};