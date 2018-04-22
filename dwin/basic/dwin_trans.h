/*
 * @File:   dwin_trans.c 
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

#ifndef __DWIN_TRANS_H__ 
#define __DWIN_TRANS_H__ 

#include "dwin_def.h" 

rt_err_t dwin_watch_init(dwin_t dwin, const char *name, rt_uint32_t baudrate); 
rt_err_t dwin_reg_read(rt_uint8_t addr, rt_uint8_t *data, rt_uint8_t len); 
rt_err_t dwin_reg_write(rt_uint8_t addr, rt_uint8_t *data, rt_uint8_t len); 
rt_err_t dwin_var_read(rt_uint16_t addr, rt_uint16_t *data, rt_uint8_t len); 
rt_err_t dwin_var_write(rt_uint16_t addr, rt_uint16_t *data, rt_uint8_t len); 

#endif 
