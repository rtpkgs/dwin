#ifndef __DWIN_INIT_H_
#define __DWIN_INIT_H_

#include "dwin_def.h"

/* 外部调用接口 */
bool dwin_get_init_state(void); 
uint8_t dwin_init(const char *uart, uint32_t baudrate); 

#endif
