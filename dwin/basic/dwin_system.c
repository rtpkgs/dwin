/*
 * @File:   dwin_system.c 
 * @Author: liu2guang 
 * @Date:   2018-04-26 00:12:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-04-26     liu2guang    update v2 framework. 
 */ 
 
#include "dwin_system.h" 
#include "dwin_trans.h" 
#include "dwin_utils.h" 

/* system addr(var) */ 
#if (DWIN_USING_MODEL == 2) 
    
#define DWIN_SYSTEM_DEVICE_ID         (0x0000)
#define DWIN_SYSTEM_SYS_RESET         (0x0004)
#define DWIN_SYSTEM_OS_UPDATE_CMD     (0x0006)
#define DWIN_SYSTEM_NOR_FLASH_RW_CMD  (0x0008)
#define DWIN_SYSTEM_VER               (0x000F)
#define DWIN_SYSTEM_RTC               (0x0010)
#define DWIN_SYSTEM_PIC_NOW           (0x0014)
#define DWIN_SYSTEM_GUI_STATUS        (0x0015)
#define DWIN_SYSTEM_TP_STATUS         (0x0016)
#define DWIN_SYSTEM_VCC_NOW           (0x0030)
#define DWIN_SYSTEM_LED_NOW           (0x0031)
#define DWIN_SYSTEM_SYS_CFG           (0x0080)
#define DWIN_SYSTEM_LED_CFG           (0x0082)
#define DWIN_SYSTEM_PIC_SET           (0x0084)
#define DWIN_SYSTEM_RTC_SET           (0x009C)
#define DWIN_SYSTEM_MUSIC_PLAY_SET    (0x00A0)
#define DWIN_SYSTEM_BMP_DOWNLOAD      (0x00A2)
#define DWIN_SYSTEM_JPEG_DOWNLOAD     (0x00A6)
#define DWIN_SYSTEM_NAND_FLASH_RW_CMD (0x00AA)
#define DWIN_SYSTEM_DCS_BUS_DATA      (0x0100)
#define DWIN_SYSTEM_DYNAMIC_CURVE     (0x0300)
    
#endif

/* 注意数据的大小端 */ 
rt_err_t dwin_system_get_cpuid(rt_uint16_t *cpuid)
{
    rt_err_t ret = RT_EOK; 
    
    RT_ASSERT(cpuid != RT_NULL); 
    
    ret = dwin_var_read(DWIN_SYSTEM_DEVICE_ID, cpuid, 4); 
    if(ret != RT_EOK)
    {
        DWIN_DBG("Get dwin serial screen cpu id failed error code: \033[31m%d\033[0m].\n", ret); 
        return ret; 
    }
    DWIN_DBG("Get dwin serial screen cpu id: \033[32m0x%.4x%.4x%.4x%.4x\033[0m.\n", cpuid[0], cpuid[1], cpuid[2], cpuid[3]); 
    
    return RT_EOK; 
}

rt_err_t dwin_system_reset(void)
{
    rt_err_t ret = RT_EOK; 
    rt_uint16_t reset_cmd[2] = {0x55AA, 0x5AA5}; 
    
    ret = dwin_var_write(DWIN_SYSTEM_SYS_RESET, reset_cmd, 2); 
    
    /* 复位到可以接受指令需要有延时, 但是这个时间没有大量测试无法保证1s一定是在任何条件下可行的 */ 
    rt_thread_delay(RT_TICK_PER_SECOND); 
    
    if(ret != RT_EOK)
    {
        DWIN_DBG("Reset dwin failed error code: \033[31m%d\033[0m].\n", ret); 
        return ret;
    }
    DWIN_DBG("Reset dwin succeed.\n");
    
    return RT_EOK; 
}

rt_err_t dwin_system_get_version(rt_uint16_t *ver)
{
    rt_err_t ret = RT_EOK; 
    
    RT_ASSERT(ver != RT_NULL); 
    
    ret = dwin_var_read(DWIN_SYSTEM_VER, ver, 1); 
    if(ret != RT_EOK)
    {
        DWIN_DBG("Get dwin version failed error code: \033[31m%d\033[0m].\n", ret); 
        return ret; 
    }
    
    rt_uint8_t gui_ver = dwin_utils_bcd2dec(DWIN_GET_BYTEH(*ver)); 
    rt_uint8_t dwinos_ver = dwin_utils_bcd2dec(DWIN_GET_BYTEL(*ver)); 
    
    DWIN_DBG("Get dwin version: \033[32m GUI:v%d.%d, DWIN-OS: v%d.%d(0x%.4x)\033[0m.\n", 
        gui_ver/10, gui_ver%10, dwinos_ver/10, dwinos_ver%10, *ver); 
    
    return RT_EOK; 
}

rt_err_t dwin_system_get_rtc(rt_uint16_t *rtc)
{
    rt_err_t ret = RT_EOK; 
    
    RT_ASSERT(rtc != RT_NULL); 
    
    ret = dwin_var_read(DWIN_SYSTEM_RTC, rtc, 4); 
    if(ret != RT_EOK)
    {
        DWIN_DBG("Get dwin rtc failed error code: \033[31m%d\033[0m].\n", ret); 
        return ret; 
    }
    DWIN_DBG("Get dwin rtc: \033[32m20%.2d-%.2d-%.2d %.2d:%.2d:%.2d week %d\033[0m.\n", 
        DWIN_GET_BYTEH(rtc[0]), DWIN_GET_BYTEL(rtc[0]), DWIN_GET_BYTEH(rtc[1]), 
        DWIN_GET_BYTEH(rtc[2]), DWIN_GET_BYTEL(rtc[2]), DWIN_GET_BYTEH(rtc[3]),
        DWIN_GET_BYTEL(rtc[1])); 
    
    return RT_EOK; 
}

rt_err_t dwin_system_get_page(rt_uint16_t *page)
{
    rt_err_t ret = RT_EOK; 
    
    RT_ASSERT(page != RT_NULL); 
    
    ret = dwin_var_read(DWIN_SYSTEM_PIC_NOW, page, 1); 
    if(ret != RT_EOK)
    {
        DWIN_DBG("Get dwin page failed error code: \033[31m%d\033[0m].\n", ret); 
        return ret; 
    }
    DWIN_DBG("Get dwin page: \033[32m%.4d\033[0m.\n", *page); 
    
    return RT_EOK; 
}
