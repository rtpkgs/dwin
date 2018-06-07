/*
 * @File:   dwin_button.c 
 * @Author: liu2guang 
 * @Date:   2018-06-07 17:27:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-07     liu2guang    implementing the button object. 
 */ 

#include "dwin_button.h" 
#include "dwin_obj.h" 

#define DWIN_BUTTON_VAR_SIZE (1) 

struct dwin_button *dwin_button_create(struct dwin_page *page, rt_uint16_t addr, void (*cb)(void *p))
{
    struct dwin_obj *obj = RT_NULL; 
    struct dwin_button *button = RT_NULL; 
    
    button = (struct dwin_button *)rt_malloc(sizeof(struct dwin_button)); 
    if(button == RT_NULL)
    {
        DWIN_DBG("Create button failed memory is not enough.\n"); 
        goto failed; 
    }
    
    obj = dwin_obj_create(addr, DWIN_BUTTON_VAR_SIZE, DWIN_WIDGET_TYPE_BUTTON); 
    if(obj == RT_NULL)
    {
        DWIN_DBG("Create button obj failed memory is not enough.\n"); 
        goto failed; 
    }
    
    button->obj = obj; 
    dwin_obj_set_cb(button->obj, cb); 
    dwin_page_add_obj(page, button->obj); 
    
    return button; 
    
failed:
    if(obj != RT_NULL)
    {
        dwin_obj_delect_safe(obj); 
    }
    
    if(button != RT_NULL)
    {
        rt_free(button); 
    }
    
    return RT_NULL; 
}

