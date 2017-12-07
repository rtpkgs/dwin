#ifndef __DWIN_PLUGIN_BUTTON_H_ 
#define __DWIN_PLUGIN_BUTTON_H_ 

#include "dwin_space.h" 

/* type define */
typedef struct dwin_button
{
    uint8_t state;
    uint16_t match_value;
    void (*press_cb)(void *args);
    void *args;
} *dwin_button_t; 

/* button press callback func */
typedef void (*press_cb)(void *args);

/* startup and stop */
enum button_state
{
    button_state_startup = 0,
    button_state_stop
};

/* extern api */
uint8_t dwin_plugin_button_init(uint16_t addr);
uint8_t dwin_plugin_button_create(const char *name, press_cb cb, void *args);
uint8_t dwin_plugin_button_startup(const char*name);
uint8_t dwin_plugin_button_pause(const char*name);
uint8_t dwin_plugin_button_update(const char*name, press_cb cb, void *p);

#endif
