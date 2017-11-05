#ifndef __DW_SYSTEM_H_ 
#define __DW_SYSTEM_H_ 

#include "dw_common.h"
#include "dw_conf.h"

/* ≈‰÷√÷µ */
#define DW_BACKLIGHT_MAX    (0x40)

/* ºƒ¥Ê∆˜µÿ÷∑ */
#define DW_REG_ADDR_HARD_VERSION        (0x00)
#define DW_REG_ADDR_BACKLIGHT           (0x01)
#define DW_REG_ADDR_BEEP                (0x02)
#define DW_REG_ADDR_PAGEID              (0x03)
#define DW_REG_ADDR_R2                  (0x12)

/* system api */
bool dw_hard_version(uint8_t *version, uint8_t *subversion);
bool dw_soft_version(uint8_t *version, uint8_t *subversion, uint8_t *revision);

bool dw_get_backlight(uint8_t *level);
bool dw_set_backlight(uint8_t level);

bool dw_beep_10ms(uint8_t time);

bool dw_get_now_pageid(uint16_t *id);
bool dw_set_now_pageid(uint16_t id);

bool dw_set_screen_dir(uint16_t dir);

#endif

