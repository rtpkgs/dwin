/*
 * @File:   dwin_scale.h 
 * @Author: liu2guang 
 * @Date:   2018-06-10 04:36:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-10     liu2guang    implementing the button object. 
 */ 

#ifndef __DWIN_SCALE_H__ 
#define __DWIN_SCALE_H__ 

#include "dwin_def.h" 

struct dwin_scale
{
    struct dwin_obj obj; 
    
    rt_uint16_t current_value; 
    void (*input_cb)(rt_uint16_t value); 
}; 
typedef struct dwin_scale* dwin_scale_t; 

rt_err_t dwin_scale_init(void); 
struct dwin_scale *dwin_scale_create(struct dwin_page *page, rt_uint16_t addr, 
    void (*cb)(rt_uint16_t value), rt_uint16_t init_value); 
rt_err_t dwin_scale_delect(struct dwin_scale *scale); 
rt_err_t dwin_scale_set_callback(struct dwin_scale *scale, void (*cb)(rt_uint16_t value)); 
rt_err_t dwin_scale_set_active(struct dwin_scale *scale, rt_bool_t active); 
rt_err_t dwin_scale_get_value(struct dwin_scale *scale, rt_uint16_t *value); 
rt_err_t dwin_scale_set_value(struct dwin_scale *scale, rt_uint16_t value); 

#endif 
