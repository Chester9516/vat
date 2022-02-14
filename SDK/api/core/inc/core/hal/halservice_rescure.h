#ifndef _HALSERVICE_RESCURE_
#define _HALSERVICE_RESCURE_

#include <core/hal/halservice_base.h>

#define HALREG_RESCUE_CNTL				0x31
	#define RESCUE_CNTL_TAG				0xFA000000
	#define RESCUE_CNTL_WRITE			0xFA010000
	#define RESCUE_CNTL_READ			0xFA020000
	#define RESCUE_CNTL_REBOOT			0xFA0300FB
	#define RESCUE_SECTION_MASK			0x0000FFFF
	#define RESCUE_CNTL_TAG_MASK		0xFF000000

#define IS_RESCUE_CNTL_VALID(val)		((val & RESCUE_CNTL_TAG_MASK) == RESCUE_CNTL_TAG)
#define IS_RESCUR_CMD(val,cmd)			((val & 0xFFFF0000) == cmd)

#define HALREG_RESCUE_CRC32				0x32
#define HALREG_RESCUE_RESULT            0x33
	#define RESCUE_ERR_SUCCESS          0x00000000
	#define RESCUE_ERR_CRC              0x80000001
	#define RESCUE_ERR_SPIFLASH         0x80000002
	#define RESCUE_ERR_BADCNTL			0x80000003
	#define RESCUE_ERR_BADSTATUS		0x80000004

#define HALREG_APP_DEBUG                0x3E
	#define APPDBG_EN_PLAYLOAD			0x00000001

#define HALREG_SYS_DEBUG                0x3F

#define HALREG_RESCUE_DATA				0x3C00

#define HALREG_RESCUE_EN				0x4000
	#define RESCUE_EN_TAG				0xFF1229FF  /* next reboot enter rescue mode */
    
#endif
