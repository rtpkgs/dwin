#ifndef __DWIN_TRANS_H_ 
#define __DWIN_TRANS_H_ 

#include "dwin_def.h" 

/* 自动上传监视器初始化 */
uint8_t dwin_watch_init (const char *name, uint32_t bandrate);

/* 寄存器与变量读写函数 */
uint8_t dwin_var_write  (uint16_t addr, uint16_t *data, uint8_t len);
uint8_t dwin_var_read   (uint16_t addr, uint16_t *data, uint8_t len);
uint8_t dwin_reg_write  (uint8_t  addr, uint8_t  *data, uint8_t len);
uint8_t dwin_reg_read   (uint8_t  addr, uint8_t  *data, uint8_t len);

#endif
