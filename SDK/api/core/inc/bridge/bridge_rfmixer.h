#ifndef _BRIDGE_RFMIXER_
#define _BRIDGE_RFMIXER_

#include <bridge/bridge_base.h>

typedef enum _brfmixer_driver
{
	brfmixer_fintek_r2	= (BBRIDGE_TYPE_BRFMIXER | 0x100),
	brfmixer_adrf6755	= (BBRIDGE_TYPE_BRFMIXER | 0x200),
}brfmixer_driver;

#define BRFMIXER_FINTEK_R2			BBRIDGE_DRI_TO_BIT(brfmixer_fintek_r2)
#define BRFMIXER_ADV7182			BBRIDGE_DRI_TO_BIT(brfmixer_adrf6755)

/* bridge source command define */

#define BRIDGE_IP_RFMIXER_TAG		0x00F40000
#define IP_RFMIXER_SET_CMD(cmd)		BRIDGE_SET_CMD(BRIDGE_IP_RFMIXER_TAG,cmd)
#define IS_BRIDGE_RFMIXER_CMD(cmd)	((cmd & 0x00FF0000) == BRIDGE_IP_RFMIXER_TAG)

#define BRFMIXER_CMD_START			IP_RFMIXER_SET_CMD(0x101)
#define BRFMIXER_CMD_STOP			IP_RFMIXER_SET_CMD(0x102)

typedef struct _brfmixer_op_param
{
	uint32_t cntl;
	uint32_t freq_khz;
	uint32_t param_0;
	uint32_t param_1;
	uint32_t param_2;
	uint32_t param_3;
}brfmixer_op_param,*Pbrfmixer_op_param;

_bparam_start(brfmixer_op_param)
	_bparam_uint(0, brfmixer_op_param, cntl)
	_bparam_uint(1, brfmixer_op_param, freq_khz)
	_bparam_uint(2, brfmixer_op_param, param_0)
	_bparam_uint(3, brfmixer_op_param, param_1)
	_bparam_uint(4, brfmixer_op_param, param_2)
	_bparam_uint(5, brfmixer_op_param, param_3)
_bparam_end

#endif
