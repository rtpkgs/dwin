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
#include "finsh.h" 

/* dwin msh cmd */
static void dwin(uint8_t argc, char **argv) 
{
    const char *help_info[] = 
    {
        [0] = "  -t     read write reg and var debug cmd.", 
    }; 
    
    if(argc < 2)
    {
        size_t index = 0;
        
        DWIN_USING_PRINT("\033[32musage: dwin [-t] \033[0m\n"); 
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
                DWIN_INFO("User read \033[32m%d\033[0m byte(s) from \033[32m0x%.4x\033[0m reg: \n", atoi(argv[5]), atoi(argv[4])); 
                
                rt_uint8_t data[256] = {0}; 
                dwin_reg_read(atoi(argv[4]), data, atoi(argv[5])); 
            }
            else if((!strcmp(argv[2], "r")) && (!strcmp(argv[3], "var")))
            {
                DWIN_INFO("User read \033[32m%d\033[0m byte(s) from \033[32m0x%.4x\033[0m var: \n", atoi(argv[5]), atoi(argv[4])); 
                
                rt_uint16_t data[128] = {0}; 
                dwin_var_read(atoi(argv[4]), data, atoi(argv[5])); 
            }
            else if((!strcmp(argv[2], "w")) && (!strcmp(argv[3], "reg")))
            {
                DWIN_INFO("User write \033[32m%d\033[0m byte(s) from \033[32m0x%.4x\033[0m reg: \n", atoi(argv[5]), atoi(argv[4])); 
                
                rt_uint8_t index = 0; 
                rt_uint8_t data[256] = {0}; 
                
                for(index = 0; index < atoi(argv[5]); index++)
                {
                    data[index] = atoi(argv[6+index]);
                }
                
                dwin_reg_write(atoi(argv[4]), data, atoi(argv[5])); 
            }
            else if((!strcmp(argv[2], "w")) && (!strcmp(argv[3], "var")))
            {
                DWIN_INFO("User write \033[32m%d\033[0m byte(s) from \033[32m0x%.4x\033[0m var: \n", atoi(argv[5]), atoi(argv[4])); 
                
                rt_uint8_t index = 0; 
                rt_uint16_t data[128] = {0}; 
                
                for(index = 0; index < atoi(argv[5]); index++)
                {
                    data[index] = atoi(argv[6+index]);
                }
                
                dwin_var_write(atoi(argv[4]), data, atoi(argv[5])); 
            }
            else
            {
                DWIN_USING_PRINT("\033[31mdwin: error: usage: dwin -t r/w reg/var [addr] [len]. \033[0m\n"); 
                return; 
            }
        }
        else
        {
            DWIN_USING_PRINT("\033[32musage: dwin [-t] \033[0m\n");
            DWIN_USING_PRINT("dwin: error: unrecognized arguments: \033[31m%s\033[0m. \n", argv[1]); 
            return; 
        }
    }
}
MSH_CMD_EXPORT(dwin, The dwin develop and debug toolkit);
