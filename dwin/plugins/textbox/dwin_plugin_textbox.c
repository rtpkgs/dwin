/*
 * @File:   dwin_textbox.c 
 * @Author: liu2guang 
 * @Date:   2017-12-08 21:53:11 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-08     liu2guang    实现文本框. 
 */ 
 
#include "dwin_plugin_textbox.h" 
#include "dwin_trans.h" 

/* 将char字符串转换成short字符串 */ 
static uint16_t *char2short(uint8_t *string) 
{
    uint8_t temp; 
    uint8_t index;
    
    RT_ASSERT(string != RT_NULL); 
    
    for(index = 0; index < rt_strlen((const char *)string); index+=2)
    {
        temp = string[index]; 
        string[index] = string[index+1];
        string[index+1] = temp;
    }
    
    return (uint16_t *)string;
}

/* 创建TextBox控件 */ 
uint8_t dwin_textbox_create(const char *name, uint8_t *string, uint8_t length, uint8_t mode, uint32_t time)
{
    dwin_space_t space; 
    dwin_textbox_t handle;
    
    RT_ASSERT(name != RT_NULL); 
    RT_ASSERT(string != RT_NULL); 
    RT_ASSERT(length%2 == 0); 
    
    space = dwin_space_alloc(name, length/2, DWIN_TYPE_TEXTBOX);
    if(space == RT_NULL)
    {
        dwin_println("TextBox [%s] space alloc failed.", name);
        return dwin_err_error;
    } 
    
    handle = (dwin_textbox_t)rt_malloc(sizeof(struct dwin_textbox));
    if(handle == RT_NULL)
    {
        dwin_println("TextBox [%s] handle alloc failed.", name);
        return dwin_err_error;
    }
    
    handle->len = length;
    handle->text = (uint8_t *)rt_malloc(length);
    if(handle->text == RT_NULL)
    {
        dwin_println("TextBox [%s] text alloc failed.", name);
        return dwin_err_error;
    }
    
    if(mode == TEXTBOX_TYPE_NORMAL)
    {
        rt_strncpy((char *)(handle->text), (const char *)string, handle->len); 
        
        space->plugin = (void *)handle;
        dwin_var_write(space->addr, char2short(handle->text), handle->len/2); 
    }
    else if(mode == TEXTBOX_TYPE_STEP2STEP)
    {
        uint8_t index = 0; 
        
        space->plugin = (void *)handle;
        rt_memset((char *)(handle->text), 0, handle->len); 
        dwin_var_write(space->addr, char2short(handle->text), handle->len/2); 
        
        for(index = 2; index <= length; index+=2)
        {
            rt_memcpy((char *)(handle->text), string, index); 
            dwin_var_write(space->addr, char2short(handle->text), handle->len/2); 
            
            rt_thread_delay(time); 
        }
    }
    else
    {
        return dwin_err_para; 
    }
    
    return dwin_err_none;
}

uint8_t dwin_textbox_update(const char *name, uint8_t *string, uint8_t length)
{
    dwin_space_t space; 
    
    RT_ASSERT(name != RT_NULL); 
    RT_ASSERT(string != RT_NULL); 
    RT_ASSERT(length%2 == 0); 
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        dwin_println("No find space.");
        return dwin_err_error;
    }
    
    if(length > ((space->len)*2))
    {
        dwin_println("Text is too long.");
        return dwin_err_error;
    }
    
    dwin_var_write(space->addr, char2short(string), length/2);
    rt_strncpy((char *)(((dwin_textbox_t)(space->plugin))->text), (const char *)string, length);
    
    return dwin_err_none;
}
