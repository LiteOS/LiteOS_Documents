module.exports = {
  head: [
    ['link', {
      rel: 'icon',
      href: '/favicon.ico'
    }]
  ],
  ga: "UA-124955660-1",
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
    docsDir: "docs",
    editLinks: true,
    locales: {
      "/": {
        selectText: "切换语言",
        label: "简体中文",
        editLinkText: "在 GitHub 上编辑此页",
        lastUpdated: "上次更新",
        algolia: {},
        nav: [{
            text: "学习",
            items: [{
                text: "入门",
                items: [{
                    text: "快速上手",
                    link: "/quick-start/intro/"
                  },
                  {
                    text: "参与贡献",
                    link: "/quick-start/contrb/"
                  }
                ]
              },
              {
                text: "进阶",
                items: [{
                    text: "内核指南",
                    link: "/kernel/"
                  },
                  {
                    text: "SDK 指南",
                    link: "/sdk/"
                  },
                ]
              },
              {
                text: "高级",
                items: [{
                    text: "移植手册",
                    link: "/porting/"
                  },
                  {
                    text: "API 参考",
                    link: "http://liteos.github.io/api-reference/"
                  },
                ]
              },
            ]
          },
          {
            text: "实战",
            items: [{
                text: "LiteOS 动手实验室",
                items: [{
                    text: "玩转 LiteOS 内核教程",
                    link: "/tutorials/kernel/"
                  },
                  {
                    text: "LiteOS@NB-IoT 场景化样例集",
                    link: "/tutorials/scene/"
                  }
                ]
              },
              {
                text: "零基础端到端开发",
                items: [{
                    text: "NB-IoT 手把手教程（移远 BC95/BC35)",
                    link: "/step-by-step/nb-iot/"
                  },
                  {
                    text: "WiFi 手把手教程（安信可 ESP8266)",
                    link: "/step-by-step/esp8266/"
                  }
                ]
              },
              {
                text: "在线精品课程",
                items: [{
                    text: "朱老师物联网大讲堂 NB-IoT 系列课程",
                    link: "https://developer.huawei.com/ict/cn/edu-online/site-iot-nb-01"
                  },
                  {
                    text: "物联网俱乐部 NB-IoT 系列课程",
                    link: "https://developer.huawei.com/ict/cn/edu-online/site-iot-nb-002"
                  }
                ]
              }
            ]
          },
          {
            text: "生态",
            items: [{
                text: "对接云平台",
                items: [{
                    text: "华为公有云 IoT 平台",
                    link: "/connect-to-cloud/huaweicloud/"
                  },
                  {
                    text: "中国电信天翼云平台",
                    link: "/connect-to-cloud/ctyun/"
                  },
                  {
                    text: "中国移动 OneNET 平台",
                    link: "/connect-to-cloud/onenet/"
                  }
                ]
              },
              {
                text: "OpenCPU",
                items: [{
                    text: "LiteOS Boudica150 OpenCPU（公版）",
                    link: "/opencpu/public"
                  },
                  {
                    text: "Quectel BC35/28 OpenCPU",
                    link: "/opencpu/quectel"
                  }
                ]
              },
              {
                text: "其他编程语言",
                items: [{
                    text: "MapleJS 引擎（JavaScript）",
                    link: "/3rd-lang/maplejs"
                  },
                  {
                    text: "钛云物联 All-in-Java",
                    link: "/3rd-lang/tiyun-java"
                  },
                ]
              }
            ]
          },
          {
            text: "活动",
            items: [{
                text: "HUAWEI CONNECT 2018",
                items: [{
                    text: "产品展示",
                    link: "/hc2018/products/"
                  },
                  {
                    text: "动手体验",
                    link: "/hc2018/experience/"
                  }
                ]
              },
              // {
              //   text: "华为IoT开发者大赛",
              //   items: [
              //     {
              //       text: "32 强作品展示",
              //       link: "http://poster.issmart.com.cn/web/fontend/h5/other/2018927vote"
              //     }
              //   ]
              // }
            ]
          },
          // {
          //   text: "伙伴",
          //   items: [
          //     {
          //       text: "行业伙伴",
          //       link: "/partners/industry/"
          //     },
          //     {
          //       text: "平台伙伴",
          //       link: "/partners/platform/"
          //     },
          //     {
          //       text: "芯片/模组伙伴",
          //       link: "/partners/module/"
          //     },
          //     {
          //       text: "教学伙伴",
          //       link: "/partners/teaching/"
          //     },
          //   ]
          // },
          {
            text: "论坛",
            link: "https://developer.huawei.com/ict/forum/forum.php?mod=forumdisplay&fid=400045"
          },
          {
            text: "修订记录",
            link: "/changelog"
          }
        ],
        sidebar: {
          "/quick-start/intro/": [{
              title: "入门",
              collapsable: false,
              children: [""]
            },
            {
              title: "思考",
              collapsable: false,
              children: [
                "./why-use-the-rtos",
                "./why-select-liteos",
                "./how-to-study-liteos"
              ]
            },
            {
              title: "演示",
              collapsable: false,
              children: [
                "./a-simple-blinky-demo",
                "./a-comprehensive-smoky-demo"
              ]
            },
            {
              title: "扩展",
              collapsable: false,
              children: [
                //"./usage-guide-for-partner",
                "./terminology"
              ]
            }
          ],
          "/quick-start/contrb/": [
            "",
            "./code-standard",
            "./write-standard",
            "./code-info"
          ],
          "/kernel/": [{
              title: "内核",
              collapsable: false,
              children: [""]
            },
            {
              title: "基础功能",
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
              title: "错误码参考",
              collapsable: false,
              children: [
                "./errorcode-task",
                "./errorcode-queue",
                "./errorcode-event",
                "./errorcode-mutex",
                "./errorcode-semaphore",
                "./errorcode-swtmr"
              ]
            }
          ],
          "/sdk/": [{
              title: "端云互通",
              collapsable: false,
              children: [
                "",
                "./sdk-overview",
                "./sdk-apis",
                "./sdk-oceanconnect",
                "./sdk-lwm2m"
              ]
            },
            {
              title: "LPWA 增强",
              collapsable: false,
              children: [
                "./device-pnp",
                "./dtls-plus",
                //"./fault-diagnosis",
                //"./dispersed-reporting",
                "./difference-upgrade"
              ]
            }
          ],
          "/porting/": [{
              title: "准备工作",
              collapsable: false,
              children: [
                "./evb-and-tools",
                "./raw-project-with-stm32cube",
                "./raw-project-with-others",
                "./liteos-source-code"
              ]
            },
            {
              title: "OS 移植",
              collapsable: false,
              children: [
                "./stm32-keil",
                "./stm32-iar",
                "./stm32-sw4stm32",
                "./stm32-makefile",
                //"./nxp",
                //"./gd32",
                "./faq"
              ]
            },
            {
              title: "移植后验证",
              collapsable: false,
              children: ["./inspect-example", "./inspect-results"]
            },
            {
              title: "附1：IDE 工具安装指南",
              collapsable: true,
              children: ["./keil-install", "./iar-install", "./gcc-sw4stm32-install"]
            },
            {
              title: "附2：OS_CONFIG 详解",
              collapsable: true,
              children: ["./os-config"]
            }
          ],
          "/tutorials/kernel/": [{
            title: "LiteOS 内核使用指南",
            collapsable: false,
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
              "./example10",
              "./example11",
              "./example12",
              "./example13",
              "./example14",
              "./example15"
            ]
          }],
          "/tutorials/scene/": [{
            title: "场景化例程",
            collapsable: true,
            children: [
              "./light",
              "./temp-hum",
              //"./smoke", 
              //"./gps", 
              //"./wifi"
            ]
          }],
          "/partners/industry/": [{
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
          "/step-by-step/nb-iot/": [
            "",
            {
              title: "钛比 NB-IoT 开发板",
              collapsable: true,
              children: [
                "./01-inspect-terabits-board",
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
                "./01-inspect-yiqi-evb01-board",
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
              children: [
                "./01-inspect-iotclub-m1v3-board",
                "./02-build-l431rb-project-in-stm32cube",
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
              title: "朱老师 NB476 开发板",
              collapsable: true,
              children: [
                "./01-inspect-zhulaoshi-nb476-board",
                "./02-build-l476rg-project-in-stm32cube",
                "./03-design-profile-and-codec-plugin",
                "./04-using-at-commands-connect-oceanconnect",
                "./05-fortify-embedded-app-working-on-device",
                "./06-install-nodejs-and-mongodb-server",
                "./07-build-a-restful-server-with-device-management",
                "./08-design-backend-services-api-for-frontend-app",
                "./09-implement-full-web-application-development",
                "./10-make-certification-subscribe-to-platform"
              ]
            }
          ],
          "/connect-to-cloud/onenet/": [{
            title: "OneNET 平台对接",
            collapsable: true,
            children: [
              "./china-mobile-onenet"
            ]
          }],
          "/hc2018/experience/": [{
            title: "动手体验区",
            collapsable: false,
            children: [
              "",
              "./liteos-studio",
              "./opencpu-maplejs"
            ]
          }],
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
              title: 'Basic Kernel',
              collapsable: false,
              children: [
                './task',
                './memory',
                './interrupt',
                './queue',
                './event',
                './mutex',
                './semaphore',
                './time-magmt',
                './swtmr',
                './list'
              ]
            },
            {
              title: 'ErrorCode Reference',
              collapsable: false,
              children: [
                './errorcode-task',
                './errorcode-queue',
                './errorcode-event',
                './errorcode-mutex',
                './errorcode-semaphore',
                './errorcode-swtmr'
              ]
            }
          ],
          "/": [""]
        }
      }
    }
  }
};