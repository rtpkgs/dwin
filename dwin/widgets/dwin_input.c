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
 * 2018-06-12     balanceTWK   implementing the input object.
 */

#include "dwin_input.h"
#include "dwin_obj.h"
#include "dwin_page.h"
#include "dwin_parse.h"

#include "dwin_button.h"
#include "dwin_obj.h"
#include "dwin_page.h"
#include "dwin_parse.h"

static struct dwin_parse *parse = RT_NULL;

/* input解析器事件处理函数 */
static void dwin_input_event(struct dwin_obj *obj, uint8_t *data, uint8_t len)
{
    struct dwin_input *input = (struct dwin_input *)obj;

    input->input_str =(char*) &data[7];
    rt_uint32_t i = 0;

    while (input->input_str[i] !=(char) 0xff)
    {
        i++;
    }
    input->input_str[i] = 0;
    *input->str_len = i;
    input->input_cb(input->input_str,input->str_len);
}

/* 初始化input控件解析器 */
rt_err_t dwin_input_init(void)
{
    RT_ASSERT(parse == RT_NULL);

    /* 注册控件解析器 */
    parse = dwin_parse_create(DWIN_WIDGET_TYPE_INPUT, dwin_input_event);
    dwin_parse_register(parse);

    return RT_EOK;
}

struct dwin_input *dwin_input_create(struct dwin_page *page, rt_uint16_t addr, void (*cb)(char *str,rt_uint32_t *len),rt_uint32_t *len)
{
    struct dwin_input *input = RT_NULL;

    input = (struct dwin_input *)rt_malloc(sizeof(struct dwin_input));
    if (input == RT_NULL)
    {
        DWIN_DBG("Create input failed memory is not enough.\n");
        goto failed;
    }

    dwin_obj_init(&(input->obj), addr, DWIN_OBJ_VARY_LENGHT, DWIN_WIDGET_TYPE_INPUT);
    dwin_page_add_obj(page, &(input->obj));
    input->input_cb = cb;
    input->str_len = len;
    
    return input;

failed:
    if (input != RT_NULL)
    {
        rt_free(input);
    }

    return RT_NULL;
}

rt_err_t dwin_input_delect(struct dwin_input *input)
{
    RT_ASSERT(input != RT_NULL);

    dwin_page_remove_obj(&(input->obj));
    rt_free(input);

    return RT_EOK;
}

rt_err_t dwin_input_set_callback(struct dwin_input *input, void (*cb)(char *str,rt_uint32_t *len))
{
    RT_ASSERT(input != RT_NULL);
    RT_ASSERT(cb     != RT_NULL);

    input->input_cb = cb;

    return RT_EOK;
}

rt_err_t dwin_input_set_active(struct dwin_input *input, rt_bool_t active)
{
    RT_ASSERT(input != RT_NULL);

    dwin_obj_set_active(&(input->obj), active);

    return RT_EOK;
}
