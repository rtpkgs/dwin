/*
 * @File:   dwin_obj.h 
 * @Author: liu2guang 
 * @Date:   2018-06-06 17:27:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-06     liu2guang    implementing the object. 
 */ 

#ifndef __DWIN_OBJ_H__ 
#define __DWIN_OBJ_H__ 

#include "dwin_def.h" 

void dwin_obj_init(struct dwin_obj *obj, rt_uint16_t addr, rt_uint8_t size, enum dwin_obj_type type); 
rt_err_t dwin_obj_set_active(struct dwin_obj *obj, rt_bool_t active); 

#endif 
