#include "dwin_init.h" 
#include "dwin_trans.h" 
#include "dwin_space.h" 

/* dwin库初始化函数 */
uint8_t dwin_init(const char *name, uint32_t baudrate)
{
    uint8_t ret = dwin_err_none;
    
    /* 初始化监听器 */
    ret = dwin_watch_init(name, baudrate);
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
    /* 初始化空间变量 */
    ret = dwin_space_init();
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
    return dwin_err_none;
}
