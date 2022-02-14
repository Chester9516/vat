# vat
# GITHUB User Guide
Description of Source code
# VAT SDK
VAT SDK is the software to control the chip made by VATEK. VATEK provide the sample code of SDK to support user understand the SDK source code, SDK source code develop depend on OS, and it can develop on windows 10 and LINUX OS.

# SDK BUILD
SDK原先以CMAKE的方式進行編譯，進而取得範例執行檔，VATEK提供CMAKE編譯的使用說明，讓使用者可以快速上手，使用者也可以自行以熟悉的編譯平台進行編譯，閱讀文件以獲得更多相關操作說明。

# VATEK CHIP
VATEK provide two series chip, A3 chip of modulation is for transforming, and B chip of ENMODER (encoder & modulator) are for broadcasting, two series of chip are used to broadcast on DTV by output transport stream. All VATEK chip are controlled by writing and reading register.

# MDK
MDK is the software to control the VATEK chip by MCU, MDK source code is writing based on MCU of STM32, but if user have other MCU, we also provide porting function, user can port I2C, GPIO and UART interface to other MCU easily. VATEK provide the sample or project in project folder, user can open it by KEIL C, and user will see the sample code to know how to control VATEK chip.

