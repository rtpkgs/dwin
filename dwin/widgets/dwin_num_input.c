/*
 * @File:   dwin_num_input.c
 * @Author: liu2guang
 * @Date:   2018-06-13 17:36:10
 *
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-06-13     liuguang     implement the num input object.
 */

#include "dwin_num_input.h"
#include "dwin_obj.h"
#include "dwin_page.h"
#include "dwin_parse.h"

static struct dwin_parse *parse = RT_NULL; 

static void dwin_num_input_event(struct dwin_obj *obj, uint8_t *data, uint8_t len)
{
    RT_ASSERT(obj  != RT_NULL); 
    RT_ASSERT(data != RT_NULL); 
}

rt_err_t dwin_num_input_init(void)
{
    RT_ASSERT(parse == RT_NULL); 
    
    parse = dwin_parse_create(DWIN_WIDGET_TYPE_NUM_INPUT, dwin_num_input_event);
    dwin_parse_register(parse); 
    
    return RT_EOK;
}

struct dwin_num_input *dwin_num_input_create(struct dwin_page *page, rt_uint16_t addr, 
    void (*cb)(rt_uint32_t value))
{
    struct dwin_num_input *input = RT_NULL;

    input = (struct dwin_num_input *)rt_malloc(sizeof(struct dwin_num_input));
    if (input == RT_NULL)
    {
        DWIN_DBG("Create num input failed memory is not enough.\n");
        goto failed;
    }

    dwin_obj_init(&(input->obj), addr, 2, DWIN_WIDGET_TYPE_NUM_INPUT);
    dwin_page_add_obj(page, &(input->obj));
    input->input_cb = cb;
    
    return input;

failed:
    if(input != RT_NULL)
    {
        rt_free(input);
    }

    return RT_NULL;
}

rt_err_t dwin_num_input_delect(struct dwin_num_input *input)
{
    RT_ASSERT(input != RT_NULL);

    dwin_page_remove_obj(&(input->obj));
    rt_free(input);

    return RT_EOK;
}

rt_err_t dwin_num_input_set_callback(struct dwin_num_input *input, void (*cb)(rt_uint32_t value))
{
    RT_ASSERT(input != RT_NULL);
    RT_ASSERT(cb     != RT_NULL);

    input->input_cb = cb;

    return RT_EOK;
}

rt_err_t dwin_num_input_set_active(struct dwin_num_input *input, rt_bool_t active)
{
    RT_ASSERT(input != RT_NULL);

    dwin_obj_set_active(&(input->obj), active);

    return RT_EOK;
}
