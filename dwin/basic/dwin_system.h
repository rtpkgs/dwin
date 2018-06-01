/*
 * @File:   dwin_system.c 
 * @Author: liu2guang 
 * @Date:   2018-04-26 00:12:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-04-26     liu2guang    update v2 framework. 
 */ 

#ifndef __DWIN_SYSTEM_H__ 
#define __DWIN_SYSTEM_H__ 

#include "dwin_def.h" 

/* 通用API */ 
rt_err_t dwin_system_version(rt_uint32_t *ver_sn); 
rt_err_t dwin_system_get_backlight(rt_uint8_t *level); 
rt_err_t dwin_system_set_backlight(rt_uint8_t level); 
rt_err_t dwin_system_buzz(rt_uint8_t tick); 
rt_err_t dwin_system_jump(rt_uint16_t page); 
rt_err_t dwin_system_page(rt_uint16_t *page);
rt_err_t dwin_system_touch(rt_bool_t enable); 
rt_err_t dwin_system_get_rtc(struct dwin_rtc *rtc); 
rt_err_t dwin_system_set_rtc(struct dwin_rtc rtc); 
rt_err_t dwin_system_get_timestamp(time_t *timestamp); 
rt_err_t dwin_system_set_timestamp(time_t timestamp); 
rt_err_t dwin_system_key(rt_uint8_t code); 
rt_err_t dwin_system_dir(enum dwin_dir dir); 
rt_err_t dwin_system_var_from_l22_upload(rt_bool_t *enable); 
rt_err_t dwin_system_backlight_by_touch_ctr(rt_bool_t *enable); 
rt_err_t dwin_system_crc(rt_bool_t *enable); 

/* 特有API */ 
#if (DWIN_USING_TYPE == 0) 

#endif 

#if (DWIN_USING_TYPE == 1) 

#endif 

#if (DWIN_USING_TYPE == 2) 

#endif 

#endif 
