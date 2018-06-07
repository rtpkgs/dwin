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

struct dwin_obj *dwin_obj_create(rt_uint16_t addr, rt_uint8_t size, enum dwin_obj_type type)
{
    struct dwin_obj *obj = RT_NULL; 
    
    RT_ASSERT(size != 0); 
    RT_ASSERT(type <  DWIN_WIDGET_TYPE_MAX); 
    
    obj = (struct dwin_obj *)rt_malloc(sizeof(struct dwin_obj)); 
    if(obj == RT_NULL)
    {
        DWIN_DBG("Create Obj failed memory is not enough, addr 0x%.4x size %d type %d.\n"); 
        goto failed; 
    }
    
    obj->type       = type; 
    obj->value_addr = addr; 
    obj->value_size = size; 
    obj->active     = RT_FALSE; 
    
    rt_list_init(&(obj->list)); 
    
    return obj; 
    
failed:
    if(obj != RT_NULL)
    {
        rt_free(obj); 
    }
    
    return RT_NULL; 
}

rt_err_t dwin_obj_delect(struct dwin_obj *obj)
{
    rt_free(obj); 
    
    return RT_EOK; 
}

rt_err_t dwin_obj_active(struct dwin_obj *obj, rt_bool_t active)
{
    RT_ASSERT(obj != RT_NULL); 
    
    obj->active = active; 
    
    return RT_EOK; 
}

rt_err_t dwin_obj_set_cb(struct dwin_obj *obj, void (*cb)(void *p))
{
    RT_ASSERT(cb  != RT_NULL); 
    RT_ASSERT(obj != RT_NULL); 
    
    obj->cb = cb; 
    
    return RT_EOK; 
}

#if defined(DWIN_USING_DEBUG)
void dwin_obj_printf(struct dwin_obj *obj)
{
    const char *info[] = {"BUTTON", "NUMBER", "TEXT","INPUT", "SCALE","ICON", "QRCODE"};
    
    if(obj == RT_NULL)
    {
        DWIN_DBG("The 0x%.8x obj, It's an empty pointer.\n"); 
    }
    else
    {
        DWIN_DBG("The 0x%.8x obj, Type \033[32m%s\033[0m, Addr \033[32m0x%.4x\033[0m, Size \033[32m%d\033[0m Bytes, active %d.\n", 
            obj, info[obj->type], obj->value_addr, obj->value_size*2, obj->active); 
    } 
} 
#endif 
