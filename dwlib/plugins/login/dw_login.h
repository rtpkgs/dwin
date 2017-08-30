#ifndef __DW_LOGIN_H_
#define __DW_LOGIN_H_

#include "dw_space.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#define RT_DW_LOGIN_DEFAULT {0xFFFF, RT_NULL}

/* 密码结点 */
struct rt_dw_login_password
{
	rt_list_t list;
	
	rt_uint32_t password;
};
typedef struct rt_dw_login_password rt_dw_login_password_t;

/* 登陆插件句柄 */
struct rt_dw_login
{
     /* 登陆密码地址, 注意该插件要求存储密码为4字节 */
    rt_uint16_t password_addr;

    /* 登陆状态地址, 登陆标志值, 退出标志值 */
    rt_uint16_t login_exit_flag_addr;
    rt_uint16_t login_flag_value;
    rt_uint16_t exit_flag_value;
	
	/* 登陆与退出回调函数 */
	void (*login_ok)(rt_device_dw_t device);
	void (*login_error)(rt_device_dw_t device);
	void (*exit)(rt_device_dw_t device);

    /* 密码链表 */
    rt_dw_login_password_t *root_password; 
};
typedef struct rt_dw_login rt_dw_login_t;

/* 初始化登陆插件 */
int rt_dw_plugins_login_init(
	rt_device_dw_t device,
    rt_uint16_t password_addr, 
    rt_uint16_t login_exit_flag_addr, 
    rt_uint16_t login_flag_value, 
    rt_uint16_t exit_flag_value,
    rt_uint32_t root_password,
    void (*login_ok)(rt_device_dw_t device),
    void (*login_error)(rt_device_dw_t device),
    void (*exit)(rt_device_dw_t device));

/* 添加用户密码 */
rt_err_t rt_dw_plugins_login_add(rt_uint32_t password);

#ifdef __cplusplus
}
#endif

#endif
