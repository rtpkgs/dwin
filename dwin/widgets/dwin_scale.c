/*
 * @File:   dwin_scale.c 
 * @Author: liu2guang 
 * @Date:   2018-06-10 04:36:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-10     liu2guang    implementing the button object. 
 */ 

#include "dwin_scale.h" 
#include "dwin_obj.h" 
#include "dwin_page.h" 
#include "dwin_parse.h" 

#define DWIN_SCALE_VAR_SIZE (1) 

static struct dwin_parse *parse = RT_NULL; 

/* Scale解析器事件处理函数 */ 
static void dwin_scale_event(struct dwin_obj *obj, uint8_t *data, uint8_t len)
{
    struct dwin_scale *scale = (struct dwin_scale *)obj; 
    
    scale->current_value = DWIN_SET_SHORT(data[7], data[8]); 
    scale->input_cb(scale->current_value); 
}

/* 初始化Button控件解析器 */ 
rt_err_t dwin_scale_init(void)
{
    RT_ASSERT(parse == RT_NULL); 
    
    /* 注册控件解析器 */ 
    parse = dwin_parse_create(DWIN_WIDGET_TYPE_SCALE, dwin_scale_event); 
    dwin_parse_register(parse); 
    
    return RT_EOK; 
}

struct dwin_scale *dwin_scale_create(struct dwin_page *page, rt_uint16_t addr, void (*cb)(rt_uint16_t value))
{
    struct dwin_scale *scale = RT_NULL; 
    
    scale = (struct dwin_scale *)rt_malloc(sizeof(struct dwin_scale)); 
    if(scale == RT_NULL)
    {
        DWIN_DBG("Create scale failed memory is not enough.\n"); 
        goto failed; 
    }
    
    dwin_obj_init(&(scale->obj), addr, DWIN_SCALE_VAR_SIZE, DWIN_WIDGET_TYPE_SCALE); 
    dwin_page_add_obj(page, &(scale->obj)); 
    scale->current_value = 0; 
    scale->input_cb = cb; 
    
    return scale; 
    
failed:
    if(scale != RT_NULL)
    {
        rt_free(scale); 
    }
    
    return RT_NULL; 
}
