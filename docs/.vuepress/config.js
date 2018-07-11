module.exports = {
  locales: {
    '/': {
      lang: 'zh-CN',
      title: 'Huawei LiteOS | 中文网',
      description: '轻量级物联网操作系统，物的智能化使能平台'
    },
    '/en/': {
      lang: 'en-US',
      title: 'Huawei LiteOS | English Site',
      description: 'A Lightweight IoT Operating System that Makes Everything Around Us Smart'
    }
  },
  themeConfig: {
    repo: 'LiteOS/LiteOS_Documents',
    repoLabel: 'Github',
    docsDir: 'doc',
    editLinks: true,
    locales: {
      '/': {
        selectText: '选择语言',
        label: '简体中文',
        editLinkText: '在 GitHub 上编辑此页',
        lastUpdated: '上次更新',
        algolia: {},
        nav: [{
          text: '入门',
          link: '/quick-start/'
        }, {
          text: '内核（Kernel）',
          link: '/kernel/'
        }, {
          text: '组件（SDK）',
          link: '/components/'
        }, {
          text: '移植',
          link: '/porting/'
        }, {
          text: '教程',
          link: '/tutorials/'
        }, {
          text: '案例',
          link: '/case-study/'
        }, {
          text: '精选',
          items: [{
              text: '端到端教程',
              link: '/step-by-step/'
            },
            {
              text: '连接第三方云平台',
              link: '/connect-to-cloud/'
            },
            {
              text: 'LiteOS 源码剖析',
              link: '/connect-to-cloud/'
            },
            {
              text: '朱老师 NB476 开发板视频',
              link: '/connect-to-cloud/'
            }
          ]
        }],
        sidebar: {
          '/quick-start/': [{
            title: '入门',
            collapsable: false,
            children: [
              '',
              './terminology',
              './why-use-the-rtos',
              './why-select-huawei-liteos'
            ]
          }, {
            title: '快速上手',
            collapsable: true,
            children: [
              './a-simple-blink-demo',
              './a-comprehensive-smoke-demo'
            ]
          }],
          '/kernel/': [{
              title: '内核',
              collapsable: false,
              children: [
                ''
              ]
            },
            {
              title: '基础',
              collapsable: true,
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
            }, {
              title: '扩展',
              collapsable: true,
              children: [
                './low-power'
              ]
            },
            {
              title: '调试指南',
              collapsable: true,
              children: [
                './faq'
              ]
            }
          ],
          '/components/': [
            {
              title: '组件',
              collapsable: false,
              children: [
                '',
                './liteos-connect-cloud',
                './fota',
                './at-adaptor-framework',
                './js-engine',
                './sensors-framework'
              ]
            }
          ],
          '/porting/': [
            {
              title: '移植',
              collapsable: false,
              children: [
                ''
              ]
            },
            {
              title: '准备工作',
              collapsable: true,
              children: [
                './keil'
              ]
            }, {
              title: 'OS 移植',
              collapsable: true,
              children: [
                './stm32',
                './faq'
              ]
            }, {
              title: '移植后的验证',
              collapsable: true,
              children: [
                './verify'
              ]
            }
          ],
          '/tutorials/': [
            {
              title: '教程',
              collapsable: false,
              children: [
                ''
              ]
            },
            {
              title: '内核',
              collapsable: true,
              children: [
                './example01',
                './example02'
              ]
            }, {
              title: '传感器',
              collapsable: true,
              children: [
                './example03',
                './example04'
              ]
            }
          ],
          '/case-study/': [
            {
              title: '案例',
              collapsable: false,
              children: [
                '',
                './meter',
                './goods',
                './ofo',
                './smoke',
                './track'
              ]
            }
          ],
          '/step-by-step/': [
            {
              title: '手把手教程',
              collapsable: false,
              children: [
                ''
              ]
            },
            {
              title: 'Terabits NB-IoT 开发板',
              collapsable: true,
              children: [
                './01-inspect-terabits-f103vc-board',
                './02-build-f103vc-project-in-stm32cube',
                './03-design-profile-and-codec-plugin',
                './04-using-at-commands-connect-oceanconnect',
                './05-fortify-embedded-app-working-on-device',
                './06-install-nodejs-and-mongodb-server',
                './07-build-a-restful-server-with-device-management',
                './08-design-backend-services-api-for-frontend-app',
                './09-implement-full-web-application-development',
                './10-make-certification-subscribe-to-platform'
              ]
            },
            {
              title: 'YiQi NB-IoT 开发板',
              collapsable: true,
              children: [
                './01-inspect-yiqi-evb01-l152cb-board',
                './02-build-l152cb-project-in-stm32cube',
                './03-design-profile-and-codec-plugin',
                './04-using-at-commands-connect-oceanconnect',
                './05-fortify-embedded-app-working-on-device',
                './06-install-nodejs-and-mongodb-server',
                './07-build-a-restful-server-with-device-management',
                './08-design-backend-services-api-for-frontend-app',
                './09-implement-full-web-application-development',
                './10-make-certification-subscribe-to-platform'
              ]
            },
            {
              title: 'IoTClub NB-IoT 开发板',
              collapsable: true,
              children: [
                './iotclub-nbiot-overview'
              ]
            },
            {
              title: '朱老师 NB-IoT 开发板',
              collapsable: true,
              children: [
                './nb476-nbiot-overview'
              ]
            }
          ],
          '/connect-to-cloud/': [
            {
              title: '第三方云平台',
              collapsable: false,
              children: [
                '',
                './china-mobile-onenet'
              ]
            }
          ],
          '/': [
            ''
          ]
        }
      },
      '/en/': {
        selectText: 'Languages',
        label: 'English',
        editLinkText: 'Edit this page on GitHub',
        algolia: {},
        nav: [{
          text: 'Getting Started',
          link: '/en/getting-started'
        }, {
          text: 'Develop Guide',
          link: '/en/kernel/'
        }],
        sidebar: {
          '/en/kernel/': [{
              title: 'Guide',
              collapsable: false,
              children: [
                './'
              ]
            },
            {
              title: 'Kernel',
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
              title: 'External',
              collapsable: false,
              children: [
                './low-power'
              ]
            }
          ],

          '/': [
            ''
          ]
        }
      }
    }
  }
}