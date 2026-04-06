---
title: "obsidianmd/obsidian-clipper: Highlight and capture the web in your favorite browser. The official Web Clipper extension for Obsidian."
source: "https://github.com/obsidianmd/obsidian-clipper/blob/main/README.md"
author:
published:
created: 2026-04-03
description: "Highlight and capture the web in your favorite browser. The official Web Clipper extension for Obsidian. - obsidianmd/obsidian-clipper"
tags:
  - "clippings"
---
Obsidian Web Clipper 可帮助您在常用浏览器中高亮显示并截取网页内容。您保存的所有内容都会以持久的 Markdown 文件格式存储，方便您离线阅读并长期保存。

## 开始使用

请从浏览器官方目录下载并安装扩展程序：

- 适用于 Chrome、Brave、Arc、Orion 和其他基于 Chromium 内核的浏览器的 **[Chrome 网上应用商店。](https://chromewebstore.google.com/detail/obsidian-web-clipper/cnjifjpddelmedmihgijeibhnjfabmlf)**
- 适用于 Firefox 和 Firefox Mobile 的 **[Firefox 附加组件。](https://addons.mozilla.org/en-US/firefox/addon/web-clipper-obsidian/)**
- 适用于 macOS、iOS 和 iPadOS 的 **[Safari 扩展程序。](https://apps.apple.com/us/app/obsidian-web-clipper/id6720708363)**
- **[适用于 Microsoft Edge 的 Edge 插件](https://microsoftedge.microsoft.com/addons/detail/obsidian-web-clipper/eigdjhmgnaaeaonimdklocfekkaanfme)** 。

## 使用扩展程序

[Obsidian 帮助网站](https://help.obsidian.md/web-clipper) 上提供了相关文档 ，其中涵盖了如何使用 [高亮显示](https://help.obsidian.md/web-clipper/highlight) 、 [模板](https://help.obsidian.md/web-clipper/templates) 、 [变量](https://help.obsidian.md/web-clipper/variables) 、 [过滤器](https://help.obsidian.md/web-clipper/filters) 等功能。

## 贡献

### 翻译

[您可以帮助将 Web Clipper 翻译成您的语言。请使用/\_locales](https://github.com/obsidianmd/obsidian-clipper/blob/main/src/_locales) 文件夹中提供的格式，通过 pull request 提交您的翻译 。

### 功能和错误修复

请查看带有 [“需要帮助”](https://github.com/obsidianmd/obsidian-clipper/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22) 标签的问题，这些问题欢迎大家贡献力量。

## 路线图

排名不分先后：

- Web Clipper 的单独图标
- 标注高亮部分
- 模板目录
- 模板验证
- 模板逻辑（if/for）
- 将图像保存到本地 [（Obsidian 1.8.0 版本新增功能）](https://obsidian.md/changelog/2024-12-18-desktop-v1.8.0/)
- 将用户界面翻译成更多语言——欢迎提供帮助

## 开发者

构建扩展程序：

```
npm run build
```

这将创建三个目录：

- `dist/` 适用于 Chromium 版本
- `dist_firefox/` 适用于 Firefox 版本
- `dist_safari/` 适用于 Safari 版本

### 在本地安装扩展程序

对于基于 Chromium 内核的浏览器，例如 Chrome、Brave、Edge 和 Arc：

1. 打开浏览器并导航至 `chrome://extensions`
2. 启用 **开发者模式**
3. 点击 **“加载已解压的文件”** ，然后选择 `dist` 目录

适用于 Firefox 浏览器：

1. 打开 Firefox 并导航至 `about:debugging#/runtime/this-firefox`
2. 点击 **加载临时插件**
3. 导航到该 `dist_firefox` 目录并选择该 `manifest.json` 文件

如果你想永久运行该扩展程序，可以使用 Firefox 的 Nightly 版本或开发者版本。

1. `about:config` 在地址栏中 输入
2. 在搜索框中输入 `xpinstall.signatures.required`
3. 双击该首选项，或右键单击并选择“切换”，将其设置为 `false` 。
4. 前往 `about:addons` \> 齿轮图标 > **从文件安装插件…**

在 macOS 上进行 iOS 模拟器测试：

1. 运行 `npm run build` 以构建扩展程序
2. `xcode/Obsidian Web Clipper/Obsidian Web Clipper.xcodeproj` 在 Xcode 中 打开
3. 从方案选择器中 选择 **Obsidian Web Clipper (iOS)方案**
4. 选择 iOS 模拟器设备，然后点击 **“运行”** 以构建并启动应用。
5. 应用在模拟器上运行后，打开 **Safari 浏览器。**
6. 打开网页，然后点击Safari 浏览器中的 **“扩展”** 按钮，即可访问 Web Clipper 扩展程序。

### 运行测试

```
npm test
```

或者在开发过程中以监视模式运行：

```
npm run test:watch
```

## 第三方库

- 用于浏览器兼容性的 [webextension-polyfill](https://github.com/mozilla/webextension-polyfill)
- 用于内容提取和 Markdown 转换的 [defuddle](https://github.com/kepano/defuddle)
- [dayjs](https://github.com/iamkun/dayjs) 用于日期解析和格式化
- [使用 lz-string](https://github.com/pieroxy/lz-string) 压缩模板以减少存储空间
- [lucide](https://github.com/lucide-icons/lucide) 图标
- [使用 dompurify](https://github.com/cure53/DOMPurify) 对 HTML 进行清理