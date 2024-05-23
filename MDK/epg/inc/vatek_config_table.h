
#ifndef _HALREG_CONFIG_TABLE_
#define _HALREG_CONFIG_TABLE_

#include <stdint.h>
#include <vatek_define.h>
#include <hal\halservice_broadcast.h>

static halreg_table halreg_config_table[41] = {
/*	input config */
//	{ HALREG_BROADCAST_STREAM         , 0x00000003,}, /* source               - stream - stream source*/
/*                                                       -test_mode            - 0x00000000 */
/*                                                       -usb                  - 0x00000001 */
/*                                                       -ts_input             - 0x00000002 */
/*                                                       -encoder              - 0x00000003 */
//	{ HALREG_MUX_PCR_PID            , 0x00000100,}, /* pcr_pid              - remux - pcr pid*/
//	{ HALREG_MUX_PADDING_PID        , 0x00001fff,}, /* padding_pid          - remux - padding pid*/
//	{ HALREG_MUX_BITRATE            , 0x00000000,}, /* bitrate              - remux - bitrate (bps)*/
//	{ HALREG_ENCODER_MODE             , 0x00000000,}, /* mode                 - enc - encoder mode*/
/*                                                       -colorbar             - 0x00000000 */
/*                                                       -bootlogo             - 0x00000001 */
/*                                                       -vi_0                 - 0x00000002 */
/*                                                       -test_vi_0            - 0x00000003 */
/*                                                       -test_enc_frame       - 0x00000004 */
/*                                                       -test_enc_gop         - 0x00000005 */
//	{ HALREG_ENCODER_FLAGS            , 0x00000000,}, /* encoder_flags        - enc - function flag*/
/*                                                       -en_software_pcr      - 0x00000001 */
/*                                                       -en_interlaced_frame  - 0x00000002 */
/*                                                       -en_progressive_2_i   - 0x00000004 */
/*                                                       -disable_adts_crc     - 0x00000008 */
//	{ HALREG_ENCODER_TAG              , 0x00000000,}, /* encoder_tag          - enc - resource tag*/
//	{ HALREG_VI_0_FLAGS               , 0x00000050,}, /* vi_flags             - vi - function flags*/
/*                                                       -en_dataline_16       - 0x00000010 */
/*                                                       -en_separated_sync    - 0x00000040 */
/*                                                       -en_clk_negative      - 0x00000200 */
/*                                                       -en_hsync_negative    - 0x00000400 */
/*                                                       -en_vsync_negative    - 0x00001000 */
/*                                                       -en_half_fps          - 0x10000000 */
//	{ HALREG_VI_0_PIXELCLOCK          , 0x00024414,}, /* vi_pixelclk          - vi - pixel clock (KHz)*/
//	{ HALREG_VI_0_OFFSET_X            , 0x00000000,}, /* offset_x             - vi - offset x*/
//	{ HALREG_VI_0_OFFSET_Y            , 0x00000000,}, /* offset_y             - vi - offset y*/
//	{ HALREG_VIDEO_CODEC              , 0x00000000,}, /* vcodec               - video - codec*/
/*                                                       -mpeg2                - 0x00000000 */
/*                                                       -h264                 - 0x00000001 */
/*                                                       -hevc                 - 0x00000002 */
//	{ HALREG_VIDEO_RESOLUTION         , 0x00000006,}, /* resolution           - video - resolution*/
/*                                                       -1080p                - 0x00000006 */
/*                                                       -1080i                - 0x00000005 */
/*                                                       -720p                 - 0x00000004 */
/*                                                       -576p                 - 0x00000003 */
/*                                                       -576i                 - 0x00000002 */
/*                                                       -480p                 - 0x00000001 */
/*                                                       -480i                 - 0x00000000 */
//	{ HALREG_VIDEO_FRAMERATE          , 0x00000007,}, /* framerate            - video - frame rate*/
/*                                                       -23.97                - 0x00000000 */
/*                                                       -24                   - 0x00000001 */
/*                                                       -25                   - 0x00000002 */
/*                                                       -29.97                - 0x00000003 */
/*                                                       -30                   - 0x00000004 */
/*                                                       -50                   - 0x00000005 */
/*                                                       -59.94                - 0x00000006 */
/*                                                       -60                   - 0x00000007 */
//	{ HALREG_VIDEO_ASPECTRATE         , 0x00000001,}, /* aspectrate           - video - aspect rate*/
/*                                                       -4_3                  - 0x00000000 */
/*                                                       -16_9                 - 0x00000001 */
//	{ HALREG_AUDIO_CODEC              , 0x00000001,}, /* acodec               - audio - codec type*/
/*                                                       -mp1_l2               - 0x00000000 */
/*                                                       -aac_lc_adts          - 0x00000001 */
/*                                                       -ac_3                 - 0x00000002 */
/*                                                       -aac_lc_latm          - 0x00000003 */
//	{ HALREG_AUDIO_SAMPLERATE         , 0x00000002,}, /* samplerate           - audio - sample rate*/
/*                                                       -ss_32                - 0x00000000 */
/*                                                       -ss_44_1              - 0x00000001 */
/*                                                       -ss_48                - 0x00000002 */
//	{ HALREG_AUDIO_CHANNEL            , 0x00000001,}, /* channel              - audio - channel*/
/*                                                       -mute                 - 0x00000000 */
/*                                                       -stereo               - 0x00000001 */
/*                                                       -mono_L               - 0x00000002 */
/*                                                       -mono_R               - 0x00000003 */
/*                                                       -stereo_mono_L        - 0x00000004 */
/*                                                       -stereo_mono_R        - 0x00000005 */
//	{ HALREG_QUALITY_RCMODE           , 0x00000001,}, /* rcmode               - quality - rate control mode*/
/*                                                       -rc_vbr               - 0x00000001 */
/*                                                       -rc_fixed_q           - 0x00000002 */
//	{ HALREG_QUALITY_MINQ             , 0x00000003,}, /* minq                 - quality - min quant*/
//	{ HALREG_QUALITY_MAXQ             , 0x0000000a,}, /* maxq                 - quality - max quant*/
//	{ HALREG_QUALITY_GOP              , 0x00000010,}, /* goplen               - quality - gop length*/
//	{ HALREG_QUALITY_LATENCY          , 0x000001f4,}, /* latency              - quality - latency (ms)*/
//	{ HALREG_QUALITY_BITRATE          , 0x0121eac0,}, /* bitrate              - quality - bitrate (bps)*/
/*	output config */
//	{ HALREG_BROADCAST_MODE         , 0x00000000,}, /* output               - output - output mode*/
/*                                                       -modulator            - 0x00000000 */
/*                                                       -sinewave             - 0x00000001 */
/*                                                       -ts_out               - 0x00000002 */
//	{ HALREG_MOD_MODE                 , 0x00000005,}, /* type                 - modulator - modulator type*/
/*                                                       -dvb_t                - 0x00000000 */
/*                                                       -j83a                 - 0x00000001 */
/*                                                       -atsc                 - 0x00000002 */
/*                                                       -j83b                 - 0x00000003 */
/*                                                       -dtmb                 - 0x00000004 */
/*                                                       -isdb_t               - 0x00000005 */
/*                                                       -j83c                 - 0x00000006 */
/*                                                       -dvb_t2               - 0x00000007 */
//	{ HALREG_MOD_BW_SB                , 0x00000006,}, /* bandwidth_symbolrate - modulator - symbol rate (ksps) or bandwidth(MHz)*/
//	{ HALREG_MOD_IFMODE               , 0x00000001,}, /* ifmode               - dac_if - dac if mode*/
/*                                                       -disable              - 0x00000001 */
/*                                                       -if_freq              - 0x00000002 */
/*                                                       -iq_offset            - 0x00000003 */
//	{ HALREG_MOD_IFFREQ               , 0x00000000,}, /* iffreq_offset        - dac_if - frequancy*/
//	{ HALREG_MOD_DACGAIN              , 0x00000000,}, /* dac_gain             - dac_if - dac gain control*/
//	{ HALREG_ISDB_T_CONSTELLATION     , 0x00000003,}, /* constellation        - isdb-t - constellation*/
/*                                                       -qpsk                 - 0x00000001 */
/*                                                       -qam_16               - 0x00000002 */
/*                                                       -qam_64               - 0x00000003 */
//	{ HALREG_ISDB_T_FFT               , 0x00000001,}, /* fft                  - isdb-t - fft mode*/
/*                                                       -fft_2k               - 0x00000000 */
/*                                                       -fft_8k               - 0x00000001 */
/*                                                       -fft_4k               - 0x00000002 */
//	{ HALREG_ISDB_T_GUARDINTERVAL     , 0x00000001,}, /* guardinterval        - isdb-t - guard interval*/
/*                                                       -gi_1_32              - 0x00000000 */
/*                                                       -gi_1_16              - 0x00000001 */
/*                                                       -gi_1_8               - 0x00000002 */
/*                                                       -gi_1_4               - 0x00000003 */
//	{ HALREG_ISDB_T_CODERATE          , 0x00000003,}, /* coderate             - isdb-t - code rate*/
/*                                                       -cr_1_2               - 0x00000000 */
/*                                                       -cr_2_3               - 0x00000001 */
/*                                                       -cr_3_4               - 0x00000002 */
/*                                                       -cr_5_6               - 0x00000003 */
/*                                                       -cr_7_8               - 0x00000004 */
//	{ HALREG_ISDB_T_TIME_INTERLEAVED  , 0x00000003,}, /* timeinterleaved      - isdb-t - time interleaved mode*/
/*                                                       -disable              - 0x00000000 */
/*                                                       -mode_1               - 0x00000001 */
/*                                                       -mode_2               - 0x00000002 */
/*                                                       -mode_3               - 0x00000003 */
/*	mux_psi config */
//	{ HALREG_MUXPSI_MODE                 , 0x00000002,}, /* mode                 - mux - mux mode*/
/*                                                       -pure                 - 0x00000000 */
/*                                                       -iso_13818            - 0x00000001 */
/*                                                       -dtv_spec             - 0x00000002 */
	{ HALREG_ENCODER_PMTPID               , 0x00001001,}, /* pmt_pid              - mux - pmt_pid*/
//	{ HALREG_VIDEO_PID             , 0x00001002,}, /* video_pid            - mux - video_pid*/
//	{ HALREG_AUDIO_PID             , 0x00001004,}, /* audio_pid            - mux - audio_pid*/
//	{ HALREG_MUXPSI_ISO_TSID             , 0x00000020,}, /* transport_stream_id  - mux - iso tsid*/
//	{ HALREG_MUXPSI_ISO_PROGRAM_NUM      , 0x00000005,}, /* program_number       - mux - iso program_num*/
};
#define HALREG_TABLE_SIZE (sizeof(halreg_config_table)/sizeof(halreg_table))
#define HALREG_TABLE	halreg_config_table
/*-- halreg_config_table --*/




#endif

