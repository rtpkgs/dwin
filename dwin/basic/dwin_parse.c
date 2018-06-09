/*
 * @File:   dwin_parse.c 
 * @Author: liu2guang 
 * @Date:   2018-04-07 14:52:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-04-07     liu2guang    update v2 framework. 
 */ 
 
#include "dwin_parse.h" 
#include "dwin_obj.h" 
#include "dwin_page.h" 

rt_inline rt_uint16_t dwin_parse_addr(uint8_t *data)
{
    return (data[4]<<8) + data[5]; 
}

rt_inline rt_uint16_t dwin_parse_size(uint8_t *data)
{
    return data[6]; 
}

struct dwin_parse *dwin_parse_create(enum dwin_obj_type type, void (*event)(struct dwin_obj *obj))
{
    struct dwin_parse *parse = RT_NULL; 
    
    RT_ASSERT(type  <  DWIN_WIDGET_TYPE_MAX); 
    RT_ASSERT(event != RT_NULL); 
    
    parse = (struct dwin_parse *)rt_malloc(sizeof(struct dwin_parse)); 
    if(parse == RT_NULL)
    {
        DWIN_DBG("Create parse failed memory is not enough.\n"); 
        goto failed; 
    }
    
    rt_list_init(&(parse->list)); 
    parse->type = type; 
    parse->event = event; 
    
    return parse; 
    
failed:
    if(parse != RT_NULL)
    {
        dwin_parse_delect_safe(parse); 
    }
    
    return RT_NULL; 
}

rt_err_t dwin_parse_delect(struct dwin_parse *parse)
{
    rt_free(parse); 
    
    return RT_EOK; 
}

rt_err_t dwin_parse_register(struct dwin_parse *parse)
{
    RT_ASSERT(parse != RT_NULL); 
    
    rt_list_insert_before(&(dwin.parses), &(parse->list)); 
    dwin.parse_num++; 
    
    return RT_EOK; 
}

/* Todo: 移除解析器 */ 

/* Todo: 待优化结构, 和思考如何和控件优雅的分离, 控件解析注册机制??? */ 
void dwin_parse_exe(uint8_t *data, uint8_t len)
{
    rt_list_t *list = RT_NULL; 
    struct dwin_page *page = dwin_page_current(); 
    
    RT_ASSERT(data != RT_NULL); 
    
    /* 遍历当前页中所有控件, 查找地址和长度匹配并且激活的obj控件对象 */ 
    
    struct dwin_obj *obj_temp = RT_NULL; 
    struct dwin_obj *obj_widget = RT_NULL; 
    
    for(list = page->objs.next; list != &(page->objs); list = list->next)
    {
        obj_temp = rt_list_entry(list, struct dwin_obj, list); 
        
        if((obj_temp->value_addr == dwin_parse_addr(data)) && 
           (obj_temp->value_size == dwin_parse_size(data)) && 
           (obj_temp->active == RT_TRUE))
        {
            obj_widget = obj_temp;
            break;
        }
    }
    
    /* 按照注册的方式处理控件事件 */ 
    struct dwin_parse *parse = RT_NULL; 
    
    if(obj_widget != RT_NULL)
    {
        for(list = dwin.parses.next; list != &(dwin.parses); list = list->next)
        {
            parse = rt_list_entry(list, struct dwin_parse, list); 
            
            if((parse->type == obj_widget->type) && (parse->event != RT_NULL))
            {
                parse->event(obj_widget); 
                return; 
            }
        }
    }
    
    DWIN_DBG("Data frame no find parse.\n"); 
}
