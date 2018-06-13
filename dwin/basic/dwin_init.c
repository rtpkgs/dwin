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
#include "dwin.h"

struct dwin dwin = {0}; 

static void dwin_data_init(struct dwin *dwin)
{
    RT_ASSERT(dwin != RT_NULL); 
    
    rt_memset(dwin, 0x00, sizeof(struct dwin)); 
    rt_list_init(&(dwin->pages)); 
    dwin->page_num = 0; 
    dwin->page_cur = RT_NULL; 
    rt_list_init(&(dwin->parses));
    dwin->parse_num = 0; 
    dwin->parse_mq = rt_mq_create("dwin_parse", sizeof(struct dwin_data_frame), DWIN_USING_DATA_FRAME_DEPTH, RT_IPC_FLAG_FIFO); 
}

static void dwin_verion_output(void)
{
    DWIN_INFO("Welcome to the dwin \033[32mv%d.%d.%d\033[0m lib.\n", DWIN_VERSION_M, DWIN_VERSION_S, DWIN_VERSION_R); 
    DWIN_INFO("You can find the laster from <\033[31mhttps://github.com/liu2guang/dwin\033[0m>.\n"); 
}

static rt_err_t dwin_self_check(void)
{
    rt_err_t ret = RT_EOK; 
    rt_bool_t enable = RT_FALSE; 
    
    dwin_system_touch(RT_TRUE); 
    
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
    
    return RT_EOK; 
}

static rt_err_t dwin_widgets_init(void)
{
    dwin_button_init(); 
    dwin_scale_init(); 
    dwin_gbk_input_init();
    dwin_num_init(); 
    dwin_num_input_init(); 
    
    return RT_EOK; 
}

/* 初始化dwin库 */ 
int dwin_init(void)
{
    if(dwin.init == RT_TRUE)
    {
        DWIN_INFO("The dwin lib initialized.\n"); 
        return RT_EBUSY;
    }
    
    /* 初始化dwin结构体 */ 
    dwin_data_init(&dwin); 
    
    /* 打印信息 */ 
    dwin_verion_output(); 
    
    /* 启动自动上传数据监听器 */ 
    extern rt_err_t dwin_watch_init(dwin_t dwin, const char *name, rt_uint32_t baudrate); 
    rt_err_t ret = dwin_watch_init(&dwin, DWIN_USING_UART, DWIN_USING_BAUDRATE); 
    if(ret != RT_EOK)
    {
        DWIN_INFO("The dwin watch start failed!\n"); 
        return ret; 
    } 
    
    extern rt_err_t dwin_parse_init(void); 
    dwin_parse_init(); 
    
    /* 开机自检配置 */ 
    dwin_self_check(); 
    
    /* 初始化控件 */ 
    dwin_widgets_init(); 
    
    dwin.init = RT_TRUE; 
    
    return RT_EOK; 
}
INIT_APP_EXPORT(dwin_init); 
