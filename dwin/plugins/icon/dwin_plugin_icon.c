/*
 * @File:   dwin_plugin_icon.c 
 * @Author: liu2guang 
 * @Date:   2017-12-08 21:53:11 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-08     liu2guang    实现图标插件. 
 */ 
#include "dwin_plugin_icon.h" 
#include "dwin_trans.h" 

/* 内部使用宏 */
#define DWIN_ICON_SPACE_BYTE  (1)         /* 按键变量空间大小 */

/* 创建icon组 */
uint8_t dwin_plugin_icon_create(const char *name, uint16_t min, uint16_t max, uint16_t current)
{
    dwin_space_t icon_space;
    dwin_icon_t  icon_handle;
    
    RT_ASSERT(name != RT_NULL);
    
    /* 分配按键空间 */
    icon_space = dwin_space_alloc(name, DWIN_ICON_SPACE_BYTE, dwin_type_icon);
    if(icon_space == RT_NULL)
    {
        dwin_println("Icon [%s] space alloc failed");
        return dwin_err_error;
    }
    
    /* 分配按键句柄 */
    icon_handle = (dwin_icon_t)rt_malloc(sizeof(struct dwin_icon));
    if(icon_handle == RT_NULL)
    {
        dwin_println("Intput [%s] handle alloc failed");
        return dwin_err_error;
    }
    
    /* 填充结构体 */
    icon_handle->state = icon_start;
    icon_handle->cur_index = current;
    icon_handle->min_index = min;
    icon_handle->max_index = max;
    icon_space ->plugin = (void *)icon_handle;
    
    /* 改变icon序号 */
    dwin_plugin_icon_update(name, current);
    
    return dwin_err_none;
}

/* 改变icon组显示状态 */
uint8_t dwin_plugin_icon_mode(const char *name, uint8_t mode)
{
    dwin_space_t space;
    dwin_icon_t icon;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    icon = (dwin_icon_t)(space->plugin);
    icon->state = mode;
    
    return dwin_err_none;
}

/* 更新icon组显示序号 */
uint8_t dwin_plugin_icon_update(const char *name, uint16_t current)
{
    uint8_t ret = dwin_err_none;
    dwin_space_t space;
    dwin_icon_t icon;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    icon = (dwin_icon_t)(space->plugin);
    icon->cur_index = current;
    
    if((icon->cur_index < icon->min_index) || (icon->cur_index > icon->max_index))
    {
        return dwin_err_error;
    }
    
    ret = dwin_var_write(space->addr, &(icon->cur_index), 1);
    if(ret != dwin_err_none)
    {
        return dwin_err_error;
    }
        
    return dwin_err_none;
}

/* 读取当前显示的icon组的序号 */
uint16_t dwin_plugin_icon_read(const char *name)
{
    dwin_space_t space;
    dwin_icon_t icon;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    icon = (dwin_icon_t)(space->plugin);
    
    return (icon->cur_index);
}
