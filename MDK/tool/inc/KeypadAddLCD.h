#include "vatek_define.h"
#define display(fmt,...) printf("LOG: [%s-%d]"fmt"\r\n",__func__,__LINE__,##__VA_ARGS__)


typedef struct param_val_{
	uint32_t separate_val;
	uint32_t buswidth_val;
	video_encode_type video_enc_type;
	audio_encode_type audio_enc_type;
	uint32_t de_interlaced_val;
	uint32_t latency_val;
	uint32_t gop_val;
	uint32_t minq_val;
	uint32_t maxq_val;
	uint32_t bitrate_val;
}param_val, *Pparam_val;

/*API*/
uint8_t vatek_key_press_detect();