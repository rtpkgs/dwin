#include "dw_port.h"

/* 用户头文件 */
#include "rtthread.h"

/* 用户变量 */
static rt_device_t dev = RT_NULL;
static struct rt_semaphore serial_res;

// 串口初始化port函数
bool dw_port_serial_init(const char *name, uint32_t baudrate)
{
    rt_err_t ret = RT_ERROR;
    
    dev = rt_device_find(name);
    if(dev == RT_NULL)
    {
        return false;
    }
    
    /* 可读写操作 + 中断接收 */
    ret = rt_device_open(dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    if(ret != RT_EOK)
    {
        return false;
    }
    
    return true;
}

// 发送一个字节数据到串口
bool dw_port_serial_putc(char byte)
{
    rt_size_t size = 0;
    
    size = rt_device_write(dev, 0, &byte, 1);
    if(size != 1)
    {
        return false;
    }
    
    return true;
}

// 从串口接收一个数据, 要求串口是异步接收
bool dw_port_serial_getc(char *byte)
{
    rt_size_t size = 0;
    
    size = rt_device_read(dev, 0, byte, 1);
    if(size != 1)
    {
        return false;
    }
    
    return true;
}

/* 初始化串口资源锁 */
bool dw_port_serial_resource_init(void)
{
    rt_err_t ret = RT_ERROR;
    
    ret = rt_sem_init(&serial_res, "dw res", 0x01 , RT_IPC_FLAG_PRIO);
    if(ret != RT_EOK)
    {
        return false;
    }
    
    return true;
}

/* 带超时时间获取串口资源锁 */
bool dw_port_serial_resource_take(int32_t timeout)
{
    rt_err_t ret = RT_ERROR;
    
    ret = rt_sem_take(&serial_res, timeout);
    if(ret != RT_EOK)
    {
        return false;
    }
    
    return true;
}

/* 释放串口资源锁 */
bool dw_port_serial_resource_release(void)
{
    rt_err_t ret = RT_ERROR;
    
    ret = rt_sem_release(&serial_res);
    if(ret != RT_EOK)
    {
        return false;
    }
    
    return true;
}
