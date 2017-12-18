# dwin库 #

[![build](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/liu2guang/dwlib)
[![release](https://img.shields.io/badge/Release-v1.4.0-orange.svg)](https://github.com/liu2guang/dwlib/releases)

---

# 1. 介绍
[dwin](https://github.com/liu2guang/dwin) 是一款便携性、高剪裁性的迪文科技显示屏驱动库。该库主要是解决用户去重复设计底层的问题, 让用户放更多时间去设计应用层代码。

## 1.1 主要特性

- 支持dwin显示屏触摸数据自动上传解析；
- 支持用户主动式读写寄存器和数据空间； 
- 支持系统配置功能；
- 支持[RT-Thread3.0](http://www.rt-thread.org/) ENV新特性，不支持其他OS平台和裸机平台；
- 支持 **按键** 、 **数字输入框** 、 **文本显示框** 和 **图标** 插件，保留自定义插件编写接口；
- 支持空间信息导出，在代码中申请的组件可以在finsh中导出，然后去dwin工程配置, 解决来回修改工程的问题和空间分配混乱的问题；

# 2. 功能

## 2.1 系统功能

### 获取软件版本

获取当前dwin库版本。
```
uint32_t dwin_system_sw_version(uint8_t *major, uint8_t *minor, uint8_t *revise)
```