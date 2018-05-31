/*
 * @File:   dwin_cmd.c 
 * @Author: liu2guang 
 * @Date:   2018-04-22 14:52:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-04-24     liu2guang    update v2 framework. 
 */ 
 
#include "dwin_cmd.h" 
#include "dwin_trans.h" 
#include "dwin_system.h" 
#include "finsh.h" 

static rt_uint32_t str2int(const char *str)
{
    int num = 0; 
    
    if(!strstr(str, "0x") && !strstr(str, "0X")) 
    {
        num = atoi(str); 
    }
    else
    {
        sscanf(str, "%x", &num); 
    }
    
    return num; 
} 

static void uasge(void)
{
    DWIN_PRINT("\033[36mUsage: dwin [options] [arg...] \033[0m\n\n"); 
    DWIN_PRINT("optional arg: \n"); 
    DWIN_PRINT("  -t    read/write dwin var/reg.\n"); 
    DWIN_PRINT("  -s    debug dwin system function.\n"); 
}

static void uasge_t(void)
{
    DWIN_PRINT("\033[31mERR, \033[0mformat: \033[36mdwin -t <r|w> <reg|var> <addr> <len> [data...]\033[0m.\n"); 
}

static void uasge_s(void)
{
    DWIN_PRINT("\033[31mERR, \033[0mformat: \033[36mdwin -s \033[0m.\n"); 
}

/* 只有开启调试模式, 才有该命令 */ 
static int dwin_cmd(uint8_t argc, char **argv) 
{
    if(argc < 2)
    {
        uasge(); 
        return RT_EOK; 
    }
    else
    {
        if(!strcmp(argv[1], "-t") || !strcmp(argv[1], "--transfer")) 
        {
            /* 读寄存器 */ 
            if((!strcmp(argv[2], "r")) && (!strcmp(argv[3], "reg")) && (argc >= 6))
            {
                rt_uint8_t data[256] = {0}; 
                rt_uint8_t  len  = (rt_uint8_t )str2int(argv[5]); 
                rt_uint16_t addr = (rt_uint16_t)str2int(argv[4]); 
                
                DWIN_DBG("User read \033[32m%d\033[0m byte(s) from \033[32m0x%.4x\033[0m reg: \n", len, addr); 
                dwin_reg_read(addr, data, len); 
                
                return RT_EOK; 
            }
            
            /* 读变量 */ 
            else if((!strcmp(argv[2], "r")) && (!strcmp(argv[3], "var")) && (argc >= 6))
            {
                rt_uint16_t data[128] = {0}; 
                rt_uint8_t  len  = (rt_uint8_t )str2int(argv[5]); 
                rt_uint16_t addr = (rt_uint16_t)str2int(argv[4]); 
                
                DWIN_DBG("User read \033[32m%d\033[0m byte(s) from \033[32m0x%.4x\033[0m var: \n", len, addr); 
                dwin_var_read(addr, data, len); 
                
                return RT_EOK; 
            }
            
            /* 写寄存器 */ 
            else if((!strcmp(argv[2], "w")) && (!strcmp(argv[3], "reg")) && (argc >= 7))
            {
                rt_uint8_t index = 0; 
                rt_uint8_t data[256] = {0}; 
                rt_uint8_t  len  = (rt_uint8_t )str2int(argv[5]); 
                rt_uint16_t addr = (rt_uint16_t)str2int(argv[4]); 
                
                for(index = 0; index < len; index++)
                {
                    data[index] = (rt_uint8_t)str2int(argv[6+index]);
                }
                
                DWIN_DBG("User write \033[32m%d\033[0m byte(s) from \033[32m0x%.4x\033[0m reg: \n", len, addr); 
                dwin_reg_write(addr, data, len); 
                return RT_EOK; 
            }
            
            /* 写变量 */ 
            else if((!strcmp(argv[2], "w")) && (!strcmp(argv[3], "var")) && (argc >= 7))
            {
                rt_uint8_t index = 0; 
                rt_uint16_t data[128] = {0}; 
                rt_uint8_t  len  = (rt_uint8_t )str2int(argv[5]); 
                rt_uint16_t addr = (rt_uint16_t)str2int(argv[4]); 
                
                for(index = 0; index < len; index++)
                {
                    data[index] = (rt_uint16_t)str2int(argv[6+index]);
                }
                
                DWIN_DBG("User write \033[32m%d\033[0m byte(s) from \033[32m0x%.4x\033[0m var: \n", len, addr); 
                dwin_var_write(addr, data, len); 
                return RT_EOK; 
            } 
            
            else
            {
                uasge_t(); 
                return RT_ERROR; 
            }
        }
        
        else if(!strcmp(argv[1], "-s") || !strcmp(argv[1], "--system")) 
        {
//            if()
//            {
//            
//            }
//            
//            else
//            {
//                uasge_s(); 
//                return RT_ERROR; 
//            }
        }
        
        else
        {
            DWIN_PRINT("\033[31mERR options:\033[0m %s\n", argv[1]); 
            uasge(); 
            return RT_ERROR;
        }
    }
    
    return RT_EOK; 
}

MSH_CMD_EXPORT_ALIAS(dwin_cmd, dwin, The dwin develop and debug toolkit);
