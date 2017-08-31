#include "dw_init.h"

#if defined(RT_USING_DW_SYSTEM)
#include "dw_system.h"
#endif

#if defined(RT_USING_DW_CMD)
#include "dw_cmd.h"
#endif

static rt_err_t dw_read(rt_device_t device, char *c)
{
	rt_size_t size = 0;
	
	RT_ASSERT(c != RT_NULL);
	RT_ASSERT(device != RT_NULL);
	
	size = rt_device_read(device, 0, c, 1);
	
	if(size != 1)
	{
		/* 写数据错误 */
		return RT_EIO;
	}
	
    return RT_EOK;
}

static rt_err_t dw_write(rt_device_t device, char *c)
{
	rt_size_t size = 0;
	
	RT_ASSERT(c != RT_NULL);
	RT_ASSERT(device != RT_NULL);
	
	size = rt_device_write(device, 0, c, 1);
	
	if(size != 1)
	{
		/* 写数据错误 */
		return RT_EIO;
	}
	
    return RT_EOK;
}

static struct rt_dw_ops dw_ops = 
{
    dw_read,
    dw_write
};

/* 注册迪文屏幕设备,目前只支持普通发送, 中断接收的串口设备 */
rt_err_t rt_device_dw_init(rt_device_dw_t device, const char *name, rt_uint8_t init_bl)
{
    rt_err_t ret = RT_ERROR;

    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(name   != RT_NULL);

    device->parent = rt_device_find(name);

    if(device->parent == RT_NULL)
    {
        /* 没有发现串口设备 */
        return RT_EIO;
    }

    ret = rt_device_open(device->parent, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);

    if(ret != RT_EOK)
    {
        /* 打开设备失败 */
        device->parent = RT_NULL;
        return ret;
    }

    device->ops   = &dw_ops;
	
	device->state = RT_DW_STATE_INIT;
	
	/* 获取屏幕相关参数 */
#if defined(RT_USING_DW_DEBUG) && defined(RT_USING_DW_SYSTEM)
	{
		rt_uint8_t rx_data[10] = {0};
		
		rt_kprintf("\n");
		
		/* 打印固件版本 */
		rx_data[0] = rt_dw_fw_version(device);
		rt_kprintf("[dwin]firmware version 0x%.2x\n", rx_data[0]);
		
		/* 上电默认亮度 */
		rt_dw_set_now_bl(device, init_bl);
		
		/* 打印屏幕亮度 */
		rx_data[0] = rt_dw_get_now_bl(device);
		rt_kprintf("[dwin]now backlight level 0x%.2x\n", rx_data[0]);
		
		/* 蜂鸣1s */
		rt_dw_beep_nx10ms(device, 50);
		
		/* 上电初始化调整到第一幅页面 */
		rt_dw_jump_page(device, 0);	/* 这里有个bug, 当有弹窗时，无法关闭弹窗 */
		
		*((rt_uint16_t *)rx_data) = rt_dw_get_now_pageid(device);
		rt_kprintf("[dwin]now page id %d\n", *((rt_uint16_t *)rx_data));

        /* 获取串口波特率 */
        rx_data[0] = rt_dw_baudrate(device);
        rt_kprintf("[dwin]baudrate index %d\n", rx_data[0]);

        /* 开机设备屏幕方向 */
        rt_dw_set_screen_dir(device, RT_DW_SCREEN_DIR_000);
		
		/* 获取屏幕方向 */
        rt_dw_get_screen_dir(device, (rt_dw_screen_dir_t *)&rx_data[0]);
        rt_kprintf("[dwin]screen dir %d\n", rx_data[0]);

		/* 设置是否打开触摸控制背光 */
		rt_dw_set_bl_ctr_enable(device, RT_DW_ENABLE);

		/* 获取是否打开触摸控制背光 */
        rt_dw_get_bl_ctr_enable(device, &rx_data[0]);
        rt_kprintf("[dwin]touch ctr backlight enable %d\n", rx_data[0]);

		/* 获取是否打开帧CRC功能 */
        rt_dw_get_crc_enable(device, &rx_data[0]);
        rt_kprintf("[dwin]frame crc enable %d\n", rx_data[0]);

		/* 设置触摸数据不自动上传 */
		rt_dw_set_data_auto_update(device, RT_DW_DISABLE);

		/* 获取数据是否自动上传 */
		rt_dw_get_data_auto_update(device, &rx_data[0]);
		rt_kprintf("[dwin]data auto update %d\n", rx_data[0]);

		/* 设置数据寄存器上电初始化模式, 0上电初始化为0x00, 1上电有L22字库文件加载 */
		rt_dw_set_var_init_mode(device, RT_DW_VAR_INIT_MODE_1);

		/* 获取数据寄存器上电初始化模式, 0上电初始化为0x00, 1上电有L22字库文件加载 */
		rt_dw_get_var_init_mode(device, (rt_dw_var_init_mode_t *)&rx_data[0]);
		rt_kprintf("[dwin]var init mode %d\n", rx_data[0]);

		/* 设置蜂鸣器是否开启, 0开启 1未开启 注意必须是DWIN周期200MS才可以正常工作, 这里有BUG */
		// rt_dw_set_buzzer_enable(device, RT_DW_ENABLE);

		/* 获取蜂鸣器是否开启, 0开启 1未开启 */
		rt_dw_get_buzzer_enable(device, &rx_data[0]);
		rt_kprintf("[dwin]buzzer enable %d\n", rx_data[0]);
		
		/* 初始化默认时间 */
		// rt_dw_set_rtc(device, 17, 8, 22, 22, 23, 0);
		// rt_dw_set_rtc_timestamp(device, 1503412777UL);

		/* 获取RTC时间 */
		rt_dw_get_rtc(device, 
			&rx_data[0], &rx_data[1], &rx_data[2], 
			&rx_data[3], &rx_data[4], &rx_data[5]);
		rt_kprintf("[dwin]now time 20%.2d-%.2d-%.2d %.2d:%.2d:%.2d\n", 
			rx_data[0], rx_data[1], rx_data[2], 
			rx_data[3], rx_data[4], rx_data[5]);

		/* 获取RTC时间戳 */
		rt_dw_get_rtc_timestamp(device, (time_t *)rx_data);
		rt_kprintf("[dwin]now timestamp %ld\n", *(time_t *)rx_data);
	}	
#endif
		
    return RT_EOK;
}
