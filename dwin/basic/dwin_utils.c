/*
 * @File:   dwin_utils.c 
 * @Author: liu2guang 
 * @Date:   2018-04-22 14:52:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-04-22     liu2guang    update v2 framework. 
 */ 
 
#include "dwin_utils.h" 
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

/* converts dec and hex strings into int types */ 
rt_uint32_t dwin_utils_convert(const char *str)
{
    int num = 0; 
    
    if(strchr(str, 'x') == RT_NULL && strchr(str, 'X') == RT_NULL)
    {
        num = atoi(str); 
    }
    else
    {
        sscanf(str, "%x", &num); 
    }
    
    return num; 
}

rt_uint8_t dwin_utils_bcd2dec(rt_uint8_t bcd)
{
    return bcd-(bcd>>4)*6;
}

rt_uint8_t dwin_utils_dec2bcd(rt_uint8_t dec)
{
    return dec+(dec/10)*6;
}
