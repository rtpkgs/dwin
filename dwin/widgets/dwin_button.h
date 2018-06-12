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
    struct dwin_obj obj; 
    void (*push_cb)(void); 
}; 
typedef struct dwin_button* dwin_button_t; 

rt_err_t dwin_button_init(void); 
struct dwin_button *dwin_button_create(struct dwin_page *page, rt_uint16_t addr, 
    void (*push_cb)(void)); 
rt_err_t dwin_button_delect(struct dwin_button *button); 
rt_err_t dwin_button_set_callback(struct dwin_button *button, void (*cb)(void)); 
rt_err_t dwin_button_set_active(struct dwin_button *button, rt_bool_t active); 

#endif 
