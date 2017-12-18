#ifndef __DWIN_SYSTEM_H_ 
#define __DWIN_SYSTEM_H_ 

#include "dwin_trans.h" 

/* 外部函数接口 */ 
uint32_t    dwin_system_sw_version          (uint8_t *major, uint8_t *minor, uint8_t *revise);
uint32_t    dwin_system_hw_version          (uint8_t *major, uint8_t *minor, uint8_t *revise);
uint8_t     dwin_system_get_backlight_lvl   (uint8_t *lvl);
uint8_t     dwin_system_set_backlight_lvl   (uint8_t lvl);
uint8_t     dwin_system_set_beep            (uint8_t time);
uint8_t     dwin_system_get_page_id         (uint16_t *id);
uint8_t     dwin_system_set_page_id         (uint16_t id);
uint8_t     dwin_system_set_backligth       (uint8_t enable);
uint8_t     dwin_system_set_autoupdate      (uint8_t enable);
uint8_t     dwin_system_set_dataload_mode   (uint8_t enable);   /* 未测试 */
uint8_t     dwin_system_set_buzzer          (uint8_t enable);
uint8_t     dwin_system_get_time            (uint8_t *year, uint8_t *month, uint8_t *day, 
                                             uint8_t *hour, uint8_t *minute, uint8_t *seconds);
uint8_t     dwin_system_set_time            (uint8_t year, uint8_t month, uint8_t day, 
                                             uint8_t hour, uint8_t minute, uint8_t seconds);
                                             
#endif
