#include "stdio.h"
#include "vatek_api.h"
#include "KeypadAddLCD.h"
#include "vatek_sample_keypad_stm32f401.h"
#include "vatek_sample_lcd1602.h"

extern vatek_result vatek_param_control(Pparam_val p_val);

/*V1 used parameter*/
uint32_t sequence_val = 0; //select parameter which like to change value

const char *enable_list[8] = {"Video Type", "Audio Type", "Output format", "bitrate value", "min Q", "max Q", "GOP", "Latency"};
static uint32_t video_type = 0;
static uint32_t audio_type = 0;
static uint32_t deinterlaced_value = 0;
char *video_encoder_list[2] = {"MPEG-2", "H.264"};
char *audio_encoder_list[4] = {"MPEG1-L2","AAC-LC-ADTS","AC-3","AAC-LC-LATM"};
//const char *separate_embbed_mode_list[2] = {"Separate","Embbeded"};
//const char *bit8_16_list[2] = {"8 bits", "16 bits"};
const char *deinterlaced_list[2] = {"de-interlaced","interlaced"};
static uint32_t bitrate_value = 19000000;
static uint32_t minq_value = 5;
static uint32_t maxq_value = 12;
static uint32_t gop_value = 16;
static uint32_t latency_value = 500;


char latency_str[4] = "";
char gop_str[2] = "";
char minq_str[2] = "";
char maxq_str[2] = "";
char bitrate_str[8] = "";
/*end*/

param_val write_val = {0};

uint8_t vatek_key_press_detect()
{
	switch(vatek_key_press(GPIOE, UP_Pin)){
		case KEY_ON:
			printf("key pressed\r\n");
			sequence_val == 0 ? sequence_val = 7 : sequence_val--;
			display("%s", enable_list[sequence_val]);
			break;
		case KEY_OFF:
			printf("key released\r\n");
			break;
		case KEY_HOLD:
			printf("key hold\r\n");
			break;
		case KEY_ERROR:
			printf("key error occur\r\n");
			break;
	}
	
	switch(vatek_key_press(GPIOE, CENTER_Pin)){
		case KEY_ON:
			printf("key pressed\r\n");
			write_val.video_enc_type = video_type + 1;
			write_val.audio_enc_type = audio_type + 1;
			write_val.de_interlaced_val = deinterlaced_value;
			write_val.bitrate_val = bitrate_value;
			write_val.latency_val = latency_value;
			write_val.gop_val = gop_value;
			write_val.minq_val = minq_value;
			write_val.maxq_val = maxq_value;
#if defined(VATEK_V1)
			vatek_param_control(&write_val);
#endif
			break;
		case KEY_OFF:
			printf("key released\r\n");
			break;
		case KEY_HOLD:
			printf("key hold\r\n");
			break;
		case KEY_ERROR:
			printf("key error occur\r\n");
			break;
	}
	
	switch(vatek_key_press(GPIOE, DOWN_Pin)){
		case KEY_ON:
			printf("key pressed\r\n");
			sequence_val >= 7 ? sequence_val = 0 : sequence_val++;
			display("%s", enable_list[sequence_val]);
			break;
		case KEY_OFF:
			printf("key released\r\n");
			break;
		case KEY_HOLD:
			printf("key hold\r\n");
			break;
		case KEY_ERROR:
			printf("key error occur\r\n");
			break;
	}
	
	switch(vatek_key_press(GPIOE, LEFT_Pin)){
		case KEY_ON:
			printf("key pressed\r\n");
			switch (sequence_val){
				case 0:
					video_type <= 0 ? video_type = 0 : video_type--;
					display("%s",video_encoder_list[video_type]);
					break;
				case 1:
					audio_type <= 0 ? audio_type = 0 : audio_type--;
					display("%s",audio_encoder_list[audio_type]);
					break;
				case 2:
					deinterlaced_value <= 0 ? deinterlaced_value = 0 : deinterlaced_value--;
					display("%s", deinterlaced_list[deinterlaced_value]);
					break;
				case 3:
					if(bitrate_value <= 4000000){bitrate_value = 4000000;}else{bitrate_value -= 1000000;}
					display("%d", bitrate_value);
					break;
				case 4:
					minq_value <= 3 ? minq_value = 3 : minq_value--;
					write_val.minq_val = minq_value;
					display("%d", minq_value);
					break;
				case 5:
					maxq_value <= 3 ? maxq_value = 3 : maxq_value--;
					display("%d",maxq_value);
					break;
				case 6:
					gop_value <= 1 ? gop_value = 1 : gop_value--;
					display("%d", gop_value);
					break;
				case 7:
					if(latency_value <= 100){latency_value = 100;}else{latency_value-=100;}
					display("%d", latency_value);
					break;
			};
			break;
		case KEY_OFF:
			printf("key released\r\n");
			break;
		case KEY_HOLD:
			printf("key hold\r\n");
			break;
		case KEY_ERROR:
			printf("key error occur\r\n");
			break;
	}
	
	switch(vatek_key_press(GPIOE, RIGHT_Pin)){
		case KEY_ON:
			printf("key pressed\r\n");
			switch (sequence_val){
				case 0:
					video_type > 0 ? video_type = 1 : video_type++;
					display("%s",video_encoder_list[video_type]);
					break;
				case 1:
					audio_type >= 3 ? audio_type = 3 : audio_type++;
					display("%s",audio_encoder_list[audio_type]);
					break;
				case 2:
					deinterlaced_value > 0 ? deinterlaced_value = 1 : deinterlaced_value++;
					display("%s", deinterlaced_list[deinterlaced_value]);
					break;
				case 3:
					if(bitrate_value >= 20000000){bitrate_value = 20000000;}else{bitrate_value+=1000000;}
					display("%d", bitrate_value);
					break;
				case 4:
					minq_value > 32 ? minq_value = 32 : minq_value++;
					display("%d", minq_value);
					break;
				case 5:
					maxq_value > 32 ? maxq_value = 32 : maxq_value++;
					display("%d",maxq_value);
					break;
				case 6:
					gop_value > 30 ? gop_value = 30 : gop_value++;
					display("%d", gop_value);
					break;
				case 7:
					if(latency_value >= 1000){latency_value = 1000;}else{latency_value+=100;}
					display("%d", latency_value);
					break;
			};
			break;
		case KEY_OFF:
			printf("key released\r\n");
			break;
		case KEY_HOLD:
			printf("key hold\r\n");
			break;
		case KEY_ERROR:
			printf("key error occur\r\n");
			break;
	}
	HAL_Delay(20);
	
}