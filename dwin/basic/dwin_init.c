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

/* dwin库初始化状态标志位 */
static uint8_t init_flag = dwin_uninit;

/* dwin库初始化函数 */
uint8_t dwin_init(const char *name, uint32_t baudrate)
{
    uint8_t ret = dwin_err_none;
    
    /* 检测是否已经初始化 */
    if(init_flag == dwin_inited)
    {
        return dwin_err_reinit; 
    }
    
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
    
#if (PKG_DWIN_DEBUG >= 1)
    {
        uint8_t major, minor, revise, data[8];
        
        /* 版本信息 */
        dwin_system_hw_version(&major, &minor, &revise);
        dwin_println("Hardware ver %d.%d.%d", major, minor, revise);
        dwin_system_sw_version(&major, &minor, &revise);
        dwin_println("Sortware ver %d.%d.%d", major, minor, revise);
        
        dwin_system_get_backlight_lvl(data);
        dwin_println("Backlight 0x%.2x(%d)", data[0], data[0]);
        
        /* 启动数据自动上传 */
        dwin_system_set_autoupdate(1); 
        
        /* l22加载数据 */
        dwin_system_set_dataload_mode(1); 
        
        /* 默认使能蜂鸣器 */
        dwin_system_set_buzzer(1); 
    }
#endif
    
    /* 初始化完成 */
    init_flag = dwin_inited;
    
    return dwin_err_none;
}
