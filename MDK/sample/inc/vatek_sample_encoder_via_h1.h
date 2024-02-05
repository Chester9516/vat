#include "stdio.h"
#include "vatek_define.h"


#define log(fmt,...) printf("LOG: [%s-%d]"fmt"\r\n",__func__,__LINE__,##__VA_ARGS__)
#define errlog(fmt,...) printf("ERRLOG: [%s-%d]"fmt"\r\n",__func__,__LINE__,##__VA_ARGS__)





vatek_result vatek_sample_encoder_start(Pboard_handle handle); //chip initial
vatek_result vatek_encoder_v1_polling();

