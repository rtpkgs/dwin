#ifndef __DW_ICON_H_ 
#define __DW_ICON_H_ 

#include "dw_common.h"
#include "dw_conf.h"
#include "dw_space_manager.h" 

typedef struct dw_icon_index
{
    uint16_t match_value;
    char name[PKG_DWLIB_ICON_NAME_LENGTH];
} *dw_icon_index_t;

typedef struct dw_icon
{
    dw_space_block_t icon;
    list_t *list;
} *dw_icon_t;

bool dw_plugin_icon_init(void);
dw_icon_t dw_plugin_icon_create(const char *name, uint8_t num, uint8_t init_icon); 
bool dw_plugin_icon_add(dw_icon_t icon, const char *name, uint16_t match_value);
bool dw_plugin_icon_change(dw_icon_t icon, uint16_t value);

#endif
