#ifndef _VATEK_SDK_BROADCAST_
#define _VATEK_SDK_BROADCAST_

#include <service/service_broadcast.h>

typedef void* hvatek_broadcast;
typedef void* hvatek_aux;

typedef int32_t(*fpaux_check_update)(hvatek_aux haux);
typedef vatek_result(*fpaux_get_packets)(hvatek_aux haux, uint8_t* ppktbuf, int32_t pktnums);

typedef struct _broadcast_auxstream
{
    hvatek_aux haux;
    usbparam_aux aux;
    fpaux_check_update check_update;
    fpaux_get_packets get_packets;
}broadcast_auxstream;

typedef broadcast_auxstream* Pbroadcast_auxstream;

#ifdef __cplusplus
extern "C" {
#endif

    HAL_API vatek_result vatek_broadcast_check_auxstream(hvatek_chip hchip);
    HAL_API vatek_result vatek_broadcast_open(hvatek_chip hchip,hvatek_broadcast* hbc);

    HAL_API Pbroadcast_info vatek_broadcast_get_info(hvatek_broadcast hbc);

    HAL_API vatek_result vatek_broadcast_start(hvatek_broadcast hbc, Pbroadcast_param pbcparam, Pbroadcast_auxstream paux,uint32_t freqkhz);
    HAL_API vatek_result vatek_broadcast_polling(hvatek_broadcast hbc, Pbroadcast_info* pinfo);
    HAL_API vatek_result vatek_broadcast_stop(hvatek_broadcast hbc);

    HAL_API void vatek_broadcast_close(hvatek_broadcast hbc);

#ifdef __cplusplus
}
#endif

#endif
