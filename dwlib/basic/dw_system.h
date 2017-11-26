#ifndef __DW_SYSTEM_H_ 
#define __DW_SYSTEM_H_ 

#include "dw_common.h"
#include "dw_conf.h"

/* 时间结构体 */
typedef struct dw_time
{
    uint16_t year; 
    uint8_t month;  
    uint8_t day; 
    uint8_t hour;
    uint8_t minute;
    uint8_t seconds;
} *dw_time_t;

/* 配置值 */
#define DW_BACKLIGHT_MAX    (0x40)

/* 寄存器地址 */
#define DW_REG_ADDR_HARD_VERSION        (0x00)
#define DW_REG_ADDR_BACKLIGHT           (0x01)
#define DW_REG_ADDR_BEEP                (0x02)
#define DW_REG_ADDR_PAGEID              (0x03)
#define DW_REG_ADDR_R2                  (0x12)
#define DW_REG_ADDR_W_TIME              (0x1F)
#define DW_REG_ADDR_R_TIME              (0x20)

/* system api */
bool dw_read_reg(uint8_t addr, uint8_t *buffer, uint8_t length);
bool dw_write_reg(uint8_t addr, uint8_t *buffer, uint8_t length);
bool dw_read_val(uint16_t addr, uint16_t *buffer, uint16_t length);
bool dw_write_val(uint16_t addr, uint16_t *buffer, uint16_t length);

bool dw_hard_version(uint8_t *version, uint8_t *subversion);
bool dw_soft_version(uint8_t *version, uint8_t *subversion, uint8_t *revision);

bool dw_get_backlight(uint8_t *level);
bool dw_set_backlight(uint8_t level);

bool dw_beep_10ms(uint8_t time);

bool dw_get_now_pageid(uint16_t *id);
bool dw_set_now_pageid(uint16_t id);

bool dw_set_screen_dir(uint16_t dir);

bool dw_touch_blacklight_enable(bool enable);

bool dw_touch_data_update_enable(bool enable);

bool dw_l22_mode_init(void);
bool dw_touch_beep_enable(bool enable);

bool dw_get_time(struct dw_time* time);
bool dw_set_time(struct dw_time time);
bool dw_get_timestamp(uint32_t *timestamp);
bool dw_set_timestamp(uint32_t timestamp);

#endif

