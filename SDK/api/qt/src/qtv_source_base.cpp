#include "./internal/source/qsource_base.h"
#include <core/qtv_service.h>

/* qtsin_source_factory */

qtsin_source_factory::qtsin_source_factory()
{
	m_name = "MPEG-TS Interface";
}

qtsin_source_factory::~qtsin_source_factory()
{

}

bool qtsin_source_factory::checkSupported(qtvServiceBase* sevice)
{
	return true;
}

vatek_result qtsin_source_factory::createProperties(qtvServiceBase* service, qtvPropertyCollection** properties)
{
	vatek_result nres = vatek_unsupport;
	if (service->_mode() == qservice_transfrom)
	{
		*properties = new qi_source_properties(service->_device(), stream_source_tsin, qproperties_source_filter);
		nres = vatek_success;
	}
	return nres;
}

vatek_result qtsin_source_factory::checkProperties(qtvPropertyCollection* properties)
{
	return vatek_success;
}

vatek_result qtsin_source_factory::createSource(qtvServiceBase* service, qtvPropertyCollection* properties, qtvSourceBase** source)
{
	vatek_result nres = checkProperties(properties);
	if (is_vatek_success(nres))
	{
		qi_source_properties* qsource = static_cast<qi_source_properties*>(properties);
		if (qsource)*source = new qtsin_source(service, qsource, this);
		else nres = vatek_badparam;
	}
	return nres;
}

/* qtsin_source */

qtsin_source::qtsin_source(qtvServiceBase* service, qi_source_properties* properties, qtvSourceFactory* factory) :
	m_status(qstatus_idle),
	m_service(service),
	m_properties(properties),
	m_factory(factory)
{

}

qtsin_source::~qtsin_source()
{
	delete m_properties;
}

vatek_result qtsin_source::open()
{
	vatek_result nres = vatek_success;
	if (m_status == qstatus_idle)m_status = qstatus_lock;
	else nres = vatek_badstatus;
	return nres;
}

vatek_result qtsin_source::start()
{
	vatek_result nres = vatek_success;
	if (m_status == qstatus_lock)m_status = qstatus_running;
	else nres = vatek_badstatus;
	return nres;
}

vatek_result qtsin_source::polling()
{
	if(m_status == qstatus_running)return vatek_success;
	return vatek_badstatus;
}

void qtsin_source::stop()
{
	if (m_status == qstatus_running)
		m_status = qstatus_lock;
}

void qtsin_source::close()
{
	m_status = qstatus_idle;
}
