#ifndef _SX1804A_INTERNAL_
#define _SX1804A_INTERNAL_

#include <driver/av/avsource_sx1804a.h>
#include <driver/av/tools/hdmi_tool.h>

#define SX1804A_I2C_ADDR                        0xE0
#define SX1804A_I2C_WRITE                       (SX1804A_I2C_ADDR)
#define SX1804A_I2C_READ                        (SX1804A_I2C_WRITE | 0x01)


#define SX1804A_VENDER_ID 	                    0x422C
#define SX1804A_CHIP_ID		                    0x0418
/* SX1804A Register Map */
//		REGISTER    
#define SX1804A_HDMI_FLAG			            0x0180
	#define SX1804A_HDMI_FLAG_A_CH				0x08
	#define SX1804A_HDMI_FLAG_V_CH				0x04
	#define SX1804A_HDMI_FLAG_PCLK_1001			0x01
    
#define SX1804A_SYS_STATUS			            0x0200
	#define SX1804A_SYS_STATUS_AUTH				0x08
	#define SX1804A_SYS_STATUS_DECRYPT			0x04
    
#define SX1804A_SYS_STATUS2			            0x0201	
	#define SX1804A_SYS_STATUS2_LINK			0x80	
	#define SX1804A_SYS_STATUS2_DE				0x40
	#define SX1804A_SYS_STATUS2_PWR5V			0x10
    
#define SX1804A_V_STATUS			            0x0301
	#define SX1804A_V_STATUS_VTIME				0xFF
    
#define SX1804A_A_STATUS			            0x0400	
	#define SX1804A_A_STATUS_DST_ADO			0x40
	#define SX1804A_A_STATUS_HBR_ADO			0x20
	#define SX1804A_A_STATUS_DSD_ADO			0x10
	#define SX1804A_A_STATUS_STD_ADO			0x08
	#define SX1804A_A_STATUS_SAMP_FREQ			0x07
    
#define SX1804A_AVI_INFO			            0x0804	
	#define SX1804A_AVI_INFO_VIC				0xFF
    
#define SX1804A_POL_CNT				            0x2001
	#define SX1804A_POL_CNT_VS					0x02
	#define SX1804A_POL_CNT_HS					0x01
    
#define SX1804A_V_OUT				            0x2002		
	#define SX1804A_V_OUT_DCLK_POL				0x02
	
#define SX1804A_MAN_VFMT			            0x2301
	#define SX1804A_MAN_VFMT_OUT422				0x80
	#define SX1804A_MAN_VFMT_IN422				0x40
	#define SX1804A_MAN_VFMT_OUT_YCC			0x20
	#define SX1804A_MAN_VFMT_IN_YCC				0x10
	#define SX1804A_MAN_VFMT_CS					0x04
    
#define SX1804A_SCALE_H_OUT_H		            0x2500	
	#define SX1804A_SCALE_HOUT_H				0x1F
    
#define SX1804A_SCALE_H_OUT_L		            0x2501		
#define SX1804A_SCALE_V_OUT_H		            0x2502	
	#define SX1804A_SCALE_VOUT_H				0x1F
    
#define SX1804A_SCALE_V_OUT_L		            0x2503

#define SX1804A_SCALE_H_DELAY_L		            0x2504
	#define SX1804A_SCALE_HDELAY_L				0x03
    
#define SX1804A_SCALE_H_DELAY_H		            0x2505

	
#define SX1804A_V_MODE				            0x3080
	#define SX1804A_V_MODE_VOUT_EN				0x80
    #define SX1804A_V_MODE_COLOR_RGB            0x00
    #define SX1804A_V_MODE_COLOR_YCRCB_422      0x01
    #define SX1804A_V_MODE_COLOR_YCRCB_444      0x02
    
#define SX1804A_V_IN_DEF			            0x30A0
	#define SX1804A_V_IN_DEF_SCALE_BYPASS		0x04
	#define SX1804A_V_IN_DEF_V_INVERT			0x02
	#define SX1804A_V_IN_DEF_H_INVERT			0x01
    
#define SX1804A_V_CNT_NUM			            0x30A5
#define SX1804A_V_SCALE				            0x30AA
#define SX1804A_V_VTOTAL_L			            0x30B6
#define SX1804A_V_VTOTAL_H			            0x30B7
#define SX1804A_V_BT656				            0x30B8
#define SX1804A_V_CNT_BLANK			            0x30B9

#define SX1804A_A_INTERFACE			            0x3040
#define SX1804A_A_MUTE1				            0x3041
#define SX1804A_A_MUTE2				            0x3042
#define SX1804A_A_CNT1				            0x3044
#define SX1804A_A_CNT2				            0x3045
#define SX1804A_A_ACR_CTS_L			            0x30C6
#define SX1804A_A_ACR_CTS_H			            0x30C7
#define SX1804A_A_ACR_N_L			            0x30C8
#define SX1804A_A_ACR_N_H			            0x30C9
#define SX1804A_A_CLK_DIV			            0x30CA
#define SX1804A_A_FREQ_CNT			            0x30CC

#endif
