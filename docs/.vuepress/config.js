module.exports = {
  locales: {
    '/': {
      lang: 'zh-CN',
      title: '华为 LiteOS | 中文网',
      description: '轻量级物联网操作系统，物的智能化使能平台',
      demo: '123'
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
          text: '内核',
          link: '/kernel/'
        }, {
          text: '组件',
          link: '/components/'
        }, {
          text: '移植',
          link: '/porting/'
        }, {
          text: '教程',
          link: '/tutorials/'
        }, {
          text: '案例',
          items: [{
            text: '抄表类',
            link: '/case-study/meter'
          }, {
            text: '家电类',
            link: '/case-study/goods'
          }, {
            text: '出行类',
            link: '/case-study/ofo'
          }, {
            text: '安防类',
            link: '/case-study/smoke'
          }, {
            text: '跟踪类',
            link: '/case-study/track'
          }]
        }],
        sidebar: {
          '/quick-start/': [
            '',
            './why-use-the-rtos',
            './why-select-huawei-liteos',
            './a-simple-blink-demo',
            './a-comprehensive-smoke-demo'
          ],
          '/kernel/': [
            '',
            {
              title: '内核基础',
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
            }, {
              title: '内核扩展',
              collapsable: false,
              children: [
                './low-power'
              ]
            },
            {
              title: '调试指南',
              collapsable: false,
              children: [
                './faq'
              ]
            }
          ],
          '/components/': [
            '', {
              title: '端云互通',
              collapsable: false,
              children: [
                './liteos-connect-cloud'
              ]
            }, {
              title: 'AT适配框架',
              collapsable: false,
              children: [
                './at-adaptor-framework'
              ]
            }
          ],
          '/porting/': [
            '',
            {
              title: '准备工作',
              collapsable: false,
              children: [
                './gcc',
                './keil',
                './iar'
              ]
            }, {
              title: '华为 LiteOS 移植',
              collapsable: false,
              children: [
                './stm32',
                './gd32',
                './faq'
              ]
            }, {
              title: '移植后，验证',
              collapsable: false,
              children: [
                './verify'
              ]
            }
          ],
          '/tutorials/': [
            {
              title: '手把手教程',
              collapsable: false,
              children: [
                '',
                './step-by-step/01-inspect-terabits-f103vc-board',
                './step-by-step/02-build-raw-project-in-stm32cube',
                './step-by-step/03-design-profile-and-codec-plugin',
                './step-by-step/04-using-at-commands-connect-oceanconnect',
                './step-by-step/05-fortify-embedded-app-working-on-device',
                './step-by-step/06-install-nodejs-and-mongodb-server',
                './step-by-step/07-build-a-restful-server-with-device-management',
                './step-by-step/08-design-backend-services-api-for-frontend-app',
                './step-by-step/09-implement-full-web-application-development',
                './step-by-step/10-make-certification-subscribe-to-platform'
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
                './doubly-linked-list'
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