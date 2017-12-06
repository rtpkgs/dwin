#ifndef __DWIN_TRANSPORT_H_ 
#define __DWIN_TRANSPORT_H_ 

#include "dwin_def.h" 

/* var/reg rw api */ 
uint8_t dwin_var_write(uint16_t addr, uint16_t *data, uint8_t len, uint32_t timeout);
uint8_t dwin_var_read(uint16_t addr, uint16_t *data, uint8_t len, uint32_t timeout);
uint8_t dwin_reg_write(uint8_t addr, uint8_t *data, uint8_t len, uint32_t timeout);
uint8_t dwin_reg_read(uint8_t addr, uint8_t *data, uint8_t len, uint32_t timeout);

/* auto upload watch thread api */
uint8_t dwin_watch_init(const char *name, uint32_t bandrate);
uint8_t dwin_watch_start(void);
uint8_t dwin_watch_stop(void);

#endif
