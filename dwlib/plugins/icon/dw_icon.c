#include "dw_icon.h"
#include "dw_system.h"
#include "string.h"

bool dw_plugin_icon_init(void)
{
    return true;
}

/* 创建icon */
dw_icon_t dw_plugin_icon_create(const char *name, uint8_t num, uint8_t init_icon)
{
    dw_icon_t icon = (dw_icon_t)DW_MALLOC(sizeof(struct dw_icon));
    if(icon == NULL)
    {
        return NULL;
    }
    
    icon->icon = dw_plugin_sm_apply(name, 1);
    if(icon->icon == NULL)
    {
        DW_FREE(icon);
        return NULL;
    }
    
    /* 按键链表 */
    icon->list = list_new();
    if(icon->list == NULL)
    {
        DW_FREE(icon);
        return NULL;
    }
    
    return icon;
}

/* 按指针添加图标 */
/* 按名称添加图标 */
bool dw_plugin_icon_add(dw_icon_t icon, const char *name, uint16_t match_value)
{
    list_node_t *node = NULL;
    dw_icon_index_t icon_index = NULL;
    
    /* 分配内存 */
    icon_index = DW_MALLOC(sizeof(struct dw_icon_index));
    if(icon_index == NULL)
    {
        return false;
    }
    
    /* 拷贝名称 */
    if(strcpy(icon_index->name, name) == NULL)
    {
        DW_FREE(icon_index);
        return false;
    }
    
    icon_index->match_value = match_value;
    
    /* 添加内存结构体指针插入到链表中 */
    node = list_node_new((void *)icon_index);
    list_rpush(icon->list, node);
    
    return true;
}

/* 按名称切换图标 */
/* 按编号切换图标 */
bool dw_plugin_icon_change(dw_icon_t icon, uint16_t value)
{
    dw_write_val(icon->icon->addr, &value, 1);
    
    return true;
}

