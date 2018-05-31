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

/* 系统功能地址 */ 
#if (DWIN_USING_TYPE == 0) 
#define DWIN_SYSTEM_REG_ADDR_VERSION    0x0000
#define DWIN_SYSTEM_REG_ADDR_BACKLIGHT  0x0001
#endif 

#if (DWIN_USING_TYPE == 1) 
#endif 

#if (DWIN_USING_TYPE == 2) 
#endif 

/* 通用系统功能API */ 
/* 版本 = 种类*10000 + 硬件版本*100 + 屏幕内嵌OS版本*1 */ 
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
    DWIN_DBG("The dwin type is DGUSMINI, HWVer is %d.%d, OSVer None, VER_SN is %d.\n", data/10, data%10, *ver_sn);
    
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

rt_err_t dwin_system_set_backlight(rt_uint8_t level)
{
#if (DWIN_USING_TYPE == 0) 
    rt_uint8_t data = level; 
    
    if(dwin_reg_write(DWIN_SYSTEM_REG_ADDR_BACKLIGHT, &data, 1) != RT_EOK)
    {
        goto failed; 
    }
    
    DWIN_DBG("The dwin backlight level set %d.\n", data);
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

/* 种类特有API */ 
#if (DWIN_USING_TYPE == 0) 

#endif 

#if (DWIN_USING_TYPE == 1) 

#endif 

#if (DWIN_USING_TYPE == 2) 

#endif 
