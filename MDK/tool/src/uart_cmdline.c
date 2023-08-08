
#if defined(DEBUG)

#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <stdint.h>
#include "uart_cmdline.h"
#include "vatek_hms.h"
#include "vatek_system.h"
#include "reg_map.h"

#define CMDLINE_ERR(fmt,...)    printf("error [%s-%d]"fmt"\r\n",__func__ ,__LINE__,##__VA_ARGS__)
#define CMDLINE_LOG(fmt,...)    printf(fmt"\r\n",##__VA_ARGS__)
#define CMD_LEN 128
static uint8_t Line[CMD_LEN] = {0};
static Pcmdline_table cmdlist = NULL;
static uint16_t cmdnum = 0;

static void _default_cli_help(void);
static void _default_cli_dump_reg_all(void);
static void _default_cli_dump_reg_one(uint32_t addr);
static void _default_cli_dump_reg_range(uint32_t min_addr, uint32_t max_addr);
static void _default_cli_write_reg_one(uint32_t addr, uint32_t val);

static Phms_handle handle = NULL;

static cmdline_table _default_cli[] = 
{
    {"help"          , (void*)_default_cli_help           , 0},
    {"dump_reg_all"  , (void*)_default_cli_dump_reg_all   , 0},
    {"dump_reg_one"  , (void*)_default_cli_dump_reg_one   , 1},
    {"dump_reg_range", (void*)_default_cli_dump_reg_range , 2},
    {"write_reg_one" , (void*)_default_cli_write_reg_one  , 2},
};

static void _default_cli_help(void)
{
    CMDLINE_LOG("All Command");
}

static void _default_cli_dump_reg_all(void)
{
    uint32_t val = 0, i = 0;
    vatek_result result = vatek_result_unknown;
    
    for(i=0; i<sizeof(vi_reg_table)/sizeof(reg_map_table); i++)
    {
        result = vatek_hms_read_reg(handle, vi_reg_table[i].mreg, &val);
        if(result==vatek_result_success)
            CMDLINE_LOG("[0x%08x] - 0x%08x", vi_reg_table[i].reg, val);
        else
        {
            CMDLINE_LOG("reg get fail %d", result);
            break;
        }
    }
}

static void _default_cli_dump_reg_one(uint32_t addr)
{
    uint32_t val = 0;
    vatek_result result = vatek_hms_read_reg(handle, addr, &val);
    if(result==vatek_result_success)
        CMDLINE_LOG("[0x%08x] - 0x%08x", addr, val);
    else CMDLINE_LOG("reg get fail %d", result);
}

static void _default_cli_dump_reg_range(uint32_t min_addr, uint32_t max_addr)
{
}

static void _default_cli_write_reg_one(uint32_t addr, uint32_t val)
{
}
    


static int _getline(uint8_t *data, int len)
{
    int bytes_read = 0;
    char tmp;
    
    if(data == NULL)
        return bytes_read;
    
    for (bytes_read = 0; bytes_read < len-1; bytes_read++)
    {
        tmp = getc(stdin);

        if (tmp == '\0')
        {
            if (bytes_read)
            {
                bytes_read--;
                continue;
            }
            else
                break;  //timeout & no input data
        }
        
        if (tmp == '\r' || tmp == '\n')
            break;
        
        *data++ = tmp;
    }
    *(data++) = '\0';
    
    return bytes_read;
}

static uint8_t uart_cmdline_parsing(uint8_t *cmd, uint8_t len)
{
    uint8_t tmp_buf[CMD_LEN] = "";
    char *buf = NULL;
    char *p[CMD_LEN];
    uint32_t in = 0;
    uint16_t idx = 0;
    uint8_t bfind = 0;
    
    memcpy(tmp_buf, cmd, len);
    
    buf = (char *)tmp_buf;
    while((p[in] = strtok(buf, " \t\n")) != NULL)
    {
        buf = NULL;
        in++;
    }

    for (idx = 0; idx < cmdnum; idx++)
    {
        if (memcmp(p[0], cmdlist[idx].keyword, strlen((const char *)p[0])) == 0)
        {
            bfind = 1;
            break;
        }
    }

    if (bfind && cmdlist[idx].fn != NULL)
    {
        if ((in-1) < cmdlist[idx].parmnum)
        {
            CMDLINE_LOG("incorrect parm num\n");
            return 1;
        }

        switch (cmdlist[idx].parmnum)
        {
            case 0:
                cmdlist[idx].fn(0);
                break;

            case 1:
                cmdlist[idx].fn(atoi(p[1]));
                break;
            
            case 2:
                cmdlist[idx].fn(atoi(p[1]), atoi(p[2]));
                break;

            case 3:
                cmdlist[idx].fn(atoi(p[1]), atoi(p[2]), atoi(p[3]));
                break;
            
            case 4:
                cmdlist[idx].fn(atoi(p[1]), atoi(p[2]), atoi(p[3]), atoi(p[4]));
                break;
            
            default:
                CMDLINE_LOG("unsupport input parm num : %d\n", cmdlist[idx].parmnum);
                return 1;
        }
    }
    else 
    {
        CMDLINE_LOG("unknown cmd = %s\n", p[0]);
        return 1;
    }
      
    return 0;
}   

vatek_result uart_cmdline_get_interface(Phms_handle handler)
{
    if(handler==NULL)
        return vatek_result_invalidparm;
    
    handle = handler;
    
    return vatek_result_success;
}

uint32_t uart_cmdline_init(Pcmdline_table list, uint16_t num)
{
    if (list == NULL || num == 0)
    {
        cmdlist = _default_cli;
        cmdnum  = sizeof(_default_cli)/sizeof(cmdline_table);
    }else
    {
        cmdlist = list;
        cmdnum = num;
    }   
    
    return 0;
}

uint32_t uart_cmdline_polling(void)
{
    memset(&Line, '\0', CMD_LEN);
    if (_getline(Line, CMD_LEN)!=0)
    {
        if (uart_cmdline_parsing(Line, strlen((const char *)Line)))
            CMDLINE_LOG("[%s] : CMD Fail\n", Line);
        else
            CMDLINE_LOG("[%s] : CMD Done\n", Line);
    }
    else
        return 1;
    
    return 0;
}

#endif
