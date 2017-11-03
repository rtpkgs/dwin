#ifndef __SW_CONF_H_ 
#define __SW_CONF_H_ 

#include "dw_port.h"
#include "rtthread.h"

/* 内存分配 */
#if !defined(DW_MALLOC) 
#define DW_MALLOC malloc
#endif

#if !defined(DW_FREE) 
#define DW_FREE free
#endif

/* dwlib库空间管理插件 */
#if defined(PKG_DWLIB_ENABLE_SM) 

/* 空间变量名 */
#if !defined(PKG_DWLIB_SM_NAME_LENGTH) 
    #define PKG_DWLIB_SM_NAME_LENGTH (8)
#endif
    
/* 空间变量最大地址(字节) */
#if !defined(PKG_DWLIB_SM_SPACE_MAXSIZE) 
    #define PKG_DWLIB_SM_SPACE_MAXSIZE (4096)
#endif

#endif

#endif
