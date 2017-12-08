#include "dwin_plugin_inputbox.h" 

/* 内部使用宏 */
#define DWIN_INPUTBOX_SPACE_BYTE (2)    /* 输入框变量空间大小 */

/* 创建按键 */
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
