#ifndef QSOURCE_USBSTREAM_H
#define QSOURCE_USBSTREAM_H

#include <core/qtv_source.h>
#include "../qinternal_properties.h"
#include <cross/cross_stream.h>

#define QSOURCE_USBSTREAM_NAME	"USB-Stream [TS FILE | UDP-TS]"

typedef struct _qusbstream_param
{
	qtv_usbstream_param usbstream;
	qtv_source_param source;
}qusbstream_param,*Pqusbstream_param;

class qusbstream_properties : public qi_properties_base
{
public:
	explicit qusbstream_properties();
	~qusbstream_properties();

	void resetProperties() override;
	Pqusbstream_param _param() { return &m_param; }

	static vatek_result checkUSBStreamValid(qusbstream_properties* props);
protected:
	qusbstream_param m_param;

	void insertProperty(const Pui_prop_item uiprop, uint8_t* raw) override;
	void refreshProperties() override;
	bool checkPropertiesValid() override;
};

class qusbstream_source_factory : public qtvSourceFactory
{
public:
	explicit qusbstream_source_factory();
	~qusbstream_source_factory();

	QString& _name() override { return m_name; }
	qtv_source_mode _mode() override { return qsource_usbstream; }

	bool checkSupported(qtvServiceBase* sevice) override;
	vatek_result createProperties(qtvServiceBase* service, qtvPropertyCollection** properties) override;
	vatek_result checkProperties(qtvPropertyCollection* properties) override;
	vatek_result createSource(qtvServiceBase* service, qtvPropertyCollection* properties, qtvSourceBase** source) override;

protected:
	QString m_name;
};

class qusbstream_source : public qtvSourceUSBStream
{
public:
	explicit qusbstream_source(qtvServiceBase* service, qtvSourceFactory* factory, qusbstream_properties* properties);
	~qusbstream_source();

	/* qtvSourceBase */
	qtv_status _status() override { return m_status; }
	qtvPropertyCollection* _properites() override { return m_properties; }
	Pqtv_source_param _param() override { return &m_properties->_param()->source; }
	const qtvSourceFactory* _factory() { return m_factory; }

	/* qtvSourceBase */
	vatek_result open() override;
	vatek_result start() override;
	vatek_result polling() override;
	void stop() override;
	void close() override;

	/* qtvSourceTransform */
	stream_mode _stream_mode() override { return stream_remux; }
	bool _enable_enum_context()override { return (_usbstream_param()->filter == qfilter_enum); }
	vatek_result getStreamPackets(uint8_t** slice) override;

	/* qtvSourceTransform */
	Pqtv_usbstream_param _usbstream_param() override { return &m_properties->_param()->usbstream; }

protected:
	qtv_status m_status;
	qtvSourceFactory* m_factory;
	qtvServiceBase* m_service;
	qusbstream_properties* m_properties;
	hcross_stream m_hstream;
};


#endif
