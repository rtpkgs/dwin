#include "dw_vicon.h"

#if defined(RT_USING_FINSH) && defined(RT_USING_DW_DEBUG)
#include <finsh.h>
#endif

/* 添加变量图标 */
rt_dw_vicon_t rt_dw_plugins_vicon_add(rt_device_dw_t device, 
	rt_uint16_t addr, rt_uint16_t default_value, 
    rt_uint16_t min_value, rt_uint16_t max_value)
{
    rt_err_t ret = RT_EOK;
    rt_dw_vicon_t vicon_temp = RT_NULL;

    /* 判断默认值是否超过最大小值 */
    if((default_value < min_value) || (default_value > max_value))
    {
        rt_kprintf("value > max_value or value < min_value\n");
        return RT_NULL;
    }

    /* 创建变量图标 */
    vicon_temp = (rt_dw_vicon_t)rt_malloc(sizeof(struct rt_dw_vicon));

    /* 内存分配失败 */
    if(vicon_temp == RT_NULL)
    {
        rt_kprintf("create vicon failed:addr = 0x.4%x, value = %d\n", addr, default_value);
        return RT_NULL;
    }
    
    vicon_temp->addr      = addr;
    vicon_temp->max_value = max_value;
    vicon_temp->min_value = min_value;

    /* 写变量空间 */
    ret = rt_dw_write_var_space(device, vicon_temp->addr, 1, &default_value);

    if(ret != RT_EOK)
    {
        /* 写变量空间失败 */
        return RT_NULL;
    }

    return vicon_temp;
}

/* 光标图标 */
rt_err_t rt_dw_plugins_vicon_change(rt_device_dw_t device, rt_dw_vicon_t vicon, rt_uint16_t value)
{
    rt_err_t ret = RT_EOK;

    /* 判断默认值是否超过最大小值 */
    if((value < vicon->min_value) || (value > vicon->max_value))
    {
        rt_kprintf("value > max_value or value < min_value\n");
        return RT_ERROR;
    }

    /* 写变量空间 */
    ret = rt_dw_write_var_space(device, vicon->addr, 1, &value);

    if(ret != RT_EOK)
    {
        /* 写变量空间失败 */
        return ret;
    }
	
	return RT_EOK;
}
