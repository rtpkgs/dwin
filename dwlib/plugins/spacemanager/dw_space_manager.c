#include "dw_space_manager.h"
#include <stdbool.h>
#include <stdint.h>

#if 0
typedef struct
{
    const char name[xxx];
    
    uint16_t addr;
    uint32_t size;
} dw_space_block;
#endif

/* 初始化变量空间管理器 */
bool dw_plugin_sm_init(int32_t space_byte)
{
    return true;
}

/* 申请n字节的变量空间 */
bool dw_plugin_sm_apply(uint32_t size)
{
    return true;
}

/* 释放n字节的变量空间 */
// 暂时没有这个需求
