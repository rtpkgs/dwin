#include "dw_space.h"

/* 寄存器读写缓存, 使用全局是为了减少变量分配和释放的时间 */
static rt_uint8_t reg_rx_buffer[RT_DW_FRAME_BUFFER_MAX_LENGTH];
static rt_uint8_t reg_tx_buffer[RT_DW_FRAME_BUFFER_MAX_LENGTH];

/* 变量读写缓存, 使用全局是为了减少变量分配和释放的时间 */
static rt_uint8_t var_rx_buffer[RT_DW_FRAME_BUFFER_MAX_LENGTH];
static rt_uint8_t var_tx_buffer[RT_DW_FRAME_BUFFER_MAX_LENGTH];

/* 读寄存器空间等待响应, 线程安全阻塞等待 */
static rt_err_t wait_read_reg_response(rt_device_dw_t device, rt_uint8_t length, rt_uint8_t *rx_data, rt_tick_t timeout)
{
    rt_uint8_t c;
    rt_uint8_t rx_count        = 0;
    rt_tick_t  timeout_tick    = 0;
    rt_uint8_t response_length = 0;

    RT_ASSERT(device  != RT_NULL);
    RT_ASSERT(rx_data != RT_NULL);

    response_length = length + 6;
    timeout_tick = rt_tick_get() + timeout;

    while(rx_count != response_length)
    {
        /* 判断等待响应是否超时 */
        if(rt_tick_get() >= timeout_tick)
        {
            return RT_ETIMEOUT;
        }

        /* 获取响应字节 */
        if(device->ops->read(device->parent, (char *)&c) == RT_EOK)
        {
            rx_data[rx_count++] = c;
        }
    }

    return RT_EOK;
}

/* 读变量空间等待响应, 线程安全阻塞等待 */
static rt_err_t wait_read_var_response(rt_device_dw_t device, rt_uint8_t length, rt_uint8_t *rx_data, rt_tick_t timeout)
{
    rt_uint8_t c;
    rt_uint8_t rx_count        = 0;
    rt_tick_t  timeout_tick    = 0;
    rt_uint8_t response_length = 0;

    RT_ASSERT(device  != RT_NULL);
    RT_ASSERT(rx_data != RT_NULL);

    response_length = length*2 + 7;
    timeout_tick = rt_tick_get() + timeout;

    while(rx_count != response_length)
    {
        /* 判断等待响应是否超时 */
        if(rt_tick_get() >= timeout_tick)
        {
            return RT_ETIMEOUT;
        }

        /* 获取响应字节 */
        if(device->ops->read(device->parent, (char *)&c) == RT_EOK)
        {
            rx_data[rx_count++] = c;
        }
    }

    return RT_EOK;
}

/* 读寄存器空间 */
rt_err_t rt_dw_read_reg_space(rt_device_dw_t device, rt_uint8_t addr, rt_uint8_t length, rt_uint8_t *rx_data)
{
    rt_uint8_t index = 0;
    rt_err_t ret = RT_ERROR;

    RT_ASSERT(device  != RT_NULL);
    RT_ASSERT(rx_data != RT_NULL);

    /* 检测迪文屏幕设备是否已经初始化 */
    if(device->state == RT_DW_STATE_UNINIT)
    {
        /* 迪文屏幕未初始化 */
        return RT_ENOSYS;
    }

#if 0
    if((addr < RT_DW_REG_SPACE_MIN_ADDRESS) || (addr > RT_DW_REG_SPACE_MAX_ADDRESS))
    {
        /* 读寄存器空间地址错误 */
        return RT_ERROR;
    }
#endif

    /* 发送准备读的寄存器空间地址与长度 */
    reg_tx_buffer[RT_DW_REG_SPACE_FRAME_HIGH_OFFSET]  = RT_DW_FRMAE_HIGH_CONTENT;
    reg_tx_buffer[RT_DW_REG_SPACE_FRAME_LOW_OFFSET]   = RT_DW_FRMAE_LOW_CONTENT;

    /* 准备读取的寄存器空间数据长度不可能超过1个字节, 所以数据段只可能是1, 所以长度就固定了 */
    reg_tx_buffer[RT_DW_REG_SPACE_DATA_LENGTH_OFFSET] = 3;  
    reg_tx_buffer[RT_DW_REG_SPACE_COMMAND_OFFSET]     = RT_DW_COMMAND_READ_REG_SPACE;
    reg_tx_buffer[RT_DW_REG_SPACE_ADDRESS_OFFSET]     = addr;
    reg_tx_buffer[RT_DW_REG_SPACE_DATA_OFFSET]        = length;

    /* 读寄存器空间发送的帧数据长度固定为6个字节 */
    RT_DW_CRITICAL_START();
    for(index = 0; index < 6; index++)
    {
	    ret = device->ops->write(device->parent, (char *)&reg_tx_buffer[index]);

        if(ret != RT_EOK)
        {
            /* 读数据错误 */
            return ret;
        }
    }
    RT_DW_CRITICAL_END();

    /* 等待迪文屏幕响应待询问的数据 */
    ret = wait_read_reg_response(device, length, reg_rx_buffer, RT_TICK_PER_SECOND/10);

    if(ret != RT_EOK)
    {
        /* 等待响应超时 */
        return ret;
    }

    /* 简单验证响应数据是否正确 */
    if((reg_rx_buffer[RT_DW_REG_SPACE_FRAME_HIGH_OFFSET]  != RT_DW_FRMAE_HIGH_CONTENT)     ||
       (reg_rx_buffer[RT_DW_REG_SPACE_FRAME_LOW_OFFSET]   != RT_DW_FRMAE_LOW_CONTENT)      ||
       (reg_rx_buffer[RT_DW_REG_SPACE_DATA_LENGTH_OFFSET] != length + 3)                   ||
       (reg_rx_buffer[RT_DW_REG_SPACE_COMMAND_OFFSET]     != RT_DW_COMMAND_READ_REG_SPACE) ||
       (reg_rx_buffer[RT_DW_REG_SPACE_ADDRESS_OFFSET]     != addr)                         ||
       (reg_rx_buffer[RT_DW_REG_SPACE_DATA_OFFSET]        != length))
    {
        /* 读命令获取的响应帧错误 */
        return RT_ERROR;
    }

    for(index = 0; index < length; index++)
    {
        rx_data[index] = reg_rx_buffer[RT_DW_REG_SPACE_DATA_OFFSET+1+index];
    }

    return RT_EOK;
}

/* 写寄存器空间 */
rt_err_t rt_dw_write_reg_space(rt_device_dw_t device, rt_uint8_t addr, rt_uint8_t length, rt_uint8_t *tx_data)
{
	rt_uint8_t index = 0;
	rt_err_t ret = RT_ERROR;
	
    RT_ASSERT(device  != RT_NULL);
    RT_ASSERT(tx_data != RT_NULL);
	
    /* 检测迪文屏幕设备是否已经初始化 */
    if(device->state == RT_DW_STATE_UNINIT)
    {
        /* 迪文屏幕未初始化 */
        return RT_ENOSYS;
    }	
	
#if 0
    if((addr < RT_DW_REG_SPACE_MIN_ADDRESS) || (addr > RT_DW_REG_SPACE_MAX_ADDRESS))
    {
        /* 读寄存器空间地址错误 */
        return RT_ERROR;
    }
#endif
	
	/* 发送准备写的寄存器地址与长度和内容 */
    reg_tx_buffer[RT_DW_REG_SPACE_FRAME_HIGH_OFFSET]  = RT_DW_FRMAE_HIGH_CONTENT;
    reg_tx_buffer[RT_DW_REG_SPACE_FRAME_LOW_OFFSET]   = RT_DW_FRMAE_LOW_CONTENT;
    reg_tx_buffer[RT_DW_REG_SPACE_DATA_LENGTH_OFFSET] = length + 2;  
    reg_tx_buffer[RT_DW_REG_SPACE_COMMAND_OFFSET]     = RT_DW_COMMAND_WRITE_REG_SPACE;
    reg_tx_buffer[RT_DW_REG_SPACE_ADDRESS_OFFSET]     = addr;
	
	for(index = 0; index < (length + 5); index++)
	{
		reg_tx_buffer[RT_DW_REG_SPACE_DATA_OFFSET + index] = tx_data[index]; 
	}
	
	/* 写寄存器空间 */
    RT_DW_CRITICAL_START();
    for(index = 0; index < (length + 5); index++)
    {
	    ret = device->ops->write(device->parent, (char *)&reg_tx_buffer[index]);

        if(ret != RT_EOK)
        {
            /* 读数据错误 */
            return ret;
        }
    }
    RT_DW_CRITICAL_END();

	return RT_EOK;
}

rt_err_t rt_dw_read_var_space(rt_device_dw_t device, rt_uint16_t addr, rt_uint8_t length, rt_uint16_t *rx_data)
{
    rt_uint8_t index = 0;
    rt_err_t ret = RT_ERROR;	
	
    RT_ASSERT(device  != RT_NULL);
    RT_ASSERT(rx_data != RT_NULL);
	
    /* 检测迪文屏幕设备是否已经初始化 */
    if(device->state == RT_DW_STATE_UNINIT)
    {
        /* 迪文屏幕未初始化 */
        return RT_ENOSYS;
    }
	
#if 1
    if(/*(addr < RT_DW_REG_SPACE_MIN_ADDRESS) || */(addr > RT_DW_REG_SPACE_MAX_ADDRESS))
    {
        /* 读寄存器空间地址错误 */
        return RT_ERROR;
    }
#endif

	/* 发送准备读的变量空间地址与长度 */
    var_tx_buffer[RT_DW_VAR_SPACE_FRAME_HIGH_OFFSET]  = RT_DW_FRMAE_HIGH_CONTENT;
	var_tx_buffer[RT_DW_VAR_SPACE_FRAME_LOW_OFFSET]   = RT_DW_FRMAE_LOW_CONTENT;
	
	/* 准备读取的变量空间数据长度不可能超过1个字节, 所以数据段只可能是1, 所以长度就固定了 */
	var_tx_buffer[RT_DW_VAR_SPACE_DATA_LENGTH_OFFSET] = 4;
	var_tx_buffer[RT_DW_VAR_SPACE_COMMAND_OFFSET]     = RT_DW_COMMAND_READ_VAR_SPACE;
	var_tx_buffer[RT_DW_VAR_SPACE_ADDRESS_OFFSET]     = (rt_uint8_t)((addr & 0xFF00) >> 8);
	var_tx_buffer[RT_DW_VAR_SPACE_ADDRESS_OFFSET+1]   = (rt_uint8_t)((addr & 0x00FF) >> 0);
	var_tx_buffer[RT_DW_VAR_SPACE_DATA_OFFSET]        = length;
	
	/* 读变量空间发送的帧数据长度固定为7个字节 */
    RT_DW_CRITICAL_START();
    for(index = 0; index < 7; index++)
    {
	    ret = device->ops->write(device->parent, (char *)&var_tx_buffer[index]);

        if(ret != RT_EOK)
        {
            /* 读数据错误 */
            return ret;
        }
    }
    RT_DW_CRITICAL_END();
	
    /* 等待迪文屏幕响应待询问的数据 */
    ret = wait_read_var_response(device, length, var_rx_buffer, RT_TICK_PER_SECOND/10);

    if(ret != RT_EOK)
    {
        /* 等待响应超时 */
        return ret;
    }
	
    /* 简单验证响应数据是否正确 */
    if((var_rx_buffer[RT_DW_VAR_SPACE_FRAME_HIGH_OFFSET]  != RT_DW_FRMAE_HIGH_CONTENT)           ||
       (var_rx_buffer[RT_DW_VAR_SPACE_FRAME_LOW_OFFSET]   != RT_DW_FRMAE_LOW_CONTENT)            ||
       (var_rx_buffer[RT_DW_VAR_SPACE_DATA_LENGTH_OFFSET] != length*2 + 4)                       ||
       (var_rx_buffer[RT_DW_VAR_SPACE_COMMAND_OFFSET]     != RT_DW_COMMAND_READ_VAR_SPACE)       ||
       (var_rx_buffer[RT_DW_VAR_SPACE_ADDRESS_OFFSET]     != (rt_uint8_t)((addr & 0xFF00) >> 8)) ||
	   (var_rx_buffer[RT_DW_VAR_SPACE_ADDRESS_OFFSET+1]   != (rt_uint8_t)((addr & 0x00FF) >> 0)) ||
       (var_rx_buffer[RT_DW_VAR_SPACE_DATA_OFFSET]        != length))
    {
        /* 读命令获取的响应帧错误 */
        return RT_ERROR;
    }

    for(index = 0; index < length*2; index += 2)
    {
		((rt_uint8_t *)rx_data)[index + 0] = (rt_uint8_t)var_rx_buffer[RT_DW_VAR_SPACE_DATA_OFFSET + 2 + index];
		((rt_uint8_t *)rx_data)[index + 1] = (rt_uint8_t)var_rx_buffer[RT_DW_VAR_SPACE_DATA_OFFSET + 1 + index];
    }
	
	return RT_EOK;
}

/* 写变量空间 */
rt_err_t rt_dw_write_var_space(rt_device_dw_t device, rt_uint16_t addr, rt_uint8_t length, rt_uint16_t *tx_data)
{
	rt_uint8_t index = 0;
	rt_err_t ret = RT_ERROR;
	
    RT_ASSERT(device  != RT_NULL);
    RT_ASSERT(tx_data != RT_NULL);
	
    /* 检测迪文屏幕设备是否已经初始化 */
    if(device->state == RT_DW_STATE_UNINIT)
    {
        /* 迪文屏幕未初始化 */
        return RT_ENOSYS;
    }
	
#if 1
    if(/*(addr < RT_DW_REG_SPACE_MIN_ADDRESS) || */(addr > RT_DW_REG_SPACE_MAX_ADDRESS))
    {
        /* 读寄存器空间地址错误 */
        return RT_ERROR;
    }
#endif
	
	/* 发送准备写的变量地址与长度和内容 */
    var_tx_buffer[RT_DW_VAR_SPACE_FRAME_HIGH_OFFSET]  = RT_DW_FRMAE_HIGH_CONTENT;
    var_tx_buffer[RT_DW_VAR_SPACE_FRAME_LOW_OFFSET]   = RT_DW_FRMAE_LOW_CONTENT;
    var_tx_buffer[RT_DW_VAR_SPACE_DATA_LENGTH_OFFSET] = length*2 + 3;  
    var_tx_buffer[RT_DW_VAR_SPACE_COMMAND_OFFSET]     = RT_DW_COMMAND_WRITE_VAR_SPACE;
    var_tx_buffer[RT_DW_VAR_SPACE_ADDRESS_OFFSET]     = (rt_uint8_t)((addr & 0xFF00) >> 8);
	var_tx_buffer[RT_DW_VAR_SPACE_ADDRESS_OFFSET+1]   = (rt_uint8_t)((addr & 0x00FF) >> 0);

	for(index = 0; index < length*2; index += 2)
	{
		var_tx_buffer[RT_DW_VAR_SPACE_DATA_OFFSET + index]     = ((char *)tx_data)[index + 1];
		var_tx_buffer[RT_DW_VAR_SPACE_DATA_OFFSET + index + 1] = ((char *)tx_data)[index];
	}
	
	/* 写变量空间 */
    RT_DW_CRITICAL_START();
    for(index = 0; index < (length*2 + 6); index++)
    {
	    ret = device->ops->write(device->parent, (char *)&var_tx_buffer[index]);

        if(ret != RT_EOK)
        {
            /* 读数据错误 */
            return ret;
        }
    }
    RT_DW_CRITICAL_END();

	return RT_EOK;	
}

/* 读参数寄存器, 参数寄存器是在FLASH ID12的系统区域, 但是参数寄存器映射到了寄存器空间 */
rt_err_t rt_dw_read_para_reg(rt_device_dw_t device, rt_uint8_t addr, rt_uint8_t *rx_data)
{
	rt_err_t ret = RT_ERROR;
	
	ret = rt_dw_read_reg_space(device, addr, 1, rx_data);
	
	if(ret != RT_EOK)
	{
		/* 读寄存器空间错误 */
		return ret;
	}
	
	return RT_EOK;
}

/* 写参数寄存器, 参数寄存器是在FLASH ID12的系统区域, 但是参数寄存器映射到了寄存器空间 */
rt_err_t rt_dw_write_para_reg(rt_device_dw_t device, rt_uint8_t addr, rt_uint8_t tx_data)
{
	rt_err_t ret = RT_ERROR;
	
	ret = rt_dw_write_reg_space(device, addr, 1, &tx_data);
	
	if(ret != RT_EOK)
	{
		/* 写寄存器空间错误 */
		return ret;
	}
	
	return RT_EOK;
}
