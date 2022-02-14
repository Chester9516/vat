#include <ui/base/qtv_ui_tab.h>
#include "ui_qtvUITabStatus.h"

#define UITAB_STATUS_NAME	"Status"
#define UITAB_STATUS_DESC	"show service status"

qtvUITabStatus::qtvUITabStatus(qtvUIMain* main, QWidget* parent) :
	qtvUITabBase(main, UITAB_STATUS_NAME, UITAB_STATUS_DESC),
	ui(new Ui::qtvUITabStatus)
{
	qtvUIMainTabBase* tabmain = qobject_cast<qtvUIMainTabBase*>(m_main);
	ui->setupUi(this);
	connect(ui->cb_analysis, SIGNAL(currentIndexChanged(int)), this, SLOT(recvAnalysusChanged(int)));
	if(tabmain)
		connect(tabmain, SIGNAL(notifyServiceChanged(qtvServiceBase*)), this, SLOT(recvServiceChanged(qtvServiceBase*)));
}

qtvUITabStatus::~qtvUITabStatus()
{
#if 0
	qtvUIMainTabBase* tabmain = qobject_cast<qtvUIMainTabBase*>(m_main);
	if(tabmain)
		disconnect(tabmain, SIGNAL(notifyServiceChanged(qtvServiceBase*)), this, SLOT(recvServiceChanged(qtvServiceBase*)));
#endif
	disconnect(ui->cb_analysis, SIGNAL(currentIndexChanged(int)), this, SLOT(recvAnalysusChanged(int)));
	delete ui;
}

void qtvUITabStatus::tabEnter()
{
	refreshAnalysis();
}

void qtvUITabStatus::tabFinish()
{

}

void qtvUITabStatus::tabLeave()
{
	clearAnalysis();
}

void qtvUITabStatus::clearAnalysis()
{
	int32_t i = 0;
	qtvUIMainTabBase* tabmain = qobject_cast<qtvUIMainTabBase*>(m_main);

	ui->chart_ui->detachProvider();
	for (i = 0; i < ui->cb_analysis->count(); i++)
	{
		qtvChartUIProvider* provider = qvariant_cast<qtvChartUIProvider*>(ui->cb_analysis->itemData(i));
		delete provider;
	}
	ui->cb_analysis->clear();
}

void qtvUITabStatus::refreshAnalysis()
{
	clearAnalysis();
	qtvChartUIProviderService* newprovider = qtvChartUIProviderService::createServiceProvider(NULL);
	if (newprovider)
		ui->cb_analysis->addItem(newprovider->_name(), QVariant::fromValue<qtvChartUIProvider*>(newprovider));

	if (m_main)
	{
		if(m_main->_service())
			newprovider->connectChart(m_main->_service());
	}
}

void qtvUITabStatus::recvAnalysusChanged(int index)
{
	if (index != -1)
	{
		qtvChartUIProvider* provider = qvariant_cast<qtvChartUIProvider*>(ui->cb_analysis->itemData(index));
		if (provider)ui->chart_ui->attachProvider(provider);
		else ui->chart_ui->detachProvider();
	}
}

void qtvUITabStatus::recvServiceChanged(qtvServiceBase* service)
{
	int32_t i = 0;
	for (i = 0; i < ui->cb_analysis->count(); i++)
	{
		qtvChartUIProvider* provider = qvariant_cast<qtvChartUIProvider*>(ui->cb_analysis->itemData(i));
		qtvChartUIProviderService* pservice = qobject_cast<qtvChartUIProviderService*>(provider);
		if (pservice)
		{
			if (service)pservice->connectChart(service);
			else pservice->disconnectChart();
		}
	}
}