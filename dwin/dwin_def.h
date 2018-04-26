/*
 * @File:   dwin_def.h 
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

#ifndef __DWIN_DEF_H__ 
#define __DWIN_DEF_H__ 

#include "rtthread.h"
#include "rtdevice.h" 

/* Default config */ 
#ifndef DWIN_USING_MODEL
#error "Please define 'DWIN_USING_MODEL' macro!" 
#endif

#ifndef DWIN_USING_UART
#error "Please define 'DWIN_USING_UART' macro!" 
#endif

#ifndef DWIN_USING_BAUDRATE
#define DWIN_USING_BAUDRATE 115200
#endif

#ifndef DWIN_USING_HEADH
#define DWIN_USING_HEADH 0x5A
#endif
#ifndef DWIN_USING_HEADL
#define DWIN_USING_HEADL 0xA5
#endif

#ifndef DWIN_USING_PRINT
#define DWIN_USING_PRINT rt_kprintf
#endif

#define DWIN_GET_BYTEH(short) (rt_uint8_t)(((short) & 0xFF00) >> 8)
#define DWIN_GET_BYTEL(short) (rt_uint8_t)(((short) & 0x00FF) >> 0)
#define DWIN_SET_SHORT(b1, b2) (rt_uint16_t)((b1 << 8) | (b2 & 0xff))

/* Debug */ 
#ifndef DWIN_USING_DEBUG
#define DWIN_DBG(fmt, ...) 
#else
#define DWIN_DBG(fmt, ...)                      \
do{                                             \
    DWIN_USING_PRINT("[\033[32mdwin\033[0m] "); \
    DWIN_USING_PRINT(fmt, ##__VA_ARGS__);       \
}while(0)
#endif
    
/* Info */ 
#define DWIN_INFO(fmt, ...)                     \
do{                                             \
    DWIN_USING_PRINT("[\033[32mdwin\033[0m] "); \
    DWIN_USING_PRINT(fmt, ##__VA_ARGS__);       \
}while(0)

enum dwin_watch_state
{
    DWIN_WATCH_STATE_IDLE = 0, 
    DWIN_WATCH_STATE_HEADH, 
    DWIN_WATCH_STATE_HEADL, 
    DWIN_WATCH_STATE_DATE
}; 
typedef enum dwin_watch_state dwin_watch_state_t; 

struct dwin_watch
{
    rt_device_t serial; 
    rt_uint32_t baudrate; 
    rt_sem_t rxsem; 
    rt_thread_t thread; 
    
    uint8_t data[256]; 
}; 
typedef struct dwin_watch *dwin_watch_t; 
    
struct dwin
{
    dwin_watch_t watch; 
    
    rt_bool_t init; 
}; 
typedef struct dwin *dwin_t; 

#endif
