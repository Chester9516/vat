#ifndef QSOURCE_BASE_H
#define QSOURCE_BASE_H

#include <core/qtv_source.h>
#include "../qinternal_properties.h"

#define QSOURCE_TSIN_NAME		"tsin general"
#define QSOURCE_ENCODER_NAME	"encoder general"

class qtsin_source_factory : public qtvSourceFactory
{
public:

	explicit qtsin_source_factory();
	~qtsin_source_factory();

	QString& _name() override { return m_name; }
	qtv_source_mode _mode() { return qsource_tsin; }

	bool checkSupported(qtvServiceBase* sevice) override;
	vatek_result createProperties(qtvServiceBase* service, qtvPropertyCollection** properties) override;
	vatek_result checkProperties(qtvPropertyCollection* properties) override;
	vatek_result createSource(qtvServiceBase* service, qtvPropertyCollection* properties, qtvSourceBase** source) override;

protected:
	QString m_name;
};

class qtsin_source : public qtvSourceTransform
{
public:
	explicit qtsin_source(qtvServiceBase* service, qi_source_properties* properties, qtvSourceFactory* factory);
	~qtsin_source();

	/* qtvSourceBase */
	qtv_source_mode _source_mode() override { return qsource_tsin; }
	qtv_status _status() override { return m_status; }
	qtvPropertyCollection* _properites() override { return m_properties; }
	Pqtv_source_param _param() override { return m_properties->_param(); }
	const qtvSourceFactory* _factory() override { return m_factory; }

	vatek_result open() override;
	vatek_result start() override;
	vatek_result polling() override;
	void stop() override;
	void close() override;

	/* qtvSourceTransform */
	stream_mode _stream_mode() override { return _param()->stream.tsin.streammode; }
	bool _enable_enum_context() override { return (_param()->filter == qfilter_enum); }

protected:
	qtv_status m_status;
	qtvServiceBase* m_service;
	qi_source_properties* m_properties;
	qtvSourceFactory* m_factory;
};

class qencoder_source_factory : public qtvSourceFactory
{
public:

	qencoder_source_factory();
	~qencoder_source_factory();

	QString& _name() override { return m_name; }
	qtv_source_mode _mode() { return qsource_encoder; }

	bool checkSupported(qtvServiceBase* sevice) override;
	vatek_result createProperties(qtvServiceBase* service, qtvPropertyCollection** properties) override;
	vatek_result checkProperties(qtvPropertyCollection* properties) override;
	vatek_result createSource(qtvServiceBase* service, qtvPropertyCollection* properties, qtvSourceBase** source) override;

protected:
	QString m_name;
};

class qencoder_source : public qtvSourceBroadcast
{
public:

	explicit qencoder_source(qtvServiceBase* service, qi_source_properties* properties, qtvSourceFactory* factory);
	~qencoder_source();

	/* qtvSourceBase */
	qtv_source_mode _source_mode() override { return qsource_encoder; }
	qtv_status _status() override { return m_status; }
	qtvPropertyCollection* _properites() override { return m_properties; }
	Pqtv_source_param _param() override { return m_properties->_param(); }
	const qtvSourceFactory* _factory() override { return m_factory; }

	vatek_result open() override;
	vatek_result start() override;
	vatek_result polling() override;
	void stop() override;
	void close() override;

protected:
	qtv_status m_status;
	qtvServiceBase* m_service;
	qi_source_properties* m_properties;
	qtvSourceFactory* m_factory;
};

#endif
