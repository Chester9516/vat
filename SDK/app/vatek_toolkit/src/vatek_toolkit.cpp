#include "vatek_toolkit.h"
#include <ui/qtv_ui_service_base.h>
#include <core/qtv_device.h>
#define TOOLKIT_TIMEOUT_MS		1000

/* vatek_toolkit */

vatek_toolkit::vatek_toolkit(QWidget* parent) :
	m_uimain(this),
	m_timemode(tktimer_null),
	m_source(NULL),
	m_chipservice(NULL),
	m_muxhandle(NULL),
	QMainWindow(parent)
{
	ui.setupUi(this);
	ui.deviceTabs->attachUIMainTab(&m_uimain);
	startTimer(tktimer_waitsource);
	setFixedSize(size());
	statusBar()->setSizeGripEnabled(false);
}

vatek_toolkit::~vatek_toolkit()
{
	stopTimer();
	if (m_chipservice)
	{
		if (m_chipservice->_status() == qstatus_running)
			m_chipservice->stop();
		closeService();
		qtvDeviceManage::closeDevice(m_device,0);
	}
	ui.deviceTabs->detachUIMainTab();
}

void vatek_toolkit::startTimer(toolkit_timer tktimer)
{
	vatek_result nres = vatek_success;
	if (m_timemode != tktimer_null)stopTimer();
	if (tktimer == tktimer_waitsource)
		connect(&m_timer, SIGNAL(timeout()), this, SLOT(recvTimerCheckDevice()));
	else if (tktimer == tktimer_update)
		connect(&m_timer, SIGNAL(timeout), this, SLOT(recvTimerUpdate()));
	else nres = vatek_badparam;

	if (is_vatek_success(nres))
	{
		m_timemode = tktimer;
		m_timer.start(TOOLKIT_TIMEOUT_MS);
	}
}

void vatek_toolkit::stopTimer()
{
	if (m_timemode != tktimer_null)
	{
		m_timer.stop();
		if(m_timemode == tktimer_waitsource)
			disconnect(&m_timer, SIGNAL(timeout()), this, SLOT(recvTimerCheckDevice()));
		else if(m_timemode == tktimer_update)
			disconnect(&m_timer, SIGNAL(timeout), this, SLOT(recvTimerUpdate()));
	}
	m_timemode = tktimer_null;
}

vatek_result vatek_toolkit::openService(qtvServiceBase* service)
{
	vatek_result nres = vatek_success;
	if (m_chipservice)closeService();
	{
		m_chipservice = qobject_cast<qtvServiceChipBase*>(service);
		if (m_chipservice)
		{
			m_uimain.setService(m_chipservice);
			nres = vatek_success;
		}
	}
	return nres;
}

void vatek_toolkit::closeService()
{
	if (m_chipservice)
	{
		m_uimain.setService(NULL);
		delete m_chipservice;
	}
	m_chipservice = NULL;
}

vatek_result vatek_toolkit::startBroadcast(qtvSourceBase* source, qtvMuxServiceBase* muxhandle)
{
	vatek_result nres = vatek_badstatus;
	if (m_chipservice)
	{
		m_source = source;
		m_muxhandle = muxhandle;
		nres = m_chipservice->startBroadcast(m_source, m_muxhandle);
	}
	return nres;
}

void vatek_toolkit::stopBroadcast()
{
	vatek_result nres = vatek_success;
	if (m_chipservice)
	{
		if (m_chipservice->_status() == qstatus_running)
			nres = m_chipservice->stop();
	}
	closeService();
	qtvDeviceManage::closeDevice(m_device, 1);
	if (m_muxhandle)delete m_muxhandle;
	if (m_source)delete m_source;
	m_muxhandle = NULL;
	m_source = NULL;
	m_device = NULL;

	startTimer(tktimer_waitsource);
}

void vatek_toolkit::recvTimerCheckDevice()
{
	vatek_result nres = qtvDeviceManage::enumDevices(DEVICE_BUS_ALL, service_unknown);
	if (nres > vatek_success)
	{
		qtvDevice* device = NULL;
		nres = qtvDeviceManage::openDevice(0, &device);
		if (is_vatek_success(nres))
		{
			qtvServiceBase* service;
			nres = qtvServiceBase::createService(device, &service);
			if (is_vatek_success(nres))
			{
				nres = openService(service);
				if (!is_vatek_success(nres))delete service;
			}
			if (!is_vatek_success(nres))qtvDeviceManage::closeDevice(device, 0);
			else m_device = device;
		}

		if (is_vatek_success(nres))stopTimer();
		else VWAR("open device and create service fail : %d", nres);
	}
}

void vatek_toolkit::recvTimerUpdate()
{

}

/* UIMainToolKit */

vatek_toolkit::UIMainToolKit::UIMainToolKit(vatek_toolkit* toolkit) :
	m_toolkit(toolkit)
{

}

bool vatek_toolkit::UIMainToolKit::startBroadcast()
{
	qtvUIWizardBroadcast* bcwizard = NULL;
	vatek_result nres = qtvUIWizardBroadcast::createWizard(m_toolkit->_service(), &bcwizard, m_toolkit);
	if (is_vatek_success(nres))
	{
		nres = bcwizard->startWizard();
		if (is_vatek_success(nres))
		{
			qtvSourceBase* source = NULL;
			qtvMuxServiceBase* muxhandle = NULL;
			bcwizard->getResult(NULL, &source, &muxhandle);
			if (source)nres = m_toolkit->startBroadcast(source, muxhandle);
			else nres = vatek_unsupport;
		}
		qtvUIWizardBroadcast::freeWizard(bcwizard);
	}
	if (is_vatek_success(nres))return true;
	return false;
}

void vatek_toolkit::UIMainToolKit::startCalibration()
{
	calibrationDialig newdlg(this,m_toolkit);
	newdlg.exec();
}

bool vatek_toolkit::UIMainToolKit::startUpdate()
{
	return false;
}

void vatek_toolkit::UIMainToolKit::stop()
{
	m_toolkit->stopBroadcast();
}

/* calibrationDialig */

calibrationDialig::calibrationDialig(qtvUIMain* main, QWidget* parent) :
	QDialog(parent)
{
	setFixedSize(QSize(450,300));
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowTitle("calibration");
	m_tabbase = new qtvUITabCalibration(main, this);
	setLayout(m_tabbase->layout());
}

calibrationDialig::~calibrationDialig()
{
	delete m_tabbase;
}
