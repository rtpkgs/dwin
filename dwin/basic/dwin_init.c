/*
 * @File:   dwin_init.c 
 * @Author: liu2guang 
 * @Date:   2018-04-22 14:52:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-04-22     liu2guang    update v2 framework. 
 */ 

#include "dwin_init.h" 

static struct dwin dwin = {0}; 

int dwin_init(void)
{
    rt_err_t ret = RT_EOK; 
    
    if(dwin.init == RT_TRUE)
    {
        DWIN_DBG("The dwin lib initialized.\n"); 
        return RT_EBUSY;
    }
    
    rt_memset(&dwin, 0x00, sizeof(struct dwin)); 
    
    DWIN_PRINT("\n");
    DWIN_INFO("Welcome to the dwin \033[32mv2.0.0\033[0m lib.\n"); 
    DWIN_INFO("You can find the laster from <\033[31mhttps://github.com/liu2guang/dwin\033[0m>.\n\n"); 
    
    extern rt_err_t dwin_watch_init(dwin_t dwin, const char *name, rt_uint32_t baudrate); 
    ret = dwin_watch_init(&dwin, DWIN_USING_UART, DWIN_USING_BAUDRATE); 
    if(ret != RT_EOK)
    {
        return ret; 
    } 
    
    dwin.init = RT_TRUE; 

    return RT_EOK; 
}
INIT_APP_EXPORT(dwin_init); 
