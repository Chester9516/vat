#ifndef QTV_DEVICE_H
#define QTV_DEVICE_H

#include <core/qtv_define.h>
#include <core/base/chip_define.h>
#include <core/base/calibration_define.h>
#include <vatek_sdk_bridge.h>

class QTV_DECL qtvBridge : public QObject
{
	Q_OBJECT
public:
	virtual hvatek_bridge _handle() = 0;
	virtual Pbdevice_info _info() = 0;
	virtual QList<Pbridge_source> &_sources() = 0;

	virtual vatek_result updateSource(int32_t idx, Pbridge_source psource) = 0;

	/* check current active source status */
	virtual vatek_result startSource(Pbridge_source psource) = 0;
	virtual vatek_result checkSource(Pbridge_source psource) = 0;
	virtual vatek_result stopSource(Pbridge_source psource) = 0;
};

class QTV_DECL qtvDevice : public QObject
{
	Q_OBJECT
public:
	virtual hvatek_chip _handle() = 0;
	virtual Pchip_info _chip_info() = 0;
	virtual uint16_t _chip_bus() = 0;
	virtual hal_service_mode _chip_service() = 0;
	virtual qtvBridge* _bridge() = 0;
	virtual QString& _name() = 0;

	static const char* getServiceName(qtvDevice* device);
};

class QTV_DECL qtvDeviceManage : public QObject
{
	Q_OBJECT
public:
	static vatek_result enumDevices(uint32_t bus, hal_service_mode mode);
	static vatek_result openDevice(int32_t idx, qtvDevice** device);
	static void closeDevice(qtvDevice* device, int32_t isreboot);
};

#endif
