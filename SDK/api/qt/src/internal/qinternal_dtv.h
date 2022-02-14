#ifndef QINTERNAL_DTV_H
#define QINTERNAL_DTV_H

#include <core/qtv_dtv.h>

#include "qinternal_core.h"
#include "./dtv/dtv_channel.h"
#include "./dtv/dtv_locale.h"

class qi_dtvchannel : public qtvDTVChannel
{
public:
	explicit qi_dtvchannel(const Pqdtv_ch ch) : m_channel(ch){}

	const char* _name() override { return m_channel->name; }
	uint32_t _frequency() override { return m_channel->freqency; }

protected:
	const Pqdtv_ch m_channel;
};

class qi_dtvchannel_table
{
public:
	explicit qi_dtvchannel_table(const Pqdtv_ch_table table);
	~qi_dtvchannel_table();

	const Pqdtv_ch_table _table() { return m_table; }
	QList<qtvDTVChannel*> _channels() { return m_channels; }

protected:
	const Pqdtv_ch_table m_table;
	QList<qtvDTVChannel*> m_channels;
};

class qi_dtvchannel_table_manage
{
public:
	static qi_dtvchannel_table_manage& getInstance();

	qi_dtvchannel_table* getChannels(Pqdtv_ch_table table);
protected:
	qi_dtvchannel_table_manage();
	~qi_dtvchannel_table_manage();
	QList<qi_dtvchannel_table*> m_tables;
};

class qi_dtvlocale : public qtvDTVLocale
{
public:
	explicit qi_dtvlocale(const Pqdtv_country locale);
	~qi_dtvlocale();

	const char* _name() override { return m_locale->name; }
	mux_spec_mode _spec() override { return m_locale->spec; }
	mux_country_code _country() override { return m_locale->country; }
	uint32_t _bandwidth_symbolrate() override { return m_locale->bw_sw; }
	modulator_type _modulation() override { return m_locale->modulation; }
	video_codec _video() override { return m_locale->video; }
	audio_codec _audio() override { return m_locale->audio; }

	const QList<qtvDTVChannel*> _channels() override { return m_channels->_channels(); }

protected:
	const Pqdtv_country m_locale;
	qi_dtvchannel_table* m_channels;
};

class qi_dtvlocale_param : public qtvDTVProperties
{
public:
	qi_dtvlocale_param(qtvDTVLocale* locale, qtvDTVChannel* channel) : m_locale(locale,channel) {}
	Pqtv_locale_param _param() override { return m_locale._param(); }
	void setChannel(qtvDTVChannel* channel) override { m_locale.setChannel(channel); }
	void setLocale(qtvDTVLocale* locale) override { m_locale.setLocale(locale); }
	qtvPropertyCollection* getPropertyCollection() override { return &m_locale; }

protected:
	class qi_dtvlocale_properties : public qi_properties_base
	{
	public:
		explicit qi_dtvlocale_properties(qtvDTVLocale* locale, qtvDTVChannel* channel);
		Pqtv_locale_param _param() { return &m_param; }

		void resetProperties() override;
		void setLocale(qtvDTVLocale* locale);
		void setChannel(qtvDTVChannel* channel);

	protected:
		qtv_locale_param m_param;
		qtvDTVLocale* m_locale;
		qtvDTVChannel* m_channel;

		void refreshProperties() override;
	};

	qi_dtvlocale_properties m_locale;
};

class qi_dtvlocale_manage
{
public:
	static qi_dtvlocale_manage& getInstance();
	QList<qtvDTVLocale*> getLocale() { return m_locales; }

protected:
	qi_dtvlocale_manage();
	~qi_dtvlocale_manage();
	qi_dtvchannel_table_manage* m_tables;
	QList<qtvDTVLocale*> m_locales;
};

#endif
