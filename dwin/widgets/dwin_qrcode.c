/*
 * @File:   dwin_qrcode.c
 * @Author: balanceTWK
 * @Date:   2018-06-15 14:00:00
 *
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-06-15     balanceTWK    implementing the button object.
 */

#include "dwin_qrcode.h"
#include "dwin_obj.h"
#include "dwin_page.h"
#include "dwin_trans.h"
#include "string.h"

/* 初始化控件解析器 */ 
rt_err_t dwin_qrcode_init(void)
{
    return RT_EOK; 
}

struct dwin_qrcode *dwin_qrcode_create(struct dwin_page *page, rt_uint16_t addr, rt_uint16_t max_len)
{
    struct dwin_qrcode *qrcode = RT_NULL; 
    
    RT_ASSERT(page    != RT_NULL); 
    RT_ASSERT(max_len != 0); 
    
    qrcode = (struct dwin_qrcode *)rt_malloc(sizeof(struct dwin_qrcode)); 
    if(qrcode == RT_NULL)
    {
        DWIN_DBG("Create qrcode failed memory is not enough.\n"); 
        goto failed; 
    }
    
    qrcode->max_len = max_len; 
    dwin_obj_init(&(qrcode->obj), addr, DWIN_OBJ_VARY_LENGHT, DWIN_WIDGET_TYPE_QRCODE); 
    dwin_obj_set_active(&(qrcode->obj), RT_TRUE); 
    dwin_page_add_obj(page, &(qrcode->obj)); 
    
    return qrcode; 
    
failed:
    if(qrcode != RT_NULL)
    {
        rt_free(qrcode); 
    }
    
    return RT_NULL; 
}

rt_err_t dwin_qrcode_delect(struct dwin_qrcode *qrcode)
{
    RT_ASSERT(qrcode != RT_NULL); 
    
    dwin_page_remove_obj(&(qrcode->obj)); 
    rt_free(qrcode); 
    
    return RT_EOK; 
} 

rt_err_t dwin_qrcode_show_string(struct dwin_qrcode *qrcode, char *string)
{
    rt_uint16_t len = 0;
    rt_uint16_t index = 0; 
    
    RT_ASSERT(qrcode != RT_NULL);
    RT_ASSERT(string != RT_NULL);
    
    len = strlen(string); 
    RT_ASSERT(((len/2 + len%2)*2) <= qrcode->max_len); 
    
    rt_uint8_t data[(len/2 + len%2)*2]; 
    
    rt_memset(data, 0x00, sizeof(data)); 
 
    /* 复制数据 */
    for(index = 0; index < len; index++)
    {
        data[index] = string[index]; 
    }
    
    dwin_var_write(qrcode->obj.value_addr, dwin_string_conv(data), (len/2 + len%2));
    return RT_EOK; 
}

rt_err_t dwin_qrcode_show_url(struct dwin_qrcode *qrcode, char *url)
{
    return dwin_qrcode_show_string(qrcode, url); 
}

rt_err_t dwin_qrcode_clear(struct dwin_qrcode *qrcode)
{
    rt_uint16_t data[qrcode->max_len]; 
    
    rt_memset(data, 0x00, sizeof(data)); 
    dwin_var_write(qrcode->obj.value_addr, data, qrcode->max_len);
    
    return RT_EOK; 
}
