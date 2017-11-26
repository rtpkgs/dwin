#ifndef __DW_COMMON_H_
#define __DW_COMMON_H_

/* suppurt bool type */
#include <stdbool.h>

/* suppurt uintx_t type */
#include <stdint.h>

/* suppurt clib list */
#include "list.h"
#if !defined(LIST_VERSION)
    #error dwlib need clib list suppurt. 
    #error plases download from 'https://github.com/liu2guang/clist/archive/v1.0.0.zip'
#endif

#define DW_VERSION      1L
#define DW_SUBVERSION   1L
#define DW_REVISION     0L

/* Ö¸Áî */
#define DW_REG_W_CMD    (0x80)
#define DW_REG_R_CMD    (0x81)
#define DW_VAL_W_CMD    (0x82)
#define DW_VAL_R_CMD    (0x83)

/* Î»²Ù×÷ */
#define DW_BITx(n) (1 << ((n)-1))
#define DW_BITS_8  (0xFF)
#define DW_BITS_16 (0xFFFF)
#define DW_BITS_32 (0xFFFFFFFF) 

#define DW_BITS_SET(data, bits)  ((data) |=  (bits)) 
#define DW_BITS_CLR(data, bits)  ((data) &= ~(bits)) 
#define DW_BITS_TGL(data, bits)  ((data) ^=  (bits)) 
#define DW_BITS_READ(data, bits) ((data) &   (bits))

#endif
