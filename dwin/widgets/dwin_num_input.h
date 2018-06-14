/*
 * @File:   dwin_num_input.c
 * @Author: liu2guang
 * @Date:   2018-06-13 17:36:10
 *
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-06-13     liuguang     implementing the num input object.
 */
 
#ifndef __DWIN_NUM_INPUT_H__ 
#define __DWIN_NUM_INPUT_H__ 

#include "dwin_def.h" 

struct dwin_num_input
{
    struct dwin_obj obj; 
    void (*input_cb)(rt_uint32_t value);
}; 
typedef struct dwin_num_input* dwin_num_input_t; 

rt_err_t dwin_num_input_init(void); 
struct dwin_num_input *dwin_num_input_create(struct dwin_page *page, rt_uint16_t addr, 
    void (*cb)(rt_uint32_t value)); 
rt_err_t dwin_num_input_delect(struct dwin_num_input *input); 
rt_err_t dwin_num_input_set_callback(struct dwin_num_input *input, void (*cb)(rt_uint32_t value)); 
rt_err_t dwin_num_input_set_active(struct dwin_num_input *input, rt_bool_t active); 
rt_err_t dwin_num_input_read(struct dwin_num_input *input, rt_uint32_t *value); 
rt_err_t dwin_num_input_write(struct dwin_num_input *input, rt_uint32_t value); 

#endif 
