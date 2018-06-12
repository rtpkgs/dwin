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
 * 2018-06-05     balanceTWK   add DGUSII system func. 
 */

#include "dwin_system.h"
#include "dwin_trans.h"

static rt_uint8_t bcd2dec(rt_uint8_t bcd)
{
    return bcd - (bcd >> 4) * 6;
}

static rt_uint8_t dec2bcd(rt_uint8_t dec)
{
    return dec + (dec / 10) * 6;
}

/* 系统功能地址 ***************************************************************/
#if (DWIN_USING_TYPE == 0)
#define DWIN_SYSTEM_REG_ADDR_VERSION        0x0000
#define DWIN_SYSTEM_REG_ADDR_BACKLIGHT      0x0001
#define DWIN_SYSTEM_REG_ADDR_BUZZ           0x0002
#define DWIN_SYSTEM_REG_ADDR_PAGE           0x0003
#define DWIN_SYSTEM_REG_ADDR_TOUCH_EN       0x000B
#define DWIN_SYSTEM_REG_ADDR_RTC_WFLAG      0x001F
#define DWIN_SYSTEM_REG_ADDR_RTC            0x0020
#define DWIN_SYSTEM_REG_ADDR_KEY            0x004F
#endif

#if (DWIN_USING_TYPE == 1)
#endif

#if (DWIN_USING_TYPE == 2)
#define DWIN_SYSTEM_VAR_ADDR_VERSION        0x000F
#define DWIN_SYSTEM_VAR_ADDR_GETBACKLIGHT   0x0031
#define DWIN_SYSTEM_VAR_ADDR_SETBACKLIGHT   0x0082
#define DWIN_SYSTEM_VAR_ADDR_MUSIC          0x00A0
#define DWIN_SYSTEM_VAR_ADDR_CURRENTPAGE    0x0014
#define DWIN_SYSTEM_VAR_ADDR_JUMPPAGE       0x0084
//#define DWIN_SYSTEM_REG_ADDR_TOUCH_EN     0x000B
#define DWIN_SYSTEM_VAR_ADDR_GETRTC         0x0010
#define DWIN_SYSTEM_VAR_ADDR_SETRTC         0x009c
#endif

/* 通用系统功能API ************************************************************/

/* 获取版本 */
rt_err_t dwin_system_version(rt_uint32_t *ver_sn)
{
#if (DWIN_USING_TYPE == 0)
    rt_uint8_t data = 0;

    RT_ASSERT(ver_sn != RT_NULL);

    if (dwin_reg_read(DWIN_SYSTEM_REG_ADDR_VERSION, &data, 1) != RT_EOK)
    {
        goto failed;
    }
    data = bcd2dec(data);

    *ver_sn = (DWIN_USING_TYPE * 10000) + (data) * 100 + 0;
    
    DWIN_DBG("The dwin lib is \033[32mv%d.%d.%d\033[0m.\n", DWIN_VERSION_M, DWIN_VERSION_S, DWIN_VERSION_R); 
    DWIN_DBG("The dwin type is DGUSMINI, DGUSVer is %d.%d, OSVer None, VER_SN is %d.\n", data / 10, data % 10, *ver_sn);

    return RT_EOK;
#endif

#if (DWIN_USING_TYPE == 1)

#endif

#if (DWIN_USING_TYPE == 2)
    rt_uint16_t data = 0;
    rt_uint8_t  buff[2] = {0};

    RT_ASSERT(ver_sn != RT_NULL);

    if (dwin_var_read(DWIN_SYSTEM_VAR_ADDR_VERSION, &data, 1) != RT_EOK)
    {
        goto failed;
    }
    buff[0] = (rt_uint8_t)(data >> 8);
    buff[1] = (rt_uint8_t)(data & 0x00FF);
    *ver_sn = (DWIN_USING_TYPE * 10000) + (buff[0]) * 100 + buff[1];
    
    DWIN_DBG("The dwin lib is \033[32mv%d.%d.%d\033[0m.\n", DWIN_VERSION_M, DWIN_VERSION_S, DWIN_VERSION_R); 
    DWIN_DBG("The dwin type is DGUSII, DGUSVer is %d, OSVer is %d, VER_SN is %d.\n", buff[0], buff[1], *ver_sn);

    return RT_EOK;
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

    if (dwin_reg_read(DWIN_SYSTEM_REG_ADDR_BACKLIGHT, level, 1) != RT_EOK)
    {
        goto failed;
    }

    DWIN_DBG("The dwin backlight level is %d.\n", *level);
    return RT_EOK;
#endif

#if (DWIN_USING_TYPE == 1)

#endif

#if (DWIN_USING_TYPE == 2)
    RT_ASSERT(level != RT_NULL);
    rt_uint16_t level_u16 = 0;
    if (dwin_var_read(DWIN_SYSTEM_VAR_ADDR_GETBACKLIGHT, &level_u16, 1) != RT_EOK)
    {
        goto failed;
    }
    *level = (rt_uint8_t)(level_u16 & 0x00FF);
    DWIN_DBG("The dwin backlight level is %d.\n", *level);
    return RT_EOK;
#endif

failed:
    DWIN_DBG("The dwin backlight level read \033[32mfailed\033[0m.\n");
    return RT_ERROR;
}

/* 设置背光亮度 */
rt_err_t dwin_system_set_backlight(rt_uint8_t level)
{
#if (DWIN_USING_TYPE == 0)
    if (dwin_reg_write(DWIN_SYSTEM_REG_ADDR_BACKLIGHT, &level, 1) != RT_EOK)
    {
        goto failed;
    }

    DWIN_DBG("The dwin backlight level set %d.\n", level);
    return RT_EOK;
#endif

#if (DWIN_USING_TYPE == 1)

#endif

#if (DWIN_USING_TYPE == 2)
    rt_uint16_t data = (rt_uint16_t)(level << 8);

    if (dwin_var_write(DWIN_SYSTEM_VAR_ADDR_SETBACKLIGHT, &data, 1) != RT_EOK)
    {
        goto failed;
    }

    DWIN_DBG("The dwin backlight level set %d.\n", (data >> 8));
    return RT_EOK;
#endif

failed:
    DWIN_DBG("The dwin backlight set \033[32mfailed\033[0m.\n");
    return RT_ERROR;
}

/* 蜂鸣tick*10ms */
rt_err_t dwin_system_buzz(rt_uint8_t tick)
{
#if (DWIN_USING_TYPE == 0)
    if (dwin_reg_write(DWIN_SYSTEM_REG_ADDR_BUZZ, &tick, 1) != RT_EOK)
    {
        goto failed;
    }

    DWIN_DBG("The dwin buzz %d ms.\n", tick * 10);
    return RT_EOK;
#endif

#if (DWIN_USING_TYPE == 1)

#endif

#if (DWIN_USING_TYPE == 2)
    rt_uint16_t data[2] = {0};
    data[0] = 0x0301;
    data[1] = 0x4000;
    for (rt_uint8_t i = 0; i < tick; i++)
    {
        if (dwin_var_write(DWIN_SYSTEM_VAR_ADDR_MUSIC, data, 2) != RT_EOK)
        {
            goto failed;
        }
        else
        {
            rt_thread_mdelay(100);
        }
    }
    DWIN_DBG("The dwin keytone %d times.\n", tick);
    return RT_EOK;
#endif

failed:
    DWIN_DBG("The dwin keytone \033[32mfailed\033[0m.\n");
    return RT_ERROR;
}

/* 跳转指定ID界面 */
rt_err_t dwin_system_jump(rt_uint16_t page)
{
#if (DWIN_USING_TYPE == 0)
    rt_uint8_t data[2] = {0};

    data[0] = (page >> 8);
    data[1] = (page << 8) >> 8;

    if (dwin_reg_write(DWIN_SYSTEM_REG_ADDR_PAGE, data, 2) != RT_EOK)
    {
        goto failed;
    }

    DWIN_DBG("The dwin jump page %d.\n", page);
    return RT_EOK;
#endif

#if (DWIN_USING_TYPE == 1)

#endif

#if (DWIN_USING_TYPE == 2)
    rt_uint16_t data[2] = {0};

    data[0] = (0x5A << 8) + 0x01;
    data[1] = page;

    if (dwin_var_write(DWIN_SYSTEM_VAR_ADDR_JUMPPAGE, data, 2) != RT_EOK)
    {
        goto failed;
    }

    DWIN_DBG("The dwin jump %d page.\n", page);
    return RT_EOK;
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

    if (dwin_reg_read(DWIN_SYSTEM_REG_ADDR_PAGE, data, 2) != RT_EOK)
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
    rt_uint16_t data;

    RT_ASSERT(page != RT_NULL);

    if (dwin_var_read(DWIN_SYSTEM_VAR_ADDR_CURRENTPAGE, &data, 1) != RT_EOK)
    {
        goto failed;
    }

    *page = data;
    DWIN_DBG("The dwin current page is %d.\n", *page);
    return RT_EOK;
#endif

failed:
    DWIN_DBG("The dwin read page \033[32mfailed\033[0m.\n");
    return RT_ERROR;
}

/* 启动或者关闭触摸功能 */
rt_err_t dwin_system_touch(rt_bool_t enable)
{
#if (DWIN_USING_TYPE == 0)
    rt_uint8_t data = (enable == RT_TRUE) ? 0xFF:0x00;

    if (dwin_reg_write(DWIN_SYSTEM_REG_ADDR_TOUCH_EN, &data, 1) != RT_EOK)
    {
        goto failed;
    }

    if (enable == RT_TRUE)
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
    if (enable == RT_TRUE)
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

    if (dwin_reg_read(DWIN_SYSTEM_REG_ADDR_RTC, data, 7) != RT_EOK)
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
    rt_uint16_t data[4] = {0};

    RT_ASSERT(rtc != RT_NULL);

    if (dwin_var_read(DWIN_SYSTEM_VAR_ADDR_GETRTC, data, 4) != RT_EOK)
    {
        goto failed;
    }

    rtc->year   = (data[0] >> 8) + 2000;
    rtc->month  = (rt_uint8_t)(data[0] & 0x00FF);
    rtc->day    = (rt_uint8_t)(data[1] >> 8);
    rtc->hour   = (rt_uint8_t)(data[2] >> 8);
    rtc->minute = (rt_uint8_t)(data[2] & 0xFF);
    rtc->second = (rt_uint8_t)(data[3] >> 8);
    rtc->week   = (rt_uint8_t)(data[1] & 0x00FF);

    DWIN_DBG("The dwin rtc is %.4d-%.2d-%.2d %.2d:%.2d:%.2d week:%d.\n",
             rtc->year, rtc->month, rtc->day, rtc->hour, rtc->minute, rtc->second,
             rtc->week);
    return RT_EOK;
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
    if ((rtc.month == 0 || rtc.month > 12) ||
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
    data[1] = dec2bcd(rtc.year - 2000);
    data[2] = dec2bcd(rtc.month);
    data[3] = dec2bcd(rtc.day);
    data[4] = 0x00;                /* 周几, 可以随便设定 */
    data[5] = dec2bcd(rtc.hour);
    data[6] = dec2bcd(rtc.minute);
    data[7] = dec2bcd(rtc.second);

    if (dwin_reg_write(DWIN_SYSTEM_REG_ADDR_RTC_WFLAG, data, 8) != RT_EOK)
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
    rt_uint16_t data[4] = {0};

    /* 检测RTC结构体是否合法 */
    if ((rtc.month == 0 || rtc.month > 12) ||
            (rtc.day   == 0 || rtc.day   > 31) ||
            (rtc.hour   > 24)                  ||
            (rtc.minute > 59)                  ||
            (rtc.second > 59))
    {
        DWIN_DBG("The dwin rtc set \033[32mfailed\033[0m, Invalid argument: %.4d-%.2d-%.2d %.2d:%.2d:%.2d.\n",
                 rtc.year, rtc.month, rtc.day, rtc.hour, rtc.minute, rtc.second);
        return RT_ERROR;
    }

    data[0] = 0x5AA5;
    data[1] = ((rtc.year - 2000) << 8) + rtc.month;
    data[2] = (rtc.day << 8) + rtc.hour;
    data[3] = (rtc.minute << 8) + rtc.second;

    if (dwin_var_write(DWIN_SYSTEM_VAR_ADDR_SETRTC, data, 4) != RT_EOK)
    {
        goto failed;
    }

    DWIN_DBG("The dwin rtc set %.4d-%.2d-%.2d %.2d:%.2d:%.2d.\n",
             rtc.year, rtc.month, rtc.day, rtc.hour, rtc.minute, rtc.second);
    return RT_EOK;
#endif

failed:
    DWIN_DBG("The dwin rtc set \033[32mfailed\033[0m.\n");
    return RT_ERROR;
}

/* 获取时间戳 */
rt_err_t dwin_system_get_timestamp(time_t *timestamp)
{
    struct dwin_rtc rtc = {0};
    struct tm tm_new = {0};

    dwin_system_get_rtc(&rtc);
    tm_new.tm_sec  = rtc.second;
    tm_new.tm_min  = rtc.minute;
    tm_new.tm_hour = rtc.hour;
    tm_new.tm_mday = rtc.day;
    tm_new.tm_mon  = rtc.month - 1;
    tm_new.tm_year = rtc.year - 1900;

    *timestamp = mktime(&tm_new);

    return RT_EOK;
}

/* 设置时间戳 */
rt_err_t dwin_system_set_timestamp(time_t timestamp)
{
    struct tm *p_tm;
    struct dwin_rtc rtc;

    p_tm = localtime(&timestamp);

    rtc.second = p_tm->tm_sec ;
    rtc.minute = p_tm->tm_min ;
    rtc.hour   = p_tm->tm_hour;

    rtc.day    = p_tm->tm_mday;
    rtc.month  = p_tm->tm_mon  + 1;
    rtc.year   = p_tm->tm_year + 1900;

    dwin_system_set_rtc(rtc);

    return RT_EOK;
}

/* 软件发送按键动作 */
rt_err_t dwin_system_key(rt_uint8_t code)
{
#if (DWIN_USING_TYPE == 0)
    if (dwin_reg_write(DWIN_SYSTEM_REG_ADDR_RTC, &code, 1) != RT_EOK)
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
