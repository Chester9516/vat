
#include "vatek_system.h"
#include "vatek_hms.h"
#include "vatek_userdata.h"

typedef struct
{
    uint32_t user_data_start_code:32;   //patterned bslbf   0x000001B2
    uint32_t user_identifier:32;        //ASCII bslbf   GA94
    uint32_t user_data_type_code:8;     //3
    uint8_t process_em_data_flag:1;     //default :1
    uint8_t process_cc_data_flag:1;     //default :1
    uint8_t additional_data_flag:1;     //default :0
    uint8_t cc_count:5;
    uint8_t em_data:8;                  //default :255
    Puserdata_cc_parm cc_data_pkts;
    uint8_t marker_bits:8;              //default :255
}cc_mpeg2; //cea708

typedef struct
{
    uint32_t start_code:32;             //0x00000001
    uint32_t nal_unit:8;                //6 for h264
    uint32_t payloadType:8;             //4
    uint32_t payloadSize:8;             //sizeof user_data
    uint32_t itu_t_t35_country_code:8;  //181
    uint32_t itu_t_t35_provider_code:16;//49 or 47
    uint32_t user_identifier:32;        //only provider is 49,  ASCII = GA94
    uint32_t user_data_type_code:8;     //3
    uint32_t DIRECTTV_user_data_length:8;   //only provider is 47
    uint8_t process_em_data_flag:1;     //default :1
    uint8_t process_cc_data_flag:1;     //default :1
    uint8_t additional_data_flag:1;     //default :0
    uint8_t cc_count:5;
    uint8_t em_data:8;                  //default :255
    Puserdata_cc_parm cc_data_pkts;
    uint8_t marker_bits:8;              //default :255
    uint32_t rbsp_trailing_bits:8;
}cc_h264; //cea708

static video_encode_type enctype = ve_type_mpeg2;

vatek_result vatek_userdata_setvideocodec(video_encode_type type)
{
    vatek_result result = vatek_result_success;

    if (type > ve_type_max || type < ve_type_min)
        return vatek_result_overrange;

    enctype = type;
    if (type == ve_type_unknown)
        enctype = ve_type_mpeg2;

    return result;
}

vatek_result vatek_userdata_raw_insert(Phms_handle handle, uint16_t data_num, uint8_t *data)
{
    vatek_result result = vatek_result_unknown;
    //uint8_t buf[ENC_EXTDATA_MAXLEN] = {0};
    //uint8_t *pos = buf;
    
    if (handle == NULL || data == NULL || data_num == 0)
        return vatek_result_invalidparm;

    if (data_num > MAX_USERDATA_RAW_NUM)
        return vatek_result_overrange;
    
    return result;
}

vatek_result vatek_userdata_cc_insert(Phms_handle handle, uint16_t cc_num, Puserdata_cc_parm cc)
{
    vatek_result result = vatek_result_unknown;
    uint32_t val = 0;
    uint8_t buf[ENC_EXTDATA_MAXLEN] = {0};
    uint8_t *pos = buf;
    cc_mpeg2 cea708_mp2;
    cc_h264 cea708_h264;
    uint32_t idx = 0, len = 0, cmd = 0;

    if (handle == NULL || cc == NULL || cc_num == 0)
        return vatek_result_invalidparm;

    if (cc_num > MAX_USERDATA_CC_NUM)
        return vatek_result_overrange;

    if (vatek_hms_issystemrun(handle) != vatek_result_success)
        return vatek_result_idle;  

    if ((result = vatek_hms_read_hal(handle, HALREG_ENC_EXTDATA, &val)) != vatek_result_success)
        return result;

    if (result != 0)
    {
        USERDATA_ERR("previous insert doesn't finish");
        return vatek_result_busy;
    }

    switch (enctype)
    {
        case ve_type_mpeg2:
            {
                cea708_mp2.user_data_start_code = 0x000001B2;
                cea708_mp2.user_identifier = 0x47413934; //GA94
                cea708_mp2.user_data_type_code = 3; 
                cea708_mp2.process_em_data_flag = 1;
                cea708_mp2.process_cc_data_flag = 1;
                cea708_mp2.additional_data_flag = 0;
                cea708_mp2.cc_count = cc_num;
                cea708_mp2.em_data = 0xFF;
                cea708_mp2.cc_data_pkts = cc;
                cea708_mp2.marker_bits = 0xFF;

                *pos++ = (cea708_mp2.user_data_start_code >> 24) & 0xFF;
                *pos++ = (cea708_mp2.user_data_start_code >> 16) & 0xFF;
                *pos++ = (cea708_mp2.user_data_start_code >> 8 ) & 0xFF;
                *pos++ = cea708_mp2.user_data_start_code  & 0xFF;   
                *pos++ = (cea708_mp2.user_identifier >> 24) & 0xFF;
                *pos++ = (cea708_mp2.user_identifier >> 16 ) & 0xFF;
                *pos++ = (cea708_mp2.user_identifier >> 8) & 0xFF;
                *pos++ = cea708_mp2.user_identifier & 0xFF;
                *pos++ = cea708_mp2.user_data_type_code;
                *pos++ = (cea708_mp2.process_em_data_flag << 7) | 
                         (cea708_mp2.process_cc_data_flag << 6) |
                         (cea708_mp2.additional_data_flag << 5) |
                         (cea708_mp2.cc_count & 0x1F);
                *pos++ = cea708_mp2.em_data;
                for (idx = 0; idx < (cea708_mp2.cc_count) ; idx++)
                {
                    *pos++ = cea708_mp2.cc_data_pkts[idx].attr;
                    *pos++ = cea708_mp2.cc_data_pkts[idx].ccdata1;
                    *pos++ = cea708_mp2.cc_data_pkts[idx].ccdata2;
                }
                *pos++ = cea708_mp2.marker_bits;
            }
            break;

        case ve_type_h264:
            {
                cea708_h264.start_code =  0x00000001;
                cea708_h264.nal_unit = 6;
                cea708_h264.payloadType = 4;
                cea708_h264.payloadSize = cc_num*3 + 12; //3 = sizeof(cc_construct) 12 = follwing header size
                cea708_h264.itu_t_t35_country_code = 181;
                cea708_h264.itu_t_t35_provider_code = 49;
                cea708_h264.user_identifier = 0x47413934; //GA94
                cea708_h264.user_data_type_code = 3;
                cea708_h264.process_em_data_flag = 1;
                cea708_h264.process_cc_data_flag = 1;
                cea708_h264.additional_data_flag = 0;
                cea708_h264.cc_count = cc_num;
                cea708_h264.em_data = 0xFF;
                cea708_h264.cc_data_pkts = cc;
                cea708_h264.marker_bits = 0xFF;
                cea708_h264.rbsp_trailing_bits = 0x80;


                *pos++ = (cea708_h264.start_code >> 24) & 0xFF;
                *pos++ = (cea708_h264.start_code >> 16 ) & 0xFF;
                *pos++ = (cea708_h264.start_code >> 8) & 0xFF;
                *pos++ = cea708_h264.start_code & 0xFF;   
                *pos++ = cea708_h264.nal_unit;
                *pos++ = cea708_h264.payloadType;
                *pos++ = cea708_h264.payloadSize;
                *pos++ = cea708_h264.itu_t_t35_country_code;
                *pos++ = (cea708_h264.itu_t_t35_provider_code >> 8 ) & 0xFF;
                *pos++ = cea708_h264.itu_t_t35_provider_code & 0xFF;
                *pos++ = (cea708_h264.user_identifier >> 24) & 0xFF;
                *pos++ = (cea708_h264.user_identifier >> 16 ) & 0xFF;
                *pos++ = (cea708_h264.user_identifier >> 8) & 0xFF;
                *pos++ = cea708_h264.user_identifier & 0xFF;
                *pos++ = cea708_h264.user_data_type_code;
                *pos++ = (cea708_h264.process_em_data_flag << 7) | 
                         (cea708_h264.process_cc_data_flag << 6) |
                         (cea708_h264.additional_data_flag << 5) |
                         (cea708_h264.cc_count & 0x1F);
                *pos++ = cea708_h264.em_data;
                for (idx = 0; idx < (cea708_h264.cc_count) ; idx ++)
                {
                    *pos++ = cea708_h264.cc_data_pkts[idx].attr;
                    *pos++ = cea708_h264.cc_data_pkts[idx].ccdata1;
                    *pos++ = cea708_h264.cc_data_pkts[idx].ccdata2;
                }
                *pos++ = cea708_h264.marker_bits;
                *pos++ = cea708_h264.rbsp_trailing_bits;
            }
            break;
            
        default:
            USERDATA_ERR("unsupport encode type");
            return vatek_result_unsupport;
    }

    len = pos-buf;
    cmd = ENC_EXTDATA_TAG | (ENC_EXTDATA_LEN_MASK & (len));
    len = ((len >> 2) + 1) << 2; //4-bytes aligned
    
	if ((result = vatek_hms_write_halbuf(handle, HALREG_EN_EXTDATA_START, buf, len)) != vatek_result_success)
        return result;
    
    if ((result = vatek_hms_write_hal(handle, HALREG_ENC_EXTDATA, cmd)) != vatek_result_success)
        return result;

    return result;
}

