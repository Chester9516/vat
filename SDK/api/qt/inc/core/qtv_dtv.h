#ifndef QTV_DTV_H
#define QTV_DTV_H

#include <core/qtv_property.h>

class QTV_DECL qtvDTVChannel : public QObject
{
	Q_OBJECT
public:
	virtual const char* _name() = 0;
	virtual uint32_t _frequency() = 0;
};

class QTV_DECL qtvDTVLocale : public QObject
{
	Q_OBJECT
public:
	virtual const char* _name() = 0;
	virtual mux_spec_mode _spec() = 0;
	virtual mux_country_code _country() = 0;
	virtual uint32_t _bandwidth_symbolrate() = 0;
	virtual modulator_type _modulation() = 0;
	virtual video_codec _video() = 0;
	virtual audio_codec _audio() = 0;

	virtual const QList<qtvDTVChannel*> _channels() = 0;
};

class QTV_DECL qtvDTVProperties : public QObject
{
	Q_OBJECT
public:
	virtual Pqtv_locale_param _param() = 0;
	virtual void setChannel(qtvDTVChannel* channel) = 0;
	virtual void setLocale(qtvDTVLocale* locale) = 0;
	virtual qtvPropertyCollection* getPropertyCollection() = 0;
};

class QTV_DECL qtvDTVManage : public QObject
{
	Q_OBJECT
public:
	static const QList<qtvDTVLocale*> getDTVLocals();
	static qtvDTVProperties* createParam(qtvDTVLocale* locale, qtvDTVChannel* channel);
};

#endif
