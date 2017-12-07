#include "dwin_transport.h"

/* state enum */
enum input_state
{
    STATE_NORMAL = 0,
    STATE_DATA_HEAD_H,
    STATE_DATA_HEAD_L,
    STATE_GET_DATA
};

/* static data interface */
static rt_sem_t    dwin_uart_rxsem;
static rt_thread_t dwin_uart_watch;
static rt_device_t dwin_uart_device;

/* static uart putc getc api */
static uint8_t dwin_putc(uint8_t ch);
static uint8_t dwin_getc(void);

/* static api realization */
static uint8_t dwin_putc(uint8_t ch)
{
    uint8_t ret = dwin_err_none;
    
    if(rt_device_write(dwin_uart_device, (-1), &ch, 1) != 1)
    {
        ret = dwin_err_error;
    }
    
    return ret;
}

static uint8_t dwin_getc(void)
{
    uint8_t ch;
    
    while(rt_device_read(dwin_uart_device, (-1), &ch, 1) != 1)
    {
        rt_sem_take(dwin_uart_rxsem, RT_WAITING_FOREVER);
    }
    
    return ch;
}

/* uart rx callback */
static rt_err_t dwin_uart_rxcb(rt_device_t dev, rt_size_t size)
{
    return rt_sem_release(dwin_uart_rxsem);
}

/* uart auto upload watch */
void dwin_autoupload_watch(void *p)
{
    uint8_t ch;
    uint8_t state = STATE_NORMAL;
    uint8_t index = 0;
    uint8_t data[256] = {0};
    
    for(;;)
    {
        ch = dwin_getc();
        
        /* found head high byte */
        if(state == STATE_NORMAL)
        {
            if(ch == PKG_DWIN_HEAD_H)
            {
                data[index] = ch; index++;
                state = STATE_DATA_HEAD_H;
                continue;
            }
            
            index = 0;
            continue;
        }
        
        /* found head low byte */
        if(state == STATE_DATA_HEAD_H)
        {
            if(ch == PKG_DWIN_HEAD_L)
            {
                data[index] = ch; index++;
                state = STATE_DATA_HEAD_L;
                continue;
            }
            
            index = 0;
            state = STATE_NORMAL;
            continue;
        }
        
        /* found data frame len */
        if(state == STATE_DATA_HEAD_L)
        {
            data[index] = ch;
            state = STATE_GET_DATA;
            
#ifdef DWIN_DEBUG
            dwin_print(PKG_DWIN_PROMPT);
            dwin_print("auto upload [%dByte]:", ch);
#endif
        }
        
        /* data receive */
        if(state == STATE_GET_DATA)
        {
            data[index] = ch; index++;
            
            /* data receive complete */
            if(index == data[2] + 3)
            {
                /* data paser */
                // dwin_paser(data, data[2]);
                
#ifdef DWIN_DEBUG
                {
                    uint8_t i;
                    
                    dwin_print("{");
                    for(i = 0; i < (data[2]+3); i++)
                    {
                        dwin_print("0x%.2x ", data[i]);
                    }
                    dwin_print("\b}\n");
                }
#endif
                
                /* clear data ready next watch */
                index = 0;
                state = STATE_NORMAL;
                rt_memset(data, 0, 256);
            }
        }
    }
}

/* extern api realization */
/* auto upload init */
uint8_t dwin_watch_init(const char *name, uint32_t bandrate)
{
    rt_err_t ret = RT_EOK;
    
    RT_ASSERT(name != RT_NULL);
    
    /* find uart device */
    dwin_uart_device = rt_device_find(name);
    if(dwin_uart_device == RT_NULL)
    {
        dwin_println("%s device dont found", name);
        return dwin_err_error;
    }
    
    /* open uart device */
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
        /* set rx callback func */
        rt_device_set_rx_indicate(dwin_uart_device, dwin_uart_rxcb);
    }
    
    /* create uart rxsem */
    dwin_uart_rxsem = rt_sem_create("dwinrxsem", 0, RT_IPC_FLAG_FIFO);
    if(dwin_uart_rxsem == RT_NULL)
    {
        rt_device_set_rx_indicate(dwin_uart_device, NULL);
        rt_device_close(dwin_uart_device);
        dwin_uart_device = RT_NULL;
        dwin_println("dwin uart rxsem create failed");
        return dwin_err_error;
    }
    
    return dwin_err_none;
}

/* auto upload start */
uint8_t dwin_watch_start(void)
{
    /* create auto upload watch thread */ 
    dwin_uart_watch = rt_thread_create("twatch", dwin_autoupload_watch,  
        RT_NULL, 1024, PKG_DWIN_WATCH_PRIO, 10); 
    if(dwin_uart_watch == RT_NULL) 
    {
        dwin_println("dwin auto upload thread create failed"); 
        return dwin_err_error; 
    }

    /* startup watch thread */ 
    rt_thread_startup(dwin_uart_watch); 
    return dwin_err_none; 
}

/* auto upload stop */
uint8_t dwin_watch_stop(void)
{
    rt_err_t ret = RT_EOK;
    
    ret = rt_thread_delete(dwin_uart_watch);
    if(ret != RT_EOK)
    {
        dwin_println("dwin auto upload thread delete failed"); 
        return dwin_err_error; 
    }
    
    return dwin_err_none;
}

/* dwin var space write api */
uint8_t dwin_var_write(uint16_t addr, uint16_t *data, uint8_t len)
{
    uint8_t index;
    
    RT_ASSERT(data != RT_NULL);
    RT_ASSERT(len  != 0);
    
    dwin_putc(PKG_DWIN_HEAD_H);
    dwin_putc(PKG_DWIN_HEAD_L);
    dwin_putc((len*2) + 3);
    dwin_putc(DWIN_VAR_WRITE);
    dwin_putc((addr & 0xFF00) >> 8);
    dwin_putc((addr & 0x00FF) >> 0);
    
    /* send data to dwin */
    for(index = 0; index < len; index++)
    {
        dwin_putc((data[index] & 0xFF00) >> 8);
        dwin_putc((data[index] & 0x00FF) >> 0);
    }
    
#ifdef DWIN_DEBUG
    {
        uint8_t i;
        
        dwin_print(PKG_DWIN_PROMPT);
        dwin_print("write [0x%.4x] var [%dbyte]:", addr, len*2);
    
        dwin_print("{");
        for(i = 0; i < len; i++)
        {
            dwin_print("0x%.4x ", data[i]);
        }
        dwin_print("\b}\n");
    }
#endif
    
    return dwin_err_none;
}

uint8_t dwin_var_read(uint16_t addr, uint16_t *data, uint8_t len)
{
    uint8_t index = 0;
    uint8_t rx_data[256] = {0};
    uint8_t ret = dwin_err_none;

    /* stop watch thread */
    ret = dwin_watch_stop();
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
    /* send read var request */
    dwin_putc(PKG_DWIN_HEAD_H);
    dwin_putc(PKG_DWIN_HEAD_L);
    dwin_putc(4);
    dwin_putc(DWIN_VAR_READ);
    dwin_putc((addr & 0xFF00) >> 8);
    dwin_putc((addr & 0x00FF) >> 0);
    dwin_putc(len);
    
    for(;;)
    {
        rx_data[index++] = dwin_getc();
        
        if(index == (len*2+7))
        {
            /* rx_data is ok? */
            if((rx_data[0] != PKG_DWIN_HEAD_H)  || (rx_data[1] != PKG_DWIN_HEAD_L)  || 
               (rx_data[2] != (len*2+4))        || (rx_data[3] != DWIN_VAR_READ)    || 
               (rx_data[4] != (addr&0xFF00)>>8) || (rx_data[5] != (addr&0x00FF)>>0) ||
               (rx_data[6] != len))
            {
                ret = dwin_err_error;
                rt_memset(data, 0, len*2+7);
                break;
            }
            
            /* u8 to u16 */
            for(index = 7; index < (len*2+7); index+=2)
            {
                data[(index-7)/2] = (rx_data[index]<<8) + rx_data[index + 1];
            }
            
            ret = dwin_err_none;
            break;
        }
    }
    
    /* read var space debug */
#ifdef DWIN_DEBUG
    dwin_print(PKG_DWIN_PROMPT);
    dwin_print("user readvar [%dByte]:{", len);
    for(index = 0; index < (len*2+7); index++)
    {
        dwin_print("0x%.2x ", rx_data[index]);
    }
    dwin_print("\b}\n");
#endif
    
    /* start watch thread */
    ret = dwin_watch_start();
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
    return dwin_err_none;
}

uint8_t dwin_reg_write(uint8_t addr, uint8_t *data, uint8_t len)
{
    uint8_t index;
    
    RT_ASSERT(data != RT_NULL);
    RT_ASSERT(len  != 0);
    
    dwin_putc(PKG_DWIN_HEAD_H);
    dwin_putc(PKG_DWIN_HEAD_L);
    dwin_putc(len + 2);
    dwin_putc(DWIN_REG_WRITE);
    dwin_putc(addr);
    
    /* send data to dwin */
    for(index = 0; index < len; index++)
    {
        dwin_putc(data[index]);
    }
    
#ifdef DWIN_DEBUG
    {
        uint8_t i;
        
        dwin_print(PKG_DWIN_PROMPT);
        dwin_print("write [0x%.2x] reg [%dbyte]:", addr, len);
    
        dwin_print("{");
        for(i = 0; i < len; i++)
        {
            dwin_print("0x%.2x ", data[i]);
        }
        dwin_print("\b}\n");
    }
#endif
    
    return dwin_err_none;
}

uint8_t dwin_reg_read(uint8_t addr, uint8_t *data, uint8_t len)
{
    uint8_t index = 0;
    uint8_t rx_data[256] = {0};
    uint8_t ret = dwin_err_none;
    
    /* stop watch thread */
    ret = dwin_watch_stop();
    if(ret != dwin_err_none)
    {
        return dwin_err_error; 
    }
    
    /* send read var request */
    dwin_putc(PKG_DWIN_HEAD_H);
    dwin_putc(PKG_DWIN_HEAD_L);
    dwin_putc(3);
    dwin_putc(DWIN_REG_READ);
    dwin_putc(addr);
    dwin_putc(len);
    
    for(;;)
    {
        rx_data[index++] = dwin_getc();
        
        if(index == (len+6))
        {
            /* rx_data is ok? */
            if((rx_data[0] != PKG_DWIN_HEAD_H) || (rx_data[1] != PKG_DWIN_HEAD_L) || 
               (rx_data[2] != (len+3))         || (rx_data[3] != DWIN_REG_READ)   || 
               (rx_data[4] != addr)            || (rx_data[5] != len))
            {
                ret = dwin_err_error;
                rt_memset(rx_data, 0, len+6);
                break;
            }
            
            rt_memcpy(data, &rx_data[6], len);
            ret = dwin_err_none;
            break;
        }
    }
    
    /* read var space debug */
#ifdef DWIN_DEBUG
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
