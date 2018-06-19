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
#include "dwin_system.h"

rt_inline rt_uint16_t dwin_parse_addr(uint8_t *data)
{
    return (data[4]<<8) + data[5]; 
}

/* 创建解析器 */ 
struct dwin_parse *dwin_parse_create(enum dwin_obj_type type, 
    void (*event)(struct dwin_obj *obj, uint8_t *data, uint8_t len))
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

/* 删除解析器 */ 
rt_err_t dwin_parse_delect(struct dwin_parse *parse)
{
    rt_free(parse); 
    
    return RT_EOK; 
}

/* 注册解析器 */ 
rt_err_t dwin_parse_register(struct dwin_parse *parse)
{
    RT_ASSERT(parse != RT_NULL); 
    
    rt_list_insert_before(&(dwin.parses), &(parse->list)); 
    dwin.parse_num++; 
    
    return RT_EOK; 
}

/* 注销解析器 */ 
rt_err_t dwin_parse_unregister(struct dwin_parse *parse)
{
    RT_ASSERT(parse != RT_NULL); 
    
    if(dwin.parse_num == 0)
    {
        return RT_ERROR; 
    }
    
    rt_list_remove(&(parse->list)); 
    dwin.parse_num--; 
    
    return RT_EOK; 
}

/* 打印已经注册解析器信息 */ 
void dwin_parse_register_info(void)
{
    extern const char *widgets_info[]; 
    struct dwin_parse *parse = RT_NULL; 
    rt_list_t *list_parse  = RT_NULL; 
    
    for(list_parse = dwin.parses.next; list_parse != &(dwin.parses); list_parse = list_parse->next)
    {
        parse = rt_list_entry(list_parse, struct dwin_parse, list); 
        
        DWIN_INFO("Parse: type %s, event 0x%.8x.\n", widgets_info[parse->type], (rt_uint32_t)(parse->event)); 
    }
}

/* Todo: 低优先级线程, 消息队列FIFO机制处理事件 */ 
void dwin_parse_exe(rt_uint8_t *data, rt_uint8_t len)
{
    rt_list_t *list_widget = RT_NULL; 
    rt_list_t *list_parse  = RT_NULL; 
    
    struct dwin_parse *parse = RT_NULL; 
    
    /* 获取当前页面 */ 
    rt_uint16_t pageid = 0; 
    dwin_system_page(&pageid); 
    dwin.page_cur = dwin_page_get_from_id(pageid); 
    struct dwin_page *page = dwin_page_current(); 
    
    RT_ASSERT(data != RT_NULL); 
    
    /* 遍历当前页中所有控件, 查找地址和长度匹配并且激活的obj控件对象 */ 
    struct dwin_obj *obj_temp   = RT_NULL; 
    struct dwin_obj *obj_widget = RT_NULL; 
    
    for(list_widget = page->objs.next; list_widget != &(page->objs); list_widget = list_widget->next)
    {
        obj_temp = rt_list_entry(list_widget, struct dwin_obj, list); 
        
        if((obj_temp->value_addr == dwin_parse_addr(data)) && 
           (obj_temp->active == RT_TRUE))
        {
            obj_widget = obj_temp;
            
            for(list_parse = dwin.parses.next; list_parse != &(dwin.parses); list_parse = list_parse->next)
            {
                parse = rt_list_entry(list_parse, struct dwin_parse, list); 
                
                if((parse->type == obj_widget->type) && (parse->event != RT_NULL))
                {
                    parse->event(obj_widget, data, len); 
                    break; 
                }
            }
        }
    }
    
    if(obj_widget == RT_NULL)
    {
        DWIN_DBG("The data frame no find widget parse.\n"); 
    }
}

rt_err_t dwin_parse_send(struct dwin_data_frame *data)
{
    return rt_mq_send(dwin.parse_mq, (void *)data, sizeof(struct dwin_data_frame)); 
} 

/* 自动上传事件解析器 */ 
void dwin_parse_run(void *p)
{
    struct dwin_data_frame data = {0}; 
    
    while(1)
    {
        rt_mq_recv(dwin.parse_mq, (void *)&data, sizeof(struct dwin_data_frame), RT_WAITING_FOREVER); 
        dwin_parse_exe(data.data, data.len); 
        rt_memset(&data, 0x00, sizeof(struct dwin_data_frame)); 
    }
}

rt_err_t dwin_parse_init(void)
{
    /* Create the parse thread */ 
    dwin.parse_thread = rt_thread_create("tparse", dwin_parse_run, RT_NULL, 2048, 25, 10); 
    if(dwin.parse_thread == RT_NULL) 
    {
        DWIN_DBG("Thw dwin parse thread create failed.\n"); 
        return RT_ENOSYS; 
    }

    /* Start the watch thread */ 
    rt_thread_startup(dwin.parse_thread); 
    
    return RT_EOK; 
}
