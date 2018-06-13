/*
 * @File:   dwin_number.c 
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

enum dwin_number_type
{
    DWIN_NUMBER_TYPE_S16 = 0, 
    DWIN_NUMBER_TYPE_U16,
    
    DWIN_NUMBER_TYPE_S32, 
    DWIN_NUMBER_TYPE_U32,
    
    DWIN_NUMBER_TYPE_U64,
    
    //DWIN_NUMBER_TYPE_VP_HIGH_BYTE, 
    //DWIN_NUMBER_TYPE_VP_LOW_BYTE, 
}; 
typedef enum dwin_number_type dwin_number_type_t; 

struct dwin_number
{
    struct dwin_obj obj; 
    enum dwin_number_type type; 
}; 
typedef struct dwin_number* dwin_number_t; 

rt_err_t dwin_number_init(void); 
struct dwin_number *dwin_number_create(struct dwin_page *page, rt_uint16_t addr, 
    enum dwin_number_type type); 
rt_err_t dwin_number_delect(struct dwin_number *number); 
rt_err_t dwin_number_get_value_s16(struct dwin_number *number, rt_int16_t *value); 
rt_err_t dwin_number_get_value_u16(struct dwin_number *number, rt_uint16_t *value); 
rt_err_t dwin_number_get_value_s32(struct dwin_number *number, rt_int32_t *value); 
rt_err_t dwin_number_get_value_u32(struct dwin_number *number, rt_uint32_t *value); 
rt_err_t dwin_number_get_value_u64(struct dwin_number *number, uint64_t *value); 

rt_err_t dwin_number_set_value_s16(struct dwin_number *number, rt_int16_t value); 
rt_err_t dwin_number_set_value_u16(struct dwin_number *number, rt_uint16_t value); 
rt_err_t dwin_number_set_value_s32(struct dwin_number *number, rt_int32_t value); 
rt_err_t dwin_number_set_value_u32(struct dwin_number *number, rt_uint32_t value); 
rt_err_t dwin_number_set_value_u64(struct dwin_number *number, uint64_t value); 

#endif 
