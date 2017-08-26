#ifndef __DW_DEFINE_H_
#define __DW_DEFINE_H_

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 默认时差:东8区 */
#ifndef RT_DW_TIME_DIFFEREBCE
#define RT_DW_TIME_DIFFEREBCE (8)
#endif

/* 临界代码区 */
#define RT_DW_CRITICAL_START    rt_enter_critical
#define RT_DW_CRITICAL_END      rt_exit_critical

/* 位操作宏 */
#define RT_DW_SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define RT_DW_CLR_BIT(REG, BIT)     ((REG) &= ~(BIT))
#define RT_DW_READ_BIT(REG, BIT)    ((REG) & (BIT))

/* 使能宏 */
#define RT_DW_DISABLE (0)
#define RT_DW_ENABLE  (1)

/* 迪文屏幕状态 */
typedef enum
{
	RT_DW_STATE_UNINIT = 0,
	RT_DW_STATE_INIT,
}rt_dw_state_t;

/* 迪文屏幕底层读写数据接口 */
struct rt_dw_ops
{
    rt_err_t (*read) (rt_device_t device, char *c);
	rt_err_t (*write)(rt_device_t device, char *c);
};
typedef struct rt_dw_ops* rt_dw_ops_t;

/* 迪文设备句柄结构体 */
struct rt_device_dw
{
    rt_device_t parent;     /* 迪文屏幕句柄 */

    rt_dw_state_t state;    /* 迪文屏幕的状态 */
    rt_dw_ops_t ops;  		/* 迪文屏幕底层操作函数指针 */
};
typedef struct rt_device_dw* rt_device_dw_t;

#ifdef __cplusplus
}
#endif

#endif
