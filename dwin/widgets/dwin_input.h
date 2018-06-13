/*
 * @File:   dwin_input.h 
 * @Author: liu2guang 
 * @Date:   2018-06-10 04:36:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-12     balanceTWK   implementing the input object.
 */ 

#ifndef __DWIN_INPUT_H__ 
#define __DWIN_INPUT_H__ 

#include "dwin_def.h" 

struct dwin_input
{
    struct dwin_obj obj; 
    
    char *input_str; 
    void (*input_cb)(char *str);
}; 
typedef struct dwin_input* dwin_input_t; 

rt_err_t dwin_input_init(void); 
struct dwin_input *dwin_input_create(struct dwin_page *page, rt_uint16_t addr, 
     void (*cb)(char *str)); 
rt_err_t dwin_input_delect(struct dwin_input *input); 
rt_err_t dwin_input_set_callback(struct dwin_input *input, void (*cb)(char *str)); 
rt_err_t dwin_input_set_active(struct dwin_input *input, rt_bool_t active); 

#endif 
