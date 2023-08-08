
#ifndef _REGISTER_TABLE_
#define _REGISTER_TABLE_

#ifdef __cplusplus
    extern "C" {
#endif
    #include "stdint.h"
    
    typedef struct _reg_map_table
    {
        uint32_t idx;
        uint32_t reg;
        uint32_t mreg;
    }reg_map_table;

    static reg_map_table vi_reg_table[] = 
    {
        { 1, 0x600, 0x1800},
        { 2, 0x601, 0x1804},
        { 3, 0x602, 0x1808},
        { 4, 0x603, 0x180C},
        { 5, 0x604, 0x1810},
        { 6, 0x605, 0x1814},
        { 7, 0x606, 0x1818},
        { 8, 0x607, 0x181C},
        { 9, 0x608, 0x1820},
        {10, 0x609, 0x1824},
        {11, 0x60A, 0x1828},
        {12, 0x60B, 0x182C},
        {13, 0x60C, 0x1830},
        {14, 0x60D, 0x1834},
        {15, 0x60E, 0x1838},
        {16, 0x60F, 0x183C},
        {17, 0x630, 0x18C0},
        {19, 0x631, 0x18C4},
        {20, 0x632, 0x18C8},
        {21, 0x633, 0x18CC},
        {22, 0x634, 0x18D0},
    };


#ifdef __cplusplus
    }
#endif

#endif

