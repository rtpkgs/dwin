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

#include "dwin_num.h" 
#include "dwin_obj.h" 
#include "dwin_page.h" 
#include "dwin_trans.h" 

/* 初始化控件解析器 */ 
rt_err_t dwin_number_init(void)
{
    return RT_EOK; 
}

struct dwin_num *dwin_number_create(struct dwin_page *page, rt_uint16_t addr, enum dwin_number_type type)
{
    struct dwin_num *number = RT_NULL; 
    
    RT_ASSERT(page != RT_NULL); 
    
    number = (struct dwin_num *)rt_malloc(sizeof(struct dwin_num)); 
    if(number == RT_NULL)
    {
        DWIN_DBG("Create number failed memory is not enough.\n"); 
        goto failed; 
    }
    
    switch(type)
    {
        case DWIN_NUMBER_TYPE_S16:
        case DWIN_NUMBER_TYPE_U16:
        {
            dwin_obj_init(&(number->obj), addr, 1, DWIN_WIDGET_TYPE_NUM); 
        }
        break; 
        
        case DWIN_NUMBER_TYPE_S32:
        case DWIN_NUMBER_TYPE_U32: 
        {
            dwin_obj_init(&(number->obj), addr, 2, DWIN_WIDGET_TYPE_NUM); 
        }
        break; 
            
        case DWIN_NUMBER_TYPE_U64:
        {
            dwin_obj_init(&(number->obj), addr, 4, DWIN_WIDGET_TYPE_NUM); 
        }
        break; 
        
        default:
        {
            DWIN_DBG("Create number failed type parameter error.\n"); 
            goto failed; 
        }
    }
    
    /* 没有自动上传的数据监听就关闭激活 */ 
    dwin_obj_set_active(&(number->obj), RT_FALSE); 
    dwin_page_add_obj(page, &(number->obj)); 
    
    return number; 
    
failed:
    if(number != RT_NULL)
    {
        rt_free(number); 
    }
    
    return RT_NULL; 
}

rt_err_t dwin_number_delect(struct dwin_num *number)
{
    RT_ASSERT(number != RT_NULL); 
    
    dwin_page_remove_obj(&(number->obj)); 
    rt_free(number); 
    
    return RT_EOK; 
} 

rt_err_t dwin_number_get_value_s16(struct dwin_num *number, rt_int16_t *value)
{
    rt_uint16_t data[1] = {0}; 
    
    RT_ASSERT(number != RT_NULL); 
    RT_ASSERT(number->type != DWIN_NUMBER_TYPE_S16); 
    
    dwin_var_read(number->obj.value_addr, data, 1); 
    *value = (rt_int16_t)data[0]; 
    
    return RT_EOK; 
}

rt_err_t dwin_number_get_value_u16(struct dwin_num *number, rt_uint16_t *value)
{
    rt_uint16_t data[1] = {0}; 
    
    RT_ASSERT(number != RT_NULL); 
    RT_ASSERT(number->type != DWIN_NUMBER_TYPE_U16); 
    
    dwin_var_read(number->obj.value_addr, data, 1); 
    *value = (rt_uint16_t)data[0]; 
    
    return RT_EOK; 
}

rt_err_t dwin_number_get_value_s32(struct dwin_num *number, rt_int32_t *value)
{
    rt_uint16_t data[2] = {0}; 
    
    RT_ASSERT(number != RT_NULL); 
    RT_ASSERT(number->type != DWIN_NUMBER_TYPE_S32); 
    
    dwin_var_read(number->obj.value_addr, data, 2); 
    *value =  (rt_int32_t)DWIN_SET_INT(data[0], data[1]); 
    
    return RT_EOK; 
}

rt_err_t dwin_number_get_value_u32(struct dwin_num *number, rt_uint32_t *value)
{
    rt_uint16_t data[2] = {0}; 
    
    RT_ASSERT(number != RT_NULL); 
    RT_ASSERT(number->type != DWIN_NUMBER_TYPE_U32); 
    
    dwin_var_read(number->obj.value_addr, data, 2); 
    *value =  (rt_uint32_t)DWIN_SET_INT(data[0], data[1]); 
    
    return RT_EOK; 
}

rt_err_t dwin_number_get_value_u64(struct dwin_num *number, uint64_t *value)
{
    rt_uint16_t data[4] = {0}; 
    
    RT_ASSERT(number != RT_NULL); 
    RT_ASSERT(number->type != DWIN_NUMBER_TYPE_U64); 
    
    dwin_var_read(number->obj.value_addr, data, 4); 
    *value = (((uint64_t)data[0]) << 48) + (((uint64_t)data[1]) << 32) + 
             (((uint64_t)data[2]) << 16) + (((uint64_t)data[3]) <<  0); 
    
    return RT_EOK; 
}

rt_err_t dwin_number_set_value_s16(struct dwin_num *number, rt_int16_t value)
{
    rt_uint16_t data[1] = {0}; 
    
    RT_ASSERT(number != RT_NULL); 
    RT_ASSERT(number->type != DWIN_NUMBER_TYPE_S16); 
    
    data[0] = (rt_uint16_t)value; 
    dwin_var_write(number->obj.value_addr, data, 1); 
    
    return RT_EOK; 
}

rt_err_t dwin_number_set_value_u16(struct dwin_num *number, rt_uint16_t value)
{
    rt_uint16_t data[1] = {0}; 
    
    RT_ASSERT(number != RT_NULL); 
    RT_ASSERT(number->type != DWIN_NUMBER_TYPE_U16); 
    
    data[0] = (rt_uint16_t)value; 
    dwin_var_write(number->obj.value_addr, data, 1); 
    
    return RT_EOK; 
}

rt_err_t dwin_number_set_value_s32(struct dwin_num *number, rt_int32_t value)
{
    rt_uint16_t data[2] = {0}; 
    
    RT_ASSERT(number != RT_NULL); 
    RT_ASSERT(number->type != DWIN_NUMBER_TYPE_S32); 
    
    data[0] = (rt_uint16_t)DWIN_GET_SHORTH(value); 
    data[1] = (rt_uint16_t)DWIN_GET_SHORTL(value); 
    dwin_var_write(number->obj.value_addr, data, 2); 
    
    return RT_EOK; 
}

rt_err_t dwin_number_set_value_u32(struct dwin_num *number, rt_uint32_t value)
{
    rt_uint16_t data[2] = {0}; 
    
    RT_ASSERT(number != RT_NULL); 
    RT_ASSERT(number->type != DWIN_NUMBER_TYPE_U32); 
    
    data[0] = (rt_uint16_t)DWIN_GET_SHORTH(value); 
    data[1] = (rt_uint16_t)DWIN_GET_SHORTL(value); 
    dwin_var_write(number->obj.value_addr, data, 2); 
    
    return RT_EOK; 
}

rt_err_t dwin_number_set_value_u64(struct dwin_num *number, uint64_t value)
{
    rt_uint16_t data[4] = {0}; 
    
    RT_ASSERT(number != RT_NULL); 
    RT_ASSERT(number->type != DWIN_NUMBER_TYPE_U64); 
    
    data[0] = (rt_uint16_t)((value >> 48) & (0xFFFF)); 
    data[1] = (rt_uint16_t)((value >> 32) & (0xFFFF));
    data[2] = (rt_uint16_t)((value >> 16) & (0xFFFF));
    data[3] = (rt_uint16_t)((value >>  0) & (0xFFFF));
    dwin_var_write(number->obj.value_addr, data, 4); 
    
    return RT_EOK; 
}
