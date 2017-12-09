#include "dwin_space.h" 
#include "stdbool.h" 
#include "list.h"

/* plugin include */
#include "dwin_plugin_button.h" 
#include "dwin_plugin_inputbox.h" 
#include "dwin_plugin_icon.h" 

/* static data interface */
list_t *dwin_space_list;
static uint16_t idle_addr = 0x0000;

/* extern api */
/* alloc dwin space */
uint8_t dwin_space_init(void)
{
    static bool init_state = false;
    
    /* uninitialized */
    if(init_state == true)
    {
        return dwin_err_none;
    }
    
    /* new dwin spae list */
    dwin_space_list = list_new();
    if(dwin_space_list == RT_NULL)
    {
        dwin_println("Dwin space manger list new failed");
        return dwin_err_error;
    }
    
    init_state = true;
    
    return dwin_err_none;
}

/* alloc dwin space */
dwin_space_t dwin_space_alloc(const char *name, uint16_t len, uint8_t type)
{
    list_node_t *node  = RT_NULL;
    dwin_space_t space = RT_NULL;
    
    RT_ASSERT(name != RT_NULL);
    RT_ASSERT(len  != 0);
    RT_ASSERT(type <= dwin_type_qrcode);
    
    /* Check if space is sufficient */
    if((len*2) > dwin_space_idle())
    {
        dwin_println("Dwin is no space, idle = ", dwin_space_idle());
        return RT_NULL;
    }
    
    /* allocate memory */
    space = (dwin_space_t)rt_malloc(sizeof(struct dwin_space));
    if(space == RT_NULL)
    {
        dwin_println("Space malloc failed");
        return RT_NULL;
    }
    
    /* fill in struct */
    rt_strncpy((char *)space->name, name, PKG_DWIN_SPACE_NAME_LEN);
    space->type   = type;
    space->plugin = RT_NULL;
    space->len    = len;
    space->addr   = idle_addr;
    
    /* change idle addr */
    idle_addr += len*2;
    
    /* insert the list */
    node = list_node_new((void *)space);
    list_rpush(dwin_space_list, node);
    
    return space;
}

/* for each used space */
#ifdef DWIN_DEBUG
void dwin_space_foreach(void)
{
    list_node_t *node = RT_NULL;
    list_iterator_t *iterator = RT_NULL;
    
    if(dwin_space_list == RT_NULL)
    {
        dwin_println("No used space");
        return;
    }
    
    iterator = list_iterator_new(dwin_space_list, LIST_HEAD);
    
    dwin_print(PKG_DWIN_PROMPT);
    dwin_print("Used space %dbyte, idle space %dbyte:\n", idle_addr, dwin_space_idle());
    
    while((node = list_iterator_next(iterator)) != RT_NULL)
    {
        dwin_print("name=%s,addr=0x%.4x,len=%.6d", 
            ((dwin_space_t)(node->val))->name, 
            ((dwin_space_t)(node->val))->addr, 
            ((dwin_space_t)(node->val))->len);
        
        /* 完善插件后需要对应插件打印对应信息 */
        switch(((dwin_space_t)(node->val))->type)
        {
            /* button */
            case dwin_type_button:
            {
                dwin_print(",type=btn,matchval=0x%.4x", 
                    ((dwin_button_t)(((dwin_space_t)(node->val))->plugin))->match_value);
                
                switch(((dwin_button_t)(((dwin_space_t)(node->val))->plugin))->state)
                {
                    case button_press:
                        dwin_print(",state=press\n");
                    break;
                    
                    case button_stop:
                        dwin_print(",state=stop\n");
                    break;
                }
            }
            break;
            
            case dwin_type_inputbox:
            {
                dwin_print(",type=inputbox,input=%d", 
                    ((dwin_inputbox_t)(((dwin_space_t)(node->val))->plugin))->input_value);
                
                switch(((dwin_inputbox_t)(((dwin_space_t)(node->val))->plugin))->state)
                {
                    case inputbox_start:
                        dwin_print(",state=start\n");
                    break;
                    
                    case button_stop:
                        dwin_print(",state=stop\n");
                    break;
                }
            }
            break;
            
            case dwin_type_icon:
            {
                dwin_print(",type=icon,min=%d,max=%d,cur=%d", 
                    ((dwin_icon_t)(((dwin_space_t)(node->val))->plugin))->min_index,
                    ((dwin_icon_t)(((dwin_space_t)(node->val))->plugin))->max_index,
                    ((dwin_icon_t)(((dwin_space_t)(node->val))->plugin))->current_index);
                
                switch(((dwin_icon_t)(((dwin_space_t)(node->val))->plugin))->state)
                {
                    case icon_start:
                        dwin_print(",state=start\n");
                    break;
                    
                    case icon_stop:
                        dwin_print(",state=stop\n");
                    break;
                    
                    case icon_autoplay:
                        dwin_print(",state=autoplay\n");
                    break;
                }
            }
            break;
            
            default:
            break;
        }
    }
    
    list_iterator_destroy(iterator);
}

#include "finsh.h"
MSH_CMD_EXPORT_ALIAS(dwin_space_foreach, dsf, print used space info);
FINSH_FUNCTION_EXPORT_ALIAS(dwin_space_foreach, dsf, print used space info);
#endif

/* get dwin space idle size */
uint16_t dwin_space_idle(void)
{
    return (PKG_DWIN_VAR_MAX_BYTE - idle_addr);
}

void *dwin_space_find(const char *name)
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
    
#ifdef DWIN_DEBUG
    dwin_println("Not find space %s", name);
#endif
    
    list_iterator_destroy(iterator);
    
    return RT_NULL;
}
