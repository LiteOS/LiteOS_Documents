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
            '', 
            {
              title: '内核开发系列',
              collapsable: false,
              children: [
                './example01',
                './example02',
                './example03'
              ]
            },
            {
              title: '端云互通系列',
              collapsable: false,
              children: [
                './example03',
                './example04'
              ]
            },
            {
              title: '第三方教程',
              collapsable: false,
              children: [
                './example03',
                './example04'
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