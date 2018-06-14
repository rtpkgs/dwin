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
#include "dwin_trans.h" 

static struct dwin_parse *parse = RT_NULL; 

static void dwin_num_input_event(struct dwin_obj *obj, rt_uint8_t *data, rt_uint8_t len)
{
    rt_uint32_t value = 0; 
    
    RT_ASSERT(obj  != RT_NULL); 
    RT_ASSERT(data != RT_NULL); 
    
    struct dwin_num_input *input = (struct dwin_num_input *)obj; 
    
    value = (data[7]<<24)+(data[8]<<16)+(data[9]<<8)+data[10]; 
    
    /* Ö´ÐÐ»Øµ÷ */ 
    input->input_cb(value);
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
    
    dwin_num_input_write(input, 0); 
    
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

rt_err_t dwin_num_input_read(struct dwin_num_input *input, rt_uint32_t *value)
{
    rt_uint16_t data[2] = {0}; 
    
    RT_ASSERT(input != RT_NULL); 
    
    if(dwin_var_read(input->obj.value_addr, data, 2)!= RT_EOK)
    {
        return RT_ERROR; 
    }
    *value = DWIN_SET_INT(data[0], data[1]); 
    
    return RT_EOK; 
}

rt_err_t dwin_num_input_write(struct dwin_num_input *input, rt_uint32_t value)
{
    rt_uint16_t data[2] = {0}; 
    
    RT_ASSERT(input != RT_NULL); 
    
    data[0] = DWIN_GET_SHORTH(value); 
    data[1] = DWIN_GET_SHORTL(value);
    if(dwin_var_write(input->obj.value_addr, data, 2)!= RT_EOK)
    {
        return RT_ERROR; 
    }
    
    return RT_EOK; 
}
