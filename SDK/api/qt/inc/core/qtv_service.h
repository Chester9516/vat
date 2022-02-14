#ifndef QTV_SERVICE_H
#define QTV_SERVICE_H

#include <core/qtv_device.h>
#include <core/qtv_mux.h>
#include <core/qtv_source.h>
#include <core/qtv_dtv.h>
#include <chart/qtv_chart_define.h>

#define QCALIBRATION_EN			0x00000001
#define QCALIBRATION_R2			0x00000002

class QTV_DECL qtvServiceHandle : public QObject
{
	Q_OBJECT
public:
	virtual vatek_result start() = 0;
	virtual vatek_result polling(Pbroadcast_info* pinfo) = 0;
	virtual vatek_result calibrationGet(Pcalibration_param pcalibration) = 0;
	virtual vatek_result calibrationSet(Pcalibration_param pcalibration) = 0;
	virtual vatek_result calibrationSave() = 0;
	virtual uint32_t calibratiobCheck() = 0;
	virtual void stop() = 0;
};

class QTV_DECL qtvServiceBase : public QObject
{
	Q_OBJECT
public:
	static vatek_result createService(qtvDevice* device, qtvServiceBase** service);
	~qtvServiceBase();

	virtual qtvDevice* _device() { return m_device; }
	virtual qtv_service_mode _mode() { return m_mode; }
	virtual qtv_status _status() { return m_status; }
	virtual qtvServiceHandle* _sevice_handle() { return m_handle; }

	virtual qtvPropertyCollection* _output_properties() = 0;
	virtual Pqtv_output_param _output_param() = 0;
	virtual Pbroadcast_info _broadcast_detail() = 0;
	virtual qtvChartDataCaptureService* _chart_data() { return m_chartdata; }

protected:
	explicit qtvServiceBase(qtv_service_mode mode, qtvDevice* device);

	qtv_status m_status;
	qtv_service_mode m_mode;
	qtvDevice* m_device;
	qtvServiceHandle* m_handle;
	qtvChartDataCaptureService* m_chartdata;

	virtual vatek_result startHandle(qtvServiceHandle* handle);
	virtual void stopHandle();
	virtual void changedStatus(qtv_status status);

Q_SIGNALS:
	void notifyStatusChanged(qtv_status status);
};

class QTV_DECL qtvServiceChipBase : public qtvServiceBase
{
	Q_OBJECT
public:
	virtual vatek_result setLocalDefault(qtvDTVProperties* locale) = 0;
	virtual vatek_result startBroadcast(qtvSourceBase* source, qtvMuxServiceBase* muxservice) = 0;
	virtual vatek_result pollingRunning(Pbroadcast_info* pinfo) = 0;
	virtual vatek_result stop() = 0;

protected:
	explicit qtvServiceChipBase(qtv_service_mode mode, qtvDevice* device) : qtvServiceBase(mode, device) {}
};

class QTV_DECL qtvServiceBroadcast : public qtvServiceChipBase
{
	Q_OBJECT
public:

	static QList<qtvSourceFactory*>& getSupportedSources();

protected:
	explicit qtvServiceBroadcast(qtvDevice* device) : qtvServiceChipBase(qservice_broadcast,device){}
};

class QTV_DECL qtvServiceTransform : public qtvServiceChipBase
{
	Q_OBJECT
public:

	static QList<qtvSourceFactory*>& getSupportedSources();

	virtual vatek_result startEnumContext(qtvSourceBase* source, int32_t timeout) = 0;
	virtual qtvMuxChannel* getEnumContext() = 0;

protected:
	explicit qtvServiceTransform(qtvDevice* device) : qtvServiceChipBase(qservice_transfrom,device){}
};

#endif
