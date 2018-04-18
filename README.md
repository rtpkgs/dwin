# dwin库 #

[![build](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/liu2guang/dwlib)
[![release](https://img.shields.io/badge/Release-v1.4.x-orange.svg)](https://github.com/liu2guang/dwlib/releases)

 ** 这次更新是临时调整, 整个dwin库会在下个版本重构, 因为目前的dwin库这个轮子巨方和 天神 一样方, 谁用谁后悔, 有好的想法欢迎提出! ** 

---

# 1. 介绍
[dwin](https://github.com/liu2guang/dwin) 是一款便携性、高剪裁性的迪文科技显示屏驱动库。该库主要是解决用户去重复设计底层的问题, 让用户放更多时间去设计应用层代码。

## 1.1 主要特性

- 支持dwin显示屏触摸数据自动上传解析；
- 支持用户主动式读写寄存器和数据空间； 
- 支持系统配置功能；
- 支持[RT-Thread3.0](http://www.rt-thread.org/) ENV新特性，不支持其他OS平台和裸机平台；
- 支持 **按键** , **数字输入框** , **文本显示框** , ** 数字显示 ** 和 **图标** 插件，保留自定义插件编写接口；
- 支持空间信息导出，在代码中申请的组件可以在finsh中导出，然后去dwin工程配置, 解决来回修改工程的问题和空间分配混乱的问题；

# 2. 功能

## 2.1 系统功能

系统功能是dwin必须选中的功能件。

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
1. id：准备跳转页面id

返回值：错误码
```
uint8_t dwin_system_set_page_id(uint16_t id)
```

### 7. 获取当前页id：
1. id：获取当前页面id

返回值：错误码
```
uint8_t dwin_system_get_page_id(uint16_t *id)
```

### 8. 设置触摸控制背光：
1. enable：0失能, 1使能

返回值：错误码
```
uint8_t dwin_system_set_backligth(uint8_t enable)
```

### 9. 设置触摸数据是否自上传：
1. enable：0失能, 1使能

返回值：错误码
```
uint8_t dwin_system_set_autoupdate(uint8_t enable)
```

### 10. 设置数据寄存器上电初始化模式：
1. enable：0上电初始化为0x00, 1上电有L22字库文件加载

返回值：错误码
```
uint8_t dwin_system_set_dataload_mode(uint8_t enable)
```

### 11. 设置蜂鸣器是否开启：
1. enable：0开启 1未开启, 不影响dwin_system_set_beep接口

返回值：错误码
```
uint8_t dwin_system_set_buzzer(uint8_t enable)
```

### 12. 获取时间：
1. year：年，从2000开始，17代表2017
2. month：月
3. day：日
4. hour：时
5. minute：分
6. seconds：秒

返回值：错误码
```
uint8_t dwin_system_get_time(uint8_t *year,uint8_t *month,uint8_t *day,uint8_t *hour,uint8_t *minute,uint8_t *seconds)
```

### 13. 设置时间：
1. year：年，从2000开始，17代表2017
2. month：月
3. day：日
4. hour：时
5. minute：分
6. seconds：秒

返回值：错误码
```
uint8_t dwin_system_set_time(uint8_t year,uint8_t month, uint8_t day,uint8_t hour,uint8_t minute,uint8_t seconds)
```

## 2.2 按键插件

按键插件是可选功能, 配置宏xxxxxx。

### 1. 创建按键组件：
1. name：按键名称
2. cb：按键按下回调函数
3. args：回调函数传入参数

返回值：错误码
```
uint8_t dwin_plugin_button_create(const char *name, press_cb cb, void *args)
```

### 2. 更新按键状态：
1. name：按键名称
2. mode：button_stop停止检测按键, button_press按键按下识别

返回值：错误码
```
uint8_t dwin_plugin_button_mode(const char*name, uint8_t mode)
```

### 3. 更新按键回调函数：
1. name：按键名称
2. cb：按键按下回调函数
3. args：回调函数传入参数

返回值：错误码
```
uint8_t dwin_plugin_button_update(const char*name, press_cb cb, void *args)
```

## 2.3 图标插件

图标插件是可选功能, 配置宏xxxxxx。

### 1. 创建图标组件：
1. name：图标名称
2. min：最小图标编号
3. max：最大图标编号
4. cur：当前图标编号

返回值：错误码
```
uint8_t dwin_plugin_icon_create(const char *name, uint16_t min, uint16_t max, uint16_t cur)
```

### 2. 更新图标编号：
1. name：图标名称
2. cur：当前图标编号

返回值：错误码
```
uint8_t dwin_plugin_icon_update(const char *name, uint16_t cur)
```

### 3. 读图标编号：
1. name：图标名称

返回值：错误码
```
uint16_t dwin_plugin_icon_read(const char *name)
```

## 2.4 文本插件

文本插件是可选功能, 配置宏xxxxxx。

### 1. 创建文本组件：
1. name：文本名称
2. text：文本内容, 只支持GBK编码
3. len：文本长度(字节)

返回值：错误码
```
uint8_t dwin_plugin_textbox_create(const char *name, uint8_t *text, uint8_t len)
```

### 2. 更新文本内容：
1. name：文本名称
2. text：文本内容, 只支持GBK编码
3. len：文本长度(字节)

返回值：错误码
```
uint8_t dwin_plugin_textbox_update(const char *name, uint8_t *text, uint8_t len)
```

## 2.5 输入框插件

文本插件是可选功能, 配置宏xxxxxx。

### 1. 创建输入框组件：
1. name：输入框名称
2. cb：输入值后回调函数
3. args：回调函数输入参数

返回值：错误码
```
uint8_t dwin_plugin_inputbox_create(const char *name, inputbox_cb cb, void *args)
```

### 2. 更新输入框状态：
1. name：输入框名称
2. mode：inputbox_stop停止输入框回调，inputbox_start开始输入框回调

返回值：错误码
```
uint8_t dwin_plugin_inputbox_mode(const char *name, uint8_t mode)
```

### 3. 更新输入框回调函数：
1. name：输入框名称
2. cb：输入值后回调函数
3. args：回调函数输入参数

返回值：错误码
```
uint8_t dwin_plugin_inputbox_update(const char *name, inputbox_cb cb, void *args)
```

### 4. 读输入框上次输入值：
1. name：输入框名称

返回值：输入值
```
int32_t dwin_plugin_inputbox_read(const char *name)
```


