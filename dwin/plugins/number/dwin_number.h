#ifndef __DWIN_NUMBER_H_ 
#define __DWIN_NUMBER_H_ 

#include "dwin_space.h" 

typedef struct dwin_number 
{
    uint32_t number; 
} *dwin_number_t; 

#define DWIN_NUMBER_1TO2(n) ((n) << 16)
#define DWIN_NUMBER_2TO1(n) ((n) >> 16)

dwin_number_t dwin_number_new(const char *name, uint32_t number); 
rt_err_t dwin_number_write(const char *name, uint32_t number); 
uint32_t dwin_number_read(const char *name);

#endif
