#include "./internal/qinternal_media.h"

#define CODEC_PROPERTIES_MEMSIZE	1024

/* qtvCodec */

qtvCodec::qtvCodec(qtvCodecFactory* factory, QString name, qtv_codec_mode mode, qtvPropertyCollection* properties) :
	m_name(name),
	m_mode(mode),
	m_factory(factory),
	m_properties(properties)
{

}

/* qtvCodecVideo */

qtvCodecVideo::qtvCodecVideo(qtvCodecFactory* factory, QString name, qtvPropertyCollection* properties) :
	qtvCodec(factory, name, qcodec_video,properties)
{

}

/* qtvCodecAudio */

qtvCodecAudio::qtvCodecAudio(qtvCodecFactory* factory, QString name, qtvPropertyCollection* properties) :
	qtvCodec(factory, name, qcodec_audio, properties)
{

}

/* qtvCodecSimpleProvider */

qtvCodecSimpleProvider::qtvCodecSimpleProvider(qtv_codec_mode mode) :
	m_mode(mode)
{

}

qtvCodecSimpleProvider::~qtvCodecSimpleProvider()
{
	while (m_codecs.count())
	{
		delete m_codecs.first();
		m_codecs.removeFirst();
	}
}

/* qi_codec_properties */

qi_codec_properties::qi_codec_properties(qtvCodecProvider* provider) :
	m_codec(NULL),
	m_provider(provider)
{

}

void qi_codec_properties::resetProperties()
{

}

void qi_codec_properties::refreshProperties()
{

}

void qi_codec_properties::insertProperty(const Pui_prop_item uiprop, uint8_t* raw)
{
	qtvPropertyCollection::insertProperty(uiprop, raw);
}

void qi_codec_properties::raisePropertiesChanged()
{
	resetProperties();
	refreshProperties();
	emit notifyPropertiesChanged();
}

void qi_codec_properties::raisePropertyEdited(qtvProperty* prop)
{
	setValid(checkPropertiesValid());
}

qtvCodecProperties* qtvCodecProperties::createCodecProperties(qtvCodecProvider* provider)
{
	return new qi_codec_properties(provider);
}