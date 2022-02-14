#ifndef _HAL_USB_DEVICE_
#define _HAL_USB_DEVICE_

/* 
	_vatek_chip internal usb device interface defined :
		- usb_rescure : loader supported could update image or app
		- usb_normal : used to control chip function, update image and stream (a3 only)
*/

#include <vatek_base.h>
#include <base/tsstream_define.h>

#define USBDEV_VID						0x1934
#define USBDEV_STREAM_SLICE_LEN			24064
#define USBDEV_STREAM_SLICE_PACKETS		(USBDEV_STREAM_SLICE_LEN/TS_PACKET_LEN)

typedef void* husb_device;
typedef void* husb_device_list;

typedef enum _usbdevice_type
{
	usb_type_unknown = -1,
	usb_type_broadcast = 0,
	usb_type_rescure = 1,
	usb_type_all = 2,
}usbdevice_type;

typedef struct _usbdevice_id
{
	usbdevice_type type;
	uint16_t vid;
	uint16_t pid;
}usbdevice_id, *Pusbdevice_id;

#define VATCMD_CLASSV2_MASK                     0x40

#define VATCMD_CLASSV2_SET_MODE					0x41
	#define CLASSV2_MODE_NORMAL					0x0000
	#define CLASSV2_MODE_HWDMA					0x0001

/* put data to usb_cache then used write_ram, write_rom and run code	*/
/* param_0 : address position											*/
#define VATCMD_CLASSV2_PUTDATA					0x42

/* put 4K code then write to mcu_ram									*/
/* param_0 : section number												*/

#define VATCMD_CLASSV2_WRITE_RAM				0x43
/* put 4K code then write to mcu_rom									*/
/* param_0 : section number												*/
#define VATCMD_CLASSV2_WRITE_ROM				0x44

/* put 8K _mcu vector code then run start run code						*/
#define VATCMD_CLASSV2_RUNCODE					0x45		


#define VATCMD_CLASSV2_SETADDR                  0x4A
#define VATCMD_CLASSV2_RDREG                    0x4C
#define VATCMD_CLASSV2_WRREG                    0x4D
#define VATCMD_CLASSV2_RDMEM                    0x4E
#define VATCMD_CLASSV2_WRMEM                    0x4F

#define is_vacmd_classv2(r)                     ((r & VATCMD_CLASSV2_MASK) == VATCMD_CLASSV2_MASK)

#endif
