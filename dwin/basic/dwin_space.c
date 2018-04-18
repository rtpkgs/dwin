/*
 * @File:   dwin_space.c 
 * @Author: liu2guang 
 * @Date:   2017-12-08 15:24:29 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-08     liu2guang    实现空间管理器, 用于导出配置参数. 
 */ 
#include "list.h"
#include "dwin_space.h" 

/* 空间管理器链表 */
list_t *dwin_space_list;

/* 变量空间空闲地址 */
static uint16_t idle_addr = 0x0000;

/* 空间管理器初始化函数 */
uint8_t dwin_space_init(void)
{
    static bool init_state = false;
    
    /* 判断空间管理是否初始化 */
    if(init_state == true)
    {
        return dwin_err_reinit;
    }
    
    /* 空闲地址初始化 */
    idle_addr = 0x0000;
    
    /* 初始化空间管理器链表 */
    dwin_space_list = list_new();
    if(dwin_space_list == RT_NULL)
    {
        dwin_println("Dwin space manger list new failed");
        return dwin_err_error;
    }
    
    /* 空间管理器初始化完毕 */
    init_state = true;
    
    return dwin_err_none;
}

/* 分配空间变量 */
dwin_space_t dwin_space_alloc(const char *name, uint16_t len, uint8_t type)
{
    list_node_t *node  = RT_NULL;
    dwin_space_t space = RT_NULL;
    
    RT_ASSERT(name != RT_NULL);
    RT_ASSERT(len  != 0);
    RT_ASSERT(type <= DWIN_TYPE_MAX);
    
    /* 检查空间是否足够 */
    if((len*2) > dwin_space_idle())
    {
        dwin_println("Dwin is no space, idle = ", dwin_space_idle());
        return RT_NULL;
    }
    
    /* 分配内存 */
    space = (dwin_space_t)rt_malloc(sizeof(struct dwin_space));
    if(space == RT_NULL)
    {
        dwin_println("Space malloc failed");
        return RT_NULL;
    }
    
    /* 填充结构体 */
    rt_strncpy((char *)space->name, name, PKG_DWIN_SPACE_NAME_LEN);
    space->type   = type;
    space->plugin = RT_NULL;
    space->len    = len;
    space->addr   = idle_addr;
    
    /* 更新地址 */
    idle_addr += len*2;
    
    /* 插入链表管理 */
    node = list_node_new((void *)space);
    list_rpush(dwin_space_list, node);
    
    return space;
}

/* 导出空间变量 */
#if (PKG_DWIN_DEBUG >= 1)
#define DWIN_CSI_RED_STA "\033[31;1m"   /* 红色加粗 */
#define DWIN_CSI_GRE_STA "\033[32;1m"   /* 绿色加粗 */
#define DWIN_CSI_END     "\033[0m"      

void dwin_space_foreach(void)
{
    uint8_t index = 0;
    list_node_t *node = RT_NULL;
    list_iterator_t *iterator = RT_NULL; 
    dwin_space_t space = RT_NULL;
    
    if(dwin_space_list == RT_NULL)
    {
        dwin_println("No used space");
        return;
    }
    
    iterator = list_iterator_new(dwin_space_list, LIST_HEAD);
    
    dwin_print(PKG_DWIN_PROMPT);
    
    dwin_print(DWIN_CSI_GRE_STA); dwin_print("Space Output Info: "); dwin_print(DWIN_CSI_END);
    dwin_print("AllSpace ");
    dwin_print(DWIN_CSI_GRE_STA); dwin_print("%dbyte ", PKG_DWIN_VAR_MAX_BYTE); dwin_print(DWIN_CSI_END);
    dwin_print("UsedSpace ");
    dwin_print(DWIN_CSI_GRE_STA); dwin_print("%dbyte.\n", idle_addr); dwin_print(DWIN_CSI_END);

    dwin_print("------------ -------- ---------- -------- ------\n");
    dwin_print(DWIN_CSI_RED_STA); dwin_print("SpaceName"); 
    for(index = 0; index < (PKG_DWIN_SPACE_NAME_LEN-rt_strlen("SpaceName")); index++)
    {
        dwin_print(" ");
    }
    dwin_print(" Addr     Length     Type     Remark\n");
    dwin_print(DWIN_CSI_END);
    dwin_print("------------ -------- ---------- -------- ------\n");
    
    while((node = list_iterator_next(iterator)) != RT_NULL)
    {
        space = (dwin_space_t)(node->val);
        
        /* 导出空间名称(不够最大长度插入空格补空), 地址, 长度 */
        dwin_print("%s", space->name);
        for(index = 0; index < (PKG_DWIN_SPACE_NAME_LEN-rt_strlen((const char *)(space->name))); index++)
        {
            dwin_print(" ");
        }
        dwin_print(" 0x%.4x   %.6d     ", space->addr, space->len);
        
        /* 打印差异化信息 */
        switch(space->type)
        {
            /* button */
#ifdef PKG_DWIN_ENABLE_PLUGIN_BUTTON
            case DWIN_TYPE_BTN:
            {
                dwin_button_t button = (dwin_button_t)(space->plugin);
                
                dwin_print("button   ");
                dwin_print("0x%.4x(match)", button->match_value);
                
                switch(button->state)
                {
                    case button_press:
                        dwin_print(", start(state)\n");
                    break;
                    
                    case button_stop:
                        dwin_print(", stop(state)\n");
                    break;
                }
            }
            break;
#endif
            
#ifdef PKG_DWIN_ENABLE_PLUGIN_INPUTBOX
            case DWIN_TYPE_INPUTBOX:
            {
                dwin_inputbox_t inputbox = (dwin_inputbox_t)(space->plugin);
                
                dwin_print("inputbox ");
                dwin_print("%d(input)", inputbox->input_value);
                
                switch(inputbox->state)
                {
                    case inputbox_start:
                        dwin_print(", start(state)\n");
                    break;
                    
                    case button_stop:
                        dwin_print(", stop(state)\n");
                    break;
                }
            }
            break;
#endif
            
#ifdef PKG_DWIN_ENABLE_PLUGIN_ICON
            case DWIN_TYPE_ICON:
            {
                dwin_icon_t icon = (dwin_icon_t)(space->plugin);
                
                dwin_print("icon     ");
                dwin_print("%d-%d-%d(min-cur-max)", icon->min_index, icon->cur_index, icon->max_index);
                
                switch(icon->state)
                {
                    case icon_start:
                        dwin_print(", start(state)\n");
                    break;
                    
                    case icon_stop:
                        dwin_print(", stop(state)\n");
                    break;
                    
                    case icon_autoplay:
                        dwin_print(", autoplay(state)\n");
                    break;
                }
            }
            break;
#endif
            
#ifdef PKG_DWIN_ENABLE_PLUGIN_TEXTBOX
            case DWIN_TYPE_TEXTBOX:
            {
                uint8_t index = 0;
                dwin_textbox_t textbox = (dwin_textbox_t)(space->plugin);
                
                dwin_print("TextBox  ");
                //dwin_print("%s , GBK:0x", textbox->text);
                dwin_print("GBK:0x");
                for(index = 0; index < textbox->len; index++)
                {
                    dwin_print("%.2x", textbox->text[index]);
                }
                dwin_print("\n");
            }
            break;
#endif
            case DWIN_TYPE_NUMBER: 
            {
                dwin_number_t handle = (dwin_number_t)(space->plugin); 
                dwin_print("Number   ");
                dwin_print("%d(show)\n", handle->number);
            }
            default:
            break;
        }
    }
    
    list_iterator_destroy(iterator);
}

#include "finsh.h"
MSH_CMD_EXPORT_ALIAS(dwin_space_foreach, dwin, print used space info);
FINSH_FUNCTION_EXPORT_ALIAS(dwin_space_foreach, dwin, print used space info);
#endif

/* 获取空间剩余大小 */
uint16_t dwin_space_idle(void)
{
    return (PKG_DWIN_VAR_MAX_BYTE - idle_addr);
}

/* 通过名称获取空间变量结构体 */
dwin_space_t dwin_space_find(const char *name)
{
    dwin_space_t space;
    list_node_t *node = RT_NULL;
    list_iterator_t *iterator = RT_NULL;
    
    iterator = list_iterator_new(dwin_space_list, LIST_HEAD);
    
    while((node = list_iterator_next(iterator)) != RT_NULL)
    {
        space = (dwin_space_t)(node->val);
        
        /* 匹配名称 */
        if(rt_strcmp((const char *)space->name, name) == 0)
        {
            list_iterator_destroy(iterator);
            return space;
        }
    }
    
#if (PKG_DWIN_DEBUG >= 2)
    dwin_println("Not find space %s", name);
#endif
    
    list_iterator_destroy(iterator);
    
    return RT_NULL;
}
