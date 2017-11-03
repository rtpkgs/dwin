#include "dw_space_manager.h"
#include "string.h"

/* 空闲空间首地址 */
static uint16_t sm_idle_addr = 0x0000;

/* 已经分配的空间管理链表头 */
static list_t *sm_used_head_list = NULL;

/* 初始化变量空间管理器 */
bool dw_plugin_sm_init(void)
{
    /* 初始化空闲空间 */
    sm_idle_addr = 0x0000;
    
    sm_used_head_list = list_new();
    if(sm_used_head_list == NULL)
    {
        return false;
    }
    
    return true;
}

/* 申请n字节的变量空间, 并命名 */
dw_space_block_t dw_plugin_sm_apply(const char *name, uint32_t size)
{
    list_node_t *node = NULL;
    dw_space_block_t space = NULL;
    
    /* 判断参数 */
    if(name == NULL || size == 0)
    {
        return NULL;
    }
    
    /* 计算空闲内存大小 */
    if(dw_plugin_sm_idle_space() < size)
    {
        return NULL;
    }
    
    /* 分配内存 */
    space = DW_MALLOC(sizeof(struct dw_space_block));
    
    /* 拷贝名称 */
    if(strcpy(space->name, name) == NULL)
    {
        return NULL;
    }
    
    /* 获取申请的空间大小与地址 */
    space->size = size;
    space->addr = sm_idle_addr;
    
    /* 添加内存结构体指针插入到链表中 */
    node = list_node_new((void *)space);
    list_rpush(sm_used_head_list, node);
    
    /* 计算新的空间空闲地址 */
    sm_idle_addr += size;
    
    return space;
}

/* 获取空间剩余大小 */
uint16_t dw_plugin_sm_idle_space(void)
{
    return (PKG_DWLIB_SM_SPACE_MAXSIZE - sm_idle_addr);
}

/* 打印已经使用的内存 */
void dw_plugin_sm_used_space(void)
{
    list_node_t *node = NULL;
    list_iterator_t *iterator = NULL;
    
    if(sm_used_head_list == NULL)
    {
        dw_print("no used space");
        return;
    }
    
    iterator = list_iterator_new(sm_used_head_list, LIST_HEAD);
    
    while((node = list_iterator_next(iterator)) != NULL)
    {
        dw_print("<space name> = %s <addr> = 0x%.4x <size> = %d", 
            ((struct dw_space_block *)(node->val))->name, 
            ((struct dw_space_block *)(node->val))->addr, 
            ((struct dw_space_block *)(node->val))->size);
    }
    
    dw_print("current space addr = 0x%.4x, used space size = %dByte, free space size = %dByte", 
        sm_idle_addr, sm_idle_addr, dw_plugin_sm_idle_space());
    
    list_iterator_destroy(iterator);
}

/* 下面的带准备搬移到finsh命令插件中, 空间管理插件不需要clist以外的外部库依赖 */
#include "finsh.h"
MSH_CMD_EXPORT_ALIAS(dw_plugin_sm_used_space, sm, print used space);
FINSH_FUNCTION_EXPORT_ALIAS(dw_plugin_sm_used_space, sm, print used space);
