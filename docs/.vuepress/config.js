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
      description:
        "A Lightweight IoT Operating System that Makes Everything Around Us Smart"
    }
  },
  themeConfig: {
    repo: "LiteOS/LiteOS_Documents",
    repoLabel: "Github",
    docsDir: "docs",
    editLinks: true,
    locales: {
      "/": {
        selectText: "语言",
        label: "简体中文",
        editLinkText: "在 GitHub 上编辑此页",
        lastUpdated: "上次更新",
        algolia: {},
        nav: [
          {
            text: "入门",
            items: [
              {
                text: "LiteOS 介绍",
                link: "/quick-start/intro/"
              },
              {
                text: "快速上手",
                link: "/quick-start/demo/"
              },
              {
                text: "参与贡献",
                link: "/quick-start/contribute/"
              }
            ]
          },
          {
            text: "学习",
            items: [
              {
                text: "LiteOS Kernel",
                link: "/kernel/"
              },
              {
                text: "LiteOS SDK",
                link: "/sdk/"
              },
              {
                text: "LiteOS 移植",
                link: "/porting/"
              }
            ]
          },
          {
            text: "教程",
            items: [
              {
                text: "内核实战",
                link: "/tutorials/kernel/"
              },
              {
                text: "场景例程",
                link: "/tutorials/scene/"
              },
              {
                text: "端到端开发",
                link: "/step-by-step/nb-iot/"
              }
            ]
          },
          {
            text: "生态",
            items: [
              {
                text: "对接中移动 OneNet 云平台",
                link: "/connect-to-cloud/onenet/"
              },
              {
                text: "钛云物联 All-in-Java 开发",
                link: "/all-in-java/"
              }
            ]
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
          "/quick-start/intro/": [
            "",
            "./terminology",
            "./why-use-the-rtos",
            "./why-select-huawei-liteos",
            "./how-to-study-liteos"
          ],
          "/quick-start/demo/": [
            "",
            "./a-simple-blink-demo",
            "./a-comprehensive-smoke-demo"
          ],
          "/quick-start/contribute/": [
            "",
            "./code-standard",
            "./write-standard",
            "./code-info"
          ],
          "/kernel/": [
            {
              title: "内核",
              collapsable: false,
              children: [""]
            },
            {
              title: "基础内核功能",
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
            },
            {
              title: "API参考",
              collapsable: false,
              children: [
                "./api-reference/index.html"
              ]
            }
          ],
          "/sdk/": [
            {
              title: "组件",
              collapsable: false,
              children: [
                "",
                "./oceanconnect-agenttiny",
                "./fota",
                "./at-adaptor-framework",
                "./sensors-framework"
              ]
            }
          ],
          "/porting/": [
            {
              title: "准备工作",
              collapsable: false,
              children: [
                "",
                "./evb-and-tools",
                "./raw-project-with-stm32cube",
                //"./raw-project-with-others",
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
              title: "附录一：开发工具安装手册",
              collapsable: true,
              children: ["./keil-install", "./iar-install", "./gcc-sw4stm32-install"]
            },
            {
              title: "附录二：OS_CONFIG 系统配置详解",
              collapsable: true,
              children: ["./os-config"]
            }
          ],
          "/tutorials/kernel/": [
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
            }
          ],
          "/tutorials/scene/": [
            {
              title: "综合例程",
              collapsable: false,
              children: [
                "./temp-hum", 
                "./smoke", 
                "./gps", 
                "./light", 
                "./wifi"
              ]
            }
          ],
          "/case/": [{
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
            {
              title: "手把手教程",
              collapsable: false,
              children: [""]
            },
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
          "/": [""]
        }
      },
      "/en/": {
        selectText: "Languages",
        label: "English",
        editLinkText: "Edit this page on GitHub",
        algolia: {},
        nav: [
          {
            text: "Getting Started",
            link: "/en/getting-started"
          },
          {
            text: "Develop Guide",
            link: "/en/kernel/"
          }
        ],
        sidebar: {
          "/en/kernel/": [
            {
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
