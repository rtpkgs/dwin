#ifndef __DW_BUTTON_H_ 
#define __DW_BUTTON_H_ 

#include "dw_common.h"
#include "dw_conf.h"

typedef void (*btn_callback)(void *);

typedef struct dw_button
{
    uint32_t match_value;
    
    btn_callback func;
    void *args;
    
    char name[PKG_DWLIB_BTN_NAME_LENGTH];
} *dw_button_t;

bool dw_plugin_button_init(void); 
bool dw_plugin_button_add(const char *name, uint16_t match_value, btn_callback func, void *args);
void dw_plugin_button_used_btn(void);
void dw_plugin_button_event_poll(void);

#endif
