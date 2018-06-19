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
}; 
typedef struct dwin_qrcode* dwin_qrcode_t; 

rt_err_t dwin_qrcode_show_url(struct dwin_qrcode *qrcode,char *url);
rt_err_t dwin_qrcode_set_url(struct dwin_qrcode *qrcode,char *url);

#endif 
