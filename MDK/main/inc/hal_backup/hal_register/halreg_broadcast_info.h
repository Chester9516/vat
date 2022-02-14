#ifndef _HALREG_BROADCAST_INFO_
#define _HALREG_BROADCAST_INFO_


/* broadcast_information_ip			*/

#define HALREG_BCINFO_STREAM			0x620				/* enabled stream mode										*/	
#define HALREG_BCINFO_OUTPUT			0x621				/* enabled output mode										*/	
#define HALREG_BCINFO_MODRATE			0x622				/* modulator output bitrate									*/	
#define HALREG_BCINFO_MUXRATE			0x623				/* current mux bitrate										*/
#define HALREG_BCINFO_STATUS			0x624				/* current broadcast status									*/
	#define BCSTATUS_IDLE				0x00000000			/* - broadcast not enabled									*/
	#define BCSTATUS_WAIT_SOURCE		0x00000001			/* - broadcast not start output wait source					*/
	#define BCSTATUS_BROADCAST			0x00000002			/* - broadcast start output data							*/
	#define BCSTATUS_FINISH				0x00000003			/* - broadcast operation finish	(used with transform)		*/	

	#define BCSTATUS_FAIL_UNKNOWN       0x80000000
	#define BCSTATUS_FAIL_SOURCE        0x80000001
	#define BCSTATUS_FAIL_TIMEOUT       0x80000002
	#define BCSTATUS_FAIL_CODECDROP     0x80000003
	#define BCSTATUS_FAIL_BUFFER        0x80000004
	#define BCSTATUS_FAIL_MUXER         0x80000005
	#define BCSTATUS_FAIL_ENCODE        0x80000006
	#define BCSTATUS_FAIL_MEDIA         0x80000007
	#define BCSTATUS_FAIL_DEMUX			0x80000008

#define HALREG_BCINFO_CURRATE			0x625				/* current mux valid data bitrate							*/	
#define HALREG_BCINFO_DATARATE			0x626				/* current main pes data bitrate							*/
															/* - broadcast : mean video encode bitrate					*/
															/* - transform : mean _demux bitrate						*/

#define HALREG_BCINFO_QUERYBUF          0x627               /* buffer query_buffer len									*/
															/* - broadcast : video result buffer remain size			*/
															/* - transform : _demux query buffer remain size			*/	
#define HALREG_BCINFO_LAST				0x62F

#endif
