/*
 * @File:   dwin_input.c
 * @Author: liu2guang
 * @Date:   2018-06-10 04:36:10
 *
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-06-12     balanceTWK   implement the gbk input object. 
 */

#include "dwin_gbk_input.h"
#include "dwin_obj.h"
#include "dwin_page.h"
#include "dwin_parse.h"
#include "dwin_trans.h"

static struct dwin_parse *parse = RT_NULL;

rt_err_t dwin_gbk_input_read(struct dwin_gbk_input *input,char *str,rt_uint8_t len)
{
    rt_uint16_t data[len];
    if (dwin_var_read(input->obj.value_addr, data, len) != RT_EOK)
    {
        DWIN_DBG("dwin_gbk_input_read failed .\n");
        return RT_ERROR;
    }
    else
    {
        DWIN_DBG("dwin_gbk_input_read succeed .\n");
        for(rt_uint32_t i=0;i<len;i++)
        {
            str[i*2]=(char)(data[i]>>8);
            str[i*2+1]=(char)(data[i]);
        }
        DWIN_DBG("dwin_gbk_input_read : %s\n",str);
    }
    return RT_EOK;
}

rt_err_t dwin_gbk_input_write(struct dwin_gbk_input *input,char *str,rt_uint8_t len)
{
    len=len/2+len%2;
    rt_uint16_t data[len];
    for(rt_uint32_t i=0;i<len;i++)
    {
        data[i]=(rt_uint16_t)(str[i*2]<<8)+(rt_uint16_t)(str[i*2+1]);
    }
    if (dwin_var_write(input->obj.value_addr,data, len) != RT_EOK)
    {
        DWIN_DBG("dwin_var_write(input->obj.value_addr,(rt_uint16_t*)str, len) != RT_EOK .\n");
        return RT_ERROR;
    }
    else
    {
        DWIN_DBG("dwin_gbk_input_write : %s\n",str);
    }
    return RT_EOK;
}

/* input解析器事件处理函数 */
static void dwin_gbk_input_event(struct dwin_obj *obj, uint8_t *data, uint8_t len)
{
    rt_uint8_t i = 0;
    char *input_str = (char*)&data[7]; 
    struct dwin_gbk_input *input = (struct dwin_gbk_input *)obj;
    
    while(input_str[i] != (char)0xff)
    {
        i++;
        if(input_str[i] == (char)0x00)
        {
            goto nogbkinput;
        }
    }
    input_str[i] = 0;
    input->input_cb(input_str, i);
    
    return ;
    
nogbkinput:    
    DWIN_DBG("gbk_input_event occurred,but no content was received.\n");
}

/* 初始化input控件解析器 */
rt_err_t dwin_gbk_input_init(void)
{
    RT_ASSERT(parse == RT_NULL);

    /* 注册控件解析器 */
    parse = dwin_parse_create(DWIN_WIDGET_TYPE_GBK_INPUT, dwin_gbk_input_event);
    dwin_parse_register(parse);

    return RT_EOK;
}

struct dwin_gbk_input *dwin_gbk_input_create(struct dwin_page *page, rt_uint16_t addr, 
    void (*cb)(char *str, rt_uint8_t len))
{
    struct dwin_gbk_input *input = RT_NULL;

    input = (struct dwin_gbk_input *)rt_malloc(sizeof(struct dwin_gbk_input));
    if (input == RT_NULL)
    {
        DWIN_DBG("Create gbk input failed memory is not enough.\n");
        goto failed;
    }

    dwin_obj_init(&(input->obj), addr, DWIN_OBJ_VARY_LENGHT, DWIN_WIDGET_TYPE_GBK_INPUT);
    dwin_page_add_obj(page, &(input->obj));
    input->input_cb = cb;
    
    return input;

failed:
    if (input != RT_NULL)
    {
        rt_free(input);
    }

    return RT_NULL;
}

rt_err_t dwin_gbk_input_delect(struct dwin_gbk_input *input)
{
    RT_ASSERT(input != RT_NULL);

    dwin_page_remove_obj(&(input->obj));
    rt_free(input);

    return RT_EOK;
}

rt_err_t dwin_gbk_input_set_callback(struct dwin_gbk_input *input, void (*cb)(char *str, rt_uint8_t len))
{
    RT_ASSERT(input != RT_NULL);
    RT_ASSERT(cb     != RT_NULL);

    input->input_cb = cb;

    return RT_EOK;
}

rt_err_t dwin_gbk_input_set_active(struct dwin_gbk_input *input, rt_bool_t active)
{
    RT_ASSERT(input != RT_NULL);

    dwin_obj_set_active(&(input->obj), active);

    return RT_EOK;
}
