#include "vatek_api.h"

#define SELF_ERR(fmt,...)    printf("self sample  :  error [%s-%d]"fmt"\r\n",__func__ ,__LINE__,##__VA_ARGS__)
#define SELF_LOG(fmt,...)    printf("self sample  :  "fmt"\r\n",##__VA_ARGS__)

vatek_result a_modulator_select(Phtransform tf_handle, modulator_type m_type,uint32_t bandwidth);
vatek_result b_modulator_select(Phbroadcast bc_handle, modulator_type m_type,uint32_t bandwidth);