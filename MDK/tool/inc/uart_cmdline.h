
#ifndef _UART_CMDLINE_
#define _UART_CMDLINE_

#ifdef __cplusplus
    extern "C" {
#endif
    #include "vatek_hms.h"
    #include "vatek_system.h"

    typedef void (*Pfn)(uint32_t num, ...);
    typedef struct _cmdline_table
    {
        uint8_t keyword[32];
        Pfn fn;
        uint8_t parmnum;
    }cmdline_table, *Pcmdline_table;
    

    uint32_t uart_cmdline_init(Pcmdline_table list, uint16_t num);    
    uint32_t uart_cmdline_polling(void);
    
    vatek_result uart_cmdline_get_interface(Phms_handle handler);


#ifdef __cplusplus
    }
#endif

#endif

