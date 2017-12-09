#include "dwin_plugin_inputbox.h" 

/* 内部使用宏 */
#define DWIN_INPUTBOX_SPACE_BYTE (2)    /* 输入框变量空间大小 */

/* 创建inputbox */
uint8_t dwin_plugin_inputbox_create(const char *name, inputbox_cb cb, void *args)
{
    dwin_space_t input_space;
    dwin_inputbox_t input_handle;
    
    RT_ASSERT(name != RT_NULL);
    
    /* 分配按键空间 */
    input_space = dwin_space_alloc(name, DWIN_INPUTBOX_SPACE_BYTE, dwin_type_inputbox);
    if(input_space == RT_NULL)
    {
        dwin_println("intput [%s] space alloc failed");
        return dwin_err_error;
    }
    
    /* 分配按键句柄 */
    input_handle = (dwin_inputbox_t)rt_malloc(sizeof(struct dwin_inputbox));
    if(input_handle == RT_NULL)
    {
        dwin_println("intput [%s] handle alloc failed");
        return dwin_err_error;
    }
    
    /* 填充结构体 */
    input_handle->state = inputbox_start;
    input_handle->input_value = 0;
    input_handle->inputbox_cb = cb;
    input_handle->args = args;
    input_space ->plugin = (void *)input_handle;
    
    return dwin_err_none;
}

/* 修改inputbox模式 */
uint8_t dwin_plugin_inputbox_mode(const char *name, uint8_t mode)
{
    dwin_space_t space;
    dwin_inputbox_t inputbox;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    inputbox = (dwin_inputbox_t)(space->plugin);
    inputbox->state = mode;
    
    return dwin_err_none;
}

/* 更新inputbox信息 */
uint8_t dwin_plugin_inputbox_update(const char *name, inputbox_cb cb, void *args)
{
    dwin_space_t space;
    dwin_inputbox_t inputbox;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    inputbox = (dwin_inputbox_t)(space->plugin);
    inputbox->args = args;
    inputbox->inputbox_cb = cb;
    
    return dwin_err_none;
}

/* 读inputbox值 */
int32_t dwin_plugin_inputbox_read(const char *name)
{
    dwin_space_t space;
    dwin_inputbox_t inputbox;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return -dwin_err_error;
    }
    
    inputbox = (dwin_inputbox_t)(space->plugin);
    
    return (int32_t)(inputbox->input_value);
}

/* 清除inputbox值 */
uint8_t dwin_plugin_inputbox_clear(const char *name)
{
    dwin_space_t space;
    dwin_inputbox_t inputbox;
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        return dwin_err_error;
    }
    
    inputbox = (dwin_inputbox_t)(space->plugin);
    inputbox->input_value = (-1);
    
    return dwin_err_none;
}
