#include "dwin_plugin_textbox.h" 
#include "dwin_trans.h" 

// #define DWIN_TEXTBOX_SPACE_BYTE  (0) 

/* char转short */ 
static uint16_t *char2short(uint8_t *string)
{
    uint8_t temp, index; 
    
    for(index = 0; index < rt_strlen((const char *)string); index+=2)
    {
        temp = string[index]; 
        string[index] = string[index+1];
        string[index+1] = temp;
    }
    
    return (uint16_t *)string;
}

uint8_t dwin_plugin_textbox_create(const char *name, uint8_t *text, uint8_t len)
{
    dwin_space_t space; 
    dwin_textbox_t handle;
    
    /* 判断文本长度 */
    if(len%2 == 1)
    {
        dwin_println("Text is singular");
        return dwin_err_error;
    }
    
    space = dwin_space_alloc(name, len/2, dwin_type_textbox);
    if(space == RT_NULL)
    {
        dwin_println("textbox [%s] space alloc failed");
        return dwin_err_error;
    }
    
    
    /* 分配句柄 */
    handle = (dwin_textbox_t)rt_malloc(sizeof(struct dwin_textbox));
    if(handle == RT_NULL)
    {
        dwin_println("textbox [%s] handle alloc failed");
        return dwin_err_error;
    }
    
    handle->len = len;
    
    /* 分配text缓存内存 */
    handle->text = (uint8_t *)rt_malloc(len);
    if(handle->text == RT_NULL)
    {
        dwin_println("textbox [%s] text alloc failed");
        return dwin_err_error;
    }
    
    rt_strncpy((char *)handle->text, (const char *)text, len);
    space->plugin = (void *)handle;
    
    /* 写内容 */
    dwin_var_write(space->addr, char2short(text), len/2);
    
    return dwin_err_none;
}

uint8_t dwin_plugin_textbox_update(const char *name, uint8_t *text, uint8_t len)
{
    dwin_space_t space; 
    
    space = dwin_space_find(name);
    if(space == RT_NULL)
    {
        dwin_println("No find space");
        return dwin_err_error;
    }
    
    /* 检测文字长度是否超出申请的范围 */
    if(len > ((space->len)*2))
    {
        dwin_println("Text is too long");
        return dwin_err_error;
    }
    
    /* 检测文字长度是否为单数 */
    if(len%2 == 1)
    {
        dwin_println("Text is singular");
        return dwin_err_error;
    }
    
    /* 写内容 */
    dwin_var_write(space->addr, char2short(text), len/2);
    rt_strncpy((char *)(((dwin_textbox_t)(space->plugin))->text), (const char *)text, len);
    
    return dwin_err_none;
}
