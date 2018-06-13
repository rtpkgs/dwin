/*
 * @File:   dwin_num.c 
 * @Author: liu2guang 
 * @Date:   2018-06-13 14:30:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-13     liu2guang    implementing the number object. 
 */ 
 
#ifndef __DWIN_NUMBER_H__ 
#define __DWIN_NUMBER_H__ 

#include "dwin_def.h" 

enum dwin_num_type
{
    DWIN_NUM_TYPE_S16 = 0, 
    DWIN_NUM_TYPE_U16,
    DWIN_NUM_TYPE_S32, 
    DWIN_NUM_TYPE_U32,
    DWIN_NUM_TYPE_U64, 
}; 
typedef enum dwin_num_type dwin_num_type_t; 

struct dwin_num
{
    struct dwin_obj obj; 
    enum dwin_num_type type; 
}; 
typedef struct dwin_num* dwin_num_t; 

rt_err_t dwin_num_init(void); 
struct dwin_num *dwin_num_create(struct dwin_page *page, rt_uint16_t addr, 
    enum dwin_num_type type); 
rt_err_t dwin_num_delect(struct dwin_num *number); 
rt_err_t dwin_num_get_value_s16(struct dwin_num *number, rt_int16_t *value); 
rt_err_t dwin_num_get_value_u16(struct dwin_num *number, rt_uint16_t *value); 
rt_err_t dwin_num_get_value_s32(struct dwin_num *number, rt_int32_t *value); 
rt_err_t dwin_num_get_value_u32(struct dwin_num *number, rt_uint32_t *value); 
rt_err_t dwin_num_get_value_u64(struct dwin_num *number, uint64_t *value); 

rt_err_t dwin_num_set_value_s16(struct dwin_num *number, rt_int16_t value); 
rt_err_t dwin_num_set_value_u16(struct dwin_num *number, rt_uint16_t value); 
rt_err_t dwin_num_set_value_s32(struct dwin_num *number, rt_int32_t value); 
rt_err_t dwin_num_set_value_u32(struct dwin_num *number, rt_uint32_t value); 
rt_err_t dwin_num_set_value_u64(struct dwin_num *number, uint64_t value); 

#endif 
