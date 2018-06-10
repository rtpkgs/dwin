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
#include "dwin_page.h" 
#include "dwin_parse.h" 

/* Todo:按键文字??? */
#define DWIN_BUTTON_VAR_SIZE (1) 

static struct dwin_parse *parse = RT_NULL; 

/* Button解析器事件处理函数 */ 
static void dwin_button_event(struct dwin_obj *obj, uint8_t *data, uint8_t len)
{
    ((struct dwin_button *)obj)->push_cb(); 
}

/* 初始化Button控件解析器 */ 
rt_err_t dwin_button_init(void)
{
    RT_ASSERT(parse == RT_NULL); 
    
    /* 注册控件解析器 */ 
    parse = dwin_parse_create(DWIN_WIDGET_TYPE_BUTTON, dwin_button_event); 
    dwin_parse_register(parse); 
    
    return RT_EOK; 
}

struct dwin_button *dwin_button_create(struct dwin_page *page, rt_uint16_t addr, void (*cb)(void))
{
    struct dwin_button *button = RT_NULL; 
    
    button = (struct dwin_button *)rt_malloc(sizeof(struct dwin_button)); 
    if(button == RT_NULL)
    {
        DWIN_DBG("Create button failed memory is not enough.\n"); 
        goto failed; 
    }
    
    dwin_obj_init(&(button->obj), addr, DWIN_BUTTON_VAR_SIZE, DWIN_WIDGET_TYPE_BUTTON); 
    dwin_page_add_obj(page, &(button->obj)); 
    button->push_cb = cb; 
    
    return button; 
    
failed:
    if(button != RT_NULL)
    {
        rt_free(button); 
    }
    
    return RT_NULL; 
}

rt_err_t dwin_button_delect(struct dwin_button *button)
{
    RT_ASSERT(button != RT_NULL); 
    
    dwin_page_remove_obj(&(button->obj)); 
    rt_free(button); 
    
    return RT_EOK; 
} 

rt_err_t dwin_button_set_callback(struct dwin_button *button, void (*cb)(void))
{
    RT_ASSERT(button != RT_NULL); 
    RT_ASSERT(cb     != RT_NULL); 
    
    button->push_cb = cb; 
    
    return RT_EOK; 
}

rt_err_t dwin_button_set_active(struct dwin_button *button, rt_bool_t active)
{
    RT_ASSERT(button != RT_NULL); 
    
    dwin_obj_set_active(&(button->obj), active); 
    
    return RT_EOK; 
}
