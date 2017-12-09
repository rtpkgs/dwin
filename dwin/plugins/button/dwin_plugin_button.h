#ifndef __DWIN_PLUGIN_BUTTON_H_ 
#define __DWIN_PLUGIN_BUTTON_H_ 

#include "dwin_space.h" 

/* 按键句柄 */
typedef struct dwin_button
{
    uint8_t state;
    uint16_t match_value;
    void (*press_cb)(void *args);
    void *args;
} *dwin_button_t; 

/* 按下回调函数 */
typedef void (*press_cb)(void *args);

/* 按键模式 */
enum button_state
{
    button_stop = 0,    /* 停止模式 */
    button_press        /* 单击模式 */
};

/* 外部函数 */
uint8_t dwin_plugin_button_create(const char *name, press_cb cb, void *args);
uint8_t dwin_plugin_button_mode(const char*name, uint8_t mode);
uint8_t dwin_plugin_button_update(const char*name, press_cb cb, void *args);

#endif
