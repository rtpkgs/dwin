#ifndef __DWIN_PLUGIN_ICON_H_ 
#define __DWIN_PLUGIN_ICON_H_ 

#include "dwin_space.h" 

typedef struct dwin_icon
{
    uint8_t state;
    uint16_t current_index;
    uint16_t min_index;
    uint16_t max_index;
} *dwin_icon_t; 

enum icon_state
{
    icon_stop = 0,    /* 停止模式 */
    icon_start,       /* 正常模式 */
    icon_autoplay     /* 自动播放模式 */
};

uint8_t dwin_plugin_icon_create(const char *name, uint16_t min, uint16_t max, uint16_t current);
uint8_t dwin_plugin_icon_mode(const char *name, uint8_t mode);
uint8_t dwin_plugin_icon_update(const char *name, uint16_t current);
uint16_t dwin_plugin_icon_read(const char *name);

#endif
