/*
 * @File:   dwin_utils.c 
 * @Author: liu2guang 
 * @Date:   2018-04-22 14:52:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-04-22     liu2guang    update v2 framework. 
 */ 

#ifndef __DWIN_UTILS_H__ 
#define __DWIN_UTILS_H__ 

#include "dwin_def.h" 

rt_uint32_t dwin_utils_convert(const char *str); 
rt_uint8_t dwin_utils_bcd2dec(rt_uint8_t bcd); 
rt_uint8_t dwin_utils_dec2bcd(rt_uint8_t dec); 

#endif 
