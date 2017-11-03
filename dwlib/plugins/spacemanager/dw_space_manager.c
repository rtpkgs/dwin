#include "dw_space_manager.h"
#include "string.h"

/* 空闲空间首地址 */
static uint16_t sm_idle_addr = 0x0000;

/* 初始化变量空间管理器 */
bool dw_plugin_sm_init(void)
{
    /* 初始化空闲空间 */
    sm_idle_addr = 0x0000;
    
    return true;
}

/* 申请n字节的变量空间, 并命名 */
dw_space_block_t dw_plugin_sm_apply(const char *name, uint32_t size)
{
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
    
    /* 计算新的空间空闲地址 */
    sm_idle_addr += size;
    
    return space;
}

/* 获取空间剩余大小 */
uint16_t dw_plugin_sm_idle_space(void)
{
    return (PKG_DWLIB_SM_SPACE_MAXSIZE - sm_idle_addr);
}
