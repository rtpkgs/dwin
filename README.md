DWIN For RT-Thread
================

[![Build Status](https://travis-ci.org/liu2guang/dwin.svg?branch=v2.x.x)](https://travis-ci.org/liu2guang/dwin)
[![License](https://img.shields.io/apm/l/vim-mode.svg)](https://github.com/liu2guang/dwin/blob/master/LICENSE)

DWIN库是基于MIT开源协议开源的"dwin串口屏幕快速开发库". 该库需要工作在RT-Thread(后文简写RTT)物联网操作系统上, 整个DWIN库基于RTT底层设备层框架进行开发, 只要支持串口设备的RTT BSP都可以直接env在线安装DWIN库. 

示例效果与代码
-------------------

![dwin1](https://i.imgur.com/sxzFraT.jpg)
![dwin2](https://i.imgur.com/CyAndSV.jpg)

代码: 
~~~
#include <rtthread.h> 
#include <dwin.h>

/* 页面 */ 
#define PAGE_NUM 2
dwin_page_t page[PAGE_NUM]; 
dwin_num_input_t input; 
dwin_button_t login; 
dwin_qrcode_t qrcode; 
dwin_gbk_t gbk; 
dwin_icon_t icon; 

static void input_callback(rt_uint32_t value)
{
    rt_kprintf("User input password is %.8d.\n", value); 
}

static void login_callback(void)
{
    rt_kprintf("Login.\n");
}

int main(void)
{
    rt_uint16_t index = 0; 
    
    for(index = 0; index < sizeof(page)/sizeof(dwin_page_t); index++)
    {
        page[index] = dwin_page_create(index); 
    }
    
    /* 全局控件 */ 
    gbk = dwin_gbk_create(DWIN_ALL_PAGE, DWIN_VAR_ADDR(0x000), 100); 
    dwin_gbk_show_string(gbk, "你好不好呢ppp呢"); 
    
    /* 页面0 */ 
    qrcode = dwin_qrcode_create(page[0], 0x0300, 100); 
    dwin_qrcode_show_url(qrcode, "https://github.com/liu2guang/dwin"); 
    
    icon = dwin_icon_create(page[0], DWIN_VAR_ADDR(0x400), 6); 
    dwin_icon_set_index(icon, 5); 
    
    /* 页面1 */ 
    input = dwin_num_input_create(page[1], DWIN_VAR_ADDR(0x0034), input_callback); 
    login = dwin_button_create   (page[1], DWIN_VAR_ADDR(0x0038), login_callback); 
    
    return dwin_run(0); 
}
~~~

功能简介
-------------------

 功能 | 描述  
 ---- | ----
 button | 按键控件, 支持按下执行回调函数
 scale | 滑块控件, 支持滑动数据变动执行回调函数, 支持代码控制滑块滑动
 icon | 图标显示控件, 支持改变图标 
 qrcode | 二维码控件, 支持网页、字符串生成二维码显示
 gbk | GBK字符串显示控件, 支持GBK显示, 数字和汉字混合显示
 gbk_input | GBK录入控件, 支持键盘录入GBK字符串
 num | 数字显示控件, 支持整数和浮点数显示
 num_input| 数字录入控件, 支持整数和浮点数录入
 cmd | RTT CMD调试功能

CMD功能
-------------------

![dwin_cmd](https://i.imgur.com/FBmmMf1.png)

 功能 | 描述  
 ---- | ----
 读取变量和寄存器 | dwin -t r <reg|var> <addr> <len>
 更改变量和寄存器 | dwin -t w <reg|var> <addr> <len> <data...>
 打印dwin版本信息 | dwin -s ver 
 设置背光亮度 | dwin -s bl [level]
 蜂鸣 | dwin -s buzz <tick>
 获取当前页面 | dwin -s page
 跳转页面 | dwin -s jump <page>
 开启关闭触摸功能 | dwin -s touch <enable|disable>
 设置或者读取 | dwin -s rtc [year] [mon] [day] [hour] [min] [sec]
 发送软按键 | dwin -s key <code>
 打印当前注册解析器信息 | dwin -d parse
 打印当前页面与控件信息 | dwin -d page

感谢
-------------------

- @balanceTWK
- @uestczyh222

MIT许可证
-------------------

MIT License

Copyright (c) 2018 liu2guang(liuguang)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

附加许可: 禁止DWIN公司将该库用于商业目的. 	