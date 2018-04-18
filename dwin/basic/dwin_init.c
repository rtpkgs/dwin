/*
 * @File:   dwin_init.c 
 * @Author: liu2guang 
 * @Date:   2017-12-18 14:52:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-18     liu2guang    实现dwin库初始化总入口. 
 */ 
 
#include "dwin_init.h" 
#include "dwin_trans.h" 
#include "dwin_space.h" 
#include "dwin_system.h" 

static bool dwin_is_init = false;

/* 获取初始化状态 */
bool dwin_get_init_state(void)
{
    return dwin_is_init; 
}

/* dwin库初始化函数 */
uint8_t dwin_init(const char *uart, uint32_t baudrate)
{
    uint8_t ret = dwin_err_none; 
    uint8_t major, minor, revise, data[8];
    
    /* 检测是否已经初始化 */
    if(dwin_get_init_state() == true)
    {
        dwin_println("\nwarning! dwin lib already inited!"); 
        return dwin_err_reinit; 
    }
    
    /* 初始化监听器 */
    ret = dwin_watch_init(uart, baudrate); 
    if(ret != dwin_err_none)
    {
        dwin_println("error! watch init failed %d!", ret); 
        return dwin_err_error; 
    }
    
    /* 初始化空间变量 */
    ret = dwin_space_init();
    if(ret != dwin_err_none)
    {
        dwin_println("error! watch init failed %d!", ret); 
        return dwin_err_error; 
    }
    
    dwin_system_set_page_id(0); 
    
    /* DWIN库版本 */
    dwin_system_sw_version(&major, &minor, &revise);
    dwin_println("dwin lib ver %d.%d.%d.", major, minor, revise);
    
    /* 屏幕版本信息 */
    dwin_system_hw_version(&major, &minor, &revise);
    dwin_println("Hardware ver %d.%d.%d.", major, minor, revise);
    
    /* 获取当前背光亮度 */
    dwin_system_get_backlight_lvl(data);
    dwin_println("Backlight level %d(0x%.2x).", data[0], data[0]);
    
    /* 启动数据自动上传 */
    dwin_system_set_autoupdate(1); 
    dwin_println("Config dwin autoupdate."); 

    /* l22加载数据 */
    dwin_system_set_dataload_mode(1); 
    dwin_println("Config dwin load data from l22."); 

    /* 默认使能蜂鸣器 */
    dwin_system_set_buzzer(1); 
    dwin_println("Config dwin buzzer on."); 
    
    /* 初始化完成 */
    dwin_is_init = true; 
    
    dwin_println("Welcome to the dwin library."); 
    dwin_println("You can find the latest version from https://github.com/liu2guang/dwin."); 
    
    return dwin_err_none;
}
