
#ifndef _HALREG_LICENSED_
#define _HALREG_LICENSED_

/* mux_licensed register define used with hardware licensed enabled*/
#include <hal/hal_register/halreg_mux.h>

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

/* used with HALRANGE_PLAYLOAD_START	*/	
#define HALOF_LICENSED_TAG				0x00
	#define LICENSED_EN_START			0xFF800000
	#define LICENSED_EN_END				0xFF8000FF
	#define LICENSED_V2_EN_START		0xFF810000
	#define LICENSED_V2_EN_END			0xFF8100FF

/* licensed rule section */
#define HALOF_LIC_RULE_TAG				0x00
	#define RULE_EN_START				0xFF820000
	#define RULE_EN_END					0xFF8200FF

#define HALOF_LIC_RULE_LEN				0x01


#define HALOF_LIC_CHANNEL_TAG			0x00
	#define CHANNEL_EN_START			0xFF880000
	#define CHANNEL_EN_END				0xFF8800FF

#define HALOF_LIC_CHANNEL_LEN			0x01


#define HALOF_LIC_PROGRAM_TAG			0x00
	#define PROGRAM_EN_START			0xFF890000
	#define PROGRAM_EN_END				0xFF8900FF

#define HALOF_LIC_PROGRAM_PCR_PID		0x01
#define HALOF_LIC_PROGRAM_PMT_PID		0x02
#define HALOF_LIC_PROGRAM_LEN			0x03

#define HALOF_LIC_STREAM_TAG			0x00
	#define STREAM_EN_START				0xFF8A0000
	#define STREAM_EN_END				0xFF8A00FF

#define HALOF_LIC_STREAM_TYPE			0x01
#define HALOF_LIC_STREAM_PID			0x02
#define HALOF_LIC_STREAM_ESTYPE			0x03
#define HALOF_LIC_STREAM_ESLEN			0x04

#define HALOF_LIC_SAUDIO_CODEC			0x05
#define HALOF_LIC_SAUDIO_SAMPLERATE		0x06
#define HALOF_LIC_SAUDIO_CHANNEL		0x07

#define HALOF_LIC_SVIDEO_CODEC			0x05
#define HALOF_LIC_SVIDEO_RESOLUTION		0x06
#define HALOF_LIC_SVIDEO_FRAMERATE		0x07
#define HALOF_LIC_SVIDEO_ASPECTRATE		0x08

#define HALOF_LIC_STREAM_ES				0x11

/* licensed pes_stream section */
#define HALOF_LIC_PES_TAG				0x00
	#define LIC_PES_EN_START			0xFF900000
	#define LIC_PES_EN_END				0xFF9000FF

#define HALOF_LIC_PES_PID				0x01
#define HALOF_LIC_PES_TYPE				0x02
#define HALOF_LIC_PES_ESLEN				0x03

/* licensed _epg section */

#define HALOF_LIC_EPG_TAG				0x00
	#define LIC_EPG_EN_START			0xFFA00000
	#define LIC_EPG_EN_END				0xFFA00000

#define HALOF_LIC_EPG_MODE				0x01
#define HALOF_LIC_EPG_FLAG				0x02
#define HALOF_LIC_EPG_TIMEDATA			0x03
#define HALOF_LIC_EPG_TIMETICK			0x04
#define HALOF_LIC_EPG_EVENT_ID			0x05
#define HALOF_LIC_EPG_DAYS				0x06
#define HALOF_LIC_EPG_LOOPMS			0x07
#define HALOF_LIC_EPG_SECTIONS			0x0A
#define HALOF_LIC_EPG_EXTENDDESCS		0x0B


#define HALOF_LIC_EPG_1_DURATION		0x00
#define HALOF_LIC_EPG_1_TYPE			0x01
#define HALOF_LIC_EPG_1_PRATING			0x02
#define HALOF_LIC_EPG_1_TITALMAX		0x03
#define HALOF_LIC_EPG_1_TITALLEN		0x04
#define HALOF_LIC_EPG_1_CONTENTMAX		0x05
#define HALOF_LIC_EPG_1_CONTENTLEN		0x06

#define HALOF_LIC_EPG_2_DESCTAG			0x00
	#define EPG_DESCTAG_TAG				0xAF000000
	#define EPG_DESCTAG_ARIB_DATADESC	0xAF000001

#define HALOF_LIC_EPG_2_DESC_LEN		0x01


#endif
