#ifndef __DWIN_TRANSPORT_H_ 
#define __DWIN_TRANSPORT_H_ 

#include "dwin_def.h" 

dwin_err_t dwin_var_send(dwin_var_t var, uint32_t timeout);
dwin_err_t dwin_var_recv(dwin_var_t var, uint16_t addr, uint8_t num, uint32_t timeout);
dwin_err_t dwin_reg_send(dwin_reg_t reg, uint32_t timeout);
dwin_err_t dwin_reg_recv(dwin_reg_t reg, uint16_t addr, uint8_t num, uint32_t timeout);
dwin_err_t dwin_autoupload_watch_init(const char *name);

#endif
