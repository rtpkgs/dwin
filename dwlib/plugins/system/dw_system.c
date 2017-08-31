#include "dw_system.h"

/* 获取迪文屏幕固件版本 */
rt_uint8_t rt_dw_fw_version(rt_device_dw_t device)
{
    rt_err_t ret = RT_EOK;
    rt_uint8_t version = 0;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_reg_space(device, RT_DW_REG_00H_001_VERSION, 1, &version);

    if(ret != RT_EOK)
    {
        /* 读寄存器空间错误 */
        return 0;
    }

    return version;
}

/* 获取当前背光亮度, 亮度为0x00~0x40 */
rt_uint8_t rt_dw_get_now_bl(rt_device_dw_t device)
{
    rt_err_t ret = RT_EOK;
    rt_uint8_t now_bl = 0;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_reg_space(device, RT_DW_REG_01H_001_NOW_BL, 1, &now_bl);

    if(ret != RT_EOK)
    {
        /* 读寄存器空间错误 */
        return 0;
    }

    return now_bl;
}

/* 设置当前背光亮度, 亮度为0x00~0x40 */
rt_err_t rt_dw_set_now_bl(rt_device_dw_t device, rt_uint8_t now_bl)
{
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_write_reg_space(device, RT_DW_REG_01H_001_NOW_BL, 1, &now_bl);

    if(ret != RT_EOK)
    {
        /* 写寄存器空间错误 */
        return ret;
    }

    return RT_EOK;
}

/* 控制蜂鸣n*10ms */
rt_err_t rt_dw_beep_nx10ms(rt_device_dw_t device, rt_uint8_t time)
{
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_write_reg_space(device, RT_DW_REG_02H_001_MAKE_BEEP, 1, &time);

    if(ret != RT_EOK)
    {
        /* 写寄存器空间错误 */
        return ret;
    }

    return RT_EOK;
}

/* 获取当前页 */
rt_uint16_t rt_dw_get_now_pageid(rt_device_dw_t device)
{
    rt_err_t ret = RT_EOK;
    rt_uint8_t id_temp[2];

    RT_ASSERT(device != RT_NULL);

	RT_DW_CRITICAL_START();
    ret = rt_dw_read_reg_space(device, RT_DW_REG_03H_002_NOW_PAGEID, 2, id_temp);
	RT_DW_CRITICAL_END();

    if(ret != RT_EOK)
    {
        /* 读寄存器空间错误 */
        return 0xffff;
    }
	
    return (rt_uint16_t)((id_temp[0] << 8) + id_temp[1]);
}

/* 跳转到指定页面, 但是弹窗不会关闭, 所以在迪文屏幕有弹窗出现时, 不要调用该函数 */
rt_err_t rt_dw_jump_page(rt_device_dw_t device, rt_uint16_t id)
{
    rt_err_t ret = RT_EOK;
    rt_uint8_t id_temp[2];

    RT_ASSERT(device != RT_NULL);

    id_temp[0] = (id >> 8);
    id_temp[1] = (id << 8) >> 8;

    ret = rt_dw_write_reg_space(device, RT_DW_REG_03H_002_NOW_PAGEID, 2, id_temp);

    if(ret != RT_EOK)
    {
        /* 读寄存器空间错误 */
        return ret;
    }

    return RT_EOK;
}

/* 获取串口波特率:
[00 "1.2K"  ]  [01 "2.4K"  ]  [02 "4.8K"  ]  [03 "9.6K"  ]
[04 "19.2K" ]  [05 "38.4K" ]  [06 "57.6K" ]  [07 "115.2K"]
[08 "28.8K" ]  [09 "76.8K" ]  [10 "62.5K" ]  [11 "125K"  ]
[12 "250K"  ]  [13 "230.4K"]  [14 "345.6K"]  [15 "691.2K"]
[16 "921.6K"] */
rt_uint8_t rt_dw_baudrate(rt_device_dw_t device)
{
    rt_err_t ret = RT_EOK;
    rt_uint8_t buadrate = 0;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_reg_space(device, RT_DW_REG_11H_001_BRUDRATE, 1, &buadrate);

    if(ret != RT_EOK)
    {
        /* 读寄存器空间错误 */
        return 0;
    }

    return buadrate;
}

/* 设置屏幕旋转方向 */
rt_err_t rt_dw_set_screen_dir(rt_device_dw_t device, rt_dw_screen_dir_t dir)
{
    rt_uint8_t r2 = 0;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    if(dir > RT_DW_SCREEN_DIR_270)
    {
        /* 方向参数输入错误 */
        return RT_ERROR;
    }

    ret = rt_dw_read_para_reg(device, RT_DW_REG_12H_001_SYSCFG, &r2);

    if(ret != RT_EOK)
    {
        /* 读参数寄存器错误 */
        return RT_ERROR;
    }

    switch(dir)
    {
        case RT_DW_SCREEN_DIR_000: RT_DW_CLR_BIT(r2, RT_DW_R2BIT6_HDS_MASK); RT_DW_CLR_BIT(r2, RT_DW_R2BIT7_VHS_MASK); break;
        case RT_DW_SCREEN_DIR_090: RT_DW_SET_BIT(r2, RT_DW_R2BIT6_HDS_MASK); RT_DW_SET_BIT(r2, RT_DW_R2BIT7_VHS_MASK); break;
        case RT_DW_SCREEN_DIR_180: RT_DW_SET_BIT(r2, RT_DW_R2BIT6_HDS_MASK); RT_DW_CLR_BIT(r2, RT_DW_R2BIT7_VHS_MASK); break;
        case RT_DW_SCREEN_DIR_270: RT_DW_CLR_BIT(r2, RT_DW_R2BIT6_HDS_MASK); RT_DW_SET_BIT(r2, RT_DW_R2BIT7_VHS_MASK); break;
    }

    ret = rt_dw_write_para_reg(device, RT_DW_REG_12H_001_SYSCFG, r2);

    if(ret != RT_EOK)
    {
        /* 写参数寄存器错误 */
        return RT_ERROR;
    }

    return RT_EOK;
}

/* 获取当前屏幕方向 */
rt_err_t rt_dw_get_screen_dir(rt_device_dw_t device, rt_dw_screen_dir_t *dir)
{
    rt_uint8_t r2   = 0;
    rt_uint8_t r2_6 = 0;
    rt_uint8_t r2_7 = 0;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_para_reg(device, RT_DW_REG_12H_001_SYSCFG, &r2);

    if(ret != RT_EOK)
    {
        /* 读参数寄存器错误 */
        return ret;
    }

    /* 获取r2参数寄存器第6和7位 */
    r2_6 = ((RT_DW_READ_BIT(r2, RT_DW_R2BIT6_HDS_MASK) == RT_DW_R2BIT6_HDS_MASK) ? 1:0);
    r2_7 = ((RT_DW_READ_BIT(r2, RT_DW_R2BIT7_VHS_MASK) == RT_DW_R2BIT7_VHS_MASK) ? 1:0);

    if((r2_6 == 0) && (r2_7 == 0))
    {
        *dir = RT_DW_SCREEN_DIR_000;
    }
    else if((r2_6 == 0) && (r2_7 == 0))
    {
        *dir = RT_DW_SCREEN_DIR_090;
    }
    else if((r2_6 == 0) && (r2_7 == 0))
    {
        *dir = RT_DW_SCREEN_DIR_180;
    }
    else /* ((r2_6 == 0) && (r2_7 == 0)) */
    {
        *dir = RT_DW_SCREEN_DIR_270;
    }

    return RT_EOK;
}

/* 获取背光是否由触摸控制, 0背光不受触摸控制, 1背光受触摸控制 */
rt_err_t rt_dw_get_bl_ctr_enable(rt_device_dw_t device, rt_uint8_t *enable)
{
    rt_uint8_t r2   = 0;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_para_reg(device, RT_DW_REG_12H_001_SYSCFG, &r2);

    if(ret != RT_EOK)
    {
        /* 读参数寄存器错误 */
        return ret;
    }

    /* 获取r2参数寄存器第5位 */
    *enable = ((RT_DW_READ_BIT(r2, RT_DW_R2BIT5_BL_CTR_MASK) == RT_DW_R2BIT5_BL_CTR_MASK) 
        ? RT_DW_ENABLE : RT_DW_DISABLE);

    return RT_EOK;
}

/* 设置背光是否由触摸控制, 0背光不受触摸控制, 1背光受触摸控制 */
rt_err_t rt_dw_set_bl_ctr_enable(rt_device_dw_t device, rt_uint8_t enable)
{
    rt_uint8_t r2   = 0;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_para_reg(device, RT_DW_REG_12H_001_SYSCFG, &r2);

    if(ret != RT_EOK)
    {
        /* 读参数寄存器错误 */
        return ret;
    }

    if(enable == RT_DW_DISABLE)
    {
        RT_DW_CLR_BIT(r2, RT_DW_R2BIT5_BL_CTR_MASK);
    }
    else if(enable == RT_DW_ENABLE)
    {
        RT_DW_SET_BIT(r2, RT_DW_R2BIT5_BL_CTR_MASK); 
    }
    else
    {
        /* 输入参数错误 */
        return RT_ERROR;
    }

    ret = rt_dw_write_para_reg(device, RT_DW_REG_12H_001_SYSCFG, r2);

    if(ret != RT_EOK)
    {
        /* 写参数寄存器错误 */
        return RT_ERROR;
    }

    return RT_EOK;
}

/* 获取是否开启帧CRC功能, 0未启动, 1启动 */
rt_err_t rt_dw_get_crc_enable(rt_device_dw_t device, rt_uint8_t *enable)
{
    rt_uint8_t r2   = 0;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_para_reg(device, RT_DW_REG_12H_001_SYSCFG, &r2);

    if(ret != RT_EOK)
    {
        /* 读参数寄存器错误 */
        return ret;
    }

    /* 获取r2参数寄存器第5位 */
    *enable = ((RT_DW_READ_BIT(r2, RT_DW_R2BIT4_CRC_EN_MASK) == RT_DW_R2BIT4_CRC_EN_MASK) 
        ? RT_DW_ENABLE : RT_DW_DISABLE);

    return RT_EOK;
}

/* 设置是否开启帧CRC功能, 0未启动, 1启动 */
rt_err_t rt_dw_set_crc_enable(rt_device_dw_t device, rt_uint8_t enable)
{
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

/* 目前已经知道这2款屏幕不支持帧CRC功能 */
#if defined(RT_USING_DMT48270M043_02W) || defined(RT_USING_DMT48270M050_02W)
    ret = RT_ERROR;
#endif

    return ret;
}

/* 获取触摸数据是否自动上传 */
rt_err_t rt_dw_get_data_auto_update(rt_device_dw_t device, rt_uint8_t *enable)
{
    rt_uint8_t r2   = 0;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_para_reg(device, RT_DW_REG_12H_001_SYSCFG, &r2);

    if(ret != RT_EOK)
    {
        /* 读参数寄存器错误 */
        return ret;
    }

    /* 获取r2参数寄存器第5位 */
    *enable = ((RT_DW_READ_BIT(r2, RT_DW_R2BIT3_DATA_AUTO_UPLOAD_MASK) == RT_DW_R2BIT3_DATA_AUTO_UPLOAD_MASK) 
        ? RT_DW_ENABLE : RT_DW_DISABLE);

    return RT_EOK;
}

/* 设置触摸数据是否自动上传 */
rt_err_t rt_dw_set_data_auto_update(rt_device_dw_t device, rt_uint8_t enable)
{
    rt_uint8_t r2   = 0;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_para_reg(device, RT_DW_REG_12H_001_SYSCFG, &r2);

    if(ret != RT_EOK)
    {
        /* 读参数寄存器错误 */
        return ret;
    }

    if(enable == RT_DW_DISABLE)
    {
        RT_DW_CLR_BIT(r2, RT_DW_R2BIT3_DATA_AUTO_UPLOAD_MASK);
    }
    else if(enable == RT_DW_ENABLE)
    {
        RT_DW_SET_BIT(r2, RT_DW_R2BIT3_DATA_AUTO_UPLOAD_MASK); 
    }
    else
    {
        /* 输入参数错误 */
        return RT_ERROR;
    }

    ret = rt_dw_write_para_reg(device, RT_DW_REG_12H_001_SYSCFG, r2);

    if(ret != RT_EOK)
    {
        /* 写参数寄存器错误 */
        return RT_ERROR;
    }

    return RT_EOK;
}

/* 获取数据寄存器上电初始化模式, 0上电初始化为0x00, 1上电有L22字库文件加载 */
rt_err_t rt_dw_get_var_init_mode(rt_device_dw_t device, rt_dw_var_init_mode_t *mode)
{
    rt_uint8_t r2   = 0;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_para_reg(device, RT_DW_REG_12H_001_SYSCFG, &r2);

    if(ret != RT_EOK)
    {
        /* 读参数寄存器错误 */
        return ret;
    }

    /* 获取r2参数寄存器第5位 */
    *mode = ((RT_DW_READ_BIT(r2, RT_DW_R2BIT2_VAR_INIT_MODE_MASK) == RT_DW_R2BIT2_VAR_INIT_MODE_MASK) 
        ? RT_DW_VAR_INIT_MODE_1 : RT_DW_VAR_INIT_MODE_0);

    return RT_EOK;
}

/* 设置数据寄存器上电初始化模式, 0上电初始化为0x00, 1上电有L22字库文件加载 */
rt_err_t rt_dw_set_var_init_mode(rt_device_dw_t device, rt_dw_var_init_mode_t mode)
{
    rt_uint8_t r2   = 0;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_para_reg(device, RT_DW_REG_12H_001_SYSCFG, &r2);

    if(ret != RT_EOK)
    {
        /* 读参数寄存器错误 */
        return ret;
    }

    if(mode == RT_DW_VAR_INIT_MODE_0)
    {
        RT_DW_CLR_BIT(r2, RT_DW_R2BIT2_VAR_INIT_MODE_MASK);
    }
    else if(mode == RT_DW_VAR_INIT_MODE_1)
    {
        RT_DW_SET_BIT(r2, RT_DW_R2BIT2_VAR_INIT_MODE_MASK); 
    }
    else
    {
        /* 输入参数错误 */
        return RT_ERROR;
    }

    ret = rt_dw_write_para_reg(device, RT_DW_REG_12H_001_SYSCFG, r2);

    if(ret != RT_EOK)
    {
        /* 写参数寄存器错误 */
        return RT_ERROR;
    }

    return RT_EOK;
}

/* 获取蜂鸣器是否开启, 0开启 1未开启 */
rt_err_t rt_dw_get_buzzer_enable(rt_device_dw_t device, rt_uint8_t *enable)
{
    rt_uint8_t r2   = 0;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_para_reg(device, RT_DW_REG_12H_001_SYSCFG, &r2);

    if(ret != RT_EOK)
    {
        /* 读参数寄存器错误 */
        return ret;
    }

    /* 获取r2参数寄存器第5位 */
    *enable = ((RT_DW_READ_BIT(r2, RT_DW_R2BIT0_BUZZER_EN_MASK) == RT_DW_R2BIT0_BUZZER_EN_MASK) 
        ? RT_DW_ENABLE : RT_DW_DISABLE);

    return RT_EOK;
}

/* 设置蜂鸣器是否开启, 0开启 1未开启 */
rt_err_t rt_dw_set_buzzer_enable(rt_device_dw_t device, rt_uint8_t enable)
{
    rt_uint8_t r2   = 0;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(device != RT_NULL);

    ret = rt_dw_read_para_reg(device, RT_DW_REG_12H_001_SYSCFG, &r2);

    if(ret != RT_EOK)
    {
        /* 读参数寄存器错误 */
        return ret;
    }

    if(enable == RT_DW_DISABLE)
    {
        RT_DW_CLR_BIT(r2, RT_DW_R2BIT0_BUZZER_EN_MASK);
    }
    else if(enable == RT_DW_ENABLE)
    {
        RT_DW_SET_BIT(r2, RT_DW_R2BIT0_BUZZER_EN_MASK); 
    }
    else
    {
        /* 输入参数错误 */
        return RT_ERROR;
    }

    ret = rt_dw_write_para_reg(device, RT_DW_REG_12H_001_SYSCFG, r2);

    if(ret != RT_EOK)
    {
        /* 写参数寄存器错误 */
        return RT_ERROR;
    }

    return RT_EOK;
}

static rt_uint8_t bcd2decimal(rt_uint8_t bcd)
{
    return bcd-(bcd >> 4)*6;
}

static rt_uint8_t decimal2bcd(rt_uint8_t decimal)
{
    return (rt_uint8_t)(decimal+(decimal / 10)*6);
}

/* 获取RTC时间 */
rt_err_t rt_dw_get_rtc(rt_device_dw_t device, 
    rt_uint8_t *year, rt_uint8_t *month,  rt_uint8_t *day, 
    rt_uint8_t *hour, rt_uint8_t *minute, rt_uint8_t *seconds)
{
    rt_err_t ret = RT_EOK;
    rt_uint8_t date_and_time[7] = {0};

    ret = rt_dw_read_reg_space(device, RT_DW_REG_20H_016_RTC, 7, date_and_time);

    if(ret != RT_EOK)
    {
        /* 写寄存器空间错误 */
        return ret;
    }

    /* 传入数据到函数入口 */
    *year    = bcd2decimal(date_and_time[0]);
    *month   = bcd2decimal(date_and_time[1]);
    *day     = bcd2decimal(date_and_time[2]);
    *hour    = bcd2decimal(date_and_time[4]);
    *minute  = bcd2decimal(date_and_time[5]);
    *seconds = bcd2decimal(date_and_time[6]);

    return RT_EOK;
}

/* 设置RTC时间 */
rt_err_t rt_dw_set_rtc(rt_device_dw_t device, 
    rt_uint8_t year, rt_uint8_t month,  rt_uint8_t day, 
    rt_uint8_t hour, rt_uint8_t minute, rt_uint8_t seconds)
{
    rt_err_t ret = RT_EOK;
    rt_uint8_t date_and_time[8];

    /* 真正的RTC地址为0x20, 但是修改了RTC需要在0x1F处
       写0x5a表明用户修改了RTC数据, 迪文屏幕在修改系
       RTC后会自动清零 */
    date_and_time[0] = 0x5a;	
    date_and_time[1] = decimal2bcd(year);
    date_and_time[2] = decimal2bcd(month);
    date_and_time[3] = decimal2bcd(day);
    date_and_time[4] = 0x00;				/* 周几, 可以随便设定 */
    date_and_time[5] = decimal2bcd(hour);
    date_and_time[6] = decimal2bcd(minute);
    date_and_time[7] = decimal2bcd(seconds);

    ret = rt_dw_write_reg_space(device, RT_DW_REG_1FH_001_RTC_UPDATE_FLAG, 8, date_and_time);

    if(ret != RT_EOK)
    {
        /* 写寄存器空间错误 */
        return ret;
    }

    return RT_EOK;
}

/* 时间戳相关宏与变量 */
#define MINUTE (60)
#define HOUR   (60*MINUTE)
#define DAY    (24*HOUR)
#define YEAR   (365*DAY)

static int month[12] =
{
    0,
    DAY*(31),
    DAY*(31+29),
    DAY*(31+29+31),
    DAY*(31+29+31+30),
    DAY*(31+29+31+30+31),
    DAY*(31+29+31+30+31+30),
    DAY*(31+29+31+30+31+30+31),
    DAY*(31+29+31+30+31+30+31+31),
    DAY*(31+29+31+30+31+30+31+31+30),
    DAY*(31+29+31+30+31+30+31+31+30+31),
    DAY*(31+29+31+30+31+30+31+31+30+31+30)
};

static time_t rt_mktime(struct tm *tm)
{
    long res;
    int year;
    year = tm->tm_year - 70;

    res = YEAR * year + DAY * ((year + 1) / 4);
    res += month[tm->tm_mon];

    if(tm->tm_mon > 1 && ((year + 2) % 4))
    {
        res -= DAY;
    }

    res += DAY * (tm->tm_mday - 1);
    res += HOUR * tm->tm_hour;
    res += MINUTE * tm->tm_min;
    res += tm->tm_sec;

    return res;
}

/* 获取RTC时间戳, 十进制*/
rt_err_t rt_dw_get_rtc_timestamp(rt_device_dw_t device, time_t *timestamp)
{
	rt_err_t ret = RT_EOK;
	rt_uint8_t date_and_time[6] = {0};
    struct tm time;
	
	ret = rt_dw_get_rtc(device, 
		&date_and_time[0], &date_and_time[1], &date_and_time[2],
		&date_and_time[3], &date_and_time[4], &date_and_time[5]);
	
    if(ret != RT_EOK)
    {
        /* 获取RTC错误 */
        return ret;
    }

    time.tm_year = date_and_time[0] + 100;	
	time.tm_mon  = date_and_time[1] - 1;
	time.tm_mday = date_and_time[2];
	time.tm_hour = date_and_time[3] - RT_DW_TIME_DIFFEREBCE;
	time.tm_min  = date_and_time[4];
	time.tm_sec  = date_and_time[5];
	
	*timestamp = rt_mktime(&time);

    return RT_EOK;
}

/* 使用时间戳设置RTC时间 */
rt_err_t rt_dw_set_rtc_timestamp(rt_device_dw_t device, const time_t timestamp)
{
    rt_err_t ret = RT_EOK;
    struct tm *time = RT_NULL;

    time = localtime(&timestamp);

    ret = rt_dw_set_rtc(device, 
        time->tm_year - 100, time->tm_mon + 1, time->tm_mday,
        time->tm_hour + RT_DW_TIME_DIFFEREBCE, time->tm_min, time->tm_sec);

    if(ret != RT_EOK)
    {
        /* 设置RTC错误 */
        return ret;
    }

    return RT_EOK;
}
