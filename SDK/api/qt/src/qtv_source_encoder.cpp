#include "./internal/source/qsource_base.h"
#include <core/qtv_service.h>


/* qencoder_source_factory */

qencoder_source_factory::qencoder_source_factory()
{
	m_name = "Raw-Encoder";
}

qencoder_source_factory::~qencoder_source_factory()
{

}

bool qencoder_source_factory::checkSupported(qtvServiceBase* sevice)
{
	return true;
}

vatek_result qencoder_source_factory::createProperties(qtvServiceBase* service, qtvPropertyCollection** properties)
{
	vatek_result nres = vatek_unsupport;
	if (service->_mode() == qservice_broadcast)
	{
		*properties = new qi_source_properties(service->_device(), stream_source_encoder, qproperties_source);
		nres = vatek_success;
	}
	return nres;
}

vatek_result qencoder_source_factory::checkProperties(qtvPropertyCollection* properties)
{
	return vatek_success;
}

vatek_result qencoder_source_factory::createSource(qtvServiceBase* service, qtvPropertyCollection* properties, qtvSourceBase** source)
{
	vatek_result nres = checkProperties(properties);
	if (is_vatek_success(nres))
	{
		qi_source_properties* qsource = static_cast<qi_source_properties*>(properties);
		if (qsource)*source = new qencoder_source(service, qsource, this);
		else nres = vatek_badparam;
	}
	return nres;
}

/* qencoder_source */

qencoder_source::qencoder_source(qtvServiceBase* service, qi_source_properties* properties, qtvSourceFactory* factory) :
	m_status(qstatus_idle),
	m_service(service),
	m_properties(properties),
	m_factory(factory)
{

}

qencoder_source::~qencoder_source()
{
	delete m_properties;
}

vatek_result qencoder_source::open()
{
	vatek_result nres = vatek_success;
	if (m_status == qstatus_idle)m_status = qstatus_lock;
	else nres = vatek_badstatus;
	return nres;
}

vatek_result qencoder_source::start()
{
	vatek_result nres = vatek_success;
	if (m_status == qstatus_lock)m_status = qstatus_running;
	else nres = vatek_badstatus;
	return nres;
}

vatek_result qencoder_source::polling()
{
	if (m_status == qstatus_running)return vatek_success;
	return vatek_badstatus;
}

void qencoder_source::stop()
{
	if (m_status == qstatus_running)
		m_status = qstatus_lock;
}

void qencoder_source::close()
{
	m_status = qstatus_idle;
}