/*
 * @File:   dwin_trans.c 
 * @Author: liu2guang 
 * @Date:   2018-04-22 14:52:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-04-22     liu2guang    update v2 framework. 
 */ 
 
#include "dwin_trans.h" 

static struct dwin_watch watch; 

/* Serial to send 1byte */ 
static rt_err_t dwin_watch_putc(uint8_t ch)
{
    rt_err_t ret = RT_EOK; 
    
    if(rt_device_write(watch.serial, (-1), &ch, 1) != 1)
    {
        ret = RT_EFULL; 
    }
    
    return ret;
}

/* Serial receive 1byte in block */ 
static uint8_t dwin_watch_getc(void)
{
    uint8_t ch; 
    
    while(rt_device_read(watch.serial, (-1), &ch, 1) != 1)
    {
        rt_sem_take(watch.rxsem, RT_WAITING_FOREVER);
    }
    
    return ch;
}

static rt_err_t dwin_watch_rxcb(rt_device_t dev, rt_size_t size)
{
    return rt_sem_release(watch.rxsem);
}

static void dwin_watch_run(void *p)
{
    uint8_t ch = 0; 
    uint8_t index = 0; 
    dwin_watch_state_t state = DWIN_WATCH_STATE_IDLE; 
    
    while(1)
    {
        ch = dwin_watch_getc(); 
        
        /* Watch listen to the frame header high byte */ 
        if(state == DWIN_WATCH_STATE_IDLE) 
        {
            if(ch == DWIN_USING_HEADH) 
            {
                watch.data[index++] = ch; 
                state = DWIN_WATCH_STATE_HEADH; 
                continue; 
            }
            else
            {
                index = 0; 
                state = DWIN_WATCH_STATE_IDLE; 
                continue;
            }
        }
        
        /* Watch listen to the frame header low byte */ 
        if(state == DWIN_WATCH_STATE_HEADH)
        {
            if(ch == DWIN_USING_HEADL)
            {
                watch.data[index++] = ch; 
                state = DWIN_WATCH_STATE_HEADL;
                continue;
            }
            else
            {
                index = 0;
                state = DWIN_WATCH_STATE_IDLE;
                continue;
            }
        }
        
        if(state == DWIN_WATCH_STATE_HEADL)
        {
            watch.data[index++] = ch;
            state = DWIN_WATCH_STATE_DATE;
            continue;
        }
        
        if(state == DWIN_WATCH_STATE_DATE)
        {
            watch.data[index++] = ch; 
            
            if(index == watch.data[2] + 3)
            {
#ifdef DWIN_USING_DEBUG
                DWIN_DBG("Listen to (%d)byte data frame:", watch.data[2]); 
                
                DWIN_USING_PRINT("{");
                for(index = 0; index < (watch.data[2]+3); index++) 
                {
                    DWIN_USING_PRINT("0x%.2x ", watch.data[index]); 
                }
                DWIN_USING_PRINT("\b}.\n");
#endif
                /* Data parse */ 
                DWIN_DBG("Start parse.\n"); 
                
                index = 0;
                state = DWIN_WATCH_STATE_IDLE;
                rt_memset(watch.data, 0x00, sizeof(watch.data));
                
                continue;
            }
        }
    }
}

static rt_err_t dwin_watch_start(void) 
{
    /* Create the watch thread */ 
    watch.thread = rt_thread_create("twatch", dwin_watch_run, RT_NULL, 2048, 6, 10); 
    if(watch.thread == RT_NULL) 
    {
        DWIN_DBG("Dwin auto upload watch thread create failed.\n"); 
        return RT_ENOSYS; 
    }

    /* Start the watch thread */ 
    rt_thread_startup(watch.thread); 
    
    return RT_EOK; 
}

static rt_err_t dwin_watch_stop(void)
{
    rt_err_t ret = RT_EOK;
    
    /* Delete the watch thread */ 
    ret = rt_thread_delete(watch.thread); 
    if(ret != RT_EOK)
    {
        DWIN_DBG("Dwin auto upload watch thread delete failed.\n"); 
        return RT_ENOSYS; 
    }
    
    return RT_EOK;
}

rt_err_t dwin_reg_read(uint8_t addr, uint8_t *data, uint8_t len)
{
#if (DWIN_USING_MODEL == 0)
    
#endif
    
#if (DWIN_USING_MODEL == 1)
    
#endif
    
#if (DWIN_USING_MODEL == 2)
    
#endif
    
    return RT_EOK; 
}

rt_err_t dwin_reg_write(uint8_t addr, uint8_t *data, uint8_t len)
{
#if (DWIN_USING_MODEL == 0)
    
#endif
    
#if (DWIN_USING_MODEL == 1)
    
#endif
    
#if (DWIN_USING_MODEL == 2)
    
#endif
    
    return RT_EOK; 
}

rt_err_t dwin_var_read(uint16_t addr, uint16_t *data, uint8_t len)
{
#if (DWIN_USING_MODEL == 0)
    
#endif
    
#if (DWIN_USING_MODEL == 1)
    
#endif
    
#if (DWIN_USING_MODEL == 2)
    
#endif
    
    return RT_EOK; 
}

rt_err_t dwin_var_write(uint16_t addr, uint16_t *data, uint8_t len)
{
#if (DWIN_USING_MODEL == 0)
    
#endif
    
#if (DWIN_USING_MODEL == 1)
    
#endif
    
#if (DWIN_USING_MODEL == 2)
    
#endif
    
    return RT_EOK; 
}

rt_err_t dwin_watch_init(dwin_t dwin, const char *name, rt_uint32_t baudrate)
{
    rt_err_t ret = RT_EOK; 
    
    RT_ASSERT(dwin != RT_NULL); 
    RT_ASSERT(name != RT_NULL); 
    
    /* Init watch struct */ 
    rt_memset(&watch, 0x00, sizeof(struct dwin_watch)); 
    dwin->watch = &watch; 
    
    /* Find uart device */ 
    dwin->watch->serial = rt_device_find(name); 
    if(dwin->watch->serial == RT_NULL)
    {
        DWIN_DBG("No found %s device.\n", name); 
        return RT_ENOSYS; 
    }
    else
    {
        DWIN_DBG("Found %s device.\n", name); 
    }
    
    /* Config uart */ 
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; 
    
    config.baud_rate = baudrate; 
    rt_device_control(dwin->watch->serial, RT_DEVICE_CTRL_CONFIG, (void *)&config); 
    
    /* Open device */ 
    ret = rt_device_open(dwin->watch->serial, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX); 
    if(ret != RT_EOK)
    {
        DWIN_DBG("Open %s device failed error code: %d.\n", ret); 
        return RT_ENOSYS; 
    }
    else
    {
        DWIN_DBG("Open %s device succeed.\n", name); 
    }
    
    /* Set the serial port callback function */ 
    rt_device_set_rx_indicate(dwin->watch->serial, dwin_watch_rxcb); 
    
    /* Create a serial to receive asynchronous processing semaphore */ 
    dwin->watch->rxsem = rt_sem_create("dwin_rx", 0, RT_IPC_FLAG_FIFO); 
    if(dwin->watch->rxsem == RT_NULL)
    {
        rt_device_set_rx_indicate(dwin->watch->serial, RT_NULL);
        rt_device_close(dwin->watch->serial);
        
        dwin->watch->serial = RT_NULL;
        DWIN_DBG("Serial rx sem create failed.\n"); 
        
        return RT_ENOSYS;
    }
    
    /* Start watch */ 
    ret = dwin_watch_start(); 
    if(ret != RT_EOK)
    {
        rt_sem_delete(dwin->watch->rxsem); 
        rt_device_set_rx_indicate(dwin->watch->serial, RT_NULL); 
        rt_device_close(dwin->watch->serial); 
        
        dwin->watch->serial = RT_NULL;
        DWIN_DBG("Watch start failed error code: %d\n", ret); 
        
        return ret; 
    }
    
    return RT_EOK; 
}
