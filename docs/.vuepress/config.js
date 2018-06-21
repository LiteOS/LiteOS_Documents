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
    repo: 'liteos/LiteOS',
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
        }],
        sidebar: {
          '/guide/': [{
              title: '指南',
              collapsable: false,
              children: [
                '/guide/'
              ]
            },
            {
              title: '基础内核',
              collapsable: false,
              children: [
                '/guide/task',
                '/guide/memory',
                '/guide/interrupt',
                '/guide/queue',
                '/guide/event',
                '/guide/mutex',
                '/guide/semaphore',
                '/guide/time-management',
                '/guide/software-timers',
                '/guide/doubly-linked-list'
              ]
            },
            {
              title: '扩展内核',
              collapsable: false,
              children: [
                '/guide/low-power'
              ]
            }
          ],
          '/porting/': [{
            title: 'ARM 系列',
            collapsable: false,
            children: [
              '/porting/arm-cortex-m3',
              '/porting/arm-cortex-m4',
              '/porting/arm-cortex-m7'
            ]
          }],
          '/sdk/': [{
            title: '嵌入式硬件',
            collapsable: false,
            children: [
              '/'
            ]
          }],

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
          link: '/getting-started/'
        }, {
          text: 'Develop Guide',
          link: '/guide/'
        }, {
          text: 'Porting Guide',
          link: '/porting/'
        }, {
          text: 'SDK',
          link: '/sdk/'
        }],
        sidebar: [{
            title: 'Overview',
            collapsable: false,
            children: [
              '/en/guide/',
              '/en/guide/overview',
            ]
          },
          {
            title: 'Basic Kernel',
            collapsable: false,
            children: [
              '/en/guide/task',
              '/en/guide/memory',
              '/en/guide/queue'
            ]
          },
          {
            title: 'External Kernel',
            collapsable: false,
            children: [

            ]
          }
        ]
      }
    }
  }
}