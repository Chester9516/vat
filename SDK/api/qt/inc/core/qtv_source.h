#ifndef QTV_SOURCE_H
#define QTV_SOURCE_H

#include <core/qtv_property.h>
#include <core/qtv_mux.h>

class qtvSourceFactory;
class qtvServiceBase;

class QTV_DECL qtvSourceBase : public QObject
{
	Q_OBJECT
public:

	virtual qtv_source_mode _source_mode() = 0;
	virtual qtv_status _status() = 0;
	virtual qtvPropertyCollection* _properites() = 0;
	virtual Pqtv_source_param _param() = 0;
	virtual const qtvSourceFactory* _factory() = 0;

	virtual vatek_result open() = 0;
	virtual vatek_result start() = 0;
	virtual vatek_result polling() = 0;
	virtual void stop() = 0;
	virtual void close() = 0;
};

class QTV_DECL qtvSourceFactory : public QObject
{
	Q_OBJECT
public:
	virtual QString& _name() = 0;
	virtual qtv_source_mode _mode() = 0;

	virtual bool checkSupported(qtvServiceBase* sevice) = 0;
	virtual vatek_result createProperties(qtvServiceBase* service,qtvPropertyCollection** properties) = 0;
	virtual vatek_result checkProperties(qtvPropertyCollection* properties) = 0;
	virtual vatek_result createSource(qtvServiceBase* service, qtvPropertyCollection* properties, qtvSourceBase** source) = 0;
};

class QTV_DECL qtvSourceBroadcast : public qtvSourceBase
{
	Q_OBJECT
public:

};

class QTV_DECL qtvSourceTransform : public qtvSourceBase
{
	Q_OBJECT
public:
	virtual stream_mode _stream_mode() = 0;
	virtual bool _enable_enum_context() = 0;
};

class QTV_DECL qtvSourceUSBStream : public qtvSourceTransform
{
	Q_OBJECT
public:
	qtv_source_mode _source_mode() override { return qsource_usbstream; }
	virtual Pqtv_usbstream_param _usbstream_param() = 0;
	virtual vatek_result getStreamPackets(uint8_t** slice) = 0;
};

class QTV_DECL qtvSourceBridgeAV : public qtvSourceBroadcast
{
	Q_OBJECT
public:
	qtv_source_mode _source_mode() override { return qsource_bridge_av; }
};

class QTV_DECL qtvSourceBridgeDemod : public qtvSourceTransform
{
	Q_OBJECT
public:
	qtv_source_mode _source_mode() override { return qsource_bridge_demod; }
};

#endif
