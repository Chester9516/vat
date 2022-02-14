#include <ui/base/qtv_ui_tab.h>
#include <ui/base/qtv_ui_table.h>
#include <ui/qtv_ui_service_base.h>
#include "ui_qtvUITabSource.h"

#define UITAB_SOURCE_NAME	"Source"
#define UITAB_SOURCE_DESC	"select service source"

qtvUITabSource::qtvUITabSource(qtvUIMain* main, QWidget* parent) :
	qtvUITabBase(main, UITAB_SOURCE_NAME, UITAB_SOURCE_DESC, parent),
	m_properties(NULL),
	ui(new Ui::qtvUITabSource)
{
	ui->setupUi(this);
	ui->lb_source->setText("sources : ");
	connect(ui->cb_source, SIGNAL(currentIndexChanged(int)), this, SLOT(recvSourceSelect(int)));
}

qtvUITabSource::~qtvUITabSource()
{
	disconnect(ui->cb_source, SIGNAL(currentIndexChanged(int)), this, SLOT(recvSourceSelect(int)));
	resetProperties();
	delete ui;
}

void qtvUITabSource::tabEnter()
{
	qtvUIMainWizardBroadcast* bcmain = qobject_cast<qtvUIMainWizardBroadcast*>(m_main);
	if (bcmain)bcmain->detachSource();
	refreshSource();
}

void qtvUITabSource::tabFinish()
{
	qtvUIMainWizardBroadcast* bcmain = qobject_cast<qtvUIMainWizardBroadcast*>(m_main);
	if (bcmain)
	{
		qtvSourceFactory* factory = qvariant_cast<qtvSourceFactory*>(ui->cb_source->itemData(ui->cb_source->currentIndex()));
		if (factory)
		{
			vatek_result nres = factory->checkProperties(m_properties);
			if (is_vatek_success(nres))
			{
				qtvSourceBase* source = NULL;
				nres = factory->createSource(m_main->_service(), finishProperties(), &source);
				if (is_vatek_success(nres))
				{
					nres = source->open();
					if (is_vatek_success(nres))bcmain->attachSource(source);
					else
					{
						VWAR("start source fail : [%s:%d]", factory->_name(), nres);
						delete source;
					}
					return;
				}
			}
			if (!is_vatek_success(nres))VWAR("create source fail : %d", nres);
			return;
		}
	}
	VWAR("unsupport main handle : %d",m_main->_mode());
}

void qtvUITabSource::tabLeave()
{
	resetProperties();
}

qtvPropertyCollection* qtvUITabSource::finishProperties()
{
	qtvPropertyCollection* rproperties = m_properties;
	disconnect(m_properties, SIGNAL(notifyValidChanged(bool)), this, SLOT(recvValidChanged(bool)));
	ui->table_source->setProperties(qui_table_normal, NULL);
	m_properties = NULL;	/* taked properties */
	return rproperties;
}

void qtvUITabSource::refreshSource()
{
	ui->cb_source->clear();
	if (m_main->_service())
	{
		QList<qtvSourceFactory*> m_sources = qtvServiceBroadcast::getSupportedSources();
		if (m_main->_service()->_mode() == qservice_transfrom)
			m_sources = qtvServiceTransform::getSupportedSources();

		for (int32_t i = 0; i < m_sources.count(); i++)
			ui->cb_source->insertItem(ui->cb_source->count(), m_sources[i]->_name(), QVariant::fromValue<qtvSourceFactory*>(m_sources[i]));
	}
}

void qtvUITabSource::resetProperties()
{
	if (m_properties)delete finishProperties();
	setValid(false);
}

void qtvUITabSource::recvSourceSelect(int index)
{
	if (index != -1)
	{
		vatek_result nres = vatek_badparam;
		qtvSourceFactory* factory = qvariant_cast<qtvSourceFactory*>(ui->cb_source->itemData(index));
		if (factory)
		{
			resetProperties();
			nres = factory->createProperties(m_main->_service(), &m_properties);
			if (is_vatek_success(nres))
			{
				ui->table_source->setProperties(qui_table_normal, m_properties);
				setValid(m_properties->_valid());
				connect(m_properties, SIGNAL(notifyValidChanged(bool)), this, SLOT(recvValidChanged(bool)));
			}
		}
		if (!is_vatek_success(nres))VWAR("qtvUITabSource create source fail : [%d:%d]", index, nres);
	}
}

void qtvUITabSource::recvValidChanged(bool bvalid)
{
	setValid(bvalid);
}
