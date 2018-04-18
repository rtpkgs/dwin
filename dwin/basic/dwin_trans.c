/*
 * @File:   dwin_trans.c 
 * @Author: liu2guang 
 * @Date:   2017-12-08 12:17:48 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-08     liu2guang    实现主动询问与自动上传数据解耦. 
 */ 
#include "dwin_trans.h" 
#include "dwin_paser.h" 

/* 自动上传数据监听器接收状态枚
   举, 内部使用 */
enum input_state
{
    STATE_NORMAL = 0,
    STATE_DATA_HEAD_H,
    STATE_DATA_HEAD_L,
    STATE_GET_DATA
};

/* 自动上传数据监听器相关数据, 
   rx同步信号, 监听器线程, 串口
   设备, 内部使用 */
static rt_sem_t    dwin_uart_rxsem;
static rt_thread_t dwin_uart_watch;
static rt_device_t dwin_uart_device;

/* 串口直接发送与阻塞接收, 内部使用 */
static uint8_t dwin_putc(uint8_t ch);
static uint8_t dwin_getc(void);

/* 串口发送字节, 内部使用 */
static uint8_t dwin_putc(uint8_t ch)
{
    uint8_t ret = dwin_err_none;
    if(rt_device_write(dwin_uart_device, (-1), &ch, 1) != 1)
    {
        ret = dwin_err_error;
    }
    return ret;
}

/* 串口阻塞接收字节, 内部使用 */
static uint8_t dwin_getc(void)
{
    uint8_t ch;
    while(rt_device_read(dwin_uart_device, (-1), &ch, 1) != 1)
    {
        rt_sem_take(dwin_uart_rxsem, RT_WAITING_FOREVER);
    }
    return ch;
}

/* 串口接收回调函数, 内部使用 */
static rt_err_t dwin_uart_rxcb(rt_device_t dev, rt_size_t size)
{
    return rt_sem_release(dwin_uart_rxsem);
}

/* 自动上传监听器处理线程, 内部使用 */
static void dwin_autoupload_watch(void *p)
{
    uint8_t ch;
    uint8_t state = STATE_NORMAL;
    uint8_t index = 0;
    uint8_t data[256] = {0};
    
    for(;;)
    {
        ch = dwin_getc();
        
        /* 监听器识别到数据帧头高字节 */
        if(state == STATE_NORMAL)
        {
            if(ch == PKG_DWIN_HEAD_H)
            {
                data[index++] = ch; 
                state = STATE_DATA_HEAD_H;
                continue;
            }
            
            /* 错误数据帧处理:丢弃之前采集数据 */
            index = 0;
            state = STATE_NORMAL;
            continue;
        }
        
        /* 监听器识别到数据帧头低字节 */
        if(state == STATE_DATA_HEAD_H)
        {
            if(ch == PKG_DWIN_HEAD_L)
            {
                data[index++] = ch;
                state = STATE_DATA_HEAD_L;
                continue;
            }
            
            /* 错误数据帧处理:丢弃之前采集数据 */
            index = 0;
            state = STATE_NORMAL;
            continue;
        }
        
        /* 监听器获取数据帧长度 */
        if(state == STATE_DATA_HEAD_L)
        {
            /* 调试打印, 打印监听器识别到的数据帧长度 */
#if (PKG_DWIN_DEBUG >= 2)
            dwin_print(PKG_DWIN_PROMPT);
            dwin_print("Listen to %dbyte data frame:", ch+3);
#endif
            data[index++] = ch;
            state = STATE_GET_DATA;
            continue;
        }
        
        /* 监听器获取数据帧数据 */
        if(state == STATE_GET_DATA)
        {
            data[index++] = ch;
            
            /* 判断数是否完成接收 */
            if(index == data[2] + 3)
            {
                /* 监听器接收数据帧打印 */
#if (PKG_DWIN_DEBUG >= 2)
                dwin_print("{");
                for(index = 0; index < (data[2]+3); index++)
                {
                    dwin_print("0x%.2x ", data[index]);
                }
                dwin_print("\b}\n");
#endif
                
                /* 数据帧解析, 这里采用直接调用会不
                   会影响监听器的数据接收能力, 如果
                   使用邮箱传递信息, 或者信号量传递
                   进行同步会不会造成解析器还未完成
                   解析又有数据被监听到了, 会不会造
                   成丢数据, 这里需要重点测试, 优化 */
                dwin_paser(data, data[2]);

                /* 数据帧识别完成, 清空变量, 等待下次数据监听 */
                index = 0;
                state = STATE_NORMAL;
                rt_memset(data, 0, 256);
                continue;
            }
        }
    }
}

/* 启动自动上传监听器处理线程 */
static uint8_t dwin_watch_start(void)
{
    /* 创建监听器线程 */ 
    dwin_uart_watch = rt_thread_create("twatch", dwin_autoupload_watch,  
        RT_NULL, 2048, PKG_DWIN_WATCH_PRIO, 10); 
    if(dwin_uart_watch == RT_NULL) 
    {
        dwin_println("dwin auto upload thread create failed"); 
        return dwin_err_error; 
    }

    /* 启动监听器线程 */ 
    rt_thread_startup(dwin_uart_watch); 
    return dwin_err_none; 
}

/* 暂停自动上传监测器处理 */
static uint8_t dwin_watch_stop(void)
{
    rt_err_t ret = RT_EOK;
    
    /* 删除监听器线程 */ 
    ret = rt_thread_delete(dwin_uart_watch);
    if(ret != RT_EOK)
    {
        dwin_println("dwin auto upload thread delete failed"); 
        return dwin_err_error; 
    }
    
    return dwin_err_none;
}

/* 自动上传监测器初始化 */
uint8_t dwin_watch_init(const char *name, uint32_t baudrate)
{
    rt_err_t ret = RT_EOK;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    
    RT_ASSERT(name != RT_NULL);
    
    /* 查找设备 */
    dwin_uart_device = rt_device_find(name);
    if(dwin_uart_device == RT_NULL)
    {
        dwin_println("%s device dont found", name);
        return dwin_err_error;
    }
    
    /* 修改设备波特率 */
    config.baud_rate = baudrate;
    rt_device_control(dwin_uart_device, RT_DEVICE_CTRL_CONFIG, (void *)&config);
    
    /* 打开设备 */
    ret = rt_device_open(dwin_uart_device, RT_DEVICE_OFLAG_RDWR | 
        RT_DEVICE_FLAG_INT_RX);
    if(ret != RT_EOK)
    {
        dwin_uart_device = RT_NULL;
        dwin_println("%s device open failed", name);
        return dwin_err_error;
    }
    else
    {
        /* 设置串口回调函数 */
        rt_device_set_rx_indicate(dwin_uart_device, dwin_uart_rxcb);
    }
    
    /* 创建串口接收异步处理信号量 */
    dwin_uart_rxsem = rt_sem_create("dwinrxsem", 0, RT_IPC_FLAG_FIFO);
    if(dwin_uart_rxsem == RT_NULL)
    {
        rt_device_set_rx_indicate(dwin_uart_device, NULL);
        rt_device_close(dwin_uart_device);
        dwin_uart_device = RT_NULL;
        dwin_println("dwin uart rxsem create failed");
        return dwin_err_error;
    }
    
    /* 启动监听器 */
    if(dwin_watch_start() != dwin_err_none)
    {
        rt_sem_delete(dwin_uart_rxsem);
        rt_device_set_rx_indicate(dwin_uart_device, NULL);
        rt_device_close(dwin_uart_device);
        dwin_uart_device = RT_NULL;
        dwin_println("dwin uart rxsem create failed");
        return dwin_err_error;
    }
    
    return dwin_err_none;
}

/* 写变量 */
uint8_t dwin_var_write(uint16_t addr, uint16_t *data, uint8_t len)
{
    uint8_t index;
    
    RT_ASSERT(data != RT_NULL);
    RT_ASSERT(len  != 0);
    
     /* 发送写变量请求 */
    dwin_putc(PKG_DWIN_HEAD_H);
    dwin_putc(PKG_DWIN_HEAD_L);
    dwin_putc((len*2) + 3);
    dwin_putc(DWIN_VAR_WRITE);
    dwin_putc((addr & 0xFF00) >> 8);
    dwin_putc((addr & 0x00FF) >> 0);

    for(index = 0; index < len; index++)
    {
        dwin_putc((data[index] & 0xFF00) >> 8);
        dwin_putc((data[index] & 0x00FF) >> 0);
    }
    
    /* 调试信息 */
#if (PKG_DWIN_DEBUG >= 2)
    dwin_print(PKG_DWIN_PROMPT);
    dwin_print("write [0x%.4x] var [%dbyte]:", addr, len*2);

    dwin_print("{");
    for(index = 0; index < len; index++)
    {
        dwin_print("0x%.4x ", data[index]);
    }
    dwin_print("\b}\n");
#endif
    
    return dwin_err_none;
}

/* 读变量 */
uint8_t dwin_var_read(uint16_t addr, uint16_t *data, uint8_t len)
{
    uint8_t index = 0;
    uint8_t rx_data[256] = {0};
    uint8_t ret = dwin_err_none;

    /* 停止监听器 */
    ret = dwin_watch_stop();
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
    /* 发送读变量请求 */
    dwin_putc(PKG_DWIN_HEAD_H);
    dwin_putc(PKG_DWIN_HEAD_L);
    dwin_putc(4);
    dwin_putc(DWIN_VAR_READ);
    dwin_putc((addr & 0xFF00) >> 8);
    dwin_putc((addr & 0x00FF) >> 0);
    dwin_putc(len);
    
    /* 阻塞式等待数据 */
    for(;;)
    {
        /* 接收数据 */
        rx_data[index++] = dwin_getc();
        
        if(index == (len*2+7))
        {
            /* 识别接收数据是否合理 */
            if((rx_data[0] != PKG_DWIN_HEAD_H)  || (rx_data[1] != PKG_DWIN_HEAD_L)  || 
               (rx_data[2] != (len*2+4))        || (rx_data[3] != DWIN_VAR_READ)    || 
               (rx_data[4] != (addr&0xFF00)>>8) || (rx_data[5] != (addr&0x00FF)>>0) ||
               (rx_data[6] != len))
            {
                ret = dwin_err_error;
                rt_memset(data, 0, len*2+7);
                break;
            }
            
            /* 字符流数据转换为uint16_t数据 */
            for(index = 7; index < (len*2+7); index+=2)
            {
                data[(index-7)/2] = (rx_data[index]<<8) + rx_data[index + 1];
            }
            
            ret = dwin_err_none;
            break;
        }
    }
    
    /* 调试信息打印 */
#if (PKG_DWIN_DEBUG >= 2)
    dwin_print(PKG_DWIN_PROMPT);
    dwin_print("user readvar [%dByte]:{", len);
    for(index = 0; index < (len*2+7); index++)
    {
        dwin_print("0x%.2x ", rx_data[index]);
    }
    dwin_print("\b}\n");
#endif
    
    /* 启动监听器 */
    ret = dwin_watch_start();
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
    return dwin_err_none;
}

/* 写寄存器 */
uint8_t dwin_reg_write(uint8_t addr, uint8_t *data, uint8_t len)
{
    uint8_t index;
    
    RT_ASSERT(data != RT_NULL);
    RT_ASSERT(len  != 0);
    
    /* 发送写寄存器指令 */
    dwin_putc(PKG_DWIN_HEAD_H);
    dwin_putc(PKG_DWIN_HEAD_L);
    dwin_putc(len + 2);
    dwin_putc(DWIN_REG_WRITE);
    dwin_putc(addr);
    
    for(index = 0; index < len; index++)
    {
        dwin_putc(data[index]);
    }
    
#if (PKG_DWIN_DEBUG >= 2)
    dwin_print(PKG_DWIN_PROMPT);
    dwin_print("user write [0x%.2x] reg [%dbyte]:", addr, len);

    dwin_print("{");
    for(index = 0; index < len; index++)
    {
        dwin_print("0x%.2x ", data[index]);
    }
    dwin_print("\b}\n");
#endif
    
    return dwin_err_none;
}

/* 读寄存器 */
uint8_t dwin_reg_read(uint8_t addr, uint8_t *data, uint8_t len)
{
    uint8_t index = 0;
    uint8_t rx_data[256] = {0};
    uint8_t ret = dwin_err_none;
    
    /* 停止监听器 */
    ret = dwin_watch_stop();
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
    /* 发送读寄存器请求命令 */
    dwin_putc(PKG_DWIN_HEAD_H);
    dwin_putc(PKG_DWIN_HEAD_L);
    dwin_putc(3);
    dwin_putc(DWIN_REG_READ);
    dwin_putc(addr);
    dwin_putc(len);
    
    /* 阻塞式等待数据 */
    for(;;)
    {
        rx_data[index++] = dwin_getc();
        
        if(index == (len+6))
        {
            /* 接收数据 */
            if((rx_data[0] != PKG_DWIN_HEAD_H) || (rx_data[1] != PKG_DWIN_HEAD_L) || 
               (rx_data[2] != (len+3))         || (rx_data[3] != DWIN_REG_READ)   || 
               (rx_data[4] != addr)            || (rx_data[5] != len))
            {
                ret = dwin_err_error;
                rt_memset(rx_data, 0, len+6);
                break;
            }
            
            /* 拷贝数据到用户buffer */
            rt_memcpy(data, &rx_data[6], len);
            ret = dwin_err_none;
            break;
        }
    }
    
    /* 调试信息 */
#if (PKG_DWIN_DEBUG >= 2)
    dwin_print(PKG_DWIN_PROMPT);
    dwin_print("user readreg [%dByte]:{", len);
    for(index = 0; index < (len+6); index++)
    {
        dwin_print("0x%.2x ", rx_data[index]);
    }
    dwin_print("\b}\n");
#endif
    
    ret = dwin_watch_start();
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
    return dwin_err_none;
}
