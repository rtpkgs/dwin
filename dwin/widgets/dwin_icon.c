/*
 * @File:   dwin_icon.c 
 * @Author: liu2guang 
 * @Date:   2018-06-14 15:56:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-14     liu2guang    implement the icon object. 
 */ 

#include "dwin_icon.h" 
#include "dwin_obj.h" 
#include "dwin_page.h" 
#include "dwin_parse.h" 
#include "dwin_trans.h" 

#define DWIN_ICON_VAR_SIZE (1)  

/* 初始化Button控件解析器 */ 
rt_err_t dwin_icon_init(void)
{
    return RT_EOK; 
}

struct dwin_icon *dwin_icon_create(struct dwin_page *page, rt_uint16_t addr, rt_uint16_t init_index)
{
    struct dwin_icon *icon = RT_NULL; 
    
    RT_ASSERT(page != RT_NULL); 
    
    icon = (struct dwin_icon *)rt_malloc(sizeof(struct dwin_icon)); 
    if(icon == RT_NULL)
    {
        DWIN_DBG("Create icon failed memory is not enough.\n"); 
        goto failed; 
    }
    
    dwin_obj_init(&(icon->obj), addr, DWIN_ICON_VAR_SIZE, DWIN_WIDGET_TYPE_ICON); 
    dwin_page_add_obj(page, &(icon->obj)); 
    dwin_icon_set_index(icon, init_index); 
    
    return icon; 
    
failed:
    if(icon != RT_NULL)
    {
        rt_free(icon); 
    }
    
    return RT_NULL; 
}

rt_err_t dwin_icon_delect(struct dwin_icon *icon)
{
    RT_ASSERT(icon != RT_NULL); 
    
    dwin_page_remove_obj(&(icon->obj)); 
    rt_free(icon); 
    
    return RT_EOK; 
} 

rt_err_t dwin_icon_set_index(struct dwin_icon *icon, rt_uint16_t index)
{
    RT_ASSERT(icon != RT_NULL); 
    
    icon->current_index = index; 
    if(dwin_var_write(icon->obj.value_addr, &index, 1)!= RT_EOK)
    {
        return RT_ERROR; 
    }
    
    return RT_EOK; 
}
