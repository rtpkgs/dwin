#include "dw_login.h"

/* 登陆插件内部控制句柄 */
rt_dw_login_t login_head = RT_DW_LOGIN_DEFAULT;

/* 登陆插件监控线程 */
void login_poll_thread_entry(void *p)
{
	rt_device_dw_t device = RT_NULL;
	rt_dw_login_password_t *password_temp = RT_NULL;
	
	rt_uint16_t rx_data[2];
	rt_uint16_t tx_data[2];
	rt_uint32_t password = 0;
	
	device = (rt_device_dw_t)p;
	password_temp = login_head.root_password;
	
    while(1)
    {
__poll_start:		
		
		/* 读取登陆退出标志位地址的键值 */
		rt_dw_read_var_space(device, login_head.login_exit_flag_addr, 1, rx_data);
		
		/* 判断是否是登陆标志(代表屏幕上登陆事件发生) */
		if(rx_data[0] == login_head.login_flag_value)
		{
			/* 将登陆事件清除 */
			tx_data[0] = 0xFFFF;	
			rt_dw_write_var_space(device, login_head.login_exit_flag_addr, 1, tx_data);
			
			/* 读取密码 */
			rt_dw_read_var_space(device, login_head.password_addr, 2, rx_data);
			password = rx_data[0]*65536 + rx_data[1];
			
			/* 打印密码便于调试 */
			rt_kprintf("\nuser input password = %d\n", password);
			
			/* 遍历密码链表 */
			do
			{
				if(password_temp->password == password)
				{
					/* 清除密码 */
					tx_data[0] = 0;
					tx_data[1] = 0;
					rt_dw_write_var_space(device, login_head.password_addr, 2, tx_data);					
					
					/* 密码正确调用回调含食宿 */
					login_head.login_ok(device);
									
					/* 进入下一次轮询循环 */
					goto __poll_start;
				}
				
				/* 指向下一个密码 */
				password_temp = (rt_dw_login_password_t *)(password_temp->list.next);
				
			}while(password_temp != login_head.root_password);	/* 判断是否遍历完成 */
			
			/* 清除密码 */
			tx_data[0] = 0;
			tx_data[1] = 0;
			rt_dw_write_var_space(device, login_head.password_addr, 2, tx_data);			
			
			/* 所以密码都错误, 判定为错误 */
			login_head.login_error(device);
		}
		
		/* 判断是否是退出标志(代表屏幕上退出事件发生) */
		if(rx_data[0] == login_head.exit_flag_value)
		{
			/* 将登陆事件清除 */
			tx_data[0] = 0xFFFF;	
			rt_dw_write_var_space(device, login_head.login_exit_flag_addr, 1, tx_data);

			login_head.exit(device);
		}
		
        rt_thread_delay(RT_TICK_PER_SECOND/100);
    }
}

/* 添加用户密码 */
rt_err_t rt_dw_plugins_login_add(rt_uint32_t password)
{
	rt_dw_login_password_t *user_password = RT_NULL;
	
	/* 分配根密码内存 */
    user_password = (rt_dw_login_password_t *)rt_malloc(sizeof(rt_dw_login_password_t));
	
	if(user_password == RT_NULL)
	{
		/* 内存分配失败 */
		return RT_ENOMEM;
	}
	
	/* 初始化链表 */
	rt_list_init(&(user_password->list));
	
	/* 尾插入密码链表 */
	rt_list_insert_before(&(login_head.root_password->list), &(user_password->list));
	
	/* 添加密码 */
	user_password->password = password;
	
	return RT_EOK;
}

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
	void (*exit)(rt_device_dw_t device))
{
	rt_uint16_t tx_data[2];
    rt_thread_t login_poll = RT_NULL;
	rt_dw_login_password_t *root = RT_NULL;

	RT_ASSERT(login_ok != RT_NULL);
	RT_ASSERT(login_error != RT_NULL);
	RT_ASSERT(exit != RT_NULL);

    /* 注册密码地址 */
    login_head.password_addr = password_addr;
	
	/* 清空密码 */
	tx_data[0] = 0;
	tx_data[1] = 0;
	rt_dw_write_var_space(device, login_head.password_addr, 2, tx_data);

    /* 注册登陆退出标志位地址 */
    login_head.login_exit_flag_addr = login_exit_flag_addr;
	
	/* 清空密码标志位 */
	tx_data[0] = 0xFFFF;
	rt_dw_write_var_space(device, login_head.login_exit_flag_addr, 1, tx_data);

    /* 注册登陆退出键值 */
    login_head.login_flag_value = login_flag_value;
    login_head.exit_flag_value  = exit_flag_value;
	
	/* 注册回调函数 */
	login_head.login_ok    = login_ok;
	login_head.login_error = login_error;
	login_head.exit        = exit;

	/* 分配根密码内存 */
    root = (rt_dw_login_password_t *)rt_malloc(sizeof(rt_dw_login_password_t));
	login_head.root_password = root;
	
	/* 添加根密码 */
	login_head.root_password->password = root_password;
	
	/* 初始化链表 */
	rt_list_init(&(login_head.root_password->list));

	/* 启动登陆轮询函数 */
    login_poll = rt_thread_create(
        "login poll", login_poll_thread_entry, 
        (void *)device, 512, 4, 10);

    if(login_poll != RT_NULL)
    {
        rt_thread_startup(login_poll);
    }

    return RT_EOK;
}
