/*
 * @File:   dwin_page.h 
 * @Author: liu2guang 
 * @Date:   2018-06-05 17:27:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-05     liu2guang    implementing the page object. 
 */ 

#ifndef __DWIN_PAGE_H__ 
#define __DWIN_PAGE_H__ 

#include "dwin_def.h" 

struct dwin_page *dwin_page_create(rt_uint16_t id); 
#define dwin_page_delect_safe(page)         \
do{                                         \
    dwin_page_delect(page); page = RT_NULL; \
}while(0) 
rt_err_t dwin_page_add_obj(struct dwin_page *page, struct dwin_obj *obj); 
rt_err_t dwin_page_remove_obj(struct dwin_obj *obj); 
struct dwin_page *dwin_page_current(void); 
struct dwin_page *dwin_page_get_from_id(rt_uint16_t id); 
rt_err_t dwin_page_jump(struct dwin_page *page); 
rt_err_t dwin_page_jump_id(rt_uint16_t id); 

#endif 
