#include "dwin_plugin_button.h" 

/* static macro definition */
#define DWIN_BUTTON_SPACE_BYTE (1)

/* extern api */
/* 按键插件初始化 */
/* create button */
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
    button_handle->state       = button_state_startup;
    button_handle->match_value = 0x0D0D;
    
    button_space ->plugin = (void *)button_handle;
    
    return dwin_err_none;
}
