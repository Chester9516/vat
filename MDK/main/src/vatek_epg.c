#include <string.h>
#include <vatek_epg.h>
#include "vatek_rtcimpl.h"
#include "vatek_define.h"
#include "vatek_hms.h"
#include "vatek_lic_arib_japan.h"
#include "vatek_config_table.h"

extern Plicensed_table licensed_get_table(const char* name,Plicensed_table tables);
extern Plicensed_prop licensed_get_prop(const char* name,Plicensed_prop props);

extern uint32_t licensed_get_uint(int32_t pos,uint8_t* pblock);
extern void licensed_set_uint(int32_t pos,uint32_t val,uint8_t* pblock);
extern void licensed_set_string(int32_t pos,uint8_t* pblock,uint8_t* pbuf,int32_t len);

static uint32_t licensed_pos = 0;
static int32_t m_vatek_is_enextend_stream = 0;

xapp_handle h_xapp = {0,};

/*struct of EEP*/
union{
struct { 
	uint8_t		CONTEXT;
	uint8_t		CONTEXT2;
	uint8_t		EPG_Time_divide;
	uint8_t		EPG_Time_divide2;
	VATLOCAL_JAPAN	plocaljp;
	char 		Provider[17];
	char 		ServiceName[17];
	uint8_t		Copy_Protect;
	char		Event_Name[65];
	char		Event_Content[65];
	char		Event_Name2[65];
	char		Event_Content2[65];
	
	}Byte;
}EEP;

/*RTC*/
vatek_result vatek_board_set_rtc_time(Phms_handle pi2c, Pvatek_time_rtc prtc)
{
    uint32_t val = 0, rd_val = 0;
    vatek_result result = vatek_hms_read_hal(pi2c,HALREG_MUX_TIME_CNTL,&val);

    if(is_success(result))
    {
			if(val != 0)
			{
				if(val != MUX_TIME_EN_UPDATE)
					result = vatek_hms_write_hal(pi2c,HALREG_MUX_TIME_CNTL,0);
			}
			if(is_success(result))
			{
				val = (prtc->year << 16) | (prtc->month << 8) | (prtc->day);
				result = vatek_hms_write_hal(pi2c,HALREG_MUX_TIME_DATE,val);
				result |= vatek_hms_read_hal( pi2c, HALREG_MUX_TIME_DATE, &rd_val);
				if(is_success(result) && rd_val==val)
				{
					val = (prtc->hour << 16) | (prtc->min << 8) | (prtc->secound);
					result = vatek_hms_write_hal(pi2c,HALREG_MUX_TIME_CLOCK,val);
					result |= vatek_hms_read_hal( pi2c, HALREG_MUX_TIME_CLOCK, &rd_val);
					if(is_success(result) && rd_val==val)
							result = vatek_hms_write_hal(pi2c,HALREG_MUX_TIME_CNTL,MUX_TIME_EN_UPDATE);
				}
			}
    }
    return result;
}

vatek_result xapp_polling_check_running(Phms_handle pi2c)
{
    vatek_result result = vatek_result_success;
    static uint8_t res_change_retry = 0;

		if(h_xapp.tot_tick == 0 || (vatek_system_gettick() - h_xapp.tot_tick) > XAPP_TOT_INTERVAL_MS)
		{
				PVAT_RTC_DATETIME time = VATRTCGetTime(RTC_TIMEFMT_BIN);
				vatek_time_rtc rtctime;
				
				rtctime.year = 2000 + time->nYear;
				rtctime.month = time->nMonth;
				rtctime.day = time->nDay;
				rtctime.hour = time->nHours;
				rtctime.min = time->nMinutes;
				rtctime.secound = time->nSeconds;

				result = vatek_board_set_rtc_time(pi2c, &rtctime);
				if(is_success(result))h_xapp.tot_tick = vatek_system_gettick();
				else
				{
						if(result == vatek_result_badstatus)result = vatek_result_success;
				}
		}
    return result;
}

/*EPG*/
vatek_result hal_licensed_set_table_version(const char* name,uint8_t* pblock,Plicensed_table tables,uint16_t version)
{
    Plicensed_table ptr = licensed_get_table(name,tables);
    vatek_result result = vatek_result_invalidparm;
    if(ptr != NULL)
    {
        licensed_set_uint(ptr->offset + 8,version,pblock);
        result = vatek_result_success;
    }
    return result;
}

vatek_result hal_licensed_set_table_interval(const char* name,uint8_t* pblock,Plicensed_table tables,uint16_t interval)
{
    Plicensed_table ptr = licensed_get_table(name,tables);
    vatek_result result = vatek_result_invalidparm;
    if(ptr != NULL)
    {
        licensed_set_uint(ptr->offset + 12,interval,pblock);
        result = vatek_result_success;
    }
    return result;    
}

vatek_result hal_licensed_set_prop(const char* name,uint8_t* pblock,Plicensed_prop props,uint32_t val)
{
    Plicensed_prop ptr = licensed_get_prop(name,props);
    vatek_result result = vatek_result_invalidparm;
    
    if(ptr != NULL)
    {
        if((ptr->type & PROP_TYPE_STRING) == 0)
        {
            licensed_set_uint(ptr->offset,val,pblock);
            result = vatek_result_success;
        }
    }
    return result;
}

vatek_result hal_licensed_set_string(const char* name,uint8_t* pblock,Plicensed_prop props,uint8_t* pbuf,int32_t len)
{
    Plicensed_prop ptr = licensed_get_prop(name,props);
    vatek_result result = vatek_result_invalidparm;
    
    if(ptr != NULL)
    {
        if((ptr->type & PROP_TYPE_STRING) && (ptr->type & 0xFFF) >= len)
        {
            licensed_set_uint(ptr->offset,len,pblock);
            licensed_set_string(ptr->offset + 4,pblock,pbuf,len);
            result = vatek_result_success;
        }
    }
    return result;    
}
	
vatek_result hal_licensed_start(Phms_handle pi2c,uint8_t* pblock,int32_t len)
{
    vatek_result result = vatek_result_success;
		
    licensed_pos = HALRANGE_PLAYLOAD_START;
    
    result = vatek_hms_write_hal(pi2c,licensed_pos++,LICENSED_EN_START);
    if(is_success(result))
    {
        result = vatek_hms_write_hal(pi2c,licensed_pos++,RULE_EN_START);
        if(is_success(result))
        {
            result = vatek_hms_write_hal(pi2c,licensed_pos++,len);
            if(is_success(result))
            {
                result = vatek_hms_write_halbuf(pi2c,licensed_pos,&pblock[0],len);
                if(is_success(result))
                {
                    licensed_pos += (len >> 2);
                    result = vatek_hms_write_hal(pi2c,licensed_pos++,RULE_EN_END);
                }
            }
        }
    }
		
    return result;
    
}

#define wrhal(val)          vatek_hms_write_hal(pi2c,licensed_pos++,val)
#define wrbuf(buf,len)      vatek_hms_write_halbuf(pi2c,licensed_pos,buf,len);
#define rdhal(val)          vatek_hms_read_hal(pi2c,licensed_pos-1, &val)

extern vatek_result hal_licensed_add_epg_section(Phms_handle pi2c,Plicensed_epg_section pepg);
extern int32_t hal_licensed_get_epg_section_nums(Plicensed_epg pepg);
extern int32_t hal_licensed_get_epg_desc_nums(Plicensed_epg pepg);

vatek_result hal_licensed_add_epg(Phms_handle pi2c,Plicensed_epg pepg)
{
    uint32_t rd_val = 0;
    
   	vatek_result result = wrhal(LIC_EPG_EN_START);
	if (is_success(result))result = wrhal(pepg->mode);
    if (is_success(result))result = rdhal(rd_val);
	printf("epg mode = 0x%x\r\n",rd_val);
	if (is_success(result) && rd_val==pepg->mode)result = wrhal(pepg->epg_flags);
    if (is_success(result))result = rdhal(rd_val);
	printf("epg flag = 0x%x\r\n",rd_val);
	if (is_success(result) && rd_val==pepg->epg_flags)result = wrhal(pepg->start_time_date);
    if (is_success(result))result = rdhal(rd_val);
	printf("start time date = 0x%x\r\n",rd_val);
	if (is_success(result) && rd_val==pepg->start_time_date)result = wrhal(pepg->start_time_clock);
    if (is_success(result))result = rdhal(rd_val);
	printf("start time clock = 0x%x\r\n",rd_val);
	if (is_success(result) && rd_val==pepg->start_time_clock)result = wrhal(pepg->event_id);
    if (is_success(result))result = rdhal(rd_val);
	printf("event id = 0x%x\r\n",rd_val);
	if (is_success(result) && pepg->event_id)result = wrhal(pepg->days);
    if (is_success(result))result = rdhal(rd_val);
	printf("days = 0x%x\r\n",rd_val);
	if (is_success(result) && pepg->days)result = wrhal(pepg->loop_ms);
	if (is_success(result))result = rdhal(rd_val);
	printf("loop ms = 0x%x\r\n",rd_val);
    
	if (is_success(result) && pepg->loop_ms)
	{
        result = wrhal(hal_licensed_get_epg_section_nums(pepg));
        if(is_success(result))result = wrhal(0);//hal_licensed_get_epg_desc_nums(pepg)
        
        if(is_success(result))
        {
            Plicensed_epg_section psection = pepg->sections;
            while(psection)
            {
                result = hal_licensed_add_epg_section(pi2c,psection);
                if(!is_success(result))break;
                psection = psection->next;
            }
        }
        
		if (is_success(result))result = wrhal(LIC_EPG_EN_END);

	}
    return result;
}

int32_t hal_licensed_get_epg_section_nums(Plicensed_epg pepg)
{
    int32_t nums = 0;
    Plicensed_epg_section psection = pepg->sections;
    while(psection)
    {
        nums++;
        psection = psection->next;
    }
    return nums;
}

int32_t hal_licensed_get_epg_desc_nums(Plicensed_epg pepg)
{
    int32_t nums = 0;
    Plicensed_epg_desc pdesc = pepg->descs;
    while(pdesc)
    {
        pdesc = pdesc->next;
        nums++;
    }
    return nums;
}


vatek_result hal_licensed_add_epg_section(Phms_handle pi2c,Plicensed_epg_section psection)
{
	uint32_t reg_val = 0;
   	vatek_result result = wrhal(8);//psection->duration
	rdhal(reg_val);
	printf("section->duration = 0x%x\r\n",reg_val);
    if(is_success(result))result = wrhal(psection->type);
	rdhal(reg_val);
	printf("section->type = 0x%x\r\n",reg_val);
    if(is_success(result))result = wrhal(psection->parental_rating);
	rdhal(reg_val);
	printf("section->parental_rating = 0x%x\r\n",reg_val);
		if (is_success(result))result = wrhal(LIC_EPG_TITAL_LEN);
	rdhal(reg_val);
	printf("license tital len = 0x%x\r\n",reg_val);
		if (is_success(result))result = wrhal(psection->title_len);
	rdhal(reg_val);
	printf("section tital len = 0x%x\r\n",reg_val);

	if (is_success(result))
	{
		result = wrbuf(&psection->title[0], LIC_EPG_TITAL_LEN);
		if (is_success(result))licensed_pos += LIC_EPG_TITAL_LEN >> 2;
	}

	if (is_success(result))
	{
		if (is_success(result))result = wrhal(LIC_EPG_CONTENT_LEN);
			rdhal(reg_val);
			printf("EPG content len = 0x%x\r\n",reg_val);
		if (is_success(result))result = wrhal(psection->content_len);
			rdhal(reg_val);
			printf("section content len = 0x%x\r\n",reg_val);
		if (is_success(result))
		{
			result = wrbuf(&psection->content[0], LIC_EPG_CONTENT_LEN);
			if (is_success(result))licensed_pos += LIC_EPG_CONTENT_LEN >> 2;
		}
	}
	return result; 
}

vatek_result hal_licensed_end(Phms_handle pi2c)
{
    vatek_result result = vatek_hms_write_hal(pi2c,licensed_pos,LICENSED_EN_END);
    return result;
}

Plicensed_table licensed_get_table(const char* name,Plicensed_table tables)
{
    int32_t i = 0;
    while(tables[i].name != NULL)
    {
        if(strcmp(tables[i].name,name) == 0)return &tables[i];
        i++;
    }
    return NULL;
}

Plicensed_prop licensed_get_prop(const char* name,Plicensed_prop props)
{
    int32_t i = 0;
    while(props[i].name != NULL)
    {
        if(strcmp(props[i].name,name) == 0)return &props[i];
        i++;
    }
    return NULL;
}

uint32_t licensed_get_uint(int32_t pos,uint8_t* pblock)
{
    uint8_t* ptr = &pblock[pos];
    return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
}

void licensed_set_uint(int32_t pos,uint32_t val,uint8_t* pblock)
{
    uint8_t* ptr = &pblock[pos];
    ptr[0] = val >> 24;
    ptr[1] = val >> 16;
    ptr[2] = val >> 8;
    ptr[3] = val;
}

void licensed_set_string(int32_t pos,uint8_t* pblock,uint8_t* pbuf,int32_t len)
{
    uint8_t* psur = &pblock[pos];
    while(len)
    {
        *psur++ = *pbuf++;
        len--;
    }
}

uint8_t convert_vatek_context_type(uint8_t type)
{
	/*
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
	*/
	
    if(type == 0)return 1;          /* news         */
    else if(type == 1)return 2;     /* sports       */
    else if(type == 2)return 6;     /* info         */
    else if(type == 3)return 4;     /* drama        */
    else if(type == 4)return 5;     /* music        */
    else if(type == 5)return 7;     /* variety      */
    else if(type == 6)return 3;     /* movie        */
    else if(type == 7)return 8;     /* animation    */
    else if(type == 8)return 9;     /* documentry   */
    else if(type == 9)return 10;     /* theatre      */
    else if(type == 10)return 11;    /* hobby        */
    else if(type == 11)return 12;    /* welfare      */
    else if(type == 12)return 0xFF;    /* reserved1    */
    else if(type == 13)return 0xFF;    /* reserved2    */
    else if(type == 14)return 0xFF;    /* expansion    */
    else if(type == 15)return 0xFF;    /* others       */
    else return 1;
    
}

void UTF2JIS(char *buf,char *buf2)
{
uint16_t len,i;

	len=strlen(buf2);
	memcpy(buf,buf2,len);
	for(i=0;i<len;i++){
		*(buf+i) &=0x7f;
	}
	*(buf+len)=0;
}

static char EIT_EVENT_JIS_NAME[65];
static char EIT_EVENT_JIS_NAME2[65];
static char EIT_EVENT_JIS_NAME3[65];
static char EIT_EVENT_JIS_NAME4[65];

const uint8_t EPG_Time_divide_tbl[14]={1,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0x10,0x11,0x12,0x24};
const uint8_t EPG_Time_divide_tblDEC[14]={1,1,2,3,4,5,6,7,8,9,10,11,12,24};

extern vatek_result xapp_set_epg_param(Plicensed_epg_section pepg,uint8_t duration,uint8_t type,const char* title,const char* text);

void init_UTF2JIS()
{
	uint8_t i;
	for (i=0;i<65;i++){EEP.Byte.Event_Name[i]=0;} //EPG title
	EEP.Byte.Event_Name[0]=0x3C+0x80;
	EEP.Byte.Event_Name[1]=0x2B+0x80;
	EEP.Byte.Event_Name[2]=0x3C+0x80;
	EEP.Byte.Event_Name[3]=0x67+0x80;
	EEP.Byte.Event_Name[4]=0x4A+0x80;
	EEP.Byte.Event_Name[5]=0x7C+0x80;
	EEP.Byte.Event_Name[6]=0x41+0x80;
	EEP.Byte.Event_Name[7]=0x77+0x80;
	EEP.Byte.Event_Name[8]=0xA3;
	EEP.Byte.Event_Name[9]=0xB1;
	EEP.Byte.Event_Name[10]=0;


	for (i=0;i<65;i++){EEP.Byte.Event_Content[i]=0;} //EPG content
	EEP.Byte.Event_Content[0]=0x3C+0x80;
	EEP.Byte.Event_Content[1]=0x2B+0x80;
	EEP.Byte.Event_Content[2]=0x3C+0x80;
	EEP.Byte.Event_Content[3]=0x67+0x80;
	EEP.Byte.Event_Content[4]=0x4A+0x80;
	EEP.Byte.Event_Content[5]=0x7C+0x80;
	EEP.Byte.Event_Content[6]=0x41+0x80;
	EEP.Byte.Event_Content[7]=0x77+0x80;
	EEP.Byte.Event_Content[8]=0xC6;
	EEP.Byte.Event_Content[9]=0xE2;
	EEP.Byte.Event_Content[10]=0xCD;
	EEP.Byte.Event_Content[11]=0xC6;
	EEP.Byte.Event_Content[12]=0xa3;
	EEP.Byte.Event_Content[13]=0xb1;
	EEP.Byte.Event_Content[14]=0;
	EEP.Byte.Event_Content[15]=0;


	EEP.Byte.EPG_Time_divide2=1; //program time long

	for (i=0;i<65;i++){EEP.Byte.Event_Name2[i]=0;}//EPG title
	EEP.Byte.Event_Name2[0]=0xbc;
	EEP.Byte.Event_Name2[1]=0xab;
	EEP.Byte.Event_Name2[2]=0xbc;
	EEP.Byte.Event_Name2[3]=0xe7;
	EEP.Byte.Event_Name2[4]=0xca;
	EEP.Byte.Event_Name2[5]=0xfc;
	EEP.Byte.Event_Name2[6]=0xc1;
	EEP.Byte.Event_Name2[7]=0xf7;
	EEP.Byte.Event_Name2[8]=0xa3;
	EEP.Byte.Event_Name2[9]=0xb2;
	EEP.Byte.Event_Name2[10]=0;


	for (i=0;i<65;i++){EEP.Byte.Event_Content2[i]=0;} //EPG content
	EEP.Byte.Event_Content2[0]=0xbc;
	EEP.Byte.Event_Content2[1]=0xab;
	EEP.Byte.Event_Content2[2]=0xbc;
	EEP.Byte.Event_Content2[3]=0xe7;
	EEP.Byte.Event_Content2[4]=0xca;
	EEP.Byte.Event_Content2[5]=0xfc;
	EEP.Byte.Event_Content2[6]=0xc1;
	EEP.Byte.Event_Content2[7]=0xf7;
	EEP.Byte.Event_Content2[8]=0xc6;
	EEP.Byte.Event_Content2[9]=0xe2;
	EEP.Byte.Event_Content2[10]=0xcd;
	EEP.Byte.Event_Content2[11]=0xc6;
	EEP.Byte.Event_Content2[12]=0xa3;
	EEP.Byte.Event_Content2[13]=0xb2;
	EEP.Byte.Event_Content2[14]=0;
	EEP.Byte.Event_Content2[15]=0;
	
	UTF2JIS(&EIT_EVENT_JIS_NAME[0],&EEP.Byte.Event_Name[0]);
	UTF2JIS(&EIT_EVENT_JIS_NAME3[0],&EEP.Byte.Event_Name2[0]);
	UTF2JIS(&EIT_EVENT_JIS_NAME2[0],&EEP.Byte.Event_Content[0]);
	UTF2JIS(&EIT_EVENT_JIS_NAME4[0],&EEP.Byte.Event_Content2[0]);
}

extern vatek_result xapp_set_epg_param(Plicensed_epg_section pepg,uint8_t duration,uint8_t type,const char* title,const char* text);

vatek_result vatek_app_epg_set_present_follow(uint8_t duration,uint8_t type,const char* title,const char* text)
{
    return xapp_set_epg_param(&h_xapp.epg_param->sections[0],duration,type,title,text);
}

vatek_result vatek_app_epg_set_schedule(uint8_t duration,uint8_t type,const char* title,const char* text)
{
    return xapp_set_epg_param(&h_xapp.epg_param->sections[1],duration,type,title,text);
}

vatek_result xapp_set_epg_param(Plicensed_epg_section pepg,uint8_t duration,uint8_t type,const char* title,const char* text)
{
    vatek_result nres = vatek_result_invalidparm;
    
    if(strlen(title) <= XAPP_EPG_TITLE_MAXLEN && strlen(text) <= XAPP_EPG_TEXT_MAXLEN && duration <= 24)
    {
        pepg->duration = duration;
        pepg->type = type;
        pepg->title_len = strlen(title);
        strcpy((char*)&pepg->title[0],title);
        pepg->content_len = strlen(text);
        strcpy((char*)&pepg->content[0],text);
        nres = vatek_result_success;
    }else printf("bad epg param\r\n");
    return nres;    
}

#define EPG_SECTION_BASE_LEN	sizeof(licensed_epg_section)
Plicensed_epg_section malloc_epg_section(uint8_t* pbuf, int32_t* pos);

Plicensed_epg xapp_create_epg_param(licensed_epg_mode mode)
{
    int32_t len = sizeof(licensed_epg);
	uint8_t* rawbuf = NULL;

	if (mode == licensed_epg_dvb || mode == licensed_epg_arib_0)len += EPG_SECTION_BASE_LEN;
	else if (mode == licensed_epg_arib_1)len += EPG_SECTION_BASE_LEN * 2;

	rawbuf = malloc(len);

	if (rawbuf)
	{
		Plicensed_epg pnewepg = (Plicensed_epg)&rawbuf[0];
		memset(rawbuf, 0, len);
		len = sizeof(licensed_epg);

		pnewepg->sections = malloc_epg_section(rawbuf, &len);
		if(mode == licensed_epg_arib_1)
			pnewepg->sections->next = malloc_epg_section(rawbuf, &len);

		pnewepg->days = 8;
		pnewepg->epg_flags = 0;
		pnewepg->event_id = 0x2000;
		pnewepg->loop_ms = 10000;
		pnewepg->mode = mode;
        return pnewepg;
	}
	return NULL;
    
}

vatek_result vatek_epg_set_schedule()
{
	vatek_result nres = vatek_result_unknown;
	nres = vatek_app_epg_set_present_follow(EPG_Time_divide_tbl[EEP.Byte.EPG_Time_divide],
                                             convert_vatek_context_type(EEP.Byte.CONTEXT),
                                             &EIT_EVENT_JIS_NAME[0],
                                             &EIT_EVENT_JIS_NAME2[0]);
    if(is_success(nres))
        nres = vatek_app_epg_set_schedule(EPG_Time_divide_tbl[EEP.Byte.EPG_Time_divide2],
                                           convert_vatek_context_type(EEP.Byte.CONTEXT2),
                                           &EIT_EVENT_JIS_NAME3[0],
                                           &EIT_EVENT_JIS_NAME4[0]);
	
	/* arib psi_table properties */
	EEP.Byte.plocaljp = LOCAL_JPN_ISDBT_DEFPARAM;
	uint32_t i = 0;
	nres = hal_licensed_set_prop("region_id", &rule_block[0], (Plicensed_prop)&rule_props[0], EEP.Byte.plocaljp.nLocalCode);
	if(is_success(nres))nres = hal_licensed_set_prop("broadcaster_id", &rule_block[0], (Plicensed_prop)&rule_props[0],EEP.Byte.plocaljp.nStationCode);
	if(is_success(nres))nres = hal_licensed_set_prop("remote_control_key_id", &rule_block[0], (Plicensed_prop)&rule_props[0],EEP.Byte.plocaljp.nRemoteCode);
	if(is_success(nres))nres = hal_licensed_set_prop("service_no", &rule_block[0], (Plicensed_prop)&rule_props[0],EEP.Byte.plocaljp.nServiceNo);
	if(is_success(nres))nres = hal_licensed_set_prop("copy_flag", &rule_block[0], (Plicensed_prop)&rule_props[0],DIGITAL_COPY_FREE);
	
	EEP.Byte.Provider[0]='M';
	EEP.Byte.Provider[1]='y';
	EEP.Byte.Provider[2]='S';
	EEP.Byte.Provider[3]='t';
	EEP.Byte.Provider[4]='a';
	EEP.Byte.Provider[5]='t';
	EEP.Byte.Provider[6]='i';
	EEP.Byte.Provider[7]='o';
	EEP.Byte.Provider[8]='n';
	
	EEP.Byte.ServiceName[0]='S';
	EEP.Byte.ServiceName[1]='e';
	EEP.Byte.ServiceName[2]='r';
	EEP.Byte.ServiceName[3]='v';
	EEP.Byte.ServiceName[4]='i';
	EEP.Byte.ServiceName[5]='c';
	EEP.Byte.ServiceName[6]='e';
	EEP.Byte.ServiceName[7]='N';
	EEP.Byte.ServiceName[8]='a';
	EEP.Byte.ServiceName[9]='m';
	
	if(is_success(nres))nres = vatek_board_set_rule_string("network_name",(uint8_t*)&EEP.Byte.ServiceName[0],strlen(&EEP.Byte.ServiceName[0]));
	if(is_success(nres))nres = vatek_board_set_rule_string("service_name",(uint8_t*)&EEP.Byte.ServiceName[0],strlen(&EEP.Byte.ServiceName[0]));
	if(is_success(nres))nres = vatek_board_set_rule_string("ts_name",(uint8_t*)&EEP.Byte.Provider[0],strlen(&EEP.Byte.Provider[0]));

//	for(i=0;i<=1728;i++){
//		if(i%10 == 0){
//			printf("%d ",i);
//		}
//		printf("0x%02x ",rule_block[i]);
//		if(i >= 9){
//			if(i%10 == 9){
//				printf("\r\n");
//			}
//		}
//	}
	printf("\r\n");
}
#define _AC(ch)	(ch+0x80)
#define _AC_ASCII(ch)	(ch - 0x80)
static const char EIT_EVENT_JIS_TITLE[] = {0x3C,0x2B,0x3C,0x67,0x4A,0x7C,0x41,0x77,0x23,0x31,0,};
static const char EIT_EVENT_JIS_CONTENT[] = {0x3C,0x2B,0x3C,0x67,0x4A,0x7C,0x41,0x77,0x46,0x62,0x4d,0x46,0x23,0x31,0,};
    
Plicensed_epg_section malloc_epg_section(uint8_t* pbuf, int32_t* pos)
{
  	int32_t ptr = *pos;
	Plicensed_epg_section psection = (Plicensed_epg_section)&pbuf[ptr];
	ptr += sizeof(licensed_epg_section);

	psection->duration = 4;
	psection->parental_rating = 1;
	psection->type = 1;
	psection->title_len = sizeof(EIT_EVENT_JIS_TITLE);
	memcpy(&psection->title[0], &EIT_EVENT_JIS_TITLE[0], psection->title_len);
	psection->content_len = sizeof(EIT_EVENT_JIS_CONTENT);
	memcpy(&psection->content[0], &EIT_EVENT_JIS_CONTENT[0], psection->content_len);

	*pos = ptr;
	return psection;  
}

vatek_result vatek_epg_sample(Phms_handle pi2c)
{
	vatek_result result = vatek_result_success;
	PVAT_RTC_DATETIME time = VATRTCGetTime(RTC_TIMEFMT_BIN);
	if(is_success(result))
	{
			h_xapp.epg_param = xapp_create_epg_param(licensed_epg_arib_1);
		
			h_xapp.epg_param->start_time_date = ((2000 + time->nYear) << 16) | (time->nMonth << 8) | time->nDay;
			h_xapp.epg_param->start_time_clock = (time->nHours << 16) | (time->nMinutes << 8) | (time->nSeconds);
			vatek_epg_set_schedule();
		
			result = hal_licensed_start(pi2c,&rule_block[0],sizeof(rule_block));
			if(is_success(result))result = hal_licensed_add_epg(pi2c,h_xapp.epg_param);
			if(is_success(result))
			{
					result = hal_licensed_end(pi2c);
			}
	}
	result = hal_lite_write_table(pi2c,&HALREG_TABLE[0],HALREG_TABLE_SIZE);
	return result;
}
