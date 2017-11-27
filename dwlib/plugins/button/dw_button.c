#include "dw_button.h" 
#include "dw_system.h" 
#include "dw_space_manager.h" 
#include "string.h"

static dw_space_block_t btn_space = NULL;

/* 已经分配的空间管理链表头 */
static list_t *btn_head_list = NULL;

/* 初始化变量空间管理器 */
bool dw_plugin_button_init(void)
{
    /* 申请2字节空间用于按键插件 */
    btn_space = dw_plugin_sm_apply("btn_plugin", 1);
    if(btn_space == NULL)
    {
        dw_print("button space apply failed");
        return false;
    }
    
    /* 按键链表 */
    btn_head_list = list_new();
    if(btn_head_list == NULL)
    {
        return false;
    }
    
    return true;
}

/* 注册按键 */ 
bool dw_plugin_button_add(const char *name, uint16_t match_value, btn_callback func, void *args)
{
    list_node_t *node = NULL;
    dw_button_t btn = NULL;
    
    /* 判断参数 */
    if(name == NULL)
    {
        return NULL;
    }
    
    /* 分配内存 */
    btn = DW_MALLOC(sizeof(struct dw_button));
    if(btn == NULL)
    {
        return false;
    }
    
    /* 拷贝名称 */
    if(strcpy(btn->name, name) == NULL)
    {
        return false;
    }
    
    /* 注册回调函数和匹配值 */
    btn->func        = func;
    btn->args        = NULL;
    btn->match_value = match_value;
    
    /* 添加内存结构体指针插入到链表中 */
    node = list_node_new((void *)btn);
    list_rpush(btn_head_list, node);
    
    return true;
}

/* 打印已经注册的按键 */
void dw_plugin_button_used_btn(void)
{
    uint16_t count = 0;
    list_node_t *node = NULL;
    list_iterator_t *iterator = NULL;
    
    if(btn_head_list == NULL)
    {
        dw_print("no used btn");
        return;
    }
    
    iterator = list_iterator_new(btn_head_list, LIST_HEAD);
    
    while((node = list_iterator_next(iterator)) != NULL)
    {
        count++;
        dw_print("<btn name> = %s <match_value> = 0x%.4x", 
            ((struct dw_button *)(node->val))->name, 
            ((struct dw_button *)(node->val))->match_value);
    }
    
    dw_print("current button add count = %d", count);
    
    list_iterator_destroy(iterator);
}
/* 下面的带准备搬移到finsh命令插件中, 按键插件不需要clist以外的外部库依赖 */
#include "finsh.h"
MSH_CMD_EXPORT_ALIAS(dw_plugin_button_used_btn, smb, print used button);
FINSH_FUNCTION_EXPORT_ALIAS(dw_plugin_button_used_btn, smb, print used button);

void dw_plugin_button_event_poll(void)
{
    uint16_t val = 0x0000;
    list_node_t *node = NULL;
    list_iterator_t *iterator = NULL;
    
    if(btn_head_list == NULL)
    {
        dw_print("no used btn");
        return;
    }
    
    /* 读取当前变量值 */
    dw_read_val(btn_space->addr, &val, 1);
    
    iterator = list_iterator_new(btn_head_list, LIST_HEAD);
    while((node = list_iterator_next(iterator)) != NULL)
    {
        if(((struct dw_button *)(node->val))->match_value == val && 
           ((struct dw_button *)(node->val))->func != NULL)
        {
            ((struct dw_button *)(node->val))->func(((struct dw_button *)(node->val))->args);
            
            val = 0x0000;
            dw_write_val(btn_space->addr, &val, 1);
        }
        
    }
    
    list_iterator_destroy(iterator);
}
