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

static rt_uint8_t bcd2dec(rt_uint8_t bcd)
{
    return bcd-(bcd>>4)*6;
}

static rt_uint8_t dec2bcd(rt_uint8_t dec)
{
    return dec+(dec/10)*6;
}

/* 系统功能地址 ***************************************************************/ 
#if (DWIN_USING_TYPE == 0) 
#define DWIN_SYSTEM_REG_ADDR_VERSION    0x0000
#define DWIN_SYSTEM_REG_ADDR_BACKLIGHT  0x0001
#define DWIN_SYSTEM_REG_ADDR_BUZZ       0x0002
#define DWIN_SYSTEM_REG_ADDR_PAGE       0x0003
#define DWIN_SYSTEM_REG_ADDR_TOUCH_EN   0x000B
#define DWIN_SYSTEM_REG_ADDR_RTC_WFLAG  0x001F
#define DWIN_SYSTEM_REG_ADDR_RTC        0x0020
#define DWIN_SYSTEM_REG_ADDR_KEY        0x004F
#endif 

#if (DWIN_USING_TYPE == 1) 
#endif 

#if (DWIN_USING_TYPE == 2) 
#endif 

/* 通用系统功能API ************************************************************/ 

/* 获取版本 */ 
rt_err_t dwin_system_version(rt_uint32_t *ver_sn)
{
#if (DWIN_USING_TYPE == 0) 
    rt_uint8_t data = 0; 
    
    RT_ASSERT(ver_sn != RT_NULL); 
    
    if(dwin_reg_read(DWIN_SYSTEM_REG_ADDR_VERSION, &data, 1) != RT_EOK)
    {
        goto failed; 
    } 
    data = bcd2dec(data); 
    
    *ver_sn = (DWIN_USING_TYPE*10000)+(data)*100+0; 
    DWIN_DBG("The dwin type is DGUSMINI, DGUSVer is %d.%d, OSVer None, VER_SN is %d.\n", data/10, data%10, *ver_sn);
    
    return RT_EOK; 
#endif 
    
#if (DWIN_USING_TYPE == 1) 
    
#endif 
    
#if (DWIN_USING_TYPE == 2) 
    
#endif 
    
failed:
    DWIN_DBG("The dwin version info read \033[32mfailed\033[0m.\n");
    return RT_ERROR; 
}

/* 获取背光亮度 */ 
rt_err_t dwin_system_get_backlight(rt_uint8_t *level)
{
#if (DWIN_USING_TYPE == 0) 
    RT_ASSERT(level != RT_NULL); 
    
    if(dwin_reg_read(DWIN_SYSTEM_REG_ADDR_BACKLIGHT, level, 1) != RT_EOK)
    {
        goto failed; 
    } 
    
    DWIN_DBG("The dwin backlight level is %d.\n", *level);
    return RT_EOK; 
#endif 
    
#if (DWIN_USING_TYPE == 1) 
    
#endif 
    
#if (DWIN_USING_TYPE == 2) 
    
#endif 
    
failed:
    DWIN_DBG("The dwin backlight level read \033[32mfailed\033[0m.\n");
    return RT_ERROR; 
}

/* 设置背光亮度 */ 
rt_err_t dwin_system_set_backlight(rt_uint8_t level)
{
#if (DWIN_USING_TYPE == 0) 
    if(dwin_reg_write(DWIN_SYSTEM_REG_ADDR_BACKLIGHT, &level, 1) != RT_EOK)
    {
        goto failed; 
    }
    
    DWIN_DBG("The dwin backlight level set %d.\n", level);
    return RT_EOK; 
#endif 
    
#if (DWIN_USING_TYPE == 1) 
    
#endif 
    
#if (DWIN_USING_TYPE == 2) 
    
#endif 
    
failed:
    DWIN_DBG("The dwin backlight set \033[32mfailed\033[0m.\n");
    return RT_ERROR; 
}

/* 蜂鸣tick*10ms */ 
rt_err_t dwin_system_buzz(rt_uint8_t tick)
{
#if (DWIN_USING_TYPE == 0) 
    if(dwin_reg_write(DWIN_SYSTEM_REG_ADDR_BUZZ, &tick, 1) != RT_EOK)
    {
        goto failed; 
    }
    
    DWIN_DBG("The dwin buzz %d ms.\n", tick*10); 
    return RT_EOK;
#endif 
    
#if (DWIN_USING_TYPE == 1) 
    
#endif 
    
#if (DWIN_USING_TYPE == 2) 
    
#endif 
    
failed:
    DWIN_DBG("The dwin buzz \033[32mfailed\033[0m.\n"); 
    return RT_ERROR; 
}

/* 跳转指定ID界面 */ 
rt_err_t dwin_system_jump(rt_uint16_t page)
{
#if (DWIN_USING_TYPE == 0) 
    rt_uint8_t data[2] = {0}; 
    
    data[0] = (page >> 8);
    data[1] = (page << 8) >> 8;
    
    if(dwin_reg_write(DWIN_SYSTEM_REG_ADDR_PAGE, data, 2) != RT_EOK)
    {
        goto failed; 
    }
    
    DWIN_DBG("The dwin jump %d page.\n", page);
    return RT_EOK;
#endif 
    
#if (DWIN_USING_TYPE == 1) 
    
#endif 
    
#if (DWIN_USING_TYPE == 2) 
    
#endif 
    
failed:
    DWIN_DBG("The dwin jump %d page \033[32mfailed\033[0m.\n", page);
    return RT_ERROR; 
}

/* 获取当前页面ID */ 
rt_err_t dwin_system_page(rt_uint16_t *page)
{
#if (DWIN_USING_TYPE == 0) 
    rt_uint8_t data[2] = {0}; 
    
    RT_ASSERT(page != RT_NULL); 
    
    if(dwin_reg_read(DWIN_SYSTEM_REG_ADDR_PAGE, data, 2) != RT_EOK)
    {
        goto failed; 
    }
    
    *page = (rt_uint16_t)((data[0] << 8) + data[1]);
    DWIN_DBG("The dwin current page is %d.\n", *page);
    return RT_EOK;
#endif 
    
#if (DWIN_USING_TYPE == 1) 
    
#endif 
    
#if (DWIN_USING_TYPE == 2) 
    
#endif 
    
failed:
    DWIN_DBG("The dwin read page \033[32mfailed\033[0m.\n");
    return RT_ERROR; 
}

/* 启动或者关闭触摸功能 */ 
rt_err_t dwin_system_touch(rt_bool_t enable)
{
#if (DWIN_USING_TYPE == 0) 
    rt_uint8_t data = enable; 
    
    if(dwin_reg_write(DWIN_SYSTEM_REG_ADDR_TOUCH_EN, &data, 1) != RT_EOK)
    {
        goto failed; 
    }
    
    if(enable == RT_TRUE)
    {
        DWIN_DBG("The dwin touch func enable.\n");
    }
    else
    {
        DWIN_DBG("The dwin touch func disable.\n");
    }
    return RT_EOK;
#endif 
    
#if (DWIN_USING_TYPE == 1) 
    
#endif 
    
#if (DWIN_USING_TYPE == 2) 
    
#endif 
    
failed: 
    if(enable == RT_TRUE)
    {
        DWIN_DBG("The dwin touch enable \033[32mfailed\033[0m.\n");
    }
    else
    {
        DWIN_DBG("The dwin touch disable \033[32mfailed\033[0m.\n");
    }
    
    return RT_ERROR; 
}

/* 获取RTC时间 */ 
rt_err_t dwin_system_get_rtc(struct dwin_rtc *rtc)
{
#if (DWIN_USING_TYPE == 0) 
    rt_uint8_t data[7] = {0}; 
    
    RT_ASSERT(rtc != RT_NULL); 
    
    if(dwin_reg_read(DWIN_SYSTEM_REG_ADDR_RTC, data, 7) != RT_EOK)
    {
        goto failed; 
    } 
    
    rtc->year   = bcd2dec(data[0]) + 2000;
    rtc->month  = bcd2dec(data[1]);
    rtc->day    = bcd2dec(data[2]);
    rtc->hour   = bcd2dec(data[4]);
    rtc->minute = bcd2dec(data[5]);
    rtc->second = bcd2dec(data[6]);
    rtc->week   = bcd2dec(data[3]);
    
    DWIN_DBG("The dwin rtc is %.4d-%.2d-%.2d %.2d:%.2d:%.2d week:%d.\n", 
        rtc->year, rtc->month, rtc->day, rtc->hour, rtc->minute, rtc->second, 
        rtc->week);
    return RT_EOK; 
#endif 
    
#if (DWIN_USING_TYPE == 1) 
    
#endif 
    
#if (DWIN_USING_TYPE == 2) 
    
#endif 
    
failed:
    DWIN_DBG("The dwin rtc info read \033[32mfailed\033[0m.\n");
    return RT_ERROR; 
}

rt_err_t dwin_system_set_rtc(struct dwin_rtc rtc)
{
#if (DWIN_USING_TYPE == 0) 
    rt_uint8_t data[8] = {0}; 
    
    /* 检测RTC结构体是否合法 */ 
    if( (rtc.month == 0 || rtc.month > 12) || 
        (rtc.day   == 0 || rtc.day   > 31) ||
        (rtc.hour   > 24)                  ||
        (rtc.minute > 59)                  ||
        (rtc.second > 59))
    {
        DWIN_DBG("The dwin rtc set \033[32mfailed\033[0m, Invalid argument: %.4d-%.2d-%.2d %.2d:%.2d:%.2d.\n", 
            rtc.year, rtc.month, rtc.day, rtc.hour, rtc.minute, rtc.second);
        return RT_ERROR; 
    }
    
    
    /* 真正的RTC地址为0x20, 但是修改了RTC需要在0x1F处
       写0x5a表明用户修改了RTC数据, 迪文屏幕在修改系
       RTC后会自动清零 */
    data[0] = 0x5a;
    data[1] = dec2bcd(rtc.year-2000);
    data[2] = dec2bcd(rtc.month);
    data[3] = dec2bcd(rtc.day);
    data[4] = 0x00;                /* 周几, 可以随便设定 */
    data[5] = dec2bcd(rtc.hour);
    data[6] = dec2bcd(rtc.minute);
    data[7] = dec2bcd(rtc.second);
    
    if(dwin_reg_write(DWIN_SYSTEM_REG_ADDR_RTC_WFLAG, data, 8) != RT_EOK)
    {
        goto failed; 
    } 
    
    DWIN_DBG("The dwin rtc set %.4d-%.2d-%.2d %.2d:%.2d:%.2d.\n", 
        rtc.year, rtc.month, rtc.day, rtc.hour, rtc.minute, rtc.second);
    return RT_EOK; 
#endif 
    
#if (DWIN_USING_TYPE == 1) 
    
#endif 
    
#if (DWIN_USING_TYPE == 2) 
    
#endif 
    
failed:
    DWIN_DBG("The dwin rtc set \033[32mfailed\033[0m.\n");
    return RT_ERROR; 
}

/* 获取时间戳 */
rt_err_t dwin_system_get_timestamp(time_t *timestamp)
{
    return RT_EOK; 
}

/* 设置时间戳 */
rt_err_t dwin_system_set_timestamp(time_t timestamp)
{
    return RT_EOK; 
}

/* 软件发送按键动作 */ 
rt_err_t dwin_system_key(rt_uint8_t code)
{
#if (DWIN_USING_TYPE == 0) 
    if(dwin_reg_write(DWIN_SYSTEM_REG_ADDR_RTC, &code, 1) != RT_EOK)
    {
        goto failed; 
    } 
    
    DWIN_DBG("The dwin send key code 0x%.2x(%d).\n", code, code);
    return RT_EOK; 
#endif 
    
#if (DWIN_USING_TYPE == 1) 
    
#endif 
    
#if (DWIN_USING_TYPE == 2) 
    
#endif 
    
failed:
    DWIN_DBG("The dwin send key code 0x%.2x(%d) \033[32mfailed\033[0m.\n", code, code);
    return RT_ERROR; 
}

/* 设置屏幕方向 */ 
rt_err_t dwin_system_dir(enum dwin_dir dir)
{
    return RT_ERROR; 
}

/* 判断VAR加载数据是否是冲L22字库加载 */ 
rt_err_t dwin_system_var_from_l22_upload(rt_bool_t *enable)
{
    RT_ASSERT(enable != RT_NULL); 
    
    *enable = RT_TRUE; 
    
    return RT_EOK; 
}

/* 判断触摸控制背光亮灭是否开启 */ 
rt_err_t dwin_system_backlight_by_touch_ctr(rt_bool_t *enable)
{
    RT_ASSERT(enable != RT_NULL); 
    
    *enable = RT_TRUE; 
    
    return RT_EOK; 
}

/* 判断是否关闭CRC */ 
rt_err_t dwin_system_crc(rt_bool_t *enable)
{
    RT_ASSERT(enable != RT_NULL); 
    
    *enable = RT_TRUE; 
    
    return RT_EOK; 
}


/* 特有系统功能API ************************************************************/ 
#if (DWIN_USING_TYPE == 0) 

#endif 

#if (DWIN_USING_TYPE == 1) 

#endif 

#if (DWIN_USING_TYPE == 2) 

#endif 
