#ifndef _STREAM_BASE_
#define _STREAM_BASE_

#include <core/base/stream_encoder.h>
#include <core/hal/halreg_stream.h>
#include <core/ui/ui_props_define.h>
#include <core/base/chip_define.h>

typedef enum _stream_mode
{
    stream_bypass = STREAM_MODE_BYPASS,
    stream_smooth = STREAM_MODE_SMOOTH,
    stream_remux = STREAM_MODE_REMUX,
    stream_passthrogh = STREAM_MODE_PASSTHROUGH,
}stream_mode;

typedef enum _pcr_adjust_mode
{
    pcr_disable = PCR_MODE_DISABLE,
    pcr_adjust = PCR_MODE_ADJUST,
    pcr_retag = PCR_MODE_RETAG,
}pcr_adjust_mode;

typedef enum _stream_source{
    stream_source_unknown = -1,
    stream_source_null = STREAM_NULL,
    stream_source_usb = STREAM_USB,                     /*!< usb (transform)    */
    stream_source_tsin = STREAM_TSIN,                   /*!< tsin (transform)   */
    stream_source_encoder = STREAM_ENCODER,     		/*!< encoder (broadcast)*/
    stream_source_sine = STREAM_SINE,
    stream_source_test = STREAM_TEST,
}stream_source;

#define is_valid_stream_source(s)       (s >= stream_source_usb && s <= stream_source_test)

#define PID_FILTER_NOT_REPLACE      0x0000

typedef struct _filter_rule
{
    uint16_t orange_pid;
    uint16_t replace_pid;
}filter_rule;

typedef filter_rule* Pfilter_rule;

typedef struct _pid_filter
{
	uint32_t filter_nums;
    filter_rule filters[MUX_FILTER_NAX_NUMS];
}pid_filter;

typedef pid_filter* Ppid_filter;

typedef struct _tsin_param{  
	stream_mode streammode;
    uint32_t ts_flags;
    pcr_adjust_mode pcrmode;
}tsin_param;

typedef tsin_param* Ptsin_param;

static const tsin_param default_tsin_param = {stream_remux,0,pcr_disable};

#define is_ts_serial(flags) ((flags & TS_PIN_SERIAL) == TS_PIN_SERIAL)

typedef struct _usb_param{
	stream_mode mode;
	uint32_t usb_flags;			/* HALREG_USB_STREAM_FLAGS */
    pcr_adjust_mode pcrmode;
}usb_param;

typedef usb_param* Pusb_param;

static const usb_param default_usb_param = { stream_remux,0,pcr_disable};

#ifdef __cplusplus
extern "C" {
#endif
    
    HAL_API const Pui_prop_item stream_source_get_ui_props(stream_source source);
    HAL_API vatek_result stream_source_reset(vatek_ic_module icmodule,stream_source source, uint8_t* prawsource);
    HAL_API vatek_result stream_source_set(hvatek_chip hchip,stream_source source, uint8_t* prawsource);
    HAL_API vatek_result stream_source_get(hvatek_chip hchip,stream_source source, uint8_t* prawsource);

#ifdef __cplusplus
}
#endif

#endif
