#ifndef __DWIN_DEF_H_ 
#define __DWIN_DEF_H_ 

#include "dwin_cfg.h"
#include "stdint.h"

#define DWIN_DATA_MAX_BYTE  (246)

/* 错误码 */
typedef enum
{
    dwin_err_none = 0,
    dwin_err_error,
    dwin_err_timeout,
    dwin_err_nosys,
    dwin_err_epara
} dwin_err_t; 

/* 寄存器数据帧 */
struct dwin_reg
{
    uint8_t head1;
    uint8_t head2;
    uint8_t length;
    uint8_t cmd;
    uint8_t data[DWIN_DATA_MAX_BYTE];
}; 
typedef union 
{
    struct dwin_reg reg;
    uint8_t sendbuffer[sizeof(struct dwin_reg)]; 
} *dwin_reg_t;

/* 变量数据帧 */
struct dwin_var
{
    uint8_t head1;
    uint8_t head2;
    uint8_t length;
    uint8_t cmd;
    uint16_t data[DWIN_DATA_MAX_BYTE/2];
};
typedef union 
{
    struct dwin_var var;
    uint8_t sendbuffer[sizeof(struct dwin_var)]; 
} *dwin_var_t;

/* 断言 */
#define dwin_assert(ex)                                                        \
{                                                                              \
    if(!(ex))                                                                  \
    {                                                                          \
        dwin_print("(%s,%s,%d) assert fail\n", #ex, __FUNCTION__, __LINE__);   \
    }                                                                          \
}

#endif
