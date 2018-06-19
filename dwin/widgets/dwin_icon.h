/*
 * @File:   dwin_icon.h 
 * @Author: liu2guang 
 * @Date:   2018-06-14 15:56:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-14     liu2guang    implement the icon object. 
 */ 

#ifndef __DWIN_ICON_H__ 
#define __DWIN_ICON_H__ 

#include "dwin_def.h" 

struct dwin_icon
{
    struct dwin_obj obj; 
    rt_uint16_t current_index; 
}; 
typedef struct dwin_icon* dwin_icon_t; 

rt_err_t dwin_icon_init(void); 
struct dwin_icon *dwin_icon_create(struct dwin_page *page, rt_uint16_t addr, rt_uint16_t init_index); 
rt_err_t dwin_icon_delect(struct dwin_icon *icon); 
rt_err_t dwin_icon_set_index(struct dwin_icon *icon, rt_uint16_t index); 

#endif 
