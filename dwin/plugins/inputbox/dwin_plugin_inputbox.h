#ifndef __DWIN_PLUGIN_INPUTBOX_H_ 
#define __DWIN_PLUGIN_INPUTBOX_H_ 

#include "dwin_space.h" 

typedef struct dwin_inputbox
{
    uint8_t state;
    int32_t input_value;
    void (*inputbox_cb)(int32_t input_value, void *args);
    void *args;
} *dwin_inputbox_t; 

/* 按下回调函数 */
typedef void (*inputbox_cb)(int32_t input_value, void *args);

/* 按键模式 */
enum inputbox_state
{
    inputbox_stop = 0,    /* 停止模式 */
    inputbox_start        /* 输入模式 */
};

/* 外部函数 */
uint8_t dwin_plugin_inputbox_create(const char *name, inputbox_cb cb, void *args);
uint8_t dwin_plugin_inputbox_mode(const char *name, uint8_t mode);
uint8_t dwin_plugin_inputbox_update(const char *name, inputbox_cb cb, void *args);
int32_t dwin_plugin_inputbox_read(const char *name);

#endif
