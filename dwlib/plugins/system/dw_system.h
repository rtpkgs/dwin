#ifndef __DW_SYSTEM_H_
#define __DW_SYSTEM_H_

#include <time.h>
#include "dw_space.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    RT_DW_SCREEN_DIR_000 = 0,
    RT_DW_SCREEN_DIR_090 = 1,
    RT_DW_SCREEN_DIR_180 = 2,
    RT_DW_SCREEN_DIR_270 = 3
}rt_dw_screen_dir_t;

typedef enum
{
    RT_DW_VAR_INIT_MODE_0 = 0,  /* 默认0x00初始化 */
    RT_DW_VAR_INIT_MODE_1 = 1,  /* 按照22号字库初始化 */
}rt_dw_var_init_mode_t;

/* 获取迪文屏幕固件版本 */
rt_uint8_t rt_dw_fw_version(rt_device_dw_t device);

/* 获取当前背光亮度, 亮度为0x00~0x40 */
rt_uint8_t rt_dw_get_now_bl(rt_device_dw_t device);

/* 设置当前背光亮度, 亮度为0x00~0x40 */
rt_err_t rt_dw_set_now_bl(rt_device_dw_t device, rt_uint8_t now_bl);

/* 控制蜂鸣n*10ms */
rt_err_t rt_dw_beep_nx10ms(rt_device_dw_t device, rt_uint8_t time);

/* 获取当前页 */
rt_uint16_t rt_dw_get_now_pageid(rt_device_dw_t device);	

/* 跳转到指定页面, 但是弹窗不会关闭, 所以在迪文屏幕有弹窗出现时, 不要调用该函数 */
rt_err_t rt_dw_jump_page(rt_device_dw_t device, rt_uint16_t id);

/* 获取串口波特率:
[00 "1.2K"  ]  [01 "2.4K"  ]  [02 "4.8K"  ]  [03 "9.6K"  ]
[04 "19.2K" ]  [05 "38.4K" ]  [06 "57.6K" ]  [07 "115.2K"]
[08 "28.8K" ]  [09 "76.8K" ]  [10 "62.5K" ]  [11 "125K"  ]
[12 "250K"  ]  [13 "230.4K"]  [14 "345.6K"]  [15 "691.2K"]
[16 "921.6K"] */
rt_uint8_t rt_dw_baudrate(rt_device_dw_t device);

/* 设置屏幕旋转方向 */
rt_err_t rt_dw_set_screen_dir(rt_device_dw_t device, rt_dw_screen_dir_t dir);

/* 获取当前屏幕方向 */
rt_err_t rt_dw_get_screen_dir(rt_device_dw_t device, rt_dw_screen_dir_t *dir);

/* 获取背光是否由触摸控制, 0背光不受触摸控制, 1背光受触摸控制 */
rt_err_t rt_dw_get_bl_ctr_enable(rt_device_dw_t device, rt_uint8_t *enable);

/* 设置背光是否由触摸控制, 0背光不受触摸控制, 1背光受触摸控制 */
rt_err_t rt_dw_set_bl_ctr_enable(rt_device_dw_t device, rt_uint8_t enable);

/* 获取是否开启帧CRC功能, 0未启动, 1启动 */
rt_err_t rt_dw_get_crc_enable(rt_device_dw_t device, rt_uint8_t *enable);

/* 设置是否开启帧CRC功能, 0未启动, 1启动 */
rt_err_t rt_dw_set_crc_enable(rt_device_dw_t device, rt_uint8_t enable);

/* 获取触摸数据是否自动上传 */
rt_err_t rt_dw_get_data_auto_update(rt_device_dw_t device, rt_uint8_t *enable);

/* 设置触摸数据是否自动上传 */
rt_err_t rt_dw_set_data_auto_update(rt_device_dw_t device, rt_uint8_t enable);

/* 获取数据寄存器上电初始化模式, 0上电初始化为0x00, 1上电有L22字库文件加载 */
rt_err_t rt_dw_get_var_init_mode(rt_device_dw_t device, rt_dw_var_init_mode_t *mode);

/* 设置数据寄存器上电初始化模式, 0上电初始化为0x00, 1上电有L22字库文件加载 */
rt_err_t rt_dw_set_var_init_mode(rt_device_dw_t device, rt_dw_var_init_mode_t mode);

/* 获取蜂鸣器是否开启, 0开启 1未开启 */
rt_err_t rt_dw_get_buzzer_enable(rt_device_dw_t device, rt_uint8_t *enable);

/* 设置蜂鸣器是否开启, 0开启 1未开启 */
rt_err_t rt_dw_set_buzzer_enable(rt_device_dw_t device, rt_uint8_t enable);

/* 获取RTC时间 */
rt_err_t rt_dw_get_rtc(rt_device_dw_t device, 
    rt_uint8_t *year, rt_uint8_t *month,  rt_uint8_t *day, 
    rt_uint8_t *hour, rt_uint8_t *minute, rt_uint8_t *seconds);

/* 设置RTC时间 */
rt_err_t rt_dw_set_rtc(rt_device_dw_t device, 
    rt_uint8_t year, rt_uint8_t month, rt_uint8_t day, 
    rt_uint8_t hour, rt_uint8_t minute, rt_uint8_t seconds);

/* 使用时间戳设置RTC时间 */
rt_err_t rt_dw_set_rtc_timestamp(rt_device_dw_t device, const time_t timestamp);

/* 获取RTC时间戳*/
rt_err_t rt_dw_get_rtc_timestamp(rt_device_dw_t device, time_t *timestamp);

#ifdef __cplusplus
}
#endif

#endif
