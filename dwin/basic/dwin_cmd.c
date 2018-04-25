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
#include "dwin_utils.h" 
#include "finsh.h" 

/* dwin msh cmd */
static void dwin(uint8_t argc, char **argv) 
{
    const char *help_info[] = 
    {
        [0] = "  -t     read write reg and var debug cmd.", 
        [1] = "  -s     debug dwin system function.", 
    }; 
    
    if(argc < 2)
    {
        size_t index = 0;
        
        DWIN_USING_PRINT("\033[32musage: dwin [-t] [-s] \033[0m\n"); 
        DWIN_USING_PRINT("optional arguments: \n"); 
        for(index = 0; index < (sizeof(help_info)/sizeof(char*)); index++)
        {
            DWIN_USING_PRINT("%s\n", help_info[index]);
        }
    }
    else
    {
        if(!strcmp(argv[1], "-t")) 
        {
            if((!strcmp(argv[2], "r")) && (!strcmp(argv[3], "reg")))
            {
                rt_uint16_t addr = (rt_uint16_t)dwin_utils_convert(argv[4]); 
                DWIN_INFO("User read \033[32m%d\033[0m byte(s) from \033[32m0x%.4x\033[0m reg: \n", atoi(argv[5]), addr); 
                
                rt_uint8_t data[256] = {0}; 
                dwin_reg_read(addr, data, atoi(argv[5])); 
            }
            else if((!strcmp(argv[2], "r")) && (!strcmp(argv[3], "var")))
            {
                rt_uint16_t addr = (rt_uint16_t)dwin_utils_convert(argv[4]); 
                DWIN_INFO("User read \033[32m%d\033[0m byte(s) from \033[32m0x%.4x\033[0m var: \n", atoi(argv[5]), addr); 
                
                rt_uint16_t data[128] = {0}; 
                dwin_var_read(addr, data, atoi(argv[5])); 
            }
            else if((!strcmp(argv[2], "w")) && (!strcmp(argv[3], "reg")))
            {
                rt_uint16_t addr = (rt_uint16_t)dwin_utils_convert(argv[4]); 
                DWIN_INFO("User write \033[32m%d\033[0m byte(s) from \033[32m0x%.4x\033[0m reg: \n", atoi(argv[5]), addr); 
                
                rt_uint8_t index = 0; 
                rt_uint8_t data[256] = {0}; 
                
                for(index = 0; index < atoi(argv[5]); index++)
                {
                    data[index] = atoi(argv[6+index]);
                }
                
                dwin_reg_write(addr, data, atoi(argv[5])); 
            }
            else if((!strcmp(argv[2], "w")) && (!strcmp(argv[3], "var")))
            {
                rt_uint16_t addr = (rt_uint16_t)dwin_utils_convert(argv[4]); 
                DWIN_INFO("User write \033[32m%d\033[0m byte(s) from \033[32m0x%.4x\033[0m var: \n", atoi(argv[5]), addr); 
                
                rt_uint8_t index = 0; 
                rt_uint16_t data[128] = {0}; 
                
                for(index = 0; index < atoi(argv[5]); index++)
                {
                    data[index] = atoi(argv[6+index]);
                }
                
                dwin_var_write(addr, data, atoi(argv[5])); 
            }
            else
            {
                DWIN_USING_PRINT("\033[31mdwin: error: usage: dwin -t r/w reg/var [addr] [len]. \033[0m\n"); 
                return; 
            }
        }
        if(!strcmp(argv[1], "-s"))
        {
            if(!strcmp(argv[2], "cpu"))
            {
                rt_uint16_t data[4] = {0}; 
                
                dwin_system_get_cpuid(data); 
                
                #ifndef DWIN_USING_DEBUG
                DWIN_INFO("Get dwin serial screen cpu id: \033[32m0x%.4x%.4x%.4x%.4x\033[0m.\n", 
                    data[0], data[1], data[2], data[3]); 
                #endif
            }
            else if(!strcmp(argv[2], "rst"))
            {
                dwin_system_reset(); 
                #ifndef DWIN_USING_DEBUG
                DWIN_INFO("Reset dwin succeed.\n"); 
                #endif
            }
            else if(!strcmp(argv[2], "ver"))
            {
                rt_uint16_t data[2] = {0}; 
                
                dwin_system_get_version(data); 
                #ifndef DWIN_USING_DEBUG
                DWIN_INFO("Get dwin version: \033[32m GUI:v%d.%d, DWIN-OS: v%d.%d(0x%.4x)\033[0m.\n", 
                    gui_ver/10, gui_ver%10, dwinos_ver/10, dwinos_ver%10, *ver); 
                #endif
            }
            else if(!strcmp(argv[2], "rtc"))
            {
                rt_uint16_t data[4] = {0}; 
                
                dwin_system_get_rtc(data); 
                #ifndef DWIN_USING_DEBUG
                DWIN_INFO("Get dwin rtc: \033[32m20%.2d-%.2d-%.2d %.2d:%.2d:%.2d week %d\033[0m.\n", 
                    DWIN_GET_BYTEH(rtc[0]), DWIN_GET_BYTEL(rtc[0]), DWIN_GET_BYTEH(rtc[1]), 
                    DWIN_GET_BYTEH(rtc[2]), DWIN_GET_BYTEL(rtc[2]), DWIN_GET_BYTEH(rtc[3]),
                    DWIN_GET_BYTEL(rtc[1])); 
                #endif
            }
            else if(!strcmp(argv[2], "page"))
            {
                rt_uint16_t data = 0; 
                
                dwin_system_get_page(&data); 
                #ifndef DWIN_USING_DEBUG
                DWIN_INFO("Get dwin page: \033[32m%.4d\033[0m.\n", data); 
                #endif
            }
            else
            {
                DWIN_USING_PRINT("\033[31mdwin: error: usage: dwin -s [system func] <...> \033[0m\n"); 
                DWIN_USING_PRINT("system func arguments: \n"); 
                DWIN_USING_PRINT("  cpu    get dwin cpu id.\n"); 
                DWIN_USING_PRINT("  rst    reset dwin display.\n"); 
                DWIN_USING_PRINT("  ver    get dwin gui and dwin-os version.\n"); 
                DWIN_USING_PRINT("  rtc    get dwin rtc time info.\n"); 
                DWIN_USING_PRINT("  page   get dwin current page id.\n"); 
                return; 
            }
        }
        else
        {
            DWIN_USING_PRINT("\033[32musage: dwin [-t] [-s] \033[0m\n");
            DWIN_USING_PRINT("dwin: error: unrecognized arguments: \033[31m%s\033[0m. \n", argv[1]); 
            return; 
        }
    }
}
MSH_CMD_EXPORT(dwin, The dwin develop and debug toolkit);
