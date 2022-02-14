

#ifndef _HALREG_MUX_
#define _HALREG_MUX_

/* mux common define */

#define MUX_STREAM_TYPE_VIDEO			0x00000000
#define MUX_STREAM_TYPE_AUDIO			0x00000001
#define MUX_STREAM_TYPE_DATA			0x00000002

#define MUX_SPEC_UNKNOWN				0x00000000
#define MUX_SPEC_ISO					0x00000001
#define MUX_SPEC_DVB					0x00000002
#define MUX_SPEC_PSIP					0x00000003

/* mux_param_register */

#define HALREG_MUX_PCR_PID				0x610				
#define HALREG_MUX_PADDING_PID			0x611				
#define HALREG_MUX_BITRATE				0x612				/* mux stream target bitrate								*/
#define HALREG_MUX_STUFFING_PID			0x613
#define HALREG_MUX_FLAGS				0x614
	#define MUX_FLAG_EN_PSIP			0x00000001			/* pid_0x1FFB_psi_table would used same countine			*/
	#define MUX_FLAG_DISABLE_PCR		0x00000002			/* mux stream no create _pcr_packet						*/	
	#define MUX_FLAG_SW_PCR				0x00000004			/* mux stream _pcr_packet clock source in _sw or _hw		*/

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
	mux filter only supported in a3 mux mode
	num == 0 or no current en_tag would take all packets when actived remux at a3 chip
*/

#define HALREG_MUX_FILTER_EN			0x642				
	#define MUX_FILTER_EN_TAG			0xA5F00000

#define MUX_FILTER_MASK					0xFF
#define MUX_FILTER_NAX_NUMS				16

/* pid_filter */
#define HALREG_MUX_FILTER_0				0x670
	#define FILTER_EN_REPLACE			0x80000000
	#define FILTER_OFFSET_SUR			16
	#define FILTER_OFFSET_NEW			0
	#define FILTER_NO_REPLACE(sur)		(sur << FILTER_OFFSET_SUR)
	#define FILTER_REPLACE(sur,new)		(FILTER_EN_REPLACE | (sur << FILTER_OFFSET_SUR) | new)

#define HALREG_MUX_FILTER_1				0x671
#define HALREG_MUX_FILTER_2				0x672
#define HALREG_MUX_FILTER_3				0x673
#define HALREG_MUX_FILTER_4				0x674
#define HALREG_MUX_FILTER_5				0x675
#define HALREG_MUX_FILTER_6				0x676
#define HALREG_MUX_FILTER_7				0x677
#define HALREG_MUX_FILTER_8				0x678
#define HALREG_MUX_FILTER_9				0x679
#define HALREG_MUX_FILTER_10			0x67A
#define HALREG_MUX_FILTER_11			0x67B
#define HALREG_MUX_FILTER_12			0x67C
#define HALREG_MUX_FILTER_13			0x67D
#define HALREG_MUX_FILTER_14			0x67E
#define HALREG_MUX_FILTER_15			0x67F

#define HALREG_MUXPSI_MODE				0x2000
	#define MUX_PURE					0x00000000
	#define MUX_ISO13818				0x00000001
	#define MUX_SPEC					0x00000002
	#define MUX_DEFSPEC					0x00000003

#define HALREG_MUXPSI_ISO_TSID			0x2005
#define HALREG_MUXPSI_ISO_PROGRAM_NUM	0x2006

/* update licensed ts_mux time (used with _eit, _tot, _tdt....) */
#define HALREG_MUX_TIME_CNTL			0x2080
	#define MUX_TIME_EN_UPDATE			0x00000001

#define HALREG_MUX_TIME_DATE			0x2081
	#define MUX_TIME_YEAR_MASK			0xFFFF0000
	#define MUX_TIME_MONTH_MASK			0x0000FF00
	#define MUX_TIME_DAY_MASK			0x000000FF

#define HALREG_MUX_TIME_CLOCK			0x2082
	#define MUX_TIME_HOUR_MASK			0x00FF0000
	#define MUX_TIME_MIN_MASK			0x0000FF00
	#define MUX_TIME_SEC_MASK			0x000000FF

/* licensed pes_stream control register */
#define HALREG_SPIPES_CNTL				0x2083
	#define SPIPES_CNTL_RELOAD			0xA9A80001

#define HALREG_SPIPES_STATUS			0x2085
	#define SPIPES_STATUS_UNSUPPORT		0x00000000						/* not supported spi_pes */
	#define SPIPES_STATUS_EN_TAG		0xA9A80000
	#define SPIPES_STATUS_IDLE			(SPIPES_STATUS_EN_TAG)			/* supported licensed_spi_pesstream */
	#define SPIPES_STATUS_NODATA		(SPIPES_STATUS_EN_TAG | 0x01)	/* load spi_pes_stream not found	*/
	#define SPIPES_STATUS_ACTIVE		(SPIPES_STATUS_EN_TAG | 0x02) 	/* load spi_pes_stream_current		*/

/* update licensed ts_mux time (used with _eit, _tot, _tdt....) */
#define HALREG_MUX_TIME_CNTL			0x2080
	#define MUX_TIME_EN_UPDATE			0x00000001

#define HALREG_MUX_TIME_DATE			0x2081
	#define MUX_TIME_YEAR_MASK			0xFFFF0000
	#define MUX_TIME_MONTH_MASK			0x0000FF00
	#define MUX_TIME_DAY_MASK			0x000000FF

#define HALREG_MUX_TIME_CLOCK			0x2082
	#define MUX_TIME_HOUR_MASK			0x00FF0000
	#define MUX_TIME_MIN_MASK			0x0000FF00
	#define MUX_TIME_SEC_MASK			0x000000FF

/* licensed pes_stream control register */
#define HALREG_SPIPES_CNTL				0x2083
	#define SPIPES_CNTL_RELOAD			0xA9A80001

#define HALREG_SPIPES_STATUS			0x2085
	#define SPIPES_STATUS_UNSUPPORT		0x00000000						/* not supported spi_pes */
	#define SPIPES_STATUS_EN_TAG		0xA9A80000
	#define SPIPES_STATUS_IDLE			(SPIPES_STATUS_EN_TAG)			/* supported licensed_spi_pesstream */
	#define SPIPES_STATUS_NODATA		(SPIPES_STATUS_EN_TAG | 0x01)	/* load spi_pes_stream not found	*/
	#define SPIPES_STATUS_ACTIVE		(SPIPES_STATUS_EN_TAG | 0x02) 	/* load spi_pes_stream_current		*/

#endif

