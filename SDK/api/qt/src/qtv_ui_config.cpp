#include <ui/base/qtv_ui_config.h>
#include "./internal/qinternal_properties.h"

/* qtvUIConfigMuxChannel */

qtvUIConfigMuxChannel::qtvUIConfigMuxChannel(qtvServiceBase* service) :
	m_service(service),
	qtvUIConfigBase(qui_config_muxchannel)
{

}

qtvMuxChannel* qtvUIConfigMuxChannel::getChannel()
{
	if (m_service->_mode() == qservice_usbmuxer)
	{
		qtvServiceUSBMuxer* usbmuxer = qobject_cast<qtvServiceUSBMuxer*>(m_service);
		if (usbmuxer)
		{
			qtvUSBMuxerChannel* channel = NULL;
			if (is_vatek_success(usbmuxer->getChannel(&channel)))
				return channel;
		}
	}
	return NULL;
}

/* qtvUIConfigCodec */

qtvUIConfigCodec::qtvUIConfigCodec(qtv_ui_config_mode mode,QList<qtvCodecFactory*>* codecs) :
	qtvUIConfigBase(mode),
	m_codecs(codecs),
	m_properties(NULL)
{

}

qtvUIConfigCodec::~qtvUIConfigCodec()
{
	if (m_properties)
		delete m_properties;
}

/* qtvUIConfigProperties */

qtvUIConfigProperties::qtvUIConfigProperties(qtv_ui_config_mode mode, qtvPropertyCollection* properties) :
	m_properties(properties),
	qtvUIConfigBase(mode)
{

}
