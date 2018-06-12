/*
 * @File:   dwin_obj.c 
 * @Author: liu2guang 
 * @Date:   2018-06-06 17:27:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-06     liu2guang    implementing the object. 
 */ 

#include "dwin_obj.h" 

void dwin_obj_init(struct dwin_obj *obj, rt_uint16_t addr, rt_uint8_t size, enum dwin_obj_type type)
{
    RT_ASSERT(obj != RT_NULL); 
    
    obj->type       = type;
    obj->value_addr = addr; 
    obj->value_size = size; 
    obj->active     = RT_TRUE; 
}

rt_err_t dwin_obj_set_active(struct dwin_obj *obj, rt_bool_t active)
{
    RT_ASSERT(obj != RT_NULL); 
    
    obj->active = active; 
    
    return RT_EOK; 
}
