#include <ui/qtv_ui_app.h>

/* qtvUIMainDeviceTabs */

qtvUIMainDeviceTabs::qtvUIMainDeviceTabs() :
	qtvUIMainTabBase(qui_main_tabsdevice)
{

}


/* qtvUITabsDevice */

qtvUITabsDevice::qtvUITabsDevice(QWidget* parent) :
	qtvUITabsBase(qui_tabs_mainleft, parent)
{

}

qtvUITabsDevice::~qtvUITabsDevice()
{
	detachUIMainTab();
}

void qtvUITabsDevice::recvServiceChanged(qtvServiceBase* service)
{
	refreshTabs();
}

void qtvUITabsDevice::attachUIMainTab(qtvUIMainTabBase* main)
{
	qtvUITabsBase::attachUIMainTab(main);
	if (main)
		connect(m_tabmain, SIGNAL(notifyServiceChanged(qtvServiceBase*)), this, SLOT(recvServiceChanged(qtvServiceBase*)));
}

void qtvUITabsDevice::detachUIMainTab()
{
	if (m_tabmain)
	{
		disconnect(m_tabmain, SIGNAL(notifyServiceChanged(qtvServiceBase*)), this, SLOT(recvServiceChanged(qtvServiceBase*)));
		qtvUITabsBase::detachUIMainTab();
		m_tabmain = NULL;
		refreshTabs();
	}
}

void qtvUITabsDevice::refreshTabs()
{
	clear();
	if (m_tabmain)
	{
		qtvServiceBase* service = m_tabmain->_service();
		if (!service)pushTab(new qtvUITabNoDevice(m_tabmain, this));
		else pushTab(new qtvUITabDevice(m_tabmain, this));
	}
}