#ifndef _VATEK_API_
#define _VATEK_API_

    #include "vatek_define.h"
    #include "vatek_define_periphral.h"

#ifdef __cplusplus
    extern "C" {
#endif

    /**
    * @brief API for broadcast service.
    * @defgroup broadcast
    *
    * @defgroup B_control control
    * @brief    API for controlling in broadcast service.    
    * @ingroup  broadcast
    *
    * @defgroup B_status status
    * @brief    API for getting the status in broadcast service.    
    * @ingroup  broadcast
    *
    * @defgroup B_parm_setting parm setting
    * @brief    API for setting parameter in broadcast service.
    * @ingroup  broadcast
    *
    * @defgroup B_psitable psitable
    * @brief    API for registering psi table with psitable in broadcast service.
    * @ingroup  broadcast
    *
    * @defgroup B_psispec_rule psispec_rule
    * @brief    API for registering psi table with psispec_rule in broadcast service.
    * @ingroup  broadcast
    *
    * @defgroup B_psispec_default psispec_default
    * @brief    API for registering psi table with psispec_default in broadcast service.
    * @ingroup  broadcast
    * 
    * @defgroup B_userdata userdata
    * @brief    API for setting user data in broadcast service.   
    * @ingroup  broadcast
    *
    */
    /*------------------------ broadcast ------------------------*/
    /**
    * @brief create broadcast service.
    *
    * @ingroup  B_control
    *
    * @param[in]  hboard    i2c instance handler.
    * @param[out] handle    broadcast service handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_timeout
    * @retval vatek_result_memfail
    * @{
    */
    vatek_result vatek_broadcast_create(Pboard_handle hboard, Phbroadcast *handle);
    /** @} */
    
    /**
    * @brief destroy broadcast service.
    * @details 
    *
    * @ingroup  B_control
    *
    * @param[in] handle    broadcast service handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @{
    */
    vatek_result vatek_broadcast_destroy(Phbroadcast handle);
    /** @} */    
    
    /**
    * @brief start broadcast service.
    * @details 
    *
    * @ingroup  B_control
    *
    * @param[in] handle    broadcast service handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_timeout
    * @retval vatek_result_hwfail
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_start(Phbroadcast handle);
    /** @} */
    
    /**
    * @brief stop broadcast service.
    * @details 
    *
    * @ingroup  B_control
    *
    * @param[in] handle    broadcast service handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_timeout
    * @retval vatek_result_hwfail
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_stop(Phbroadcast handle);
    /** @} */
    
    /**
    * @brief reset broadcast service.
    * @details 
    *
    * @ingroup  B_control
    *
    * @param[in] handle    broadcast service handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_reset( Phbroadcast handle);
    /** @} */
    
    /**
    * @brief get broadcast service status.
    * @details 
    *
    * @ingroup  B_status
    *
    * @param[in] handle     broadcast service handler.
    * @param[out] status    pointer to the broadcast_status enum.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_chipstatus(Phbroadcast handle, chip_status *status);
    /** @} */
    
    /**
    * @brief get chip status.
    * @details 
    *
    * @ingroup  B_status
    *
    * @param[in] handle     broadcast service handler.
    * @param[out] status    pointer to the chip_status enum.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_bcstatus(Phbroadcast handle, broadcast_status *status);
    /** @} */
    
    /**
    * @brief get service info.
    *
    * @ingroup  B_status
    *
    * @param handle      broadcast service handler.
    * @param type        type of support info.
    * @param value       pointer to variable of info.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_getinfo(Phbroadcast handle, broadcast_infotype type, uint32_t *value);
    /** @} */

    /* parm setting */
    /**
    * @brief set input parameters in front-end of the encoder.
    * @details 
    *
    * @ingroup  B_parm_setting
    *
    * @param[in] handle    broadcast service handler.
    * @param[in] vparm     assign value to the video_input_parm structure.
    * @param[in] aprm      assing value to the audio_input_parm structure.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_encoder_setinputparm_phy(Phbroadcast handle, video_input_parm vparm, audio_input_parm aparm);
    /** @} */
    
    /**
    * @brief set input parameter for logo of the encoder.
    * @details 
    *
    * @ingroup  B_parm_setting
    *
    * @param[in] handle    broadcast service handler.
    * @param[in] parm      assign value to the logo_input_parm structure.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm 
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_encoder_setinputparm_logo(Phbroadcast handle, logo_input_parm parm);
    /** @} */    
    
    /**
    * @brief set video and audio encoded parameter of the encoder.
    * @details 
    *
    * @ingroup  B_parm_setting
    *
    * @param[in] handle    broadcast service handler.
    * @param[in] vparm     assign value to the video_encode_parm structure.
    * @param[in] aparm     assign value to the audio_encode_parm structure.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_encoder_setencodeparm(Phbroadcast handle, video_encode_parm vparm, audio_encode_parm aparm);
    /** @} */
    
    /**
    * @brief set video and audio PID parameter of the encoder.
    * @details 
    *
    * @ingroup  B_parm_setting
    *
    * @param[in] handle    broadcast service handler.
    * @param[in] parm      assign value to the psispec_default_parm structure.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_encoder_setmuxparm(Phbroadcast handle, encoder_mux_parm parm);
    /** @} */
    
    /**
    * @brief set the quality parameter of th encoder.
    * @details 
    *
    * @ingroup  B_parm_setting
    *
    * @param[in] handle    broadcast service handler.
    * @param[in] parm      assign value to the encoder_quality_parm structure.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_encoder_setqualityparm(Phbroadcast handle, encoder_quality_parm parm);
    /** @} */
    
    /**
    * @brief set the ts parameter of the muxer.
    * @details 
    *
    * @ingroup  B_parm_setting
    *
    * @param[in] handle    broadcast service handler.
    * @param[in] type      type of psi mode.
    * @param[in] parm      pointer to the tsmux_parm structure.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_tsmux_setparm(Phbroadcast handle, tsmux_type type, Ptsmux_parm parm);
    /** @} */
    
    /**
    * @brief set parameter of the modulater.
    * @details 
    *
    * @ingroup  B_parm_setting
    *
    * @param[in] handle       broadcast service handler.
    * @param[in] parm_base    assign value to the modulator_base_parm structure.
    * @param[in] parm_adv     pointer to the modulator_advanced_parm structure.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_modulator_setparm(Phbroadcast handle, modulator_base_parm parm_base, Pmodulator_advanced_parm parm_adv);
    /** @} */
    
    /**
    * @brief register psi table of the pure mode.
    * @details 
    *
    * @ingroup  B_psitable
    *
    * @param[in] handle    broadcast service handler.
    * @param[in] parm      pointer to the psitablelist_parm structure.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_psitable_register(Phbroadcast handle, Ppsitablelist_parm parm);
    /** @} */
    
    /**
    * @brief insert psi table at one times.
    * @details 
    *
    * @ingroup  B_psitable
    *
    * @param[in] handle          broadcast service handler.
    * @param[in] tspacket_num    ts packet number.
    * @param[in] tspackets       pointer to the tspacket address.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_idle
    * @retval vatek_result_bufoverflow
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_psitable_insert(Phbroadcast handle, uint16_t tspacket_num, uint8_t *tspackets);
    /** @} */
    
    /**
    * @brief initial psi table of license mode.
    * @details 
    *
    * @ingroup  B_psispec_rule
    *
    * @param handle    broadcast service handler.
    * @param rule      pointer to the psispec_rule structure.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_memfail
    * @{
    */
    vatek_result vatek_broadcast_psispec_rule_init(Phbroadcast handle, Ppsispec_rule rule);
    /** @} */
    
    /**
    * @brief set version of the psi table and transmit interval.
    * @details 
    *
    * @ingroup  B_psispec_rule
    *
    * @param[in] handle      broadcast service handler.
    * @param[in] name        set psi table name.
    * @param[in] version     set psi table version.
    * @param[in] interval    set transmission interval. the interval unit is ms.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_overrange
    * @{
    */
    vatek_result vatek_broadcast_psispec_rule_settablerule(Phbroadcast handle, const char *name, uint32_t version, uint32_t interval);
    /** @} */
    
    /**
    * @brief set channel parameter in psi table of license mode.
    *
    * @ingroup  B_psispec_rule
    *
    * @param[in] handle    broadcast service handler.
    * @param[in] name      psi table name.
    * @param[in] buf       set rule.
    * @param[in] len       set rule length.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm 
    * @retval vatek_result_i2cfail 
    * @{
    */
    vatek_result vatek_broadcast_psispec_rule_setchannelrule(Phbroadcast handle, const char *name, void *buf, uint32_t len);
    /** @} */
    
    /**
    * @brief register psi table of license mode.
    *
    * @ingroup  B_psispec_rule
    *
    * @param[in] handle    broadcast service handler.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm 
    * @retval vatek_result_i2cfail 
    * @{
    */
    vatek_result vatek_broadcast_psispec_rule_start(Phbroadcast handle);
    /** @} */
    
	/* psispec_default */
    /**
    * @brief Initialize psi table with default mode.
    *
    * @ingroup  B_psispec_default
    *
    * @param[in] handle     broadcast service handler.
    * @param[in] type       table type of default mode.
    * @param[in] country    country type of default mode.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_memfail
    * @{
    */
    vatek_result vatek_broadcast_psispec_default_init(Phbroadcast handle, psispec_default_type type, psispec_country_code country);
    /** @} */
    
    /**
    * @brief Config psi table with default mode.
    *
    * @ingroup  B_psispec_default
    *
    * @param[in] chan    channel parameter in table type of default mode.
    * @param[in] prog    program parameter in table type of default mode.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_memfail
    * @{
    */
    vatek_result vatek_broadcast_psispec_default_config( Ppsispec_default_channel chan, Ppsispec_default_program prog);
    /** @} */
    
    /**
    * @brief Start psi table with default mode.
    *
    * @ingroup  B_psispec_default
    *
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_busy
    * @retval vatek_result_idle
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_psispec_default_start(void);
    /** @} */
    
    /* userdata */
    /**
    * @brief insert closed caption(CC) to the ts packet.
    * @details 
    *
    * @ingroup  B_userdata
    *
    * @param[in] handle     broadcast service handler.
    * @param[in] cc_num     closed caption number. maximum number is 12.(MAX_USERDATA_CC_NUM)
    * @param[in] cc_parm    pointer to the userdata_cc_parm structure.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_busy
    * @retval vatek_result_idle
    * @retval vatek_result_overrange
    * @retval vatek_result_unsupport
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_broadcast_userdata_insert_cc(Phbroadcast handle, uint16_t cc_num, Puserdata_cc_parm cc_parm);
    /** @} */
    
#if defined (LOG_TABLE)
    /* log dump*/
    vatek_result vatek_broadcast_dump( Phbroadcast handle);
#endif
#if defined (DEBUG)
    vatek_result vatek_broadcast_hal_write(Phbroadcast handle, uint32_t addr, uint32_t val);
    vatek_result vatek_broadcast_hal_read(Phbroadcast handle, uint32_t addr, uint32_t* val);
    vatek_result vatek_broadcast_reg_write(Phbroadcast handle, uint32_t addr, uint32_t val);
    vatek_result vatek_broadcast_reg_read(Phbroadcast handle, uint32_t addr, uint32_t* val);
#endif


    /**
    * @brief API for transform service.
    * @defgroup transform
    *
    * @defgroup A_control control
    * @brief    API for controlling in transform service.    
    * @ingroup  transform
    *
    * @defgroup A_status status
    * @brief    API for getting the status in transform service.    
    * @ingroup  transform
    *
    *
    * @defgroup A_parm_setting parm setting
    * @brief    API for setting config parameter in transform service.
    * @ingroup  transform
    *
    * @defgroup A_psitable psitable
    * @brief    API for registering psi table with psitable in transform service.
    * @ingroup  transform
    *
    * @defgroup A_psispec_rule psispec_rule
    * @brief    API for registering psi table with psispec_rule in transform service.
    * @ingroup  transform
    *
    * @defgroup A_psispec_default psispec_default
    * @brief    API for registering psi table with psispec_default in transform service.
    * @ingroup  transform
    * 
    * @defgroup A_capture_table capture_table
    * @brief    API for getting ts table in transform service.   
    * @ingroup  transform
    *
    * @defgroup A_enum_list enum_list
    * @brief    API for getting ts list in transform service.   
    * @ingroup  transform
    *
    */		
    /*------------------------ transform ------------------------*/
    /* control */
    /**
    * @brief create transform service.
    *
    * @ingroup  A_control
    *
    * @param[in]  hboard    i2c instance handler.
    * @param[out] handle    transform service handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_timeout
    * @retval vatek_result_memfail
    * @{
    */
    vatek_result vatek_transform_create(Pboard_handle hboard, Phtransform *handle);
    /** @{ */
    
    /**
    * @brief destroy transform service.
    * @details 
    *
    * @ingroup  A_control
    *
    * @param[in] handle    transform service handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @{
    */
    vatek_result vatek_transform_destroy(Phtransform handle);
    /** @}*/
    
    /**
    * @brief start transform service.
    * @details 
    *
    * @ingroup  A_control
    *
    * @param[in] handle    transform service handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_timeout
    * @retval vatek_result_hwfail
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_transform_start(Phtransform handle);
    /** @} */
    
    /**
    * @brief stop transform service.
    * @details 
    *
    * @ingroup  A_control
    *
    * @param[in] handle    transform service handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_timeout
    * @retval vatek_result_hwfail
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_transform_stop(Phtransform handle);
    /** @} */
    
    /**
    * @brief reset transform service.
    * @details 
    *
    * @ingroup  A_control
    *
    * @param[in] handle    transform service handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_transform_reset( Phtransform handle);
    /** @} */
    
    /**
    * @brief get transform service status.
    * @details 
    *
    * @ingroup  A_status
    *
    * @param[in] handle     broadcast service handler.
    * @param[out] status    pointer to the broadcast_status enum.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_transform_chipstatus(Phtransform handle, chip_status *status);
    /** @} */
    
    /**
    * @brief get chip status.
    * @details 
    *
    * @ingroup  A_status
    *
    * @param[in] handle     transform service handler.
    * @param[out] status    pointer to the chip_status enum.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_transform_bcstatus(Phtransform handle, broadcast_status *status);
    /** @} */
    
    /**
    * @brief set the ts input parameter.
    * @details 
    *
    * @ingroup  A_parm_setting
    *
    * @param[in] handle   transform service handler.
    * @param[in] parm     assing parameter to the ts_input_parm structure.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_transform_tsp_setinputparm_ts(Phtransform handle, ts_input_parm parm);
    /** @} */
    
    /**
    * @brief set the ts filter paramter.
    * @details 
    *
    * @ingroup  A_parm_setting
    *
    * @param[in] handle   transform service handler.
    * @param[in] parm     assing parameter to the tsp_filter_parm structure.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_transform_tsp_setfilterparm(Phtransform handle, tsp_filter_parm parm);
    /** @} */
    
    /**
    * @brief set the ts parameter of the muxer.
    * @details 
    *
    * @ingroup  A_parm_setting
    *
    * @param[in] handle    transform service handler.
    * @param[in] type      type of psi mode.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_transform_tsmux_setparm(Phtransform handle, tsmux_type type);
    /** @} */
    
    /**
    * @brief set parameter of the modulater.
    * @details 
    *
    * @ingroup  A_parm_setting
    *
    * @param[in] handle       transform service handler.
    * @param[in] parm_base    assign value to the modulator_base_parm structure.
    * @param[in] parm_adv     pointer to the modulator_advanced_parm structure.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_transform_modulator_setparm(Phtransform handle, modulator_base_parm parm_base, Pmodulator_advanced_parm parm_adv);
    /** @} */

    /**
    * @brief register psi table of the pure mode.
    * @details 
    *
    * @ingroup  A_psitable
    *
    * @param[in] handle    transform service handler.
    * @param[in] parm      pointer to the psitablelist_parm structure.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_busy
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_transform_psitable_register(Phtransform handle, Ppsitablelist_parm parm);
    /** @} */
    
    /**
    * @brief insert psi table at one times.
    * @details 
    *
    * @ingroup  A_psitable
    *
    * @param[in] handle          transform service handler.
    * @param[in] tspacket_num    ts packet number.
    * @param[in] tspackets       pointer to the tspacket address.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_idle
    * @retval vatek_result_bufoverflow
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_transform_psitable_insert(Phtransform handle, uint16_t tspacket_num, uint8_t *tspackets);
    /** @} */


    /**
    * @brief initial psi table of license mode.
    * @details 
    *
    * @ingroup  A_psispec_rule
    *
    * @param handle    transform service handler.
    * @param rule      pointer to the psispec_rule structure.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_memfail
    * @{
    */
    vatek_result vatek_transform_psispec_rule_init(Phtransform handle, Ppsispec_rule rule);
    /** @} */
    
    /**
    * @brief set version of the psi table and transmit interval.
    * @details 
    *
    * @ingroup  A_psispec_rule
    *
    * @param[in] handle      transform service handler.
    * @param[in] name        set psi table name.
    * @param[in] version     set psi table version.
    * @param[in] interval    set transmission interval. the interval unit is ms.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_overrange
    * @{
    */
    vatek_result vatek_transform_psispec_rule_settablerule(Phtransform handle, const char *name, uint32_t version, uint32_t interval);
    /** @} */
    
    /**
    * @brief set channel parameter in psi table of license mode.
    *
    * @ingroup  A_psispec_rule
    *
    * @param[in] handle    transform service handler.
    * @param[in] name      psi table name.
    * @param[in] buf       set rule.
    * @param[in] len       set rule length.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm 
    * @retval vatek_result_i2cfail 
    * @{
    */
    vatek_result vatek_transform_psispec_rule_setchannelrule(Phtransform handle, const char *name, void *buf, uint32_t len);
    /** @} */
    
    /**
    * @brief register psi table of license mode.
    *
    * @ingroup  A_psispec_rule
    *
    * @param[in] handle    transform service handler.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm 
    * @retval vatek_result_i2cfail 
    * @{
    */
    vatek_result vatek_transform_psispec_rule_start(Phtransform handle);
    /** @} */
    
    /**
    * @brief add PSIP program table to psi table of license mode.
    *
    * @ingroup  A_psispec_rule
    *
    * @param[in] handle    transform service handler.
    * @param[in] program   pointer to the enum_program structure.
    * @param[in] props     pointer to the psip_props structure.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm 
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail 
    * @{
    */
    vatek_result vatek_transform_psispec_rule_addprogram_psip(Phtransform handle, Penum_program program, Ppsip_props props);
    /** @} */

    /* capture */
    /**
    * @brief capture input ts.
    *
    * @ingroup  A_capture_table
    *
    * @param[in] handle     transform service handler.
    * @param[in] param      caputre param.
    * @param[out] raw_table capture pid context. refer to the psitable_parm struct.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm 
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail 
    * @{
    */
    vatek_result vatek_transform_capture(Phtransform handle, Ppsitable_parm *raw_table, Pcapture_param param);
    /** @} */
    
    /* enum */
    /**
    * @brief emumerate input ts.
    *
    * @ingroup  A_enum_list
    *
    * @param[in] handle    transform service handler.
    * @param[out] list     enumerate program context. refer to the enum_list struct.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm 
    * @retval vatek_result_overrange
    * @retval vatek_result_i2cfail 
    * @{
    */
    vatek_result vatek_transform_enum_getlist(Phtransform handle, Penum_list *list);
    /** @} */
    
    /* psispec_default */
    /**
    * @brief Initialize psi table with default mode.
    *
    * @ingroup  A_psispec_default
    *
    * @param[in] handle     transform service handler.
    * @param[in] type       table type of default mode.
    * @param[in] country    country type of default mode.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_memfail
    * @{
    */
    vatek_result vatek_transform_psispec_default_init(Phtransform handle, psispec_default_type type, psispec_country_code country);
    /** @} */
    
    /**
    * @brief Config channel parameter of the psi table with default mode.
    *
    * @ingroup  A_psispec_default
    *
    * @param[in] handle     transform service handler.
    * @param[in] channel    channel parameter of default mode.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_memfail
    * @{
    */
    vatek_result vatek_transform_psispec_default_channel_config(Ppsispec_default_channel channel);
    /** @} */
    
    /**
    * @brief Config program parameter of the psi table with default mode.
    *
    * @ingroup  A_psispec_default
    *
    * @param[in] handle     transform service handler.
    * @param[in] program    program paramter of default mode.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_memfail
    * @{
    */
    vatek_result vatek_transform_psispec_default_program_add(Ppsispec_default_program program);
    /** @} */
    
    /**
    * @brief End tag with program of the psi table with default mode.
    *
    * @ingroup  A_psispec_default
    *
    * @retval vatek_result_success 
    * @retval vatek_result_memfail
    * @{
    */
    vatek_result vatek_transform_psispec_default_program_end(void);
    /** @} */
    
    /**
    * @brief Config stream parameter of the psi table with default mode.
    *
    * @ingroup  A_psispec_default
    *
    * @param[in] handle     transform service handler.
    * @param[in] stream     stream parameter of default mode.
    * 
    *
    * @retval vatek_result_success 
    * @retval vatek_result_memfail
    * @{
    */
    vatek_result vatek_transform_psispec_default_stream_add(Ppsispec_default_stream stream);
    /** @} */
    
    /**
    * @brief Start psi table with default mode.
    *
    * @ingroup  A_psispec_default
    *
    * @retval vatek_result_success 
    * @retval vatek_result_memfail
    * @{
    */
    vatek_result vatek_transform_psispec_default_start(void);
    /** @} */

    /**
    * @brief API for chip system.
    * @defgroup chip
    */
    /*------------------------ system ------------------------*/
    /**
    * @brief get system tick.
    *
    * @ingroup  chip
    *
    * @retval tick count
    * @{
    */
    uint32_t vatek_system_gettick(void);
    /** @} */
    
    /**
    * @brief set system delay.
    *
    * @ingroup  chip
    *
    * @param[in] msec    set delay ms.
    *
    * @retval vatek_result_success
    * @{
    */
    vatek_result vatek_system_delay(uint32_t msec);
    /** @} */
    
    
    /**
    * @brief get crc32 value.
    *
    * @ingroup  chip
    *
    * @param[in] data    need crc32 data.
    * @param[in] len     data length.
    *
    * @retval vatek_result_success
    * @{
    */
    uint32_t vatek_system_crc32(uint8_t *data, int32_t len);
    /** @} */


    /**
    * @brief API for peripheral device.
    * @details the library is for demoboard reference design.
    * @defgroup peripheral
    *
    * @defgroup phy phy
    * @brief    API for controlling phy peripheral in broadcast service.
    * @details  support phy library, see phy_type enum. 
    * @ingroup  peripheral
    *
    * @defgroup rfmixer rf mixer
    * @brief    API for controlling rfmixer peripheral in broadcast/transform service.
    * @details  support phy library, see rf_type enum.    
    * @ingroup  preipheral
    *
    * @defgroup demod demod
    * @brief    API for controlling demod peripheral in transform service.
    * @details  support phy library, see demod_type enum.    
    * @ingroup  peripheral
    *
    * @defgroup tuner tuner
    * @brief    API for controlling tuner peripheral in transform service.
    * @details  support phy library, see tuner_type enum.    
    * @ingroup  peripheral
    */
    /*------------------------ peripheral ------------------------*/
    /* phy */
    typedef void* Phphy;
    
    /**
    * @brief create front-end media system for broadcast service.
    *
    * @ingroup  phy
    *
    * @param[in]  hboard    i2c instance handler.
    * @param[in]  type      driver type of phy. refer to phy_type enum.
    * @param[out] handle    phy handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_memfail
    * @retval vatek_result_badstatus 
    * @retval vatek_result_overrange
    * @{
    */
    vatek_result vatek_phy_create(Pboard_handle hboard, phy_type type, Phphy *handle);
    /** @} */
    
    /**
    * @brief destory front-end media system for broadcast service.
    *
    * @ingroup  phy
    *
    * @param[in]  handle    phy handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @{
    */
    vatek_result vatek_phy_destroy(Phphy handle);
    /** @} */
    
    /**
    * @brief enable front-end media system for broadcast service.
    *
    * @ingroup  phy
    *
    * @param[in]  handle    phy handler.
    * @param[in]  enable    1 is enable, 0 is disable.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_unsupport
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_phy_enable(Phphy handle, uint8_t enable);
    /** @} */
    
    /**
    * @brief get status for front-end media system.
    *
    * @ingroup  phy
    *
    * @param[in]  handle    phy handler.
    * @param[out]  status   phy system status. refer to phy_status enum.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_unsupport
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_phy_status(Phphy handle, phy_status *status);
    /** @} */
    
    /**
    * @brief set video parameter in front-end media system.
    *
    * @ingroup  phy
    *
    * @param[in]  handle    phy handler.
    * @param[in]  info      video information from front-end media system. refer to phy_video_info struct.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_unsupport
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_phy_setvideoinfo(Phphy handle, Pphy_video_info info);
    /** @} */
    
    /**
    * @brief get video information for front-end media system.
    *
    * @ingroup  phy
    *
    * @param[in]  handle    phy handler.
    * @param[out]  info     video information from front-end media system. refer to phy_video_info struct.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_unsupport
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_phy_getvideoinfo(Phphy handle, Pphy_video_info info);
    /** @} */
    
    /**
    * @brief get audio information for front-end media system.
    *
    * @ingroup  phy
    *
    * @param[in]  handle    phy handler.
    * @param[out]  info     audio information from front-end media system. refer to phy_audio_info struct.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_unsupport
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_phy_getaudioinfo(Phphy handle, Pphy_audio_info info);
    /** @} */
    
    /**
    * @brief get closed caption(CC) data for front-end media system.
    *
    * @ingroup  phy
    *
    * @param[in]  handle    phy handler.
    * @param[out]  cc_parm  closed caption(CC) data. refer to userdata_cc_parm struct.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_unsupport
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_phy_getccdata(Phphy handle, Puserdata_cc_parm cc_parm);
    /** @} */
    
    /**
    * @brief set ouput mode for front-end media system.
    *
    * @ingroup  phy
    *
    * @param[in]  handle    phy handler.
    * @param[in]  mode      set output mode. refer to phy_output_mode struct.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_unsupport
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_phy_setoutputmode(Phphy handle, phy_output_mode mode);
    /** @} */
    
    /**
    * @brief set ouput pclk for front-end media system.
    *
    * @ingroup  phy
    *
    * @param[in]  handle    phy handler.
    * @param[in]  clk       set output pclk. refer to phy_output_baseclock struct.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_badstatus 
    * @retval vatek_result_unsupport
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_phy_setbaseclk(Phphy handle, phy_output_baseclock clk);
    /** @} */

    /* rfmixer */
    typedef void* Phrf;
    
    /**
    * @brief create back-end rf system for broadcast service.
    *
    * @ingroup  rfmixer
    *
    * @param[in]  hboard    i2c instance handler.
    * @param[in]  type      driver type of phy. refer to rf_type enum.
    * @param[out] handle    rf handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_memfail
    * @retval vatek_result_badstatus 
    * @retval vatek_result_overrange
    * @{
    */
    vatek_result vatek_rf_create(Pboard_handle hboard, rf_type type, Phrf *handle);
    /** @} */
    
    /**
    * @brief destory back-end rf system for broadcast service.
    *
    * @ingroup  rfmixer
    *
    * @param[in]  handle    rf handler.
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @{
    */
    vatek_result vatek_rf_destroy(Phrf handle);
    /** @} */
    
    /**
    * @brief start back-end rf system for broadcast service.
    *
    * @ingroup  rfmixer
    *
    * @param[in]  handle    rf handler.
    * @param[out]  freq     rf output frequency. the output unit of frequency is the khz.
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_timeout
    * @retval vatek_result_badstatus 
    * @retval vatek_result_hwfail
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_rf_start(Phrf handle, uint32_t freq);
    /** @} */
    
    /**
    * @brief stop back-end rf system for broadcast service.
    *
    * @ingroup  rfmixer
    *
    * @param[in]  handle    rf handler.
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_timeout
    * @retval vatek_result_badstatus 
    * @retval vatek_result_hwfail
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_rf_stop(Phrf handle);
    /** @} */
    
    /**
    * @brief get statu from back-end rf system.
    *
    * @ingroup  rfmixer
    *
    * @param[in]  handle    rf handler.
    * @param[out]  status    rf system status. refer to rf_status enum.
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_rf_getstatus(Phrf handle, rf_status *status);
    /** @} */
    
    /**
    * @brief calibrate parameter for back-end rf system.
    *
    * @ingroup  rfmixer
    *
    * @param[in]  handle    rf handler.
    * @param[in]  parm      calibrate parameter. refer to r2_calibrate struct.
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_timeout
    * @retval vatek_result_badstatus
    * @retval vatek_result_unsupport
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_rf_setcalibrate(Phrf handle, Prf_calibrate parm);
    /** @} */
    
    /**
    * @brief save calibrate parameter for back-end rf system in chip.
    *
    * @ingroup  rfmixer
    *
    * @param[in]  handle    rf handler.
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_timeout
    * @retval vatek_result_badstatus
    * @retval vatek_result_unsupport
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_rf_savecalibrate(Phrf handle);
    /** @} */

    /* demod */
    typedef void* Phdemod;
    
    /**
    * @brief create front-end demodulation system for transform service.
    *
    * @ingroup  demod
    *
    * @param[in]  hboard    i2c instance handler.
    * @param[in]  type      driver type of demod. refer to demod_type enum.
    * @param[out] handle    demod handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_memfail
    * @retval vatek_result_badstatus 
    * @retval vatek_result_overrange
    * @retval vatek_result_hwfail
    * @{
    */
    vatek_result vatek_demod_create(Pboard_handle hboard, demod_type type, Phdemod *handle);
    /** @} */
    
    /**
    * @brief destory front-end demodulation system for transform service.
    *
    * @ingroup  demod
    *
    * @param[in] handle    demod handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @{
    */
    vatek_result vatek_demod_destroy(Phdemod handle);
    /** @} */
    
    /**
    * @brief enable I2c bypass that MCU control demod system by I2C interface.
    *
    * @ingroup  demod
    *
    * @param[in] handle    demod handler.
    * @param[in] bypasson  enable I2C bypass.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_hwfail
    * @retval vatek_result_unsupport
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_demod_i2cbypass(Phdemod handle, uint8_t bypasson);
    /** @} */
    
    /**
    * @brief lock demodulator signal.
    *
    * @ingroup  demod
    *
    * @param[in]  handle    demod handler.
    * @param[in]  parm      parameter for lock singal. refer to demod_lockparm struct.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_unsupport
    * @retval vatek_result_hwfail
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_demod_lock(Phdemod handle, Pdemod_lockparm parm);
    /** @} */
    
    /**
    * @brief get statu from front-end demodulation system 
    *
    * @ingroup  demod
    *
    * @param[in]  handle    demod handler.
    * @param[out]  status    demod system status. refer to demod_status enum.
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_unsupport
    * @retval vatek_result_hwfail
    * @retval vatek_result_i2cfail
    * @{
    */
    vatek_result vatek_demod_getstatus(Phdemod handle, demod_status *status);
    /** @} */
    
    /* tuner */
    typedef void* Phtuner;
    
    /**
    * @brief create front-end tuner system for transform service.
    *
    * @ingroup  tuner
    *
    * @param[in]  hboard    i2c instance handler.
    * @param[in]  type      driver type of tuner. refer to tuner_type enum.
    * @param[out] handle    tuner handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_memfail
    * @retval vatek_result_badstatus 
    * @retval vatek_result_overrange
    * @retval vatek_result_hwfail
    * @{
    */
    vatek_result vatek_tuner_create(Ptuner_board_handle hboard, tuner_type type, Phtuner *handle);
    /** @} */
    
    /**
    * @brief destory front-end tuner system for transform service.
    *
    * @ingroup  tuner
    *
    * @param[in]  handle    tuner handler.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_hwfail
    * @{
    */
    vatek_result vatek_tuner_destroy(Phtuner handle);
    /** @} */
    
    /**
    * @brief lock tuner signal.
    *
    * @ingroup  tuner
    *
    * @param[in]  handle    tuner handler.
    * @param[in]  parm      parameter for lock singal. refer to tuner_lockparm struct.
    * 
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_unsupport
    * @retval vatek_result_hwfail
    * @{
    */
    vatek_result vatek_tuner_lock(Phtuner handle, Ptuner_lockparm parm);
    /** @} */
    
    /**
    * @brief get statu from front-end tuner system 
    *
    * @ingroup  tuner
    *
    * @param[in]  handle    tuner handler.
    * @param[out]  status    tuner system status. refer to tuner_status enum.
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_unsupport
    * @retval vatek_result_hwfail
    * @{
    */
    vatek_result vatek_tuner_getstatus(Phtuner handle, tuner_status *status);
    /** @} */
    
    /**
    * @brief get rf strength from front-end tuner system 
    *
    * @ingroup  tuner
    *
    * @param[in]  handle    tuner handler.
    * @param[out]  power    rf strength.
    *
    * @retval vatek_result_success 
    * @retval vatek_result_invalidparm
    * @retval vatek_result_unsupport
    * @retval vatek_result_hwfail
    * @{
    */
    vatek_result vatek_tuner_getrfstrength(Phtuner handle, int32_t *power);
    /** @} */
    
/*V1 API temperate save place*/

		vatek_result vatek_encoder_v1_create(Pboard_handle hboard, Phbroadcast *handle);
		vatek_result vatek_encoder_v1_destroy(Phbroadcast handle);
		vatek_result vatek_encoder_v1_start(Phbroadcast handle);
		vatek_result vatek_encoder_v1_stop(Phbroadcast handle);
		vatek_result vatek_encoder_v1_reset(Phbroadcast handle);
		vatek_result vatek_encoder_v1_bcstatus(Phbroadcast handle, broadcast_status *status);
		vatek_result vatek_encoder_v1_chipstatus(Pboard_handle hboard, chip_status *status);
		vatek_result vatek_encoder_v1_getinfo(Phbroadcast handle, broadcast_infotype type, uint32_t *value);
		vatek_result vatek_encoder_v1_setinputparm_phy(Phbroadcast handle, video_input_parm vparm, audio_input_parm aparm);
		vatek_result vatek_encoder_v1_setinputparm_logo(Phbroadcast handle, logo_input_parm parm);
		vatek_result vatek_encoder_v1_setencodeparm(Phbroadcast handle, video_encode_v1_parm vparm, audio_encode_parm aparm);
		vatek_result vatek_encoder_v1_setmuxparm(Phbroadcast handle, encoder_mux_parm parm);
		vatek_result vatek_encoder_v1_setqualityparm(Phbroadcast handle, encoder_quality_parm parm);
		vatek_result vatek_encoder_v1_tsmux_setparm(Phbroadcast handle, tsmux_type type, Ptsmux_parm parm);
		vatek_result vatek_encoder_v1_getvideoinfoparm(Phbroadcast handle, Pvideo_info_parm parm);
		vatek_result vatek_encoder_psitable_register(Phbroadcast handle, Ppsitablelist_parm parm);
		vatek_result vatek_encoder_v1_down_scale(Phbroadcast handle, video_input_parm v_parm, video_scale_resolution vi_res);
		vatek_result vatek_encoder_v1_hdmichange(Pboard_handle handle);
		vatek_result vatek_encoder_psispec_default_init(Phbroadcast handle, psispec_default_type type, psispec_country_code country);
		vatek_result vatek_encoder_psispec_default_config( Ppsispec_default_channel chan, Ppsispec_default_program prog);
		vatek_result vatek_encoder_psispec_default_start(void);
/*V1 API end*/
    
#ifdef __cplusplus
    }
#endif

#endif

