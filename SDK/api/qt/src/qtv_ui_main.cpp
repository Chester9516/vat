#include <ui/base/qtv_ui_core.h>

/* qtvUIMain */

qtvUIMain::qtvUIMain(qtv_uimain_mode mode, qtvServiceBase* service) :
	m_mode(mode),
	m_service(service)
{

}

bool qtvUIMain::is_uimain_tabs(qtv_uimain_mode mode)
{
	return ((mode & qui_main_mask) == qui_main_maintabs);
}

bool qtvUIMain::is_uimain_wizard(qtv_uimain_mode mode)
{
	return ((mode & qui_main_mask) == qui_main_bcwizard);
}

bool qtvUIMain::is_uimain_storage(qtv_uimain_mode mode)
{
	return ((mode & qui_main_mask) == qui_main_storage);
}

/* qtvUIMainTabBase */

qtvUIMainTabBase::qtvUIMainTabBase(qtv_uimain_mode mode) :
	qtvUIMain(mode, NULL),
	m_status(qstatus_idle),
	m_chartdata(NULL)
{

}

qtvUIMainTabBase::~qtvUIMainTabBase()
{
	detachService();
}

void qtvUIMainTabBase::setService(qtvServiceBase* service)
{
	if (!service)detachService();
	else attachService(service);
}

#if 0

void qtvUIMainTabBase::setChartData(qtvChartDataCapture* chartdata)
{
	if (m_chartdata)
		emit notifyChartDataChanged(NULL);
	m_chartdata = NULL;

	if (chartdata)
	{
		m_chartdata = chartdata;
		emit(notifyChartDataChanged(m_chartdata));
	}
}

#endif

void qtvUIMainTabBase::setStatus(qtv_status status)
{
	m_status = status;
	emit notifyStatusChanged(m_status);
}

void qtvUIMainTabBase::attachService(qtvServiceBase* service)
{
	if (m_service)detachService();
	if (service)
	{
		m_service = service;
		connect(m_service, SIGNAL(notifyStatusChanged(qtv_status)), this, SLOT(recvServiceStatusChanged(qtv_status)));
		emit notifyServiceChanged(m_service);
		if (m_service->_status() != _status())
			setStatus(m_service->_status());
	}
}

void qtvUIMainTabBase::detachService()
{
	if (m_service)
	{
		disconnect(m_service, SIGNAL(notifyStatusChanged(qtv_status)), this, SLOT(recvServiceStatusChanged(qtv_status)));
		m_service = NULL;
		emit notifyServiceChanged(NULL);
		setStatus(qstatus_idle);
	}
}

void qtvUIMainTabBase::recvServiceStatusChanged(qtv_status status)
{
	if (status != _status())
		setStatus(status);
}
