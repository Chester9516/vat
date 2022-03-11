## VATEK source code
VATEK source code used to control VATEK chip, VATEK provide two type of source code, base on OS and Embeded system.

### VATEK CHIP
VATEK provide two series chip, A series chip of modulation is for transforming, and B series chip of ENMODER (encoder & modulator) are for broadcasting, two series of chip are used to broadcast on DTV by output transport stream. All VATEK chip are controlled by writing and reading register. You can see more register detail in [RegisterMap_A](https://github.com/Chester9516/vat/blob/source-code/docs/220216_1_3_Asereis_Register%20Map.pdf) and [RegisterMap_B](https://github.com/Chester9516/vat/blob/source-code/docs/220216_1_3_Bseries_Register%20Map.pdf). If you want to know more about VATEK chip, click [here](http://www.vatek.com.tw/) to foward VATEK website.

### VAT SDK
VAT SDK is the software to control the chip made by VATEK. VATEK provide the sample code of SDK to support user understand the SDK source code, SDK source code develop depend on OS, and it can develop on windows 10 and LINUX OS.<br>
The SDK was originally compiled in the way of CMAKE to obtain the example execution file. Vatek provides instructions for CMAKE compilation, so that users can get started quickly. Users can also compile on their own with a familiar compilation platform and read the file to get more relevant operation. see more detail in [SDK](https://github.com/Chester9516/vat/blob/source-code/SDK/20220301_SDK.pdf) document

### VAT MDK
MDK is the software to control the VATEK chip by MCU, MDK source code is writing based on MCU of STM32, but if user have other MCU, we also provide porting function, user can port I2C, GPIO and UART interface to other MCU easily. VATEK provide the sample or project in project folder, user can open it by KEIL C, and user will see the sample code to know how to control VATEK chip. see the MDK documents [MDK_A](https://github.com/Chester9516/vat/blob/source-code/MDK/220216_MDK_Aseries.pdf) or [MDK_B](https://github.com/Chester9516/vat/blob/source-code/MDK/220216_MDK_Bseries.pdf)
