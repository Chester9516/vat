#ifndef QTV_UI_CONFIG_H
#define QTV_UI_CONFIG_H

#include <core/qtv_media.h>
#include <core/qtv_service_usb.h>

typedef enum _qtv_ui_config_mode
{
	qui_config_muxparam,
	qui_config_videocodec,
	qui_config_audiocodec,
	qui_config_muxchannel,
	qui_config_output,
}qtv_ui_config_mode;

class QTV_DECL qtvUIConfigBase : public QObject
{
	Q_OBJECT
public:
	qtvUIConfigBase(qtv_ui_config_mode mode) : m_mode(mode) {}
	virtual qtv_ui_config_mode _mode() { return m_mode; }

protected:
	qtv_ui_config_mode m_mode;
};

class QTV_DECL qtvUIConfigMuxChannel : public qtvUIConfigBase
{
	Q_OBJECT
public:
	explicit qtvUIConfigMuxChannel(qtvServiceBase* service);
	virtual qtvMuxChannel* getChannel();

protected:
	qtvServiceBase* m_service;
};

class QTV_DECL qtvUIConfigCodec : public qtvUIConfigBase
{
	Q_OBJECT
public:
	explicit qtvUIConfigCodec(qtv_ui_config_mode mode,QList<qtvCodecFactory*>* codecs);
	~qtvUIConfigCodec();

	virtual qtvCodecProperties* _properties() { return m_properties; }

protected:
	QList<qtvCodecFactory*>* m_codecs;
	qtvCodecProperties* m_properties;
};

class QTV_DECL qtvUIConfigProperties : public qtvUIConfigBase
{
	Q_OBJECT
public:
	explicit qtvUIConfigProperties(qtv_ui_config_mode mode, qtvPropertyCollection* properties);
	virtual qtvPropertyCollection* _properties() { return m_properties; }

protected:
	qtvPropertyCollection* m_properties;
};
#endif
