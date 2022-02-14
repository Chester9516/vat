#ifndef QINTERNAL_TRANSFORM_H
#define QINTERNAL_TRANSFORM_H

#include <core/qtv_service.h>
#include "qinternal_properties.h"

#include <vatek_sdk_transform.h>
#include <vatek_sdk_usbstream.h>
#include <vatek_sdk_usbmux.h>

#include <cross/cross_os_api.h>

typedef struct _qi_transform_param
{
	transform_mode mode;
	uint32_t frequency;
	union _param
	{
		transform_enum trenum;
		transform_capture trcapture;
		transform_broadcast trbc;
	}param;
}qi_transform_param,*Pqi_transform_param;

class qi_service_transform : public qtvServiceTransform
{
public:
	explicit qi_service_transform(qtvDevice* device, hvatek_transform htr);
	~qi_service_transform();

	/* qtvServiceBase */
	qtvPropertyCollection* _output_properties() override { return &m_output; }
	Pqtv_output_param _output_param() override { return m_output._param(); }
	Pbroadcast_info _broadcast_detail() override;

	/* qtvServiceTransform */
	vatek_result startEnumContext(qtvSourceBase* source, int32_t timeout) override;
	qtvMuxChannel* getEnumContext() override;
	/* qtvServiceChipBase */
	vatek_result setLocalDefault(qtvDTVProperties* locale) override;
	vatek_result startBroadcast(qtvSourceBase* source, qtvMuxServiceBase* muxservice) override;
	vatek_result pollingRunning(Pbroadcast_info* pinfo) override;
	vatek_result stop() override;

protected:
	qi_output_properties m_output;
	hvatek_transform m_htr;
	qi_transform_param m_param;
	qtvMuxChannel* m_enumcontext;

	void resetEnumContext();
	void resetParam();
	vatek_result configFilter(Ptransform_broadcast pbc, qtvMuxServiceBase* muxhandle);
	vatek_result setParamSource(qtvSourceBase* source);
};

class qi_transform_sources
{
public:
	explicit qi_transform_sources();
	~qi_transform_sources();

	QList<qtvSourceFactory*>& _sources() { return m_sources; }
	static qi_transform_sources& getInstance();

protected:
	QList<qtvSourceFactory*> m_sources;
};

#endif
