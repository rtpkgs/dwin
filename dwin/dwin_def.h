#ifndef __DWIN_DEF_H_ 
#define __DWIN_DEF_H_ 

#include "rtthread.h"

#define DWIN_DEBUG

/* dwin config */
/* transport framehead high byte */
#ifndef PKG_DWIN_HEAD_H
#define PKG_DWIN_HEAD_H (0x5A)
#endif
#ifndef PKG_DWIN_HEAD_L
#define PKG_DWIN_HEAD_L (0xA5)
#endif

/* transport framehead low byte */
#ifndef PKG_DWIN_VAR_MAX_BYTE
#define PKG_DWIN_VAR_MAX_BYTE   (4096)
#endif

/* dwin prompt */
#ifndef PKG_DWIN_PROMPT
#define PKG_DWIN_PROMPT     "[dwin]"
#endif

/* dwin watch thread priority */
#ifndef PKG_DWIN_WATCH_PRIO
#define PKG_DWIN_WATCH_PRIO (10)
#endif

/* dwin macro */
/* read and write reg/var cmd */
#define DWIN_REG_READ   (0x80) 
#define DWIN_REG_WRITE  (0x81) 
#define DWIN_VAR_READ   (0x82) 
#define DWIN_VAR_WRITE  (0x83) 

/* dwin debug */
#ifndef DWIN_DEBUG
#define dwin_print(...) 
#define dwin_println(...) 
#else
#define dwin_print(...)             \
{                                   \
    rt_kprintf(__VA_ARGS__);        \
}
#define dwin_println(...)           \
{                                   \
    rt_kprintf(PKG_DWIN_PROMPT);    \
    rt_kprintf(__VA_ARGS__);        \
    rt_kprintf("\n");               \
}
#endif

/* type define */
/* error code */
typedef enum{
    dwin_err_none = 0, 
    dwin_err_error, 
    dwin_err_timeout
} dwin_err_t;

#endif
