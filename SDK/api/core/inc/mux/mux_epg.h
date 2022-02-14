
#ifndef _MUX_EPG_
#define _MUX_EPG_

#include <mux/mux_time.h>

/* first section started at current hour (default start at 00:00:00)*/
#define EPG_FLAG_START_HOUR				0x00000001	
#define EPG_TITLE_MAX_LEN				64
#define EPG_DESC_MAX_LEN				64
#define EPG_CONTENT_MAX_LEN				256

typedef enum _mux_epg_mode{
	epg_mode_dvb = 0,
	epg_mode_arib_0 = 1,
	epg_mode_arib_1 = 2,
}mux_epg_mode;

#define  is_current_mux_epg_mode(mode)	(mode >= epg_mode_dvb && mode <= epg_mode_arib_1)

typedef enum _mux_content_type{
	mux_content_undefine = 0,
	mux_content_news = 1,
	mux_content_sport = 2,
	mux_content_movie = 3,
	mux_content_drama = 4,
	mux_content_music = 5,
	mux_content_tabloidshow = 6,
	mux_content_varietyshow = 7,
	mux_content_animation = 8,
	mux_content_documentary = 9,
	mux_content_performance = 10,
	mux_content_education = 11,
	mux_content_welfare = 12,
	mux_content_others = 0xFF,
}mux_content_type;

typedef enum _mux_parental_rating{
	rating_disable = 0,
	rating_general = 1,
	rating_guidance = 2,
	rating_restricted = 3,
}mux_parental_rating;

typedef struct _mux_epg_section{
	struct _mux_epg_section* next;
	uint8_t duration;
	uint8_t recv[3];
	mux_content_type type;
	mux_parental_rating parental_rating;
	mux_string title;
	mux_string content;
}mux_epg_section;

typedef mux_epg_section* Pmux_epg_section;

typedef struct _mux_epg_desc{
	struct _mux_epg_desc* next;
	uint32_t tag;
	uint32_t len;
	uint8_t desc_buf[EPG_DESC_MAX_LEN];
}mux_epg_desc;

typedef mux_epg_desc* Pmux_epg_desc;

typedef struct _mux_epg_param{
	mux_epg_mode mode;
	uint32_t epg_flags;
	mux_time start_time;
	uint16_t event_id;
	uint8_t days;
	uint8_t recv;
	uint32_t loop_ms;
	Pmux_epg_section sections;
	Pmux_epg_desc descriptors;
}mux_epg_param;

typedef mux_epg_param* Pmux_epg_param;

/* first section started at current hour (default start at 00:00:00)*/
#define EPG_FLAG_START_HOUR			0x00000001	

#ifdef __cplusplus
extern "C" {
#endif

	HAL_API vatek_result muxepg_create(Pth_mempool pmem, mux_epg_mode mode, Pmux_epg_param* pepg);
	HAL_API vatek_result muxepg_create_desc(Pth_mempool pmem, Pmux_epg_param pepg, Pmux_epg_desc* pdesc);
	HAL_API vatek_result muxepg_check(Pmux_epg_param pepg);

#ifdef __cplusplus
}
#endif

#endif
