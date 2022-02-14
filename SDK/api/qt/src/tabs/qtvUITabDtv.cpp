#include <ui/base/qtv_ui_tab.h>
#include <ui/base/qtv_ui_table.h>
#include <ui/qtv_ui_service_base.h>
#include <ui/qtv_ui_service_usb.h>

#include "ui_qtvUITabDtv.h"
#include "../internal/qinternal_ui.h"

#include <QLabel>
#include <QComboBox>

#define UITAB_DTV_NAME		"DTV Config"
#define UITAB_DTV_DESC		"select DTV default properties"

qtvUITabDtv::qtvUITabDtv(qtvUIMain* main, QWidget* parent) :
	m_properties(NULL),
	qtvUITabBase(main, UITAB_DTV_NAME, UITAB_DTV_DESC, parent),
	ui(new Ui::qtvUITabDtv)
{
	ui->setupUi(this);
	ui->lb_country->setText("Default");
	ui->lb_channel->setText("Channel");
	connect(ui->cb_country, SIGNAL(currentIndexChanged(int)), this, SLOT(recvCountrySelect(int)));
	connect(ui->ch_channel, SIGNAL(currentIndexChanged(int)), this, SLOT(recvChannelSelect(int)));
}

qtvUITabDtv::~qtvUITabDtv()
{
	disconnect(ui->cb_country, SIGNAL(currentIndexChanged(int)), this, SLOT(recvCountrySelect(int)));
	disconnect(ui->ch_channel, SIGNAL(currentIndexChanged(int)), this, SLOT(recvChannelSelect(int)));
	resetPropertes();
	delete ui;
}

void qtvUITabDtv::tabEnter()
{
	refreshCountry();
}

void qtvUITabDtv::tabFinish()
{
	if (m_properties)
	{
		if (m_main->_mode() == qui_main_bcwizard)
		{
			qtvUIMainWizardBroadcast* bcmain = qobject_cast<qtvUIMainWizardBroadcast*>(m_main);
			if (bcmain)
			{
				bcmain->setDefaultLocale(m_properties);
				m_properties = NULL; /* take locale properties */
			}
		}
		else if (m_main->_mode() == qui_main_usbmuxer_wizard)
		{
			qtvUIMainUSBMuxer* usbmux = qobject_cast<qtvUIMainUSBMuxer*>(m_main);
			if (usbmux)
			{
				usbmux->setDefaultLocal(m_properties);
				m_properties = NULL;
			}
		}
	}
}

void qtvUITabDtv::tabLeave()
{
	resetPropertes();
}

void qtvUITabDtv::resetPropertes()
{
	ui->table_detail->setProperties(qui_table_normal, NULL);
	if (m_properties)delete m_properties;
	m_properties = NULL;
}

void qtvUITabDtv::refreshCountry()
{
	const QList<qtvDTVLocale*> locales = qtvDTVManage::getDTVLocals();
	int32_t i = 0;

	resetPropertes();
	ui->cb_country->clear();
	ui->ch_channel->clear();

	for (i = 0; i < locales.count(); i++)
	{
		qtvDTVLocale* plocale = locales.at(i);
		ui->cb_country->addItem(plocale->_name(), QVariant::fromValue<qtvDTVLocale*>(plocale));
	}
	ui->cb_country->setCurrentIndex(0);
}

void qtvUITabDtv::refreshChannel(qtvDTVLocale* plocale)
{
	int32_t i = 0;
	const QList<qtvDTVChannel*> channels = plocale->_channels();
	for (i = 0; i < channels.count(); i++)
	{
		qtvDTVChannel* prf = channels.at(i);
		QString sztitle;
		sztitle.sprintf("[%s] : %d", prf->_name(), prf->_frequency());
		ui->ch_channel->addItem(sztitle, QVariant::fromValue<qtvDTVChannel*>(prf));
	}
	ui->ch_channel->setCurrentIndex(0);
}

void qtvUITabDtv::recvCountrySelect(int index)
{
	qtvDTVLocale* plocale = qvariant_cast<qtvDTVLocale*>(ui->cb_country->itemData(index));
	resetPropertes();
	ui->ch_channel->clear();
	if (plocale)refreshChannel(plocale);
}

void qtvUITabDtv::recvChannelSelect(int index)
{
	qtvDTVChannel* pchannel = qvariant_cast<qtvDTVChannel*>(ui->ch_channel->itemData(index));
	if (pchannel)
	{
		if (m_properties)m_properties->setChannel(pchannel);
		else
		{
			index = ui->cb_country->currentIndex();
			qtvDTVLocale* plocale = qvariant_cast<qtvDTVLocale*>(ui->cb_country->itemData(index));
			m_properties = qtvDTVManage::createParam(plocale, pchannel);
		}
		ui->table_detail->setProperties(qui_table_normal, m_properties->getPropertyCollection());
	}
}
