#include <ui/qtv_ui_service_usb.h>

/* qtvUIMainUSBMuxerConfig */

qtvUIMainUSBMuxer::qtvUIMainUSBMuxer(qtv_uimain_mode mainmode, qtv_ui_usbmuxer_mode uimode, qtvServiceUSBMuxer* service) :
	m_usbmuxermode(uimode),
	m_locale(NULL),
	m_videocodec(NULL),
	m_audiocodec(NULL),
	qtvUIMain(mainmode,service)
{
	refreshConfig();
}

qtvUIMainUSBMuxer::~qtvUIMainUSBMuxer()
{
	clearConfig();
	attachUSBMuxer(NULL);
	attachVideoCodec(NULL);
	attachAudioCodec(NULL);
}

Pqtv_locale_param qtvUIMainUSBMuxer::_local_param()
{
	if (m_locale)
		return m_locale->_param();
	return NULL;
}

void qtvUIMainUSBMuxer::attachUSBMuxer(qtvServiceUSBMuxer* usbmuxer)
{
	if (m_service)
	{
		delete m_service;
		clearConfig();
	}
	m_service = usbmuxer;
	if (m_service)
	{
		refreshConfig();
		emit notifyServiceChanged();
	}
}

void qtvUIMainUSBMuxer::attachVideoCodec(qtvCodecVideo* vcodec)
{
	if (m_videocodec)delete m_videocodec;
	m_videocodec = vcodec;
}

void qtvUIMainUSBMuxer::attachAudioCodec(qtvCodecAudio* acodec)
{
	if (m_audiocodec)delete m_audiocodec;
	m_audiocodec = acodec;
}

void qtvUIMainUSBMuxer::clearConfig()
{
	while (m_configs.count())
	{
		delete m_configs.first();
		m_configs.removeFirst();
	}
}

void qtvUIMainUSBMuxer::refreshConfig()
{
	qtvServiceUSBMuxer* usbmuxer = qobject_cast<qtvServiceUSBMuxer*>(m_service);
	clearConfig();
	if (usbmuxer)
	{
		m_configs.append(new qtvUIConfigProperties(qui_config_muxparam, usbmuxer->_mux_properties()));
		if (m_usbmuxermode == qui_usbmuxer_single)
		{
			m_configs.append(new qtvUIConfigCodec(qui_config_videocodec, NULL));
			m_configs.append(new qtvUIConfigCodec(qui_config_audiocodec, NULL));
			m_configs.append(new qtvUIConfigMuxChannel(m_service));
		}
		m_configs.append(new qtvUIConfigProperties(qui_config_output, usbmuxer->_output_properties()));
	}
}

void qtvUIMainUSBMuxer::setDefaultLocal(qtvDTVProperties* locale)
{
	qtvServiceUSBMuxer* usbmuxer = qobject_cast<qtvServiceUSBMuxer*>(m_service);
	m_locale = locale;
	if (usbmuxer)
	{
		Pqtv_locale_param pdtv = m_locale->_param();
		usbmuxer->resetChannel();
		usbmuxer->resetOutput(pdtv->frequency, pdtv->bw_sb, pdtv->modulation);
	}
}

/* qtvUIDialogConfig */

vatek_result qtvUIDialogUSBMuxerConfig::createDialog(qtvServiceUSBMuxer* service, qtvUIMainUSBMuxer* main, qtvUIDialogUSBMuxerConfig** dialog, QWidget* parent)
{
	vatek_result nres = vatek_unsupport;
	//if (service->_mode() == qservice_usbmuxer)
	{
		*dialog = new qtvUIDialogUSBMuxerConfig(QSize(640, 350), service, main, parent);
		nres = vatek_success;
	}
	return nres;
}

void qtvUIDialogUSBMuxerConfig::freeDialog(qtvUIDialogUSBMuxerConfig* dialog)
{
	delete dialog;
}

qtvUIDialogUSBMuxerConfig::qtvUIDialogUSBMuxerConfig(QSize szie, qtvServiceUSBMuxer* service, qtvUIMainUSBMuxer* main, QWidget* parent) :
	m_main(main),
	QDialog(parent)
{
	setFixedSize(szie);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowTitle("usb muxer config");
	m_tabbase = new qtvUITabConfigs(m_main, this);
	setLayout(m_tabbase->layout());
	refreshConfig();
}

qtvUIDialogUSBMuxerConfig::~qtvUIDialogUSBMuxerConfig()
{
	delete m_tabbase;
}

void qtvUIDialogUSBMuxerConfig::refreshConfig()
{
	int32_t i = 0;
	m_tabbase->resetConfigs();

	for (i = 0; i < m_main->_configs().count(); i++)
	{
		m_tabbase->addConfig(m_main->_configs().at(i));
	}
	m_tabbase->setCurrentSelect(0);
}

/* qtvUIWizardUSBMuxer */

class qUIMainUSBMuxerWizard : public qtvUIMainUSBMuxer
{
public:
	explicit qUIMainUSBMuxerWizard(qtvCodecProvider* video, qtvCodecProvider* audio) :
		m_videocodecs(video),
		m_audiocodecs(audio),
		qtvUIMainUSBMuxer(qui_main_usbmuxer_wizard, qui_usbmuxer_single, NULL)
	{

	}
	~qUIMainUSBMuxerWizard()
	{
		if (m_service)delete m_service;
		m_service = NULL;
	}

	qtvCodecProvider* getVideoProvider() override { return m_videocodecs; }
	qtvCodecProvider* getAudioProvider() override { return m_audiocodecs; }

protected:
	qtvCodecProvider* m_videocodecs;
	qtvCodecProvider* m_audiocodecs;
};

vatek_result qtvUIWizardUSBMuxer::createWizard(qtvCodecProvider* video, qtvCodecProvider* audio, qtvUIWizardUSBMuxer** wizard, QWidget* parent)
{
	qUIMainUSBMuxerWizard* newmain = new qUIMainUSBMuxerWizard(video, audio);
	*wizard = new qtvUIWizardUSBMuxer(newmain, parent);
	return vatek_success;
}

void qtvUIWizardUSBMuxer::freeWizard(qtvUIWizardUSBMuxer* wizard)
{
	delete wizard;
}

qtvUIWizardUSBMuxer::qtvUIWizardUSBMuxer(qtvUIMainUSBMuxer* main, QWidget* parent) :
	m_usbmuxermain(main),
	qtvUIWizardBase(main, parent)
{
	setWindowTitle("usb muxer wizard");

	connect(m_usbmuxermain, SIGNAL(notifyServiceChanged()), this, SLOT(recvServiceChanged()));
	pushPage(BCWIZARD_ID_LOCALE);
	pushPage(BCWIZARD_ID_MUXPARAM);
	pushPage(BCWIZARD_ID_VIDEOCODEC);
	pushPage(BCWIZARD_ID_AUDIOCODEC);
	pushPage(BCWIZARD_ID_MODULATION);
	pushPage(BCWIZARD_ID_CONTEXT);
	//setStartId(BCWIZARD_ID_LOCALE);

	if(m_main->_service())setStartId(BCWIZARD_ID_LOCALE);
	else
	{
		pushPage(BCWIZARD_ID_NODEVICE);
		setStartId(BCWIZARD_ID_NODEVICE);
	}
}

qtvUIWizardUSBMuxer::~qtvUIWizardUSBMuxer()
{
	if (m_main)
	{
		disconnect(m_usbmuxermain, SIGNAL(notifyServiceChanged()), this, SLOT(recvServiceChanged()));
		if (m_main)delete m_main;
	}

}

vatek_result qtvUIWizardUSBMuxer::startWizard()
{
	exec();
	return m_result;
}

int qtvUIWizardUSBMuxer::nextId() const 
{
	if (currentId() == BCWIZARD_ID_NODEVICE)
		return BCWIZARD_ID_LOCALE;
	else if (currentId() == BCWIZARD_ID_LOCALE)
		return BCWIZARD_ID_MUXPARAM;
	else if (currentId() == BCWIZARD_ID_MUXPARAM)
		return BCWIZARD_ID_VIDEOCODEC;
	else if (currentId() == BCWIZARD_ID_VIDEOCODEC)
		return BCWIZARD_ID_AUDIOCODEC;
	else if(currentId() == BCWIZARD_ID_AUDIOCODEC)
		return BCWIZARD_ID_MODULATION;
	else if (currentId() == BCWIZARD_ID_MODULATION)
		return BCWIZARD_ID_CONTEXT;
	return -1;
}

void qtvUIWizardUSBMuxer::recvServiceChanged()
{
	if (m_main->_service())
	{
		if (currentId() == BCWIZARD_ID_NODEVICE)
		{
			next();
			removePage(BCWIZARD_ID_NODEVICE);
		}
	}
}

void qtvUIWizardUSBMuxer::getResult(qtvUIMainUSBMuxer** main)
{
	if (m_main)
	{
		disconnect(m_usbmuxermain, SIGNAL(notifyServiceChanged()), this, SLOT(recvServiceChanged()));
		if (is_vatek_success(m_result))
			*main = qobject_cast<qtvUIMainUSBMuxer*>(m_main);
		m_usbmuxermain = NULL;
		m_main = NULL;
	}
}
