/*
 * @File:   dwin_system.c 
 * @Author: liu2guang 
 * @Date:   2018-04-26 00:12:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-04-26     liu2guang    update v2 framework. 
 */ 
 
#include "dwin_system.h" 
#include "dwin_trans.h" 

static rt_uint8_t bcd2dec(rt_uint8_t bcd)
{
    return bcd-(bcd>>4)*6;
}

static rt_uint8_t dec2bcd(rt_uint8_t dec)
{
    return dec+(dec/10)*6;
}
