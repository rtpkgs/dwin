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

rt_err_t dwin_system_get_cpuid(rt_uint16_t *cpuid); 
rt_err_t dwin_system_reset(void); 
rt_err_t dwin_system_get_version(rt_uint16_t *ver); 
rt_err_t dwin_system_get_rtc(rt_uint8_t *year, rt_uint8_t *mon, rt_uint8_t *day, rt_uint8_t *hour, rt_uint8_t *min, rt_uint8_t *sec); 
rt_err_t dwin_system_set_rtc(rt_uint8_t year, rt_uint8_t mon, rt_uint8_t day, rt_uint8_t hour, rt_uint8_t min, rt_uint8_t sec); 
rt_err_t dwin_system_get_page(rt_uint16_t *page); 
rt_err_t dwin_system_set_page(rt_uint16_t page); 

#endif 
