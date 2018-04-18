/*
 * @File:   dwin_number.c 
 * @Author: liu2guang 
 * @Date:   2018-04-08 12:38:11 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-04-08     liu2guang    Create first version. 
 */ 
 
#include "dwin_number.h" 
#include "dwin_trans.h" 

#define DWIN_NUMBER_SPACE_BYTE (2) 

dwin_number_t dwin_number_new(const char *name, uint32_t number)
{
    RT_ASSERT(name != RT_NULL); 
    
    /* malloc space */ 
    dwin_space_t space = dwin_space_alloc(name, DWIN_NUMBER_SPACE_BYTE, DWIN_TYPE_NUMBER); 
    RT_ASSERT(space != RT_NULL); 
    
    /* malloc handle */
    dwin_number_t handle = (dwin_number_t)rt_malloc(sizeof(struct dwin_number)); 
    RT_ASSERT(handle != RT_NULL); 
    
    /* fill struct */ 
    handle->number = number; 
    space ->plugin = (void *)handle; 
    
    rt_uint16_t number16[2] = {0}; 
    
    handle->number = number; 
    number16[0] = ((number & 0xFFFF0000) >> 0x10); 
    number16[1] = ((number & 0x0000FFFF) >> 0x00); 
    dwin_var_write(space->addr, number16, DWIN_NUMBER_SPACE_BYTE); 
    
    return handle; 
}

rt_err_t dwin_number_write(const char *name, uint32_t number)
{
    RT_ASSERT(name != RT_NULL); 
    
    /* find number plugin */
    dwin_space_t space = dwin_space_find(name); 
    RT_ASSERT(space != RT_NULL); 
    
    dwin_number_t handle = (dwin_number_t)(space->plugin); 
    RT_ASSERT(handle != RT_NULL); 
    
    rt_uint16_t number16[2] = {0}; 
    
    handle->number = number; 
    number16[0] = ((number & 0xFFFF0000) >> 0x10); 
    number16[1] = ((number & 0x0000FFFF) >> 0x00); 
    dwin_var_write(space->addr, number16, DWIN_NUMBER_SPACE_BYTE); 
    
    return RT_EOK; 
}

uint32_t dwin_number_read(const char *name)
{
    RT_ASSERT(name != RT_NULL); 
    
    /* find number plugin */
    dwin_space_t space = dwin_space_find(name); 
    RT_ASSERT(space != RT_NULL); 
    
    dwin_number_t handle = (dwin_number_t)(space->plugin); 
    RT_ASSERT(handle != RT_NULL); 
    
    return handle->number; 
}
