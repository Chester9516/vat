

#ifndef _TOOL_HDMI_
#define _TOOL_HDMI_

#ifdef __cplusplus
    extern "C" {
#endif

    #define EP9XXX_VTIME_SIZE           13
    #define EP9XXX_VINFO_SIZE           15
    #define EP9XXX_AINFO_SIZE           7

    #define EP9XXX_AUDIO_32KHZ          0
    #define EP9XXX_AUDIO_44_1KHZ        1
    #define EP9XXX_AUDIO_48KHZ          2

    /* CEA-861-D P65 */
    #define VINFO_COLORSPACE(a)         ((a[2] >> 5) & 0x03)
    #define VINFO_SCAN_MODE(a)          (a[2] & 0x03)
    #define VINFO_IS_ASPECT(a)          ((a[2] >> 4) & 0x01)
    #define VINFO_IS_VBAR(a)            ((a[2] >> 3) & 0x01)
    #define VINFO_IS_HBAR(a)            ((a[2] >> 2) & 0x01)
    #define VINFO_COLORIMETRY(a)        ((a[3] >> 6) & 0x03)
    #define VINFO_PASPECT(a)            ((a[3] >> 4) & 0x03)
    #define VINFO_AASPECT(a)            (a[3] & 0x0F)
    #define VINFO_EX_COLORIMETRY(a)	    ((a[4] >> 4) & 0x07)
    #define VINFO_QUANT_RANGE(a)        ((a[4] >> 2) & 0x03)
    #define VINFO_NUPS(a)               (a[4] & 0x03)
    #define VINFO_ITC(a)                ((a[4] >> 7) & 0x01)
    #define VINFO_VIDEOCODE(a)          (a[5] & 0x7F)
    #define VINFO_YCC_QUANT_RANGE(a)    ((a[6] >> 6) & 0x03)
    #define VINFO_CONTENT_TYPE(a)       ((a[6] >> 4) & 0x03)
    #define VINFO_PIXEL_REPEAT(a)       (a[6] & 0x0F)
    #define VINFO_TOPBAR(a)			    (a[6] | (a[7] << 8))
    #define VINFO_BOTTOMBAR(a)          (a[8] | (a[9] << 8))
    #define VINFO_LEFTBAR(a)            (a[10] | (a[11] << 8))
    #define VINFO_RIGHTBAR(a)           (a[11] | (a[12] << 8))

//    typedef struct _ep9xxx_video_info
//    {
//        uint8_t vid;
//        uint8_t represent;
//        video_input_resolution resolution;
//        video_input_aspectrate aspectrate;
//    }ep9xxx_video_info, *Pep9xxx_video_info;

    #define PIXEL_REPETITION_0  0x00
    #define PIXEL_REPETITION_2  0x01
    #define PIXEL_REPETITION_4  0x03

    #define is_current_repetition(n)    (n == PIXEL_REPETITION_0 || n == PIXEL_REPETITION_2 || n == PIXEL_REPETITION_4)
    #define is_1080i(res)               (res==vi_resolution_1080i60 || res==vi_resolution_1080i59_94 || res==vi_resolution_1080i50)
    #define is_1080p(res)               (res==vi_resolution_1080p60 || res==vi_resolution_1080p59_94 || res==vi_resolution_1080p50 || res==vi_resolution_1080p30 || \
                                         res==vi_resolution_1080p25 || res==vi_resolution_1080p24 || res==vi_resolution_1080p23_97)
    #define is_1080(res)                (is_1080i(res) || is_1080p(res))
    #define is_720(res)                 (res==vi_resolution_720p60 || res==vi_resolution_720p59_94 || res==vi_resolution_720p50)
    #define is_576i(res)                (res==vi_resolution_576i50)
    #define is_576p(res)                (res==vi_resolution_576p50 || res==vi_resolution_576p25)
    #define is_576(res)                 (is_576i(res) || is_576p(res))
    #define is_480i(res)                (res==vi_resolution_480i60 || res==vi_resolution_480i59_94)
    #define is_480p(res)                (res==vi_resolution_480p60 || res==vi_resolution_480p59_94 || res==vi_resolution_480p30 || res==vi_resolution_480p29_97)
    #define is_480(res)                 (is_480i(res) || is_480p(res))
    
		#define is_1440i(res)								(res == vi_resolution_1440i60)
		#define is_1440p(res)								(res == vi_resolution_1440p60)
		#define is_1440(res)								(is_1440i(res) || is_1440p(res))
		#define is_interlaced(res)          (res==vi_resolution_1080i60 || res==vi_resolution_1080i50 || res==vi_resolution_1080i59_94 || \
                                         res==vi_resolution_576i50  || res==vi_resolution_480i59_94 || res==vi_resolution_1440i60) // || res==vi_resolution_1440i30
		

    #define VID_480P_59_4_3     0x02
    #define VID_480P_59_16_9    0x03   
    #define VID_480I_59_4_3     0x06
    #define VID_480I_59_16_9    0x07

    #define VID_576P_50_4_3     0x11
    #define VID_576P_50_16_9    0x12
    #define VID_1080I_50        0x14
    #define VID_576I_50_4_3     0x15
    #define VID_576I_50_16_9    0x16

    #define VID_720P_60         0x04
		#define VID_720P_59_94      0x04
    #define VID_1080I_60        0x05
    #define VID_1080I_59_94     0x05
    #define VID_720P_50         0x13

    #define VID_1080P_60        0x10
    #define VID_1080P_59_94     0x10
    #define VID_1080P_50        0x1F
    #define VID_1080P_25        0x21
    #define VID_1080P_30        0x22
		#define VID_1080P_29_97			0x22
    #define VID_1080P_24        0x20
		
		#define VID_1440P_60				0x46
		#define VID_1440I_60				0x45
		#define VID_1440P_59_94			0x46
		#define VID_1440I_59_94			0x45
    
    typedef struct _hdmi_video_id
    {
        uint8_t vid;
        uint8_t represent;
        video_input_resolution resolution;
        video_input_aspectrate aspectrate;
        uint32_t pixelclk;
        uint16_t hfp;
        uint16_t hpw;
        uint16_t hbp;
        uint8_t vfp;
        uint8_t vbp;
        uint8_t vpw;
    }hdmi_video_id,*Phdmi_video_id;
    
    static const hdmi_video_id hdmi_video_table[] =
    {
				{VID_1080P_60       ,PIXEL_REPETITION_0  ,vi_resolution_1080p60   ,vi_aspectrate_16_9,148500,88,44,148,4,36,5,},
        {VID_1080P_59_94    ,PIXEL_REPETITION_0  ,vi_resolution_1080p59_94,vi_aspectrate_16_9,148351,88,44,148,4,36,5,},
				{VID_1080P_50       ,PIXEL_REPETITION_0  ,vi_resolution_1080p50   ,vi_aspectrate_16_9,148500,528,44,148,4,36,5,},
        {VID_1080P_24       ,PIXEL_REPETITION_0  ,vi_resolution_1080p24   ,vi_aspectrate_16_9,74250,638,44,148,4,36,5,},
        {VID_1080P_25       ,PIXEL_REPETITION_0  ,vi_resolution_1080p25   ,vi_aspectrate_16_9,74250,528,44,148,4,36,5,},
        {VID_1080P_30       ,PIXEL_REPETITION_0  ,vi_resolution_1080p30   ,vi_aspectrate_16_9,74250,88,44,148,4,36,5,},
				{VID_1080P_29_97    ,PIXEL_REPETITION_0  ,vi_resolution_1080p29_97,vi_aspectrate_16_9,74125,88,44,148,4,36,5,},
				{VID_1080I_60       ,PIXEL_REPETITION_0  ,vi_resolution_1080i60   ,vi_aspectrate_16_9,74250,88,44,148,2,15,5,},
        {VID_1080I_59_94    ,PIXEL_REPETITION_0  ,vi_resolution_1080i59_94,vi_aspectrate_16_9,74125,88,44,148,2,15,5,},
				{VID_1080I_50       ,PIXEL_REPETITION_0  ,vi_resolution_1080i50   ,vi_aspectrate_16_9,74250,528,44,148,2,15,5,},
				{VID_720P_60        ,PIXEL_REPETITION_0  ,vi_resolution_720p60    ,vi_aspectrate_16_9,74250,110,40,220,5,20,5,},
				{VID_720P_59_94     ,PIXEL_REPETITION_0  ,vi_resolution_720p59_94 ,vi_aspectrate_16_9,74250,110,40,220,5,20,5,},
        {VID_720P_50        ,PIXEL_REPETITION_0  ,vi_resolution_720p50    ,vi_aspectrate_16_9,74250,440,40,220,5,20,5,},
				{VID_576P_50_4_3    ,PIXEL_REPETITION_0  ,vi_resolution_576p50    ,vi_aspectrate_4_3 ,27000,12,64,68,5,39,5,},
        {VID_576P_50_16_9   ,PIXEL_REPETITION_0  ,vi_resolution_576p50    ,vi_aspectrate_16_9,27000,12,64,68,5,39,5,},
        {VID_576I_50_4_3    ,PIXEL_REPETITION_2  ,vi_resolution_576i50    ,vi_aspectrate_4_3 ,27000,24,126,138,2,19,3,},
        {VID_576I_50_16_9   ,PIXEL_REPETITION_2  ,vi_resolution_576i50    ,vi_aspectrate_16_9,27000,24,126,138,2,19,3,},
        {VID_480P_59_4_3    ,PIXEL_REPETITION_0  ,vi_resolution_480p59_94 ,vi_aspectrate_4_3 ,27000,16,62,60,9,20,6,},
        {VID_480P_59_16_9   ,PIXEL_REPETITION_0  ,vi_resolution_480p59_94 ,vi_aspectrate_16_9,27000,16,62,60,9,20,6,},
        {VID_480I_59_4_3    ,PIXEL_REPETITION_2  ,vi_resolution_480i59_94 ,vi_aspectrate_4_3 ,27000,38,124,114,4,15,3,},
        {VID_480I_59_16_9   ,PIXEL_REPETITION_2  ,vi_resolution_480i59_94 ,vi_aspectrate_16_9,27000,38,124,114,4,15,3,},
				{VID_1440I_60   		,PIXEL_REPETITION_0  ,vi_resolution_1440i60   ,vi_aspectrate_16_9,74250,248,204,308,2,15,5,},
				{VID_1440P_60   		,PIXEL_REPETITION_0  ,vi_resolution_1440p60   ,vi_aspectrate_16_9,148500,248,204,308,4,36,5,},
				{VID_1440I_59_94   	,PIXEL_REPETITION_0  ,vi_resolution_1440i59_94   ,vi_aspectrate_16_9,74125,248,204,308,2,15,5,},
				{VID_1440P_59_94   	,PIXEL_REPETITION_0  ,vi_resolution_1440p59_94   ,vi_aspectrate_16_9,148351,248,204,308,4,36,5,},
        
        
        
    };
    
    #define VIDEO_INFO_NUMS  (sizeof(hdmi_video_table)/sizeof(hdmi_video_id))
        
    
    typedef struct _hdmi_video_timing
    {
        uint8_t vid;
        uint8_t represent;
        video_input_resolution resolution;
        video_input_aspectrate aspectrate;
        uint16_t pixel_per_line;
        uint16_t pixel_per_frame;
        uint16_t hfp;
        uint16_t hbp;
        uint16_t hpw;
        uint8_t vfp;
        uint8_t vbp;
        uint8_t vpw;
        uint8_t _int;
    }hdmi_video_timing,*Phdmi_video_timing;


//    static const ep9xxx_video_info ep9xxx_videoinfo_tablelist[] =
//    {
//        {VID_480P_59_4_3    ,PIXEL_REPETITION_0     ,vi_resolution_480p59_94     ,vi_aspectrate_4_3   },
//        {VID_480P_59_16_9   ,PIXEL_REPETITION_0     ,vi_resolution_480p59_94     ,vi_aspectrate_16_9  },
//        {VID_720P_60        ,PIXEL_REPETITION_0     ,vi_resolution_720p60        ,vi_aspectrate_16_9  },
//        {VID_1080I_60       ,PIXEL_REPETITION_0     ,vi_resolution_1080i60       ,vi_aspectrate_16_9  },
//        {VID_480I_59_4_3    ,PIXEL_REPETITION_2     ,vi_resolution_480i59_94     ,vi_aspectrate_4_3   },
//        {VID_480I_59_16_9   ,PIXEL_REPETITION_2     ,vi_resolution_480i59_94     ,vi_aspectrate_16_9  },
//        {VID_1080P_60       ,PIXEL_REPETITION_0     ,vi_resolution_1080p60       ,vi_aspectrate_16_9  },
//        {VID_576P_50_4_3    ,PIXEL_REPETITION_0     ,vi_resolution_576p50        ,vi_aspectrate_4_3   },
//        {VID_576P_50_16_9   ,PIXEL_REPETITION_0     ,vi_resolution_576p50        ,vi_aspectrate_16_9  },
//        {VID_720P_50        ,PIXEL_REPETITION_0     ,vi_resolution_720p50        ,vi_aspectrate_16_9  },
//        {VID_1080I_50       ,PIXEL_REPETITION_0     ,vi_resolution_1080i50       ,vi_aspectrate_16_9  },
//        {VID_576I_50_4_3    ,PIXEL_REPETITION_2     ,vi_resolution_576i50        ,vi_aspectrate_4_3   },
//        {VID_576I_50_16_9   ,PIXEL_REPETITION_2     ,vi_resolution_576i50        ,vi_aspectrate_16_9  },
//        {VID_1080P_50       ,PIXEL_REPETITION_0     ,vi_resolution_1080p50       ,vi_aspectrate_16_9  },
//        {VID_1080P_24       ,PIXEL_REPETITION_0     ,vi_resolution_1080p24       ,vi_aspectrate_16_9  },
//        {VID_1080P_25       ,PIXEL_REPETITION_0     ,vi_resolution_1080p25       ,vi_aspectrate_16_9  },
//        {VID_1080P_30       ,PIXEL_REPETITION_0     ,vi_resolution_1080p30       ,vi_aspectrate_16_9  },
//    };

//    #define VIDEO_INFO_NUMS  (sizeof(ep9xxx_videoinfo_tablelist)/sizeof(ep9xxx_video_info))
        

    //vatek_result tool_ep9xxx_get_videoinfo(uint8_t vid, Pep9xxx_video_info info);
    void tool_hdmi_print_videotiming(Phdmi_video_timing ptiming);
//    vatek_result tool_ep9xxx_get_videotiming(uint8_t* vtimebuf, Phdmi_video_timing ptiming);
    
    vatek_result tool_hdmi_get_vid(Pphy_video_info pinfo, uint8_t *vid);
    vatek_result tool_hdmi_get_videoinfo(uint8_t vid, Phdmi_video_timing info);
//    vatek_result tool_hdmi_get_timing( Phdmi_video_timing ptiming);


    
#ifdef __cplusplus
}
#endif


#endif

