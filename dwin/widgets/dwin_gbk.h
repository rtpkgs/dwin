/*
 * @File:   dwin_gbk.h 
 * @Author: liu2guang 
 * @Date:   2018-06-19 14:39:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-19     liu2guang    implement the gbk object. 
 */ 
 
#ifndef __DWIN_GBK_H__ 
#define __DWIN_GBK_H__ 

#include "dwin_def.h" 

struct dwin_gbk
{
    struct dwin_obj obj; 
    rt_uint16_t max_len; 
}; 
typedef struct dwin_gbk* dwin_gbk_t; 

rt_err_t dwin_gbk_init(void); 
struct dwin_gbk *dwin_gbk_create(struct dwin_page *page, rt_uint16_t addr, rt_uint16_t max_len); 
rt_err_t dwin_gbk_delect(struct dwin_gbk *gbk); 
rt_err_t dwin_gbk_show_string(struct dwin_gbk *gbk, char *string); 
rt_err_t dwin_gbk_clear(struct dwin_gbk *gbk); 

#endif 
