#ifndef QINTERNAL_BROADCAST_H
#define QINTERNAL_BROADCAST_H

#include <core/qtv_service.h>
#include <vatek_sdk_broadcast.h>
#include "qinternal_properties.h"

class qi_service_broadcast : public qtvServiceBroadcast
{
public:
	explicit qi_service_broadcast(qtvDevice* device,hvatek_broadcast hbc);
	~qi_service_broadcast();

	qtvPropertyCollection* _output_properties() override { return &m_output; }
	Pqtv_output_param _output_param() override { return m_output._param(); }
	Pbroadcast_info _broadcast_detail() override { return vatek_broadcast_get_info(m_hbc); }

	/* qtvServiceBroadcast */
	vatek_result setLocalDefault(qtvDTVProperties* locale) override;
	vatek_result startBroadcast(qtvSourceBase* source, qtvMuxServiceBase* muxservice) override;
	vatek_result pollingRunning(Pbroadcast_info* pinfo) override;
	vatek_result stop() override;

protected:
	qi_output_properties m_output;
	hvatek_broadcast m_hbc;
	broadcast_param m_param;

	vatek_result setParam(qtvSourceBroadcast* psource);
};

class qi_broadcast_sources
{
public:
	explicit qi_broadcast_sources();
	~qi_broadcast_sources();

	QList<qtvSourceFactory*>& _sources() { return m_sources; }
	static qi_broadcast_sources& getInstance();

protected:
	QList<qtvSourceFactory*> m_sources;
};

#endif
