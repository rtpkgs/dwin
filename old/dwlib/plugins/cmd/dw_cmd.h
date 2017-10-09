#ifndef __DW_CMD_H_
#define __DW_CMD_H_

#include "rtthread.h"
#include "dw_define.h"

/* 系统插件 */
#ifdef RT_USING_DW_SYSTEM
#include "dw_system.h"
#endif

/* 登陆插件 */
#ifdef RT_USING_DW_LOGIN
#include "dw_login.h"
#endif

/* 变量图标插件 */
#ifdef  RT_USING_DW_VICON
#include "dw_vicon.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* 注册dwinlib cmd插件使用的设备句柄 */
rt_uint8_t rt_dw_plugins_cmd_init(rt_device_dw_t device);

#ifdef __cplusplus
}
#endif

#endif
