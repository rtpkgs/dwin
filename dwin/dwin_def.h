/*/
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

/* dwin lib version information */
#define DWIN_VERSION_M    2L /* main   version */
#define DWIN_VERSION_S    0L /* sub    version */
#define DWIN_VERSION_R    0L /* revise version */
#define DWIN_VERSION      ((DWIN_VERSION_M * 10000) + (DWIN_VERSION_S * 100) + DWIN_VERSION_R)

/* Default config */ 
#ifndef DWIN_USING_BAUDRATE
#define DWIN_USING_BAUDRATE 115200
#endif

#ifndef DWIN_USING_HEADH
#define DWIN_USING_HEADH    0x5A
#endif
#ifndef DWIN_USING_HEADL
#define DWIN_USING_HEADL    0xA5
#endif

#ifndef DWIN_USING_DATA_FRAME_DEPTH
#define DWIN_USING_DATA_FRAME_DEPTH 8 
#endif 

#define DWIN_USING_NUM_MAX_PER_PAGE 64 /* 64 or 128 */ 

#if   (DWIN_USING_TYPE == 0)
#define DWIN_VAR_BASE_ADDR (0x0000)
#elif (DWIN_USING_TYPE == 1)
#define DWIN_VAR_BASE_ADDR (0x0000)
#elif (DWIN_USING_TYPE == 2)
#define DWIN_VAR_BASE_ADDR (0x1000)
#endif 

#define DWIN_VAR_ADDR(addr) (DWIN_VAR_BASE_ADDR+(addr))

#define DWIN_GET_BYTEH(short)  ((rt_uint8_t)(((short) & 0xFF00) >> 8))
#define DWIN_GET_BYTEL(short)  ((rt_uint8_t)(((short) & 0x00FF) >> 0))
#define DWIN_SET_SHORT(b1, b2) ((rt_uint16_t)(((b1)<<8) | ((b2)&0xff)))

#define DWIN_GET_SHORTH(int)   ((rt_uint16_t)(((int) & 0xFFFF0000) >> 16))
#define DWIN_GET_SHORTL(int)   ((rt_uint16_t)(((int) & 0x0000FFFF) >>  0))
#define DWIN_SET_INT(b1, b2)   ((rt_uint32_t)(((b1)<<16) | ((b2)&0xffff)))

/* 将char字符串转换成short字符串 */ 
rt_inline rt_uint16_t *dwin_string_conv(rt_uint8_t *string) 
{
    rt_uint8_t temp  = 0; 
    rt_uint8_t index = 0;
    
    RT_ASSERT(string != RT_NULL); 
    
    for(index = 0; index < rt_strlen((const char *)string); index+=2)
    {
        temp            = string[index+0]; 
        string[index+0] = string[index+1];
        string[index+1] = temp;
    }
    
    return (rt_uint16_t *)string; 
}

/* 调试信息 */ 
#define DWIN_PRINT(fmt, ...)              \
do{                                       \
    rt_kprintf(fmt, ##__VA_ARGS__);       \
}while(0)

#define DWIN_INFO(fmt, ...)               \
do{                                       \
    rt_kprintf("[\033[32mdwin\033[0m] "); \
    rt_kprintf(fmt, ##__VA_ARGS__);       \
}while(0)

#ifndef DWIN_USING_DEBUG
#define DWIN_DBG(fmt, ...) 
#define DWIN_START(bool, fmt, ...) 
#else
#define DWIN_DBG(fmt, ...)                \
do{                                       \
    rt_kprintf("[\033[32mdwin\033[0m] "); \
    rt_kprintf(fmt, ##__VA_ARGS__);       \
}while(0)
#define DWIN_START(bool, fmt, ...)                 \
do{                                                \
    rt_kprintf("[\033[32mdwin\033[0m] ");          \
    rt_kprintf(fmt, ##__VA_ARGS__);                \
    if(bool == RT_FALSE)                           \
        rt_kprintf("\t\t[\033[32mFail\033[0m]\n"); \
    else                                           \
        rt_kprintf("\t\t[\033[32mOK\033[0m]\n");   \
}while(0)
#endif

#define DWIN_OBJ_VARY_LENGHT (0xFF) 

enum dwin_dir
{
    DWIN_DIR_000 = 0, 
    DWIN_DIR_090, 
    DWIN_DIR_180,
    DWIN_DIR_270
}; 
typedef enum dwin_dir dwin_dir_t; 

enum dwin_obj_type
{
    DWIN_WIDGET_TYPE_BUTTON = 0, 
    DWIN_WIDGET_TYPE_NUM,
    DWIN_WIDGET_TYPE_NUM_INPUT, 
    DWIN_WIDGET_TYPE_GBK, 
    DWIN_WIDGET_TYPE_GBK_INPUT, 
    DWIN_WIDGET_TYPE_SCALE, 
    DWIN_WIDGET_TYPE_ICON, 
    DWIN_WIDGET_TYPE_QRCODE, 
    DWIN_WIDGET_TYPE_MAX
}; 
typedef enum dwin_obj_type dwin_obj_type_t; 

#define DWIN_WIDGET_TYPE_INFO                  \
{                                              \
    [DWIN_WIDGET_TYPE_BUTTON   ] = "Button  ", \
    [DWIN_WIDGET_TYPE_NUM      ] = "Number  ", \
    [DWIN_WIDGET_TYPE_NUM_INPUT] = "NumInput", \
    [DWIN_WIDGET_TYPE_GBK      ] = "GBK     ", \
    [DWIN_WIDGET_TYPE_GBK_INPUT] = "GBKInput", \
    [DWIN_WIDGET_TYPE_SCALE    ] = "Scale   ", \
    [DWIN_WIDGET_TYPE_ICON     ] = "Icon    ", \
    [DWIN_WIDGET_TYPE_QRCODE   ] = "QRCode  "  \
}; 

struct dwin_rtc
{
    rt_uint16_t year; 
    rt_uint8_t  month; 
    rt_uint8_t  day; 
    
    rt_uint8_t  hour; 
    rt_uint8_t  minute; 
    rt_uint8_t  second; 
    
    rt_uint8_t  week;
}; 
typedef struct dwin_rtc *dwin_rtc_t; 

struct dwin_data_frame
{
    rt_uint8_t data[256]; 
    rt_uint8_t len; 
}; 
typedef struct dwin_data_frame* dwin_data_frame_t; 

struct dwin_watch
{
    rt_device_t serial; 
    rt_uint32_t baudrate; 
    rt_sem_t rxsem; 
    rt_thread_t thread; 
    struct dwin_data_frame data; 
}; 
typedef struct dwin_watch *dwin_watch_t; 

struct dwin_obj
{
    rt_list_t list; 
    
    enum dwin_obj_type type; 
    
    rt_uint16_t value_addr; 
    rt_uint16_t value_size; 
    
    rt_uint8_t  active; 
}; 
typedef struct dwin_obj* dwin_obj_t; 

struct dwin_page
{
    rt_list_t list; 
    
    rt_list_t objs; 
    rt_uint16_t id; 
}; 
typedef struct dwin_page* dwin_page_t; 

struct dwin_parse
{
    rt_list_t list;
    
    enum dwin_obj_type type; 
    
    void (*event)(struct dwin_obj *obj, uint8_t *data, uint8_t len); 
}; 
typedef struct dwin_parse* dwin_parse_t; 
    
struct dwin
{
    rt_bool_t init; 
    
    /* 页面相关 */ 
    rt_list_t   pages;          /* 页面链表 */ 
    rt_uint16_t page_num;       /* 页面数量 */ 
    struct dwin_page* page_cur; /* 当前页面 */ 
    
    /* 监听器相关 */ 
    dwin_watch_t watch; 
    
    /* 控件解析器链表 */ 
    rt_list_t parses;           /* 解析器链表 */ 
    rt_uint8_t parse_num;       /* 解析器数量 */ 
    rt_mq_t parse_mq;           /* 解析器消息队列 */ 
    rt_thread_t parse_thread;   /* 解析器线程 */ 
}; 
typedef struct dwin *dwin_t; 

extern struct dwin dwin; 

#endif
