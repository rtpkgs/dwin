#ifndef __SW_SPACE_MANGER_H_ 
#define __SW_SPACE_MANGER_H_ 

#include "dw_conf.h"

typedef struct dw_space_block
{
    uint16_t addr;
    uint32_t size;
    
    char name[PKG_DWLIB_SM_NAME_LENGTH];
} *dw_space_block_t;

bool dw_plugin_sm_init(void);
dw_space_block_t dw_plugin_sm_apply(const char *name, uint32_t size);
uint16_t dw_plugin_sm_idle_space(void);
void dw_plugin_sm_used_space(void);

#endif 
