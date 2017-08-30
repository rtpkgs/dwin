#include "dw_cmd.h"

#include <finsh.h>

rt_device_dw_t cmd_device = RT_NULL;

/* 系统功能插件 */

/* 获取固件版本 */
rt_uint8_t fw_version(void)
{
	rt_uint8_t version = rt_dw_fw_version(cmd_device);
	
	rt_kprintf("[dwin] fw version 0x%x\n", version);
	
	return version;
}
FINSH_FUNCTION_EXPORT_ALIAS(fw_version, dwver, dwinlib read reg space);
MSH_CMD_EXPORT_ALIAS       (fw_version, dwver, dwinlib read reg space);

/* 获取当前背光亮度, 亮度为0x00~0x40 */
rt_uint8_t now_bl(void)
{	
	rt_uint8_t now_bl = rt_dw_get_now_bl(cmd_device);
	
	rt_kprintf("[dwin] now backlight level 0x%x\n", now_bl);
	
	return now_bl;
}
FINSH_FUNCTION_EXPORT_ALIAS(now_bl, dwnbl, dwin srceen now backlight level);
MSH_CMD_EXPORT_ALIAS       (now_bl, dwnbl, dwin srceen now backlight level);

/* 设置当前背光亮度, 亮度为0x00~0x40 */
rt_err_t set_now_bl(rt_uint8_t now_bl)
{
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_set_now_bl(cmd_device, now_bl);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] now backlight level set succeed\n");
	}
	
	return ret;
}
FINSH_FUNCTION_EXPORT_ALIAS(set_now_bl, dwsnbl, dwin srceen set now backlight level);
MSH_CMD_EXPORT_ALIAS       (set_now_bl, dwsnbl, dwin srceen set now backlight level);

/* 控制蜂鸣n*10ms */
rt_err_t beep_nx10ms(rt_uint8_t time)
{
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_beep_nx10ms(cmd_device, time);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] beep succeed\n");
	}
	
	return ret;
}
FINSH_FUNCTION_EXPORT_ALIAS(beep_nx10ms, dwbeep, dwin srceen beep n*10ms);
MSH_CMD_EXPORT_ALIAS       (beep_nx10ms, dwbeep, dwin srceen beep n*10ms);

/* 获取当前页 */
rt_uint16_t get_now_pageid(void)
{
	rt_uint16_t page_id = 0;
	
	page_id = rt_dw_get_now_pageid(cmd_device);
	
	rt_kprintf("[dwin] now page id %d\n", page_id);
	
	return page_id;
}
FINSH_FUNCTION_EXPORT_ALIAS(get_now_pageid, dwgpage, get dwin srceen now page id);
MSH_CMD_EXPORT_ALIAS       (get_now_pageid, dwgpage, get dwin srceen now page id);

/* 跳转到指定页面, 但是弹窗不会关闭, 所以在迪文屏幕有弹窗出现时, 不要调用该函数 */
rt_err_t jump_page(rt_uint16_t id)
{
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_jump_page(cmd_device, id);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] jump page id %d succeed\n", id);
	}
	
	return ret;
}
FINSH_FUNCTION_EXPORT_ALIAS(jump_page, dwjump, jump the specified page id);
MSH_CMD_EXPORT_ALIAS       (jump_page, dwjump, jump the specified page id);

/* 获取当前波特率 */
rt_uint8_t baudrate(void)
{
	rt_uint8_t baudrate = 0;
	
	baudrate = rt_dw_baudrate(cmd_device);
	
	rt_kprintf("[dwin] baudrate = ");
	
	switch(baudrate)
	{
		case 0x00: rt_kprintf("1200bps\n");    break;
		case 0x01: rt_kprintf("2400bps\n");    break;
		case 0x02: rt_kprintf("4800bps\n");    break;
		case 0x03: rt_kprintf("9600bps\n");    break;
		case 0x04: rt_kprintf("19200bps\n");   break;
		case 0x05: rt_kprintf("38400bps\n");   break;
		case 0x06: rt_kprintf("57600bps\n");   break;
		case 0x07: rt_kprintf("115200bps\n");  break;
		case 0x08: rt_kprintf("288000bps\n");  break;
		case 0x09: rt_kprintf("768000bps\n");  break;
		case 0x0A: rt_kprintf("625000bps\n");  break;
		case 0x0B: rt_kprintf("1250000bps\n"); break;
		case 0x0C: rt_kprintf("2500000bps\n"); break;
		case 0x0D: rt_kprintf("2304000bps\n"); break;
		case 0x0E: rt_kprintf("3456000bps\n"); break;
		case 0x0F: rt_kprintf("6912000bps\n"); break;
		case 0x10: rt_kprintf("9216000bps\n"); break;
	}
	
	return baudrate;	
}
FINSH_FUNCTION_EXPORT_ALIAS(baudrate, dwbaudrate, dwin dwbaudrate);
MSH_CMD_EXPORT_ALIAS       (baudrate, dwbaudrate, dwin dwbaudrate);

/* 设置屏幕旋转方向 */
rt_err_t set_screen_dir(rt_dw_screen_dir_t dir)
{
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_set_screen_dir(cmd_device, dir);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] set screen dir succeed\n");
	}
	
	return ret;	
}
FINSH_FUNCTION_EXPORT_ALIAS(set_screen_dir, dwsdir, set dwin screen dir);
MSH_CMD_EXPORT_ALIAS       (set_screen_dir, dwsdir, set dwin screen dir);

/* 获取当前屏幕方向 */
rt_uint8_t get_screen_dir(void)
{
	rt_err_t ret = RT_EOK;
	rt_dw_screen_dir_t dir;
	
	ret = rt_dw_get_screen_dir(cmd_device, &dir);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] now screen dir\n");
	}
	
	switch(dir)
	{
		case RT_DW_SCREEN_DIR_000: rt_kprintf("000 DIR\n");    break;
		case RT_DW_SCREEN_DIR_090: rt_kprintf("090 DIR\n");    break;
		case RT_DW_SCREEN_DIR_180: rt_kprintf("180 DIR\n");    break;
		case RT_DW_SCREEN_DIR_270: rt_kprintf("270 DIR\n");    break;
	}
	
	return (rt_uint8_t)dir;
}
FINSH_FUNCTION_EXPORT_ALIAS(get_screen_dir, dwgdir, get dwin screen dir);
MSH_CMD_EXPORT_ALIAS       (get_screen_dir, dwgdir, get dwin screen dir);

/* 获取背光是否由触摸控制, 0背光不受触摸控制, 1背光受触摸控制 */
rt_uint8_t get_bl_ctr_enable(void)
{
	rt_err_t ret = RT_EOK;
	rt_uint8_t enable;
	
	ret = rt_dw_get_bl_ctr_enable(cmd_device, &enable);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] backlight level ctr is enable = %d\n", enable);
	}
	
	return enable;	
}
FINSH_FUNCTION_EXPORT_ALIAS(get_bl_ctr_enable, dwgblce, get backlight level ctr is enable);
MSH_CMD_EXPORT_ALIAS       (get_bl_ctr_enable, dwgblce, get backlight level ctr is enable);

/* 获取是否开启帧CRC功能, 0未启动, 1启动 */
rt_uint8_t get_crc_enable(void)
{
	rt_err_t ret = RT_EOK;
	rt_uint8_t enable;
	
	ret = rt_dw_get_crc_enable(cmd_device, &enable);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] serial port crc is enable = %d\n", enable);
	}
	
	return enable;		
}
FINSH_FUNCTION_EXPORT_ALIAS(get_crc_enable, dwgcrc, get serial port crc is enable);
MSH_CMD_EXPORT_ALIAS       (get_crc_enable, dwgcrc, get serial port ctr is enable);

/* 设置是否开启帧CRC功能, 0未启动, 1启动 */
rt_err_t set_crc_enable(rt_uint8_t enable)
{
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_set_crc_enable(cmd_device, enable);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] set serial port crc succeed\n");
	}
	
	return ret;	
}
FINSH_FUNCTION_EXPORT_ALIAS(set_crc_enable, dwscrc, set serial port crc enable);
MSH_CMD_EXPORT_ALIAS       (set_crc_enable, dwscrc, set serial port ctr enable);

/* 获取触摸数据是否自动上传 */
rt_uint8_t get_data_auto_update(void)
{
	rt_err_t ret = RT_EOK;
	rt_uint8_t enable;
	
	ret = rt_dw_get_data_auto_update(cmd_device, &enable);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] dwin data auto update enable = %d\n", enable);
	}
	
	return enable;	
}
FINSH_FUNCTION_EXPORT_ALIAS(get_data_auto_update, dwgupdata, get data auto update);
MSH_CMD_EXPORT_ALIAS       (get_data_auto_update, dwgupdata, get data auto update);

/* 设置触摸数据是否自动上传 */
rt_err_t set_data_auto_update(rt_uint8_t enable)
{
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_set_data_auto_update(cmd_device, enable);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] set data auto update succeed\n");
	}
	
	return ret;		
}
FINSH_FUNCTION_EXPORT_ALIAS(set_data_auto_update, dwsupdata, set data auto update);
MSH_CMD_EXPORT_ALIAS       (set_data_auto_update, dwsupdata, set data auto update);

/* 获取数据寄存器上电初始化模式, 0上电初始化为0x00, 1上电有L22字库文件加载 */
rt_uint8_t get_var_init_mode(void)
{
	rt_dw_var_init_mode_t mode;
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_get_var_init_mode(cmd_device, &mode);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] get var init mode = %d\n", mode);
	}
	
	return (rt_uint8_t)mode;	
}
FINSH_FUNCTION_EXPORT_ALIAS(get_var_init_mode, dwgvim, get var init mode);
MSH_CMD_EXPORT_ALIAS       (get_var_init_mode, dwgvim, get var init mode);

/* 设置数据寄存器上电初始化模式, 0上电初始化为0x00, 1上电有L22字库文件加载 */
rt_err_t set_var_init_mode(rt_dw_var_init_mode_t mode)
{
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_set_var_init_mode(cmd_device, mode);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] set var init mode succeed\n");
	}
	
	return ret;
}
FINSH_FUNCTION_EXPORT_ALIAS(set_var_init_mode, dwsvim, set var init mode);
MSH_CMD_EXPORT_ALIAS       (set_var_init_mode, dwsvim, set var init mode);

/* 获取蜂鸣器是否开启, 0开启 1未开启 */
rt_uint8_t get_buzzer_enable(void)
{
	rt_uint8_t enable;
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_get_buzzer_enable(cmd_device, &enable);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] dwin buzzer enable = %d\n", enable);
	}

	return (rt_uint8_t)enable;
}
FINSH_FUNCTION_EXPORT_ALIAS(get_buzzer_enable, dwgbuzzer, get buzzer enable);
MSH_CMD_EXPORT_ALIAS       (get_buzzer_enable, dwgbuzzer, get buzzer enable);

/* 设置蜂鸣器是否开启, 0开启 1未开启 */
rt_err_t set_buzzer_enable(rt_uint8_t enable)
{
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_set_buzzer_enable(cmd_device, enable);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] set buzzer enable succeed\n");
	}
	
	return ret;			
}
FINSH_FUNCTION_EXPORT_ALIAS(set_buzzer_enable, dwsbuzzer, set buzzer enable);
MSH_CMD_EXPORT_ALIAS       (set_buzzer_enable, dwsbuzzer, set buzzer enable);

/* 获取RTC时间 */
rt_err_t get_rtc(void)
{
	rt_err_t ret = RT_EOK;
	rt_uint8_t year, month, day, hour, minute, seconds;
	
	ret = rt_dw_get_rtc(cmd_device, &year, &month, &day, &hour, &minute, &seconds);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] time 20%d-%d-%d %d:%d:%d\n", 
			year, month, day, hour, minute, seconds);
	}

	return ret;
}
FINSH_FUNCTION_EXPORT_ALIAS(get_rtc, dwgrtc, get dwin rtc);
MSH_CMD_EXPORT_ALIAS       (get_rtc, dwgrtc, get dwin rtc);

/* 设置RTC时间 */
rt_err_t set_rtc(rt_uint8_t year, rt_uint8_t month, rt_uint8_t day, 
    rt_uint8_t hour, rt_uint8_t minute, rt_uint8_t seconds)
{
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_set_rtc(cmd_device, year, month, day, hour, minute, seconds);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] set rtc time succeed\n");
	}
	
	return ret;		
}
FINSH_FUNCTION_EXPORT_ALIAS(set_rtc, dwsrtc, set dwin rtc);
MSH_CMD_EXPORT_ALIAS       (set_rtc, dwsrtc, set dwin rtc);

/* 使用时间戳设置RTC时间 */
rt_err_t set_rtc_timestamp(const time_t timestamp)
{
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_set_rtc_timestamp(cmd_device, timestamp);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] set timestamp succeed\n");
	}
	
	return ret;	
}
FINSH_FUNCTION_EXPORT_ALIAS(set_rtc_timestamp, dwstimestamp, set dwin timestamp);
MSH_CMD_EXPORT_ALIAS       (set_rtc_timestamp, dwstimestamp, set dwin timestamp);

/* 获取RTC时间戳 */
time_t get_rtc_timestamp(void)
{
	time_t timestamp;
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_get_rtc_timestamp(cmd_device, &timestamp);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] timestamp %ld\n", timestamp);
	}
	
	return timestamp;
}
FINSH_FUNCTION_EXPORT_ALIAS(get_rtc_timestamp, dwgtimestamp, get dwin timestamp);
MSH_CMD_EXPORT_ALIAS       (get_rtc_timestamp, dwgtimestamp, get dwin timestamp);

/* 登陆功能插件 */
#if defined(RT_USING_DW_LOGIN)

/* 添加用户密码, 注意要初始化login插件 */
rt_err_t login_add(rt_uint32_t password)
{
	rt_err_t ret = RT_EOK;
	
	ret = rt_dw_plugins_login_add(password);
	
	if(ret == RT_EOK)
	{
		rt_kprintf("[dwin] add login user succeed\n");
	}
	
	return ret;
}
FINSH_FUNCTION_EXPORT_ALIAS(login_add, dwadduser, add dwin login user);
MSH_CMD_EXPORT_ALIAS       (login_add, dwadduser, add dwin login user);
#endif

/* 变量图标功能插件 */
#if defined(RT_USING_DW_VICON)

/* 添加变量图标 */
rt_err_t vicon_add(rt_uint16_t addr, rt_uint16_t default_value, 
    rt_uint16_t min_value, rt_uint16_t max_value)
{
	if(rt_dw_plugins_vicon_add(cmd_device, addr, default_value, min_value, max_value) == RT_NULL)
	{
		return RT_ERROR;
	}
	
	rt_kprintf("[dwin] add vicon succeed\n");
	
	return RT_EOK;
}
FINSH_FUNCTION_EXPORT_ALIAS(vicon_add, dwaddvicon, add dwin vicon);
MSH_CMD_EXPORT_ALIAS       (vicon_add, dwaddvicon, add dwin vicon);

/* 切换图标 */
rt_err_t vicon_change(rt_uint16_t addr, rt_uint16_t value)
{
    rt_err_t ret = RT_EOK;

    /* 写变量空间 */
    ret = rt_dw_write_var_space(cmd_device, addr, 1, &value);

    if(ret != RT_EOK)
    {
        /* 写变量空间失败 */
        return ret;
    }
	
	return RT_EOK;
}
FINSH_FUNCTION_EXPORT_ALIAS(vicon_change, dwcgvicon, change dwin vicon);
MSH_CMD_EXPORT_ALIAS       (vicon_change, dwcgvicon, change dwin vicon);
#endif

/* 注册dwinlib cmd插件使用的设备句柄 */
rt_uint8_t rt_dw_cmd_init(rt_device_dw_t device)
{
	RT_ASSERT(device != RT_NULL);

	/* 注册cmd插件使用的迪文设备 */
	cmd_device = device;

	return RT_EOK;
}
