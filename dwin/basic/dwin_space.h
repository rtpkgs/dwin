#ifndef __DWIN_SPACE_H_ 
#define __DWIN_SPACE_H_ 

#include "dwin_def.h" 

/* 空间描述结构体 */
typedef struct dwin_space
{
    uint8_t name[PKG_DWIN_SPACE_NAME_LEN];
    uint16_t addr;
    uint16_t len;
    void *plugin;
    uint8_t type;
} *dwin_space_t; 

/* 空间使用插件类型 */
enum plugin_type
{
    DWIN_TYPE_BTN = 0,
    DWIN_TYPE_ICON,
    DWIN_TYPE_INPUTBOX,
    DWIN_TYPE_TEXTBOX,
    DWIN_TYPE_LOGIN,
    DWIN_TYPE_QRCODE,
    DWIN_TYPE_NUMBER,
    DWIN_TYPE_MAX
};

/* 导出函数 */
uint8_t         dwin_space_init     (void);
dwin_space_t    dwin_space_alloc    (const char *name, uint16_t len, uint8_t type);
#if (PKG_DWIN_DEBUG >= 1)
void            dwin_space_foreach  (void);
#endif
uint16_t        dwin_space_idle     (void);
dwin_space_t    dwin_space_find     (const char *name);

#endif
