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

#ifndef __DWIN_GBK_INPUT_H__ 
#define __DWIN_GBK_INPUT_H__ 

#include "dwin_def.h" 

struct dwin_gbk_input
{
    struct dwin_obj obj; 
    void (*input_cb)(char *str, rt_uint8_t len);
}; 
typedef struct dwin_gbk_input* dwin_gbk_input_t; 

rt_err_t dwin_gbk_input_init(void); 
struct dwin_gbk_input *dwin_gbk_input_create(struct dwin_page *page, rt_uint16_t addr, 
    void (*cb)(char *str, rt_uint8_t len)); 
rt_err_t dwin_gbk_input_delect(struct dwin_gbk_input *input); 
rt_err_t dwin_gbk_input_set_callback(struct dwin_gbk_input *input, void (*cb)(char *str, rt_uint8_t len)); 
rt_err_t dwin_gbk_input_set_active(struct dwin_gbk_input *input, rt_bool_t active); 
rt_err_t dwin_gbk_input_read(struct dwin_gbk_input *input, char *str, rt_uint8_t *len); 
rt_err_t dwin_gbk_input_write(struct dwin_gbk_input *input, char *str, rt_uint8_t len); 

#endif 
