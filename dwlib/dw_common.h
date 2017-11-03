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

#endif
