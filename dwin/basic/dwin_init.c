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
#include "dwin_system.h"

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
    
    dwin_system_touch(RT_TRUE); 
    
    /* 检测屏幕运行配置 */ 
    rt_bool_t enable = RT_FALSE; 
    ret = dwin_system_var_from_l22_upload(&enable); 
    if(ret != RT_EOK || enable != RT_TRUE)
    {
        DWIN_INFO("The dwin lib need var L22 upload mode. dwin kill rtt system!\n"); 
        while(1); 
    }
    
    ret = dwin_system_backlight_by_touch_ctr(&enable); 
    if(ret != RT_EOK || enable != RT_TRUE)
    {
        DWIN_INFO("The dwin lib need enable backlight by touch. dwin kill rtt system!\n"); 
        while(1); 
    }
    
    ret = dwin_system_crc(&enable); 
    if(ret != RT_EOK || enable != RT_TRUE)
    {
        DWIN_INFO("The dwin lib need disable crc. dwin kill rtt system!\n"); 
        while(1); 
    }
    
    dwin.init = RT_TRUE; 
    
    /* 调试自动打印信息 */ 
#if defined(DWIN_USING_DEBUG)
    rt_uint32_t data = 0; 
    
    DWIN_PRINT("\n"); 
    dwin_system_version(&data); 
#endif 
    
    return RT_EOK; 
}
INIT_APP_EXPORT(dwin_init); 
