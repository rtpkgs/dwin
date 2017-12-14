#ifndef __DWIN_PLUGIN_TEXTBOX_H_ 
#define __DWIN_PLUGIN_TEXTBOX_H_ 

#include "dwin_space.h" 

typedef struct dwin_textbox 
{
    uint8_t len;
    uint8_t *text; 
} *dwin_textbox_t;

uint8_t dwin_plugin_textbox_create(const char *name, uint8_t *text, uint8_t len); 
uint8_t dwin_plugin_textbox_update(const char *name, uint8_t *text, uint8_t len); 

#endif
