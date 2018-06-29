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
          link: '/getting-started'
        }, {
          text: '指南',
          link: '/guide/'
        }, {
          text: '移植',
          link: '/porting/'
        }, {
          text: 'SDK',
          link: '/liteos-sdk'
        }, {
          text: '教程',
          link: '/tutorials/'
        }],
        sidebar: {
          '/guide/': [{
              title: '指南',
              collapsable: false,
              children: [
                './'
              ]
            },
            {
              title: '基础内核',
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
              title: '扩展内核',
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
          '/porting/': [{
            title: 'ARM 系列',
            collapsable: false,
            children: [
              './arm-cortex-m3',
              './arm-cortex-m4',
              './arm-cortex-m7'
            ]
          }],
          '/tutorials/': [{
              title: '内核开发系列',
              collapsable: false,
              children: [
                './example01',
                './example02',
                './example03'
              ]
            },
            {
              title: '北向应用系列',
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
          link: '/en/guide/'
        }],
        sidebar: {
          '/en/guide/': [{
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