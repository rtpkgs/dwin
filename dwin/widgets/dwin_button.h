/*
 * @File:   dwin_button.c 
 * @Author: liu2guang 
 * @Date:   2018-06-07 17:27:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-07     liu2guang    implementing the button object. 
 */ 

#ifndef __DWIN_BUTTON_H__ 
#define __DWIN_BUTTON_H__ 

#include "dwin_def.h" 

struct dwin_button
{
    struct dwin_obj *obj; 
}; 
typedef struct dwin_button* dwin_button_t; 

struct dwin_button *dwin_button_create(struct dwin_page *page, rt_uint16_t addr, void (*cb)(void *p)); 

#endif 
