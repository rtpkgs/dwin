/*
 * @File:   dwin_plugin_button.c 
 * @Author: liu2guang 
 * @Date:   2017-12-08 21:53:11 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-08     liu2guang    实现按键插件. 
 */ 
#include "dwin_plugin_button.h" 

/* 内部使用宏 */
#define DWIN_BUTTON_SPACE_BYTE  (1)         /* 按键变量空间大小 */

/* 创建按键 */
uint8_t dwin_plugin_button_create(const char *name, press_cb cb, void *args)
{
    dwin_space_t  button_space;
    dwin_button_t button_handle;
    
    RT_ASSERT(name != RT_NULL);
    
    /* 分配按键空间 */
    button_space = dwin_space_alloc(name, DWIN_BUTTON_SPACE_BYTE, dwin_type_button);
    if(button_space == RT_NULL)
    {
        dwin_println("button [%s] space alloc failed");
        return dwin_err_error;
    }
    
    /* 分配按键句柄 */
    button_handle = (dwin_button_t)rt_malloc(sizeof(struct dwin_button));
    if(button_handle == RT_NULL)
    {
        dwin_println("button [%s] handle alloc failed");
        return dwin_err_error;
    }
    
    /* 填充结构体 */
    button_handle->args        = args;
    button_handle->press_cb    = cb;
    button_handle->state       = button_press;
    button_handle->match_value = DWIN_BUTTON_PRESS_VALUE;
    
    button_space ->plugin = (void *)button_handle;
    
    return dwin_err_none;
}

/* 修改按键模式 */
uint8_t dwin_plugin_button_mode(const char *name, uint8_t mode)
{
    dwin_space_t space;
    dwin_button_t button;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    button = (dwin_button_t)(space->plugin);
    button->state = mode;
    
    return dwin_err_none;
}

/* 更新按键回调 */
uint8_t dwin_plugin_button_update(const char*name, press_cb cb, void *args)
{
    dwin_space_t space;
    dwin_button_t button;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    button = (dwin_button_t)(space->plugin);
    button->args = args;
    button->press_cb = cb;
    
    return dwin_err_none;
}
