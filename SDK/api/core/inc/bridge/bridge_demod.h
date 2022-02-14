#ifndef _BRIDGE_DEMODE_
#define _BRIDGE_DEMODE_

#include <bridge/bridge_base.h>

typedef enum _bdemod_driver
{
	bdemod_avl68xx = (BBRIDGE_TYPE_BDEMOD | 0x100),
}bdemod_driver;


#define BDEMOD_AVL68XX_BIT			BBRIDGE_DRI_TO_BIT(bdemod_avl68xx)

#define BRIDGE_IP_DEMOD_TAG        	0x00F80000
#define IP_DEMOD_SET_CMD(cmd)		BRIDGE_SET_CMD(BRIDGE_IP_DEMOD_TAG,cmd)
#define IS_BRIDGE_DEMOD_CMD(cmd)	((cmd & 0x00FF0000) == BRIDGE_IP_DEMOD_TAG)

/* bridge _demod command define */

#define BDEMOD_CMD_GETINFO			IP_DEMOD_SET_CMD(0x100)
#define BDEMOD_CMD_STATUS			IP_DEMOD_SET_CMD(0x101)
#define BDEMOD_CMD_SET_MODE			IP_DEMOD_SET_CMD(0x102)
#define BDEMOD_CMD_GET_MODE			IP_DEMOD_SET_CMD(0x103)
#define BDEMOD_CMD_START			IP_DEMOD_SET_CMD(0x104)
#define BDEMOD_CMD_STOP				IP_DEMOD_SET_CMD(0x105)

typedef struct _bdemod_info
{
	uint32_t support_mode;
	uint32_t enable_mode;
}bdemod_info,*Pbdemod_info;

#define BDEMOD_EN_TS_SERIAL			0x00000001

typedef struct _bdemod_mode_param
{
	uint32_t mod_mode;							/* MOD_XXXX_BIT defined in base/mod_define.h */
	uint32_t ts_clock;
	uint32_t ts_enable;
}bdemod_mode_param,*Pbdemod_mode_param;

typedef struct _bdemod_op_param
{
	uint32_t bandwidth_or_symbolrate;
	uint32_t freq_khz;
}bdemod_op_param,*Pbdemod_op_param;

_bparam_start(bdemod_info)
	_bparam_uint(0,bdemod_info, support_mode)
	_bparam_uint(1,bdemod_info, enable_mode)
_bparam_end

_bparam_start(bdemod_mode_param)
	_bparam_uint(0, bdemod_mode_param, mod_mode)
	_bparam_uint(1, bdemod_mode_param, ts_clock)
	_bparam_uint(2, bdemod_mode_param, ts_enable)
_bparam_end

_bparam_start(bdemod_op_param)
	_bparam_uint(0, bdemod_op_param, bandwidth_or_symbolrate)
	_bparam_uint(1, bdemod_op_param, freq_khz)
_bparam_end

#endif
