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

static struct dwin dwin; 

int dwin_init(void)
{
    rt_err_t ret = RT_EOK; 
    
    if(dwin.init == RT_TRUE)
    {
        DWIN_DBG("The dwin initialized.\n"); 
        return RT_EBUSY;
    }
    
    rt_memset(&dwin, 0x00, sizeof(struct dwin)); 
    
    extern rt_err_t dwin_watch_init(dwin_t dwin, const char *name, rt_uint32_t baudrate); 
    ret = dwin_watch_init(&dwin, DWIN_USING_UART, DWIN_USING_BAUDRATE); 
    if(ret != RT_EOK)
    {
        DWIN_DBG("Watch init failed error code: %d.\n", ret); 
        return ret; 
    } 
    else
    {
        DWIN_DBG("Watch init succeed.\n"); 
    }
    
    dwin.init = RT_TRUE; 
    
    DWIN_INFO("Welcome to the dwin library.\n"); 
    DWIN_INFO("You can find the v2.0.0 from https://github.com/liu2guang/dwin.\n"); 
    
    rt_uint16_t data[20] = {0}; 
    dwin_system_get_cpuid(data); 
    
    dwin_system_reset(); 
    dwin_system_get_version(data); 
    dwin_system_get_rtc(data); 
    dwin_system_get_page(data); 
    
    return RT_EOK; 
}
INIT_APP_EXPORT(dwin_init); 
