
#ifndef _HALREG_REMUX_
#define _HALREG_REMUX_

#define HALREG_REMUX_PCR_PID			0x610				
#define HALREG_REMUX_PADDING_PID		0x611				
#define HALREG_REMUX_BITRATE			0x612				/* remux stream target bitrate								*/
#define HALREG_REMUX_STUFFING_PID		0x613
#define HALREG_REMUX_FLAGS				0x614
	#define REMUX_FLAG_EN_PSIP			0x00000001			/* pid_0x1FFB_psi_table would used same countine			*/
	#define REMUX_FLAG_DISABLE_PCR		0x00000002			/* remux stream no create _pcr_packet						*/	
	#define REMUX_FLAG_SW_PCR			0x00000004			/* remux stream _pcr_packet clock source in _sw or _hw		*/

/* auxiliary_stream_ip				*/

#define HALREG_AUXDATA_PACKETNUMS       0x630				/* auxiliary stream valid packet len						*/

/* private_stream_ip				*/

#define HALREG_PRIVATE_START            0x639				/* private stream _playload start address					*/
#define HALREG_PRIVATE_END              0x63A				/* private stream _playload end address						*/
#define HALREG_PRIVATE_CNTL             0x63B				/* private stream control									*/
	#define PRIVATE_EN_CONTINUE         0x80000000			/* - mux when write and read pointer not equal				*/
	#define PRIVATE_EN_TIMES            0x00000001			/* - mux onces after finish auto clear						*/	

#define HALREG_PRIVATE_WPTR				0x63C				/* private stream write pointer (update by user)			*/
#define HALREG_PRIVATE_RPTR				0x63D				/* private stream read pointer	(update by hardware)		*/

/* 
	remux filter only supported in a3 remux mode 
	num == 0 or no current en_tag would take all packets when actived remux at a3 chip		
*/

#define HALREG_REMUX_FILTER_EN			0x642				
	#define REMUX_FILTER_EN_TAG			0xA5F00000
	#define REMUX_FILTER_MASK			0xFF
	#define REMUX_FILTER_NAX_NUMS		16

/* pid_filter */
#define HALREG_REMUX_FILTER_0			0x670
	#define FILTER_EN_REPLACE			0x80000000
	#define FILTER_OFFSET_SUR			16
	#define FILTER_OFFSET_NEW			0
	#define FILTER_NO_REPLACE(sur)		(sur << FILTER_OFFSET_SUR)
	#define FILTER_REPLACE(sur,new)		(FILTER_EN_REPLACE | (sur << FILTER_OFFSET_SUR) | new)

#define HALREG_REMUX_FILTER_1			0x671
#define HALREG_REMUX_FILTER_2			0x672
#define HALREG_REMUX_FILTER_3			0x673
#define HALREG_REMUX_FILTER_4			0x674
#define HALREG_REMUX_FILTER_5			0x675
#define HALREG_REMUX_FILTER_6			0x676
#define HALREG_REMUX_FILTER_7			0x677
#define HALREG_REMUX_FILTER_8			0x678
#define HALREG_REMUX_FILTER_9			0x679
#define HALREG_REMUX_FILTER_10			0x67A
#define HALREG_REMUX_FILTER_11			0x67B
#define HALREG_REMUX_FILTER_12			0x67C
#define HALREG_REMUX_FILTER_13			0x67D
#define HALREG_REMUX_FILTER_14			0x67E
#define HALREG_REMUX_FILTER_15			0x67F

#endif
