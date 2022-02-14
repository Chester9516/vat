
#ifndef _PHY_ADV7180_INTERNAL_
#define _PHY_ADV7180_INTERNAL_

#ifdef __cplusplus
    extern "C" {
#endif

    #define ADV7180_SLAVE_WRITE                 0x42
    #define ADV7180_SLAVE_READ                  ADV7180_SLAVE_WRITE | 1
    #define ADV7180_BUFFER                      0
    
    #define ADV7180_REG_Input_Control           0x00
    #define ADV7180_OUTPUT_CONTROL              0x03
        #define ADV7180_OUTPUT_CONTROL_SEL              0x40        //output pins drivers 0:enable / 1:tristated
    #define ADV7180_REG_STATUS1                 0x10
        #define ADV7180_STATUS1_IN_LOCK                 0x01
        #define ADV7180_STATUS1_LOST_LOCK               0x02
    #define ADV7180_REG_IDENT                   0x11
        #define ADV7180_VALUE_IDENT                     0x1E
    #define ADV7180_REG_STATUS3                 0x13
        #define ADV7180_STATUS3_SD_OP_50Hz              0x04        //0:SD 60Hz / 1:SD 50Hz
        #define ADV7180_STATUS3_Interlace               0x40
    #define ADV7180_ADI_CNT2				    0x1D
        #define ADV7180_ADI_CNT2_LLC				    0x80		//0:LLC pin active / 1:LLC pin tristated    
    #define ADV7180_REG_ADI_CONTROL1            0x0E
        #define ADV7180_VALUE_SUB_USR_EN                0x20
    #define ADV7180_REG_INTERRUPT_Mask4         0x50    
        #define ADV7180_VALUE_INTERRUPT_Mask4           0x01
    #define ADV7180_REG_VDP_STATUS              0x78 //R:status  W:update
        #define ADV7180_VALUE_CCAP_DETECT               0x01
        #define ADV7180_VALUE_CCAP_FIELD                0x02
    #define ADV7180_REG_CCAP_DATA0              0x79
    #define ADV7180_REG_CCAP_DATA1              0x7A


#ifdef __cplusplus
    }
#endif

#endif

