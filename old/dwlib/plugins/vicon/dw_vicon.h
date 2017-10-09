#ifndef __DW_VICON_H_
#define __DW_VICON_H_

#include "dw_space.h"

#ifdef __cplusplus
extern "C" {
#endif

struct rt_dw_vicon
{
    rt_uint16_t addr;

    rt_uint16_t max_value;
    rt_uint16_t min_value;
};
typedef struct rt_dw_vicon* rt_dw_vicon_t;

/* 添加变量图标 */
rt_dw_vicon_t rt_dw_plugins_vicon_add(rt_device_dw_t device, 
	rt_uint16_t addr, rt_uint16_t default_value, 
    rt_uint16_t min_value, rt_uint16_t max_value);

/* 光标图标 */
rt_err_t rt_dw_plugins_vicon_change(rt_device_dw_t device, rt_dw_vicon_t vicon, rt_uint16_t value);

#ifdef __cplusplus
}
#endif

#endif
