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
- 支持 **按键** , **数字输入框** , **文本显示框** 和 **图标** 插件，保留自定义插件编写接口；
- 支持空间信息导出，在代码中申请的组件可以在finsh中导出，然后去dwin工程配置, 解决来回修改工程的问题和空间分配混乱的问题；

# 2. 功能

## 2.1 系统功能

### 1. 获取当前dwin库版本：
1. major：主版本号
2. minor：次版本号
3. revise：修订版本号

返回值：版本编号：major * 10000 + minor * 100 + revise
```
uint32_t dwin_system_sw_version(uint8_t *major, uint8_t *minor, uint8_t *revise)
```

### 2. 获取当前dwin显示屏硬件版本：
1. major：主版本号
2. minor：次版本号
3. revise：修订版本号(目前无效, 只能为0)

返回值：版本编号：major * 10000 + minor * 100 + revise
```
uint32_t dwin_system_hw_version(uint8_t *major, uint8_t *minor, uint8_t *revise)
```

### 3. 获取显示屏背光亮度：
1. lvl：背光亮度返回值

返回值：错误码
```
uint8_t dwin_system_get_backlight_lvl(uint8_t *lvl)
```

### 4. 设置显示屏背光亮度：
1. lvl：待设置背光亮度

返回值：错误码
```
uint8_t dwin_system_set_backlight_lvl(uint8_t lvl)
```

### 5. 控制蜂鸣器响(不受蜂鸣器使能印象)：
1. time：控制蜂鸣器响time*10ms

返回值：错误码
```
uint8_t dwin_system_set_beep(uint8_t time)
```

### 6. 设置当前页id：
1. id：准备跳转页面

返回值：错误码
```
uint8_t dwin_system_set_page_id(uint16_t id)
```