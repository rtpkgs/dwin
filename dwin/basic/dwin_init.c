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

/* 控件头文件 */ 
#include "dwin_button.h" 
#include "dwin_scale.h" 

struct dwin dwin = {0}; 

/* 初始化dwin库 */ 
int dwin_init(void)
{
    rt_err_t ret = RT_EOK; 
    
    /* 检测是否重复初始化 */ 
    if(dwin.init == RT_TRUE)
    {
        DWIN_DBG("The dwin lib initialized.\n"); 
        return RT_EBUSY;
    }
    
    /* 初始化dwin对象 */ 
    rt_memset(&dwin, 0x00, sizeof(struct dwin)); 
    rt_list_init(&(dwin.pages)); 
    dwin.page_num = 0; 
    dwin.page_cur = RT_NULL; 
    rt_list_init(&(dwin.parses));
    dwin.parse_num = 0; 
    
    /* 打印信息 */ 
#if defined(DWIN_USING_DEBUG)
    DWIN_PRINT("\n");
    DWIN_INFO("Welcome to the dwin \033[32mv%d.%d.%d\033[0m lib.\n", DWIN_VERSION_M, DWIN_VERSION_S, DWIN_VERSION_R); 
    DWIN_INFO("You can find the laster from <\033[31mhttps://github.com/liu2guang/dwin\033[0m>.\n\n"); 
#else
    DWIN_INFO("Welcome to the dwin \033[32mv%d.%d.%d\033[0m lib.\n", DWIN_VERSION_M, DWIN_VERSION_S, DWIN_VERSION_R); 
    DWIN_INFO("You can find the laster from <\033[31mhttps://github.com/liu2guang/dwin\033[0m>.\n"); 
#endif 
    
    /* 启动自动上传数据监听器 */ 
    extern rt_err_t dwin_watch_init(dwin_t dwin, const char *name, rt_uint32_t baudrate); 
    ret = dwin_watch_init(&dwin, DWIN_USING_UART, DWIN_USING_BAUDRATE); 
    if(ret != RT_EOK)
    {
        DWIN_INFO("The dwin watch start failed!\n"); 
        return ret; 
    } 
    
#if defined(DWIN_USING_DEBUG)
    DWIN_PRINT("\n"); 
#endif 
    dwin_system_touch(RT_TRUE); 
    
    /* 检测屏幕运行配置, 当非配置为以下功能时, dwin库无法正常工作:
     * 1. 检测屏幕上电后VAR变量是否是由L22字库文件进行初始化. 
     * 2. 检测触摸背光功能是否开启. 
     * 3. 检测数据帧CRC是否关闭, 目前dwin库不支持CRC校检功能, 以后也不会支持, 不想做. 
     */ 
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
    dwin_system_version(&data); 
    DWIN_PRINT("\n");
#endif 
    
    /* 初始化控件(注册控件解析器) */ 
    dwin_button_init(); 
    dwin_scale_init(); 
    
    return RT_EOK; 
}
INIT_APP_EXPORT(dwin_init); 
