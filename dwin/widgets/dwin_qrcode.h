/*
 * @File:   dwin_qrcode.c 
 * @Author: balanceTWK 
 * @Date:   2018-06-15 14:00:00 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-15     balanceTWK    implementing the button object. 
 */ 

#ifndef __DWIN_QRCODE_H__ 
#define __DWIN_QRCODE_H__ 

#include "dwin_def.h" 

struct dwin_qrcode
{
    struct dwin_obj obj; 
    
    rt_uint16_t max_len; 
}; 
typedef struct dwin_qrcode* dwin_qrcode_t; 

rt_err_t dwin_qrcode_init(void); 
struct dwin_qrcode *dwin_qrcode_create(struct dwin_page *page, rt_uint16_t addr, rt_uint16_t max_len); 
rt_err_t dwin_qrcode_delect(struct dwin_qrcode *qrcode); 
rt_err_t dwin_qrcode_show_string(struct dwin_qrcode *qrcode, char *string); 
rt_err_t dwin_qrcode_show_url(struct dwin_qrcode *qrcode,char *url);
rt_err_t dwin_qrcode_clear(struct dwin_qrcode *qrcode); 

#endif 
