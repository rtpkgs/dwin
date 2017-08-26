#ifndef __DW_SPACE_H_
#define __DW_SPACE_H_

#include "dw_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 指令 */
/* 寄存器空间读写响应指令 */
#define RT_DW_COMMAND_READ_REG_SPACE        (0x81)
#define RT_DW_COMMAND_WRITE_REG_SPACE       (0x80)

/* 变量空间读写响应指令 */
#define RT_DW_COMMAND_READ_VAR_SPACE        (0x83)
#define RT_DW_COMMAND_WRITE_VAR_SPACE       (0x82)

/* 曲线缓存空间写指令 */
#define RT_DW_COMMAND_WRITE_CURVE_SPACE     (0x84)

/* 全局BUFFER大小 */
#define RT_DW_FRAME_BUFFER_MAX_LENGTH       (248)   /* 手册提供 */

/* 数据帧头内容 */
#define RT_DW_FRMAE_HIGH_CONTENT            (0x5A)  /* 帧头高字节内容 */
#define RT_DW_FRMAE_LOW_CONTENT             (0xA5)  /* 帧头低字节内容 */

/* 寄存器空间相关宏定义 */
/* 寄存器空间地址范围 */
#define RT_DW_REG_SPACE_MIN_ADDRESS         (0x00)  /* 寄存器空间最小地址 */
#define RT_DW_REG_SPACE_MAX_ADDRESS         (0xFF)  /* 寄存器空间最大地址 */

/* 寄存器数据帧内容偏移 */
#define RT_DW_REG_SPACE_FRAME_HIGH_OFFSET   (0) /* 帧头高字节 */
#define RT_DW_REG_SPACE_FRAME_LOW_OFFSET    (1) /* 帧头低字节 */
#define RT_DW_REG_SPACE_DATA_LENGTH_OFFSET  (2) /* 帧数据长度, 单位字节, 包含指令/地址/数据长度 */
#define RT_DW_REG_SPACE_COMMAND_OFFSET      (3) /* 帧控制命令 */
#define RT_DW_REG_SPACE_ADDRESS_OFFSET      (4) /* 读写寄存器地址 */
#define RT_DW_REG_SPACE_DATA_OFFSET         (5) /* 读写寄存器内容 */

/* 变量空间地址范围 */
#define RT_DW_VAR_SPACE_MIN_ADDRESS         (0x0000)  /* 寄存器空间最小地址 */

/* 待补充所有迪文屏幕的变量数据大小 */
#if defined(RT_USING_DMT48270M043_02W) || defined(RT_USING_DMT48270M050_02W)
#define RT_DW_VAR_SPACE_MAX_ADDRESS         (0x1000)  /* 寄存器空间最大地址:4K */
#endif

/* 变量数据帧内容偏移 */
#define RT_DW_VAR_SPACE_FRAME_HIGH_OFFSET   (0) /* 帧头高字节 */
#define RT_DW_VAR_SPACE_FRAME_LOW_OFFSET    (1) /* 帧头低字节 */
#define RT_DW_VAR_SPACE_DATA_LENGTH_OFFSET  (2) /* 帧数据长度, 单位字节, 包含指令/地址/数据长度 */
#define RT_DW_VAR_SPACE_COMMAND_OFFSET      (3) /* 帧控制命令 */
#define RT_DW_VAR_SPACE_ADDRESS_OFFSET      (4) /* 读写寄存器地址 */
#define RT_DW_VAR_SPACE_DATA_OFFSET         (6) /* 读写寄存器内容 */

/* 寄存器空间指令 */
/* 格式:RT_DW_REG_XXH(寄存器地址, 16进制)_XXX(寄存器长度, 10进制)_功能缩写 */
#if defined(RT_USING_DMT48270M043_02W) || defined(RT_USING_DMT48270M050_02W)
#define RT_DW_REG_00H_001_VERSION               (0x00)  // 迪文屏幕固件版本
#define RT_DW_REG_01H_001_NOW_BL                (0x01)  // 当前背光亮度
#define RT_DW_REG_02H_001_MAKE_BEEP             (0x02)  // 驱动蜂鸣, 单位10ms
#define RT_DW_REG_03H_002_NOW_PAGEID            (0x03)  // 系统当前页面ID
#define RT_DW_REG_05H_001_TOUCH_FLAG            (0x05)  // 触摸标志位
#define RT_DW_REG_06H_001_TOUCH_STATE           (0x06)  // 触摸状态
#define RT_DW_REG_07H_004_TOUCH_POS             (0x07)  // 触摸位置
#define RT_DW_REG_0BH_001_TOUCH_EN              (0x0B)  // 触摸使能
#define RT_DW_REG_0CH_004_RESERVE               (0x0C)  // 保留
#define RT_DW_REG_10H_001_DRV_MODE              (0x10)  // R0, 驱动模式(迪文公司内部使用)
#define RT_DW_REG_11H_001_BRUDRATE              (0x11)  // R1, 通信波特率
#define RT_DW_REG_12H_001_SYSCFG                (0x12)  // R2, 系统配置
#define RT_DW_REG_13H_001_FARAME_H              (0x13)  // R3, 通信帧头高字节
#define RT_DW_REG_14H_002_RESERVE               (0x14)  // R4R5, 保留
#define RT_DW_REG_16H_001_LEDON_LEVEL           (0x16)  // R6, 背光点亮亮度
#define RT_DW_REG_17H_001_LEDOFF_LEVEL          (0x17)  // R7, 背光熄灭亮度
#define RT_DW_REG_18H_001_LEDON_KEEPTIME        (0x18)  // R8, 背光点亮保持时间, 单位1s
#define RT_DW_REG_19H_001_RESERVE               (0x19)  // R9, 保留
#define RT_DW_REG_1AH_001_FARAME_L              (0x1A)  // RA, 通信帧头低字节
#define RT_DW_REG_1BH_004_RESERVE               (0x1B)  // 保留
#define RT_DW_REG_1FH_001_RTC_UPDATE_FLAG       (0x1F)  // 手动修改RTC更新标志位
#define RT_DW_REG_20H_016_RTC                   (0x20)  // RTC数据
#define RT_DW_REG_30H_016_RESERVE               (0x30)  // 保留
#define RT_DW_REG_40H_001_FONTLIB_OPT_EN        (0x40)  // 字库操作使能
#define RT_DW_REG_41H_001_FONTLIB_OPT_MODE      (0x41)  // 字库模式
#define RT_DW_REG_42H_001_FONTLIB_OPT_ID        (0x42)  // 字库ID
#define RT_DW_REG_43H_003_FONTLIB_OPT_ADDR      (0x43)  // 字库地址
#define RT_DW_REG_46H_002_FONTLIB_OPT_VP        (0x46)  // 05系列支持
#define RT_DW_REG_48H_002_FONTLIB_OPT_LENGTH    (0x48)  // 05系列支持
#define RT_DW_REG_4AH_002_TIMER0_16BIT_4MS      (0x4A)  // 定时器0, 16位长度, 步进4MS, 误差±4mS
#define RT_DW_REG_4CH_001_TIMER1_08BIT_4MS      (0x4C)  // 定时器1, 08位长度, 步进4MS, 误差±4mS
#define RT_DW_REG_4DH_001_TIMER2_08BIT_4MS      (0x4D)  // 定时器2, 08位长度, 步进4MS, 误差±4mS
#define RT_DW_REG_4EH_001_RESERVE               (0x4E)  // 保留
#define RT_DW_REG_4FH_001_KEYCODE               (0x4F)  // 按键值
#define RT_DW_REG_50H_155_RESERVE               (0x50)  // 保留
#define RT_DW_REG_EBH_001_CURVE_CLR             (0xEB)  // 清除曲线数据
#define RT_DW_REG_ECH_001_FAST_REFRESH          (0xEC)  // 快速刷新
#define RT_DW_REG_EDH_019_RESERVE               (0xED)  // 保留
#define RT_DW_REG_F3H_002_DOWNLOAD_LIB          (0xF3)  // 下载字库
#define RT_DW_REG_F5H_004_DOWNLOAD_PIC          (0xF5)  // 下载图片
#define RT_DW_REG_F9H_001_RESERVE               (0xF9)  // 保留
#define RT_DW_REG_FAH_001_DOWNLOAD_FLAG         (0xFA)  // 下载成功标志位
#define RT_DW_REG_FBH_005_RESERVE               (0xFB)  // 保留

/* R2 SYSCFG掩码 */
#define RT_DW_R2BIT0_BUZZER_EN_MASK             (1<<0)  // 蜂鸣器使能
#define RT_DW_R2BIT1_RESERVE_MASK               (1<<1)  // 保留
#define RT_DW_R2BIT2_VAR_INIT_MODE_MASK         (1<<2)  // 变量空间初始化模式, 1:上电全部初始化为0x00, 2由22号字库填充
#define RT_DW_R2BIT3_DATA_AUTO_UPLOAD_MASK      (1<<3)  // 触摸数据自动上传
#define RT_DW_R2BIT4_CRC_EN_MASK                (1<<4)  // 串口帧CRC使能, 不支持
#define RT_DW_R2BIT5_BL_CTR_MASK                (1<<5)  // 触摸控制背光
#define RT_DW_R2BIT6_HDS_MASK                   (1<<6)  // 旋转180度
#define RT_DW_R2BIT7_VHS_MASK                   (1<<7)  // 旋转90度
#endif

/* 读寄存器空间 */
rt_err_t rt_dw_read_reg_space(
    rt_device_dw_t device, 
    rt_uint8_t addr, 
    rt_uint8_t length, 
    rt_uint8_t *rx_data);

/* 写寄存器空间 */
rt_err_t rt_dw_write_reg_space(
    rt_device_dw_t device, 
    rt_uint8_t addr, 
    rt_uint8_t length, 
    rt_uint8_t *tx_data);

/* 读变量空间 */
rt_err_t rt_dw_read_var_space(
	rt_device_dw_t device, 
	rt_uint16_t addr, 
	rt_uint8_t length, 
	rt_uint16_t *rx_data);

/* 写变量空间 */
rt_err_t rt_dw_write_var_space(
	rt_device_dw_t device, 
	rt_uint16_t addr, 
	rt_uint8_t length, 
	rt_uint16_t *tx_data);
	
/* 读参寄存器 */
rt_err_t rt_dw_read_para_reg(rt_device_dw_t device, rt_uint8_t addr, rt_uint8_t *rx_data);
rt_err_t rt_dw_write_para_reg(rt_device_dw_t device, rt_uint8_t addr, rt_uint8_t tx_data);

/* 写参寄存器 */

#ifdef __cplusplus
}
#endif

#endif
