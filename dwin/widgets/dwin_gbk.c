/*
 * @File:   dwin_gbk.c 
 * @Author: liu2guang 
 * @Date:   2018-06-19 14:39:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-19     liu2guang    implement the gbk object. 
 */ 

#include "dwin_gbk.h" 
#include "dwin_obj.h" 
#include "dwin_page.h" 
#include "dwin_trans.h" 
#include "string.h"

/* 初始化控件解析器 */ 
rt_err_t dwin_gbk_init(void)
{
    return RT_EOK; 
}

struct dwin_gbk *dwin_gbk_create(struct dwin_page *page, rt_uint16_t addr, rt_uint16_t max_len)
{
    struct dwin_gbk *gbk = RT_NULL; 
    
    RT_ASSERT(page != RT_NULL); 
    
    gbk = (struct dwin_gbk *)rt_malloc(sizeof(struct dwin_gbk)); 
    if(gbk == RT_NULL)
    {
        DWIN_DBG("Create gbk failed memory is not enough.\n"); 
        goto failed; 
    }
    
    gbk->max_len = max_len; 
    dwin_obj_init(&(gbk->obj), addr, DWIN_OBJ_VARY_LENGHT, DWIN_WIDGET_TYPE_GBK); 
    dwin_obj_set_active(&(gbk->obj), RT_TRUE); 
    dwin_page_add_obj(page, &(gbk->obj)); 
    
    return gbk; 
    
failed:
    if(gbk != RT_NULL)
    {
        rt_free(gbk); 
    }
    
    return RT_NULL; 
}

rt_err_t dwin_gbk_delect(struct dwin_gbk *gbk)
{
    RT_ASSERT(gbk != RT_NULL); 
    
    dwin_page_remove_obj(&(gbk->obj)); 
    rt_free(gbk); 
    
    return RT_EOK; 
} 

rt_err_t dwin_gbk_show_string(struct dwin_gbk *gbk, char *string)
{
    rt_uint16_t len = 0;
    rt_uint16_t index = 0; 
    
    RT_ASSERT(gbk    != RT_NULL);
    RT_ASSERT(string != RT_NULL);
    
    len = strlen(string); 
    RT_ASSERT(((len/2 + len%2)*2) <= gbk->max_len); 
    
    rt_uint8_t data[(len/2 + len%2)*2]; 
    
    rt_memset(data, 0x00, sizeof(data)); 
 
    /* 复制数据 */
    for(index = 0; index < len; index++)
    {
        data[index] = string[index]; 
    }
    
    dwin_var_write(gbk->obj.value_addr, dwin_string_conv(data), (len/2 + len%2));
    return RT_EOK; 
}

rt_err_t dwin_gbk_clear(struct dwin_gbk *gbk)
{
    rt_uint16_t data[gbk->max_len]; 
    
    rt_memset(data, 0x00, sizeof(data)); 
    dwin_var_write(gbk->obj.value_addr, data, gbk->max_len);
    
    return RT_EOK; 
}
