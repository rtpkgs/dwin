#ifndef __DW_PORT_H_
#define __DW_PORT_H_

/* dwlib common header file */
#include "dw_common.h" 

/* Port API */
bool dw_port_serial_init(const char *name, uint32_t baudrate);
bool dw_port_serial_putc(char  byte);
bool dw_port_serial_getc(char *byte);
bool dw_port_serial_resource_init(void);
bool dw_port_serial_resource_take(int32_t timeout);
bool dw_port_serial_resource_release(void);

#endif
