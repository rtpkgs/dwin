#ifndef __DW_INIT_H_
#define __DW_INIT_H_

#include "dw_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 注册迪文屏幕设备,目前只支持普通发送, 中断接收的串口设备 */
rt_err_t rt_device_dw_init(rt_device_dw_t device, const char *name, rt_uint8_t init_bl);

#ifdef __cplusplus
}
#endif

#endif
