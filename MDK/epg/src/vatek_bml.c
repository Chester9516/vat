//#include "vatek_epg.h"

////#include <xhead2_app.h>
////#include <xhead2.h>
////#include <ff.h>
////#include <main.h>

////#include <xhead2_2.h>

////#include <tools/tool_crc32.h>
////#include <hal/hal_register/halreg_mux_licensed.h>

//#define BML_USED_SPIFLASH_EXTEND        0
//#define BML_UPDATE_UESD_PROGRESS        1

//#define BML_STREAM_PACKET_BUF           (20*1024)
//#define BML_STREAM_MAX_PACKETS          (BML_STREAM_PACKET_BUF/188)

//#define BML_FOLDER_NAME                 "/BML_DATA"
//#define BML_STREAM_UPDATE_NAME          "/BML_DATA/update"

//#define BML_FULLPATH_MAX_LEN            256
//#define BML_FLASH_SECTION_LEN           4096
//#define BML_FLASH_PAGE_LEN              256
//#define BML_RT_SPILT_MS                 10
//#define BML_MAX_PESNUMS                 ((BML_FLASH_SECTION_LEN/BML_FLASH_PAGE_LEN) - 1)

//#define BML_ALIGC_SECTION(val)          (((val + (BML_FLASH_SECTION_LEN - 1))/BML_FLASH_SECTION_LEN) * BML_FLASH_SECTION_LEN)


///* micomsoft spi_3 operaion api (spi_command.c) */
//extern void SPI3_GPIO_Write();
//extern void spi3_Read_Cont(uint8_t *buf,uint32_t addres,uint32_t size);
//extern void spi3_Write_Cont(uint8_t *buf,uint32_t addres,uint32_t size);
//extern void spi3_Erace_Cont(uint32_t addres,uint32_t size);

//extern void SPI2_GPIO_Write();
//extern void SPI2_GPIO_Read();
//extern void spi2_Read_Cont(uint8_t *buf,uint32_t addres,uint32_t size);
//extern void spi2_Write_Cont(uint8_t *buf,uint32_t addres,uint32_t size);
//extern void spi2_Erace_Cont(uint32_t addres,uint32_t size); 

//extern void LCD_PrintString(char* pstr);    /* lcd_common.c */
//extern void lcd_xy(uint8_t x, uint8_t y);   /* lcd_function.c */
//extern void LCD_CLR(void);                  /* lcd_common.c */
//            


//#if BML_USED_SPIFLASH_EXTEND
//    #define BML_FLASH_START_ADDR        0
//    #define BML_FLASH_INIT              SPI3_GPIO_Write
//    #define BML_FLASH_FREE              SPI3_GPIO_Read
//    #define BML_FLASH_READ              spi3_Read_Cont
//    #define BML_FLASH_WRITE             spi3_Write_Cont
//    #define BML_FLASH_ERASE             spi3_Erace_Cont
//#else
//    #define BML_FLASH_START_ADDR        (512*BML_FLASH_SECTION_LEN)
//    #define BML_FLASH_INIT              SPI2_GPIO_Write
//    #define BML_FLASH_FREE              SPI2_GPIO_Read
//    #define BML_FLASH_READ              spi2_Read_Cont
//    #define BML_FLASH_WRITE             spi2_Write_Cont
//    #define BML_FLASH_ERASE             spi2_Erace_Cont
//#endif


//typedef struct _xapp_pes_stream
//{
//    struct _xapp_pes_stream* next;
//    xhead2_pesstream info;
//    FIL fhandle;
//    char full_path[BML_FLASH_PAGE_LEN];
//    uint32_t flash_pos;
//    uint32_t file_size;
//    uint32_t file_write_pos;
//}xapp_pes_stream,*Pxapp_pes_stream;

//typedef struct _xapp_async_updater
//{
//    DIR fdir;
//    FILINFO bmlfl;
//    char pathbuf[BML_FULLPATH_MAX_LEN];
//    uint8_t section_buf[BML_FLASH_SECTION_LEN];
//    uint32_t updater_step;
//    uint32_t flash_pos;
//    Pxapp_pes_stream stream_ptr;
//    Pxapp_pes_stream streams;
//}xapp_async_updater,*Pxapp_async_updater;

//typedef struct _xapp_realtime_stream
//{
//    struct _xapp_realtime_stream* next;
//    licensed_pes_stream info;
//    FIL fhandle;
//    uint32_t packets_ptr;
//    uint32_t packets_nums;
//    uint32_t sendtimems;
//    uint8_t eachtime_packet;
//    uint8_t ncontinue;
//}xapp_realtime_stream,*Pxapp_realtime_stream;

//typedef struct _xapp_bml_realtime
//{
//    /* realtime need buffer */
//    DIR fdir;
//    FILINFO bmlfl;
//    char pathbuf[BML_FULLPATH_MAX_LEN];
//    uint8_t packet_buf[BML_STREAM_PACKET_BUF];
//    uint32_t packet_buf_rptr;
//    uint32_t packet_buf_wptr;
//    uint32_t packet_buf_len;
//    xhead2_pesstream pes_info;
//    /* realtime stream variable */
//    Pxapp_realtime_stream streams;
//}xapp_bml_realtime,*Pxapp_bml_realtime;

//typedef struct _bstream
//{
//    uint8_t* pbuf;
//    uint32_t pos;
//}bstream,*Pbstream;

//#define LICPES_FLASH_TAG        0xF8A50122
//#define LICPES_STREAM_TAG       0xF8A90000
//#define LICPES_NUMBER_OFFSET    20
//#define LICPES_CRC_OFFSET       (BML_FLASH_PAGE_LEN - 4)

//void bstream_put_uint(Pbstream pbs,uint32_t val);
//void bstream_put_string(Pbstream pbs,const char* val,int32_t len);
//void bstream_put_buf(Pbstream pbs,uint8_t* pbuf,int32_t len);
//void bstream_put_byte(Pbstream pbs,uint8_t val);

//uint32_t bstream_get_uint(Pbstream pbs);
//void bstream_get_string(Pbstream pbs,char* val,int32_t len);
//void bstream_get_buf(Pbstream pbs,uint8_t* pbuf,int32_t len);

//void bstream_seek(Pbstream pbs,int32_t pos);
//void bstream_offset(Pbstream pbs,int32_t offset);


//extern vatek_result bml_pesstream_create_async(const char* filename,Pxapp_pes_stream* pappstream);
//extern int32_t bml_get_stream_nums(Pxapp_pes_stream pstreams);

//extern vatek_result bml_reload_async(uint8_t* pbuf);
//extern vatek_result bml_reload_realtime(void);

//extern Plicensed_epg_desc bml_create_data_content(void);

//extern vatek_result bml_pesstream_info_get(const char* tag,const char* filename,Pxhead2_pesstream pinfo);
//extern vatek_result bml_pesstream_create_realtime(Pxhead2_pesstream pinfo,const char* fullpath,Pxapp_realtime_stream* pstream);

////extern vatek_result bml_check_update(void);
//extern vatek_result bml_async_update_start(void);
//extern vatek_result bml_async_update_write(void);
//extern vatek_result bml_async_update_stop(void);

//extern vatek_result bml_write_bml(uint8_t* pbufsect,Pxapp_pes_stream pbmls);
//extern vatek_result bml_write_bml_header(uint8_t* pbufsect,Pxapp_pes_stream proot);

///* extend stream interface */
//extern vatek_result bml_async_enable(int32_t isb2);
//extern vatek_result bml_realtime_start(void);
//extern int32_t bml_realtime_get_packets(uint8_t** packets);
///* variables */
//static  xapp_extend_stream xbml_stream = {pes_stream_idle,};
//static Pxapp_bml_realtime xbml_realtime = NULL;
//static Pxapp_async_updater xbml_async_writer = NULL;
//static int32_t xbml_async_updatecheck = 0;

//void BML_UPDATE_CHK()
//{
//    if(xbml_stream.mode == pes_stream_async)
//    {
//        if(xbml_async_updatecheck == NULL)
//            xbml_async_updatecheck = 1;
//        else printf("bml async updater busy");        
//    }
//}

//int32_t BML_STREAM_POLLING(void)
//{
//    vatek_result result = vatek_result_success;
//    if(xbml_async_updatecheck)
//    {
//        if(xbml_async_writer == NULL)
//        {
//            result = bml_async_update_start();
//            if(!is_hal_lite_success(result))printf("update bml data fail : %d\r\n",result);
//            else if(xbml_async_writer == NULL)xbml_async_updatecheck = 0;
//            else return 1;
//        }else
//        {
//            if(xbml_async_writer->updater_step == 0)result = bml_async_update_write();
//            else if(xbml_async_writer->updater_step == 1)
//            {
//                result = bml_async_update_stop();
//                return 2;
//            }
//            
//        }
//        
//        return 1;
//    }
//    return 0;
//}

//void BML_ENABLE_MODE(int32_t mode)
//{
//    vatek_result result = xhead2_pesstream_reset();
//    xbml_realtime = NULL;
//    
//    if(is_hal_lite_success(result))
//    {
//        result = vatek_result_invalidparm;
//        memset(&xbml_stream,0,sizeof(xapp_extend_stream));
//        
//        if(mode == 0)       /* large data mode */
//        {
//            xbml_stream.mode = pes_stream_async;
//            xbml_stream.route.async.enable = bml_async_enable;
//            xbml_stream.route.async.status = pesstream_status_idle;   
//            
//            BML_UPDATE_CHK();
//            result = vatek_result_success;
//        }else if(mode == 1) /* small data mode */
//        {
//            xbml_stream.mode = pes_stream_realtime;
//            xbml_stream.route.rt.enable = bml_realtime_start;
//            xbml_stream.route.rt.get_packets = bml_realtime_get_packets;
//            result = vatek_result_success;
//        }
//        if(!is_hal_lite_success(result))
//        {
//            printf("enable bml mode fail : %d - %d\r\n",mode,result);
//        }
//        else xhead2_app_set_extend_stream(&xbml_stream);
//    }
//}

//vatek_result bml_async_enable(int32_t isb2)
//{
//#if BML_USED_SPIFLASH_EXTEND
//    HAL_GPIO_WritePin(SPI_SELECT_GPIO_Port,SPI_SELECT_Pin,isb2);
//#endif
//    vatek_result result =  vatek_result_success;
//    if(isb2)
//    {
//        result = xhead2_pesstream_reset();
//        if(is_hal_lite_success(result))
//        {
//            uint8_t* pbuf = (uint8_t*)xhead2_pesstream_malloc(BML_FLASH_SECTION_LEN);
//            BML_FLASH_INIT();  /* initial spi gpio and status */
//            result = bml_reload_async(pbuf);            
//            BML_FLASH_FREE();         
//        }

//        
//        if(!is_hal_lite_success(result))
//            printf("reload bml async fail : %d\r\n",result);
//        else xhead2_app_set_data_content(bml_create_data_content());
//    }else
//    {
//        xbml_stream.route.async.status = pesstream_status_idle;
//    }
//    return result;
//}

//vatek_result bml_realtime_start(void)
//{
//    vatek_result result = vatek_result_success;
//    if(xbml_realtime == NULL)
//    {
//        result = bml_reload_realtime();
//        if(!is_hal_lite_success(result))printf("reload realtime bml fail : %d\r\n",result);
//    }
//    return result;
//}

//void bml_realtime_load_packet(Pxapp_realtime_stream pstream)
//{
//    int32_t i = 0;
//    FRESULT fr;
//    uint32_t bsize;
//    
//    uint8_t* ptr = &xbml_realtime->packet_buf[xbml_realtime->packet_buf_wptr*188];
//    
//    while(i < pstream->eachtime_packet)
//    {
//        ptr = &xbml_realtime->packet_buf[xbml_realtime->packet_buf_wptr*188];
//		LCD.Byte.TIM16_flag=0;
//        fr = f_read(&pstream->fhandle,ptr,188,&bsize);
//		LCD.Byte.TIM16_flag=1;
//        if(fr != FR_OK || 188 != bsize)printf("read file fail - %d - %d\r\n",fr,bsize);
//       
//        
//        pstream->packets_ptr++;
//        if(pstream->packets_ptr >= pstream->packets_nums)
//        {
//			LCD.Byte.TIM16_flag=0;
//            fr = f_lseek(&pstream->fhandle,0);
//			LCD.Byte.TIM16_flag=1;
//            if(fr != FR_OK)printf("seek stream file fail : %d\r\n",fr);
//            pstream->packets_ptr = 0;
//        }
//        
//        xbml_realtime->packet_buf_wptr++;
//        if(xbml_realtime->packet_buf_wptr >= xbml_realtime->packet_buf_len)
//            xbml_realtime->packet_buf_wptr = 0;
//        
//        ptr[3] = (ptr[3] & 0xF0) | (pstream->ncontinue & 0xF);
//        pstream->ncontinue++;
//        i++;
//    }
//}



//int32_t bml_realtime_get_packets(uint8_t** packets)
//{
//    Pxapp_realtime_stream pstart = xbml_realtime->streams;
//    uint32_t ticks = hal_lite_get_tick();
//    
//    while(pstart)
//    {
//        if((ticks - pstart->sendtimems) >= BML_RT_SPILT_MS)
//        {  
//            bml_realtime_load_packet(pstart);
//            pstart->sendtimems = ticks;
//        }
//        pstart = pstart->next;
//    }
//    
//    if(xbml_realtime->packet_buf_rptr != xbml_realtime->packet_buf_wptr)
//    {
//        *packets = &xbml_realtime->packet_buf[xbml_realtime->packet_buf_rptr*188];
//        xbml_realtime->packet_buf_rptr++;
//        if(xbml_realtime->packet_buf_rptr >= xbml_realtime->packet_buf_len)
//            xbml_realtime->packet_buf_rptr = 0;
//        return 1;
//    }
//    
//    return 0;
//}

//uint32_t bml_bitrate_to_packet_nums(uint32_t kbitrate)
//{
//    uint32_t packetnum = ((kbitrate * (1024/8))/188) / (1000/BML_RT_SPILT_MS);
//    packetnum++;
//    
//    if(packetnum > BML_STREAM_MAX_PACKETS)
//    {
//        printf("bml rt bitrate to fast no pcaket buffer : %d - %d\r\n",kbitrate,packetnum);
//        packetnum = BML_STREAM_MAX_PACKETS;
//    }
//    return packetnum;
//}

//vatek_result bml_reload_realtime(void)
//{
//    vatek_result result = xhead2_pesstream_reset();
////    FRESULT fr = FR_OK;
////    Pxapp_realtime_stream rootbml = NULL;
////    Pxapp_realtime_stream lastbml = NULL;
////    char* fullpath;
////    DIR* bmldir;
////    FILINFO* bmlfl;
////    
////    xbml_realtime = (Pxapp_bml_realtime)xhead2_pesstream_malloc(sizeof(xapp_bml_realtime));
////    if(xbml_realtime == NULL)printf("malloc bml buffer fail");
////    fullpath = &xbml_realtime->pathbuf[0];
////    bmldir = &xbml_realtime->fdir;
////    bmlfl = &xbml_realtime->bmlfl;
////	LCD.Byte.TIM16_flag=0;
////    fr = f_findfirst(&xbml_realtime->fdir,&xbml_realtime->bmlfl,BML_FOLDER_NAME,"*.ts");
////	LCD.Byte.TIM16_flag=1;
////  
////    while(fr == FR_OK && bmlfl->fname[0])
////    {

////        Pxapp_realtime_stream currbml = NULL;
////        printf("bml file : %s - %d\r\n",&bmlfl->fname[0],bmlfl->fsize);
////            
////        memset(fullpath,0,BML_FULLPATH_MAX_LEN);
////        memset(&xbml_realtime->pes_info,0,sizeof(xhead2_pesstream));
////        sprintf(fullpath,"%s/%s",BML_FOLDER_NAME,&bmlfl->fname[0]);
////        result = bml_pesstream_info_get("stream",fullpath,&xbml_realtime->pes_info);
////     
////        if(is_hal_lite_success(result))
////        {
////            result = bml_pesstream_create_realtime(&xbml_realtime->pes_info,fullpath,&currbml);
////            if(is_hal_lite_success(result))
////            {
////                currbml->packets_ptr = 0;
////                currbml->sendtimems = 0;
////                currbml->packets_nums = bmlfl->fsize / 188;
////                if(bmlfl->fsize % 188 != 0)printf("bad ts file format - %d\r\n",bmlfl->fsize);
////                currbml->eachtime_packet = bml_bitrate_to_packet_nums(xbml_realtime->pes_info.bitrate);
////                printf("stream : 0x%04x-%d-%d\r\n",xbml_realtime->pes_info.pid,xbml_realtime->pes_info.bitrate,currbml->eachtime_packet);
////                
////                if(rootbml == NULL)rootbml = currbml;
////                else lastbml->next = currbml;
////                lastbml = currbml;
////				LCD.Byte.TIM16_flag=0;
////                fr = f_findnext(bmldir,bmlfl);   
////				LCD.Byte.TIM16_flag=1;
////            }
////        }else if(result == vatek_result_invalidparm)
////        {
////            printf("bml file not valid : %s",&bmlfl->fname[0]);
////            result = vatek_result_success;
////			LCD.Byte.TIM16_flag=0;
////            fr = f_findnext(bmldir,bmlfl);
////			LCD.Byte.TIM16_flag=0;
////        }
////        else break;
////    }
//    
//    if(is_hal_lite_success(result))
//    {
//        xbml_realtime->packet_buf_rptr = 0;
//        xbml_realtime->packet_buf_wptr = 0;
//        xbml_realtime->packet_buf_len = BML_STREAM_MAX_PACKETS;
//        xbml_realtime->streams = rootbml;
//        xhead2_app_set_data_content(bml_create_data_content());
//    }
//    return result;
//}

//vatek_result bml_reload_async(uint8_t* pbuf)
//{
//    bstream bs;
//    vatek_result result = vatek_result_unsupport;
//    uint32_t val = 0;
//    BML_FLASH_READ(pbuf,BML_FLASH_START_ADDR,BML_FLASH_SECTION_LEN);
//    
//    bs.pbuf = pbuf;
//    bs.pos = 0;
//    
//    val = bstream_get_uint(&bs);
//    if(val == LICPES_FLASH_TAG)
//    {
//        uint32_t crc = 0;
//        printf("bml enable : %08x-%s",val,&bs.pbuf[bs.pos]);
//        bstream_seek(&bs,LICPES_CRC_OFFSET);
//        val = bstream_get_uint(&bs);
//        bstream_seek(&bs,LICPES_CRC_OFFSET);
//        bstream_put_uint(&bs,0);
//        
//        crc = tool_crc32(pbuf,BML_FLASH_SECTION_LEN);
//        result = vatek_result_invalidparm;
//        if(crc != val)printf("crc check fail : %08x - %08x",crc,val);
//        else
//        {
//            int32_t i = 0;
//            bstream_seek(&bs,LICPES_NUMBER_OFFSET);
//            crc = bstream_get_uint(&bs);
//            
//            for(i = 0;i < crc;i++)
//            {
//                uint32_t tag = 0;
//                result = vatek_result_invalidparm;
//                val = (i + 1) * BML_FLASH_PAGE_LEN;
//                bstream_seek(&bs,val);
//                
//                tag = bstream_get_uint(&bs);
//                if((tag & LICPES_STREAM_TAG) == LICPES_STREAM_TAG)
//                {
//                    Plicensed_pes_stream newpes = (Plicensed_pes_stream)xhead2_pesstream_malloc(sizeof(licensed_pes_stream));
//                    newpes->stream_pid = tag & 0xFFFF;
//                    newpes->stream_type = 0x0D;
//                    bstream_offset(&bs,16);
//                    newpes->es_info_len = bstream_get_uint(&bs);
//                    bstream_seek(&bs,val + (BML_FLASH_PAGE_LEN - 64));
//                    bstream_get_buf(&bs,&newpes->es_info[0],64);
//                    result = xhead2_pesstream_add(newpes);
//                    printf("add pes_stream : %d - [0x%04x-%02x-%d]",result,newpes->stream_pid,newpes->stream_type,newpes->es_info_len);
//                    
//                }else printf("get stream bad tag : %08x",tag);
//                
//                if(!is_hal_lite_success(result))break;
//            }
//            result = vatek_result_success;
//        }
//    }else printf("bml disable : %08x",val);
//    
//    if(result == vatek_result_unsupport)result = vatek_result_success;
//    return result;
//}


//vatek_result bml_async_update_start(void)
//{
//    vatek_result result = xhead2_pesstream_reset();
//    if(is_hal_lite_success(result))
//    {
//        xbml_async_writer = (Pxapp_async_updater)xhead2_pesstream_malloc(sizeof(xapp_async_updater));
//        if(xbml_async_writer == NULL)printf("malloc async writer fail");
////        else
////        {
////            DIR* bmldir = (DIR*)&xbml_async_writer->fdir;
////            char* fullpath = &xbml_async_writer->pathbuf[0];
////            FILINFO* bmlfl = &xbml_async_writer->bmlfl;
////			LCD.Byte.TIM16_flag=0;
////            FRESULT fr = f_findfirst(bmldir,bmlfl,BML_FOLDER_NAME,"*.ts");
////			LCD.Byte.TIM16_flag=1;
////            Pxapp_pes_stream rootbml = NULL;
////            Pxapp_pes_stream lastbml = NULL;

////            while(fr == FR_OK && bmlfl->fname[0])
////            {
////                Pxapp_pes_stream currbml = NULL;
////                printf("bml file : %s - %d\r\n",&bmlfl->fname[0],bmlfl->fsize);
////            
////                memset(fullpath,0,BML_FULLPATH_MAX_LEN);
////                sprintf(fullpath,"%s/%s",BML_FOLDER_NAME,&bmlfl->fname[0]);
////                result = bml_pesstream_create_async(fullpath,&currbml);
////                if(is_hal_lite_success(result))
////                {
////                    currbml->file_size = bmlfl->fsize;
////                    if(rootbml == NULL)rootbml = currbml;
////                    else lastbml->next = currbml;
////                    lastbml = currbml;
////					LCD.Byte.TIM16_flag=0;
////                    fr = f_findnext(bmldir,bmlfl);
////					LCD.Byte.TIM16_flag=1;
////                }else if(result == vatek_result_invalidparm)
////                {
////                    printf("bml file not valid : %s",&bmlfl->fname[0]);
////                    result = vatek_result_success;
////					LCD.Byte.TIM16_flag=0;
////                    fr = f_findnext(bmldir,bmlfl);
////					LCD.Byte.TIM16_flag=1;
////                }
////                else break;
////            }
//            
//            if(is_hal_lite_success(result) && rootbml != NULL)
//            {
//                uint32_t posflash = BML_FLASH_SECTION_LEN + BML_FLASH_START_ADDR;
//                xbml_async_writer->streams = rootbml;
//                xbml_async_writer->stream_ptr = rootbml;
//                xbml_async_writer->updater_step = 0;

//                lastbml = rootbml;
//                while(lastbml)
//                {
//                    lastbml->flash_pos = posflash;
//                    if(is_hal_lite_success(result))
//                    {
//                        posflash += BML_ALIGC_SECTION(lastbml->file_size);
//                        lastbml = lastbml->next;   
//                    }else break;
//                }
//                
//                #if !BML_UPDATE_UESD_PROGRESS
//                    LCD_CLR();
//                    lcd_xy(1,1);
//                    LCD_PrintString("Update BML");
//                    lcd_xy(1,2);
//                    LCD_PrintString("Write Data");
//                #else
////                    LCD.Byte.UPLoadingFlag=LCD_UPLoading;
////                    LCD.Byte.UPLoadingMSG=UPLOAD_SET;
////                    LCD.Byte.UPLoadingWork=0;
//                #endif
//                
//                BML_FLASH_INIT();  /* initial spi gpio and status */
//                BML_FLASH_ERASE(BML_FLASH_START_ADDR,BML_FLASH_SECTION_LEN);
//                printf("bml async start update file");
//            }else
//            {
//                if(rootbml == NULL)printf("check bml async update no valid files");
//                else printf("check bml async update fail : %d\r\n",result);
//                xbml_async_writer = NULL;
//            }
//        }
//    }
//    return result;
//}

//vatek_result bml_async_update_write(void)
//{
//    vatek_result result = vatek_result_badstatus;
//    if(xbml_async_writer != NULL)
//    {
//        Pxapp_pes_stream pstream = xbml_async_writer->stream_ptr;
//        if(pstream)
//        {
//            printf("write stream : 0x%04x-%d-%d\r\n",pstream->info.pid,pstream->file_size,pstream->file_write_pos);
//            
//            #if BML_UPDATE_UESD_PROGRESS
//                LCD.Byte.UPLoadingWork=(pstream->file_write_pos)*100/(pstream->file_size);
//                LCD.Byte.Action=LCD_Action_ON;
//            #endif
//            result = bml_write_bml(&xbml_async_writer->section_buf[0],pstream);
//            if(is_hal_lite_success(result))
//            {
//                if(pstream->file_write_pos >= pstream->file_size)
//                    xbml_async_writer->stream_ptr = pstream->next;
//            }            
//        }else
//        {
//            result = bml_write_bml_header(&xbml_async_writer->section_buf[0],xbml_async_writer->streams);
//            if(is_hal_lite_success(result))
//            {
//                printf("write bml async header finish");
//                xbml_async_writer->updater_step++;
//            }
//            else printf("write bml async header fail : %d\r\n",result);
//            
//        }
//    }
//    return result;
//}

//vatek_result bml_async_update_stop(void)
//{
//    vatek_result result = vatek_result_success;
//    FRESULT fr;
//    Pxapp_pes_stream lastbml = xbml_async_writer->streams;
//    while(lastbml)
//    {
//		LCD.Byte.TIM16_flag=0;
//        fr = f_close(&lastbml->fhandle);
//		LCD.Byte.TIM16_flag=1;
//        if(fr == FR_OK)
//        {
//			LCD.Byte.TIM16_flag=0;
//            fr = f_unlink(&lastbml->full_path[0]);
//			LCD.Byte.TIM16_flag=1;
//            if(fr != FR_OK)printf("delete file fail : %s - %d\r\n",&lastbml->full_path[0],fr);            
//        }else printf("close file fail : %s - %d\r\n",&lastbml->full_path[0],fr);          

//        lastbml = lastbml->next;
//    }
//    
//    result = xhead2_pesstream_reset();
//    xbml_async_writer = NULL;
//    xbml_async_updatecheck = 0;
//    BML_FLASH_FREE();
//    
//    #if !BML_UPDATE_UESD_PROGRESS
//        LCD_CLR();
//    #else
//        LCD.Byte.Action=LCD_Action_ON;
//        LCD.Byte.Action1st=LCD_Action_ON;
//        LCD.Byte.UPLoadingFlag=LCD_MENU;
//    #endif
//    return result;
//}

//vatek_result bml_write_bml(uint8_t* pbufsect,Pxapp_pes_stream pbmls)
//{
//    FRESULT fr = FR_OK;
//    vatek_result result = hal_lite_unknown;;
//    
//    if(pbmls->file_write_pos == 0){
//		LCD.Byte.TIM16_flag=0;
//		fr = f_lseek(&pbmls->fhandle,0);
//		LCD.Byte.TIM16_flag=1;
//	}
//    if(fr != FR_OK)printf("seek bml file fail : %d\r\n",fr);
//    else
//    {
//        int32_t pos = 0;
//        uint32_t rlen = 0;
//        int32_t each = pbmls->file_size - pbmls->file_write_pos;  
//        if(each > BML_FLASH_SECTION_LEN)each = BML_FLASH_SECTION_LEN;
//        if(each != BML_FLASH_SECTION_LEN)memset(pbufsect,0xFF,BML_FLASH_SECTION_LEN);
//		LCD.Byte.TIM16_flag=0;
//        fr = f_read(&pbmls->fhandle,pbufsect,each,&rlen);
//		LCD.Byte.TIM16_flag=1;
//        if(fr != FR_OK || each != rlen)printf("read fail : %d-%d-%d\r\n",fr,each,rlen);
//        else
//        {
//            uint32_t addr = pbmls->file_write_pos + pbmls->flash_pos;
//            BML_FLASH_ERASE(addr,BML_FLASH_SECTION_LEN);
//            LCD.Byte.MX_LWIP_Process_flag=0;
//            BML_FLASH_WRITE(pbufsect,addr,BML_FLASH_SECTION_LEN);
//            pbmls->file_write_pos += BML_FLASH_SECTION_LEN;
//            result = vatek_result_success;
//        }
//    }
//    return result;
//}

//vatek_result bml_write_bml_header(uint8_t* pbufsect,Pxapp_pes_stream proot)
//{
//    vatek_result result = vatek_result_invalidparm;
//    uint32_t nums = (uint32_t)bml_get_stream_nums(proot);
//    if(nums > BML_MAX_PESNUMS)printf("bml pes_stream overflow [%d-%d]",nums,BML_MAX_PESNUMS);
//    else
//    {
//        bstream bs;
//        Pxapp_pes_stream pnext = proot;
//        int32_t index = 0;
//        
//        bs.pbuf = pbufsect;
//        bs.pos = 0;
//        memset(pbufsect,0xFF,BML_FLASH_SECTION_LEN);
//        
//        bstream_put_uint(&bs,LICPES_FLASH_TAG);
//        bstream_put_string(&bs,"micomsoft-b01",16);
//        bstream_put_uint(&bs,(uint32_t)bml_get_stream_nums(proot));
//        
//        bstream_seek(&bs,LICPES_CRC_OFFSET);
//        bstream_put_uint(&bs,0);
//        
//        while(pnext)
//        {
//            nums = (index + 1) * BML_FLASH_PAGE_LEN;
//            bstream_seek(&bs,nums);
//            bstream_put_uint(&bs,LICPES_STREAM_TAG | pnext->info.pid);
//            bstream_put_uint(&bs,pnext->info.component_tag);
//            bstream_put_uint(&bs,pnext->info.bitrate);
//            bstream_put_uint(&bs,pnext->flash_pos);
//            bstream_put_uint(&bs,pnext->file_size);
//            bstream_put_uint(&bs,pnext->info.es_info_len);
//            bstream_seek(&bs,nums + (BML_FLASH_PAGE_LEN - 64));
//            bstream_put_buf(&bs,&pnext->info.es_info_buf[0],64);
//            index++;
//            pnext = pnext->next;
//        }
//        
//        bstream_seek(&bs,LICPES_CRC_OFFSET);
//        bstream_put_uint(&bs,tool_crc32(pbufsect,BML_FLASH_SECTION_LEN));
//        
//        BML_FLASH_WRITE(pbufsect,BML_FLASH_START_ADDR,BML_FLASH_SECTION_LEN);
//        
//        result = vatek_result_success;
//    }
//    return result;
//}

//int32_t bml_get_stream_nums(Pxapp_pes_stream pstreams)
//{
//    int32_t result = 0;
//    while(pstreams)
//    {
//        result++;
//        pstreams = pstreams->next;
//    }
//    return result;
//}

//char* bml_filename_to_int(char* ptr,int32_t* val,int32_t dec);

////static const uint8_t bml_es_info_data_component_desc[] = {0xFD,0x02,0x00,0x00,};
//static const uint8_t bml_addinfo_40[] = {0x33,0x3F,0x00,0x03,0x00,0x00,0xFF,0xBF,};
//static const uint8_t bml_addinfo_60[] = {0x1F,0xFF,0xBF,};
//static const uint8_t bml_eit_data_contents_desc[] = 
//{
//    0xC7,0x1B,0x00,0x0C,0x40,0x11,0x23,0x26,
//    0x6A,0x70,0x6E,0x00,0x03,0x00,0x00,0x01,
//    0x40,0xF2,0xFF,0xFF,0xFF,0xFF,0xBF,0x01,
//    0x60,0x6A,0x70,0x6E,0x00,
//};

//Plicensed_epg_desc bml_create_data_content(void)
//{
//    Plicensed_epg_desc pdesc = (Plicensed_epg_desc)xhead2_pesstream_malloc(sizeof(licensed_epg_desc));
//    pdesc->tag = EPG_DESCTAG_ARIB_DATADESC;
//    pdesc->len = sizeof(bml_eit_data_contents_desc);
//    memcpy(&pdesc->desc_buf[0],&bml_eit_data_contents_desc[0],pdesc->len);
//    return pdesc;
//}

//extern void bs_info_put_component(Pbstream pbs,uint8_t component_tag);
//extern void bs_info_pit_data_component(Pbstream pbs,uint16_t componentid,uint8_t* paddinfo,int32_t len);

//vatek_result bml_pesstream_info_get(const char* tag,const char* filename,Pxhead2_pesstream pinfo)
//{
//    char* szptr = strstr(filename,tag);
//    vatek_result result = vatek_result_invalidparm;
//    
//    if(szptr != NULL && szptr[strlen(tag) + 1] == '_')
//    {
//        int32_t val = 0;
//        szptr+= strlen(tag) + 2;
//        szptr = bml_filename_to_int(szptr,&val,16);
//        if(szptr != NULL && *szptr == '_')
//        {
//            szptr++;
//            pinfo->pid = (uint16_t)val;
//            szptr = bml_filename_to_int(szptr,&val,16);
//            if(szptr != NULL&& *szptr == '_')
//            {
//                szptr++;
//                pinfo->component_tag = (uint8_t)val;
//                szptr = bml_filename_to_int(szptr,&val,10);
//                if(szptr != NULL && *szptr == '.')
//                {
//                    pinfo->bitrate = (uint16_t)val;
//                    bstream bs;
//                    bs.pbuf = &pinfo->es_info_buf[0];
//                    bs.pos = 0;
//                        
//                    bs_info_put_component(&bs,pinfo->component_tag);
//                        
//                    if(pinfo->component_tag == 0x40)
//                        bs_info_pit_data_component(&bs,0x000C,(uint8_t*)&bml_addinfo_40[0],sizeof(bml_addinfo_40));
//                    else if(pinfo->component_tag == 0x60)
//                        bs_info_pit_data_component(&bs,0x000C,(uint8_t*)&bml_addinfo_60[0],sizeof(bml_addinfo_60));
//                    else bs_info_pit_data_component(&bs,0x000C,NULL,0);
//                        
//                    pinfo->es_info_len = bs.pos;
//                        
//                    printf("new %s : 0x%04x-%02x-%04d-%d\r\n",tag,pinfo->pid,pinfo->component_tag,pinfo->bitrate,pinfo->es_info_len);
//                    result = vatek_result_success;                    
//                }
//            }
//        }
//    }
//    return result;
//    
//}

//vatek_result bml_pesstream_create_realtime(Pxhead2_pesstream pinfo,const char* fullpath,Pxapp_realtime_stream* pstream)
//{
//    Pxapp_realtime_stream newstream = (Pxapp_realtime_stream)xhead2_pesstream_malloc(sizeof(xapp_realtime_stream));
//    vatek_result result = hal_lite_unknown;
//    if(newstream == NULL)printf("malloc bml rt stream fail");
//    else
//    {
//		LCD.Byte.TIM16_flag=0;
//        FRESULT fr = f_open(&newstream->fhandle,fullpath,FA_READ);
//		LCD.Byte.TIM16_flag=1;
//        result = hal_lite_unknown;
//        if(fr == FR_OK)
//        {
//            newstream->info.stream_pid = pinfo->pid;
//            newstream->info.stream_type = 0x0D;
//            newstream->info.es_info_len = pinfo->es_info_len;
//            memcpy(&newstream->info.es_info[0],&pinfo->es_info_buf[0],pinfo->es_info_len);
//            
//			LCD.Byte.TIM16_flag=0;
//            f_lseek(&newstream->fhandle,0);
//			LCD.Byte.TIM16_flag=1;
//            newstream->packets_ptr = 0;
//            
//            result = xhead2_pesstream_add(&newstream->info);
//            printf("add pes_stream : %d - [0x%04x-%02x-%d]\r\n",
//                     result,newstream->info.stream_pid,newstream->info.stream_type,newstream->info.es_info_len);            
//            if(is_hal_lite_success(result))*pstream = newstream;
//        }else printf("open file fail : %d\r\n",fr);

//    }
//    return result;
//}

//vatek_result bml_pesstream_create_async(const char* filename,Pxapp_pes_stream* pappstream)
//{
//    Pxapp_pes_stream newstream = (Pxapp_pes_stream)xhead2_pesstream_malloc(sizeof(xapp_pes_stream));
//    vatek_result result = bml_pesstream_info_get("data",filename,&newstream->info);
//    
//    if(is_hal_lite_success(result))
//    {
//		LCD.Byte.TIM16_flag=0;
//        FRESULT fr = f_open(&newstream->fhandle,filename,FA_READ);
//		LCD.Byte.TIM16_flag=1;
//        result = hal_lite_unknown;
//        if(fr == FR_OK)
//        {
//            result = vatek_result_success;
//            strncpy(&newstream->full_path[0],filename,BML_FULLPATH_MAX_LEN);
//            *pappstream = newstream;
//        }
//        else printf("open file fail : %d\r\n");
//    }
//    return result;
//}

//void bs_info_put_component(Pbstream pbs,uint8_t component_tag)
//{
//    bstream_put_byte(pbs,0x52);
//    bstream_put_byte(pbs,1);
//    bstream_put_byte(pbs,component_tag);
//}

//void bs_info_pit_data_component(Pbstream pbs,uint16_t componentid,uint8_t* paddinfo,int32_t len)
//{
//    bstream_put_byte(pbs,0xFD);
//    bstream_put_byte(pbs,2 + len);
//    bstream_put_byte(pbs,componentid >> 8);
//    bstream_put_byte(pbs,componentid);
//    
//    if(len != 0)bstream_put_buf(pbs,paddinfo,len);
//}

//char* bml_filename_to_int(char* ptr,int32_t* val,int32_t dec)
//{
//    char tmp[8];
//    int32_t pos = 0;
//    int32_t iscurrent = 0;
//    memset(&tmp[0],0,8);
//    
//    while(*ptr)
//    {
//        if(*ptr == '_' || *ptr == '.')
//        {
//            tmp[pos] = '\0';
//            *val = strtol(&tmp[0],NULL,dec);
//            iscurrent = 1;
//            break;
//        }else tmp[pos++] = *ptr++;
//        if(pos >= 8)break;
//    }
//    
//    if(iscurrent)return ptr;
//    return NULL;
//}

//void bstream_put_uint(Pbstream pbs,uint32_t val)
//{
//    uint8_t* ptr = &pbs->pbuf[pbs->pos];
//    ptr[0] = (uint8_t)(val >> 24);
//    ptr[1] = (uint8_t)(val >> 16);
//    ptr[2] = (uint8_t)(val >> 8);
//    ptr[3] = (uint8_t)(val);
//    pbs->pos += 4;
//}

//void bstream_put_string(Pbstream pbs,const char* val,int32_t len)
//{
//    uint8_t* ptr = &pbs->pbuf[pbs->pos];
//    int32_t l = strlen(val);
//    int32_t pos = 0;
//    if(l > len)l = len;
//    
//    while(l)
//    {
//        ptr[pos] = *val++;
//        l--;
//        pos++;
//    }
//    if(pos < len)ptr[pos] = '\0';
//    pbs->pos += len;
//}

//void bstream_put_buf(Pbstream pbs,uint8_t* pbuf,int32_t len)
//{
//    memcpy(&pbs->pbuf[pbs->pos],pbuf,len);
//    pbs->pos += len;
//}

//void bstream_put_byte(Pbstream pbs,uint8_t val)
//{
//    pbs->pbuf[pbs->pos++] = val;
//}

//uint32_t bstream_get_uint(Pbstream pbs)
//{
//    uint8_t* ptr = &pbs->pbuf[pbs->pos];
//    uint32_t val = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
//    pbs->pos += 4;
//    return val;
//}

//void bstream_get_string(Pbstream pbs,char* val,int32_t len)
//{
//    uint8_t* ptr = &pbs->pbuf[pbs->pos];
//    strncpy(val,(char*)ptr,len);
//    pbs->pos += len;
//}

//void bstream_get_buf(Pbstream pbs,uint8_t* pbuf,int32_t len)
//{
//    uint8_t* ptr = &pbs->pbuf[pbs->pos];
//    memcpy(pbuf,(char*)ptr,len);
//    pbs->pos += len;
//}

//void bstream_seek(Pbstream pbs,int32_t pos)
//{
//    pbs->pos = pos;
//}

//void bstream_offset(Pbstream pbs,int32_t offset)
//{
//    pbs->pos += offset;
//}



//uint16_t  BML_STATUS()
//{
//	return xbml_stream.route.async.status;
//}
