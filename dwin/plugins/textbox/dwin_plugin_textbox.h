#ifndef __DWIN_TEXTBOX_H_ 
#define __DWIN_TEXTBOX_H_ 

#include "dwin_space.h" 

typedef struct dwin_textbox 
{
    uint8_t len;
    uint8_t *text; 
} *dwin_textbox_t; 

enum dwin_textbox_type
{
    TEXTBOX_TYPE_NORMAL = 0, /* 直接显示 */
    TEXTBOX_TYPE_STEP2STEP,  /* 步进显示 */
    TEXTBOX_TYPE_SCROLL      /* 滚动显示 */
};

uint8_t dwin_textbox_create(const char *name, uint8_t *string, uint8_t length, uint8_t mode, uint32_t time); 
uint8_t dwin_textbox_update(const char *name, uint8_t *string, uint8_t length); 

#endif
