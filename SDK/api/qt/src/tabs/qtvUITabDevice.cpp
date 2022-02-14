#include <ui/base/qtv_ui_tab.h>
#include <ui/base/qtv_ui_table.h>
#include <ui/qtv_ui_app.h>

#include "ui_qtvUITabDevice.h"

#define TABDEVICE_NAEM		"Device"
#define TABDEVICE_DESC		"device detail information"

#define DEVICE_ICON_SIZE    64

qtvUITabDevice::qtvUITabDevice(qtvUIMain* main,QWidget* parent) :
	qtvUITabBase(main,TABDEVICE_NAEM, TABDEVICE_DESC, parent),
	m_maintab(NULL),
	m_chipinfo(NULL),
	m_tabstatus(NULL),
	ui(new Ui::qtvUITabDevice)
{
	ui->setupUi(this);
	connect(ui->btn_start, SIGNAL(clicked(bool)), this, SLOT(recvBtnStartClick(bool)));
	connect(ui->btn_calibration, SIGNAL(clicked(bool)), this, SLOT(recvBtnCalibrationClick(bool)));
	m_tabstatus = new qtvUITabStatus(main, ui->device_content);
	ui->device_content->addWidget(m_tabstatus);
	ui->btn_calibration->setEnabled(false);
	m_tabstatus->tabEnter();
}

qtvUITabDevice::~qtvUITabDevice()
{
	disconnect(ui->btn_start, SIGNAL(clicked(bool)), this, SLOT(recvBtnStartClick(bool)));
	resetService();
	detachUIMainTab();
	delete m_tabstatus;
	delete ui;
}

void qtvUITabDevice::tabEnter()
{
	if (qtvUIMain::is_uimain_tabs(m_main->_mode()))
	{
		attachUIMainTab(qobject_cast<qtvUIMainTabBase*>(m_main));
		return;
	}
	VWAR("qtvUITabDevice unsupport qtvUIMain");
}

void qtvUITabDevice::tabFinish()
{
	detachUIMainTab();
}

void qtvUITabDevice::tabLeave()
{
	detachUIMainTab();
}

void qtvUITabDevice::attachUIMainTab(qtvUIMainTabBase* maintab)
{
	detachUIMainTab();
	if (maintab)
	{
		m_maintab = maintab;
		connect(m_maintab, SIGNAL(notifyServiceChanged(qtvServiceBase*)), this, SLOT(recvServiceChanged(qtvServiceBase*)));
		connect(m_maintab, SIGNAL(notifyStatusChanged(qtv_status)), this, SLOT(recvStatusChanged(qtv_status)));
		connect(m_maintab, SIGNAL(notifyRunningUpdate()), this, SLOT(recvRunningUpdate()));
		if (m_maintab->_service())
			recvServiceChanged(m_maintab->_service());
		recvStatusChanged(m_maintab->_status());
	}
}

void qtvUITabDevice::detachUIMainTab()
{
	if (m_maintab)
	{
		m_tabstatus->tabLeave();
		disconnect(m_maintab, SIGNAL(notifyServiceChanged(qtvServiceBase*)), this, SLOT(recvServiceChanged(qtvServiceBase*)));
		disconnect(m_maintab, SIGNAL(notifyStatusChanged(qtv_status)), this, SLOT(recvStatusChanged(qtv_status)));
		disconnect(m_maintab, SIGNAL(notifyRunningUpdate()), this, SLOT(recvRunningUpdate()));
	}
	m_maintab = NULL;
}

void qtvUITabDevice::recvBtnCalibrationClick(bool bclk)
{
	ui->btn_calibration->setEnabled(false);
	if (m_maintab)
	{
		if (m_maintab->_mode() == qui_main_tabsdevice)
		{
			qtvUIMainDeviceTabs* tabdevice = qobject_cast<qtvUIMainDeviceTabs*>(m_maintab);
			if (tabdevice)
			{
				tabdevice->startCalibration();
				goto end_start;
			}
		}
	}
	VWAR("qtvUITabDevice recvBtnCalibrationClick not support main");

end_start:
	ui->btn_calibration->setEnabled(true);
}

void qtvUITabDevice::recvBtnStartClick(bool bclk)
{
	ui->btn_start->setEnabled(false);
	if (m_maintab)
	{
		if (m_maintab->_mode() == qui_main_tabsdevice)
		{
			qtvUIMainDeviceTabs* tabdevice = qobject_cast<qtvUIMainDeviceTabs*>(m_maintab);
			if (tabdevice)
			{
				if (tabdevice->_status() == qstatus_idle)
					tabdevice->startBroadcast();
				else tabdevice->stop();
				goto end_start;
			}
		}
	}
	VWAR("qtvUITabDevice recvBtnStartClick not support main");

end_start:
	ui->btn_start->setEnabled(true);
}

void qtvUITabDevice::resetService()
{
	ui->tableinfo->setProperties(qui_table_readonly, NULL);
	ui->txt_devicename->setText("");
	ui->txt_deviceservice->setText("");
	if (m_chipinfo)delete m_chipinfo;
	m_chipinfo = NULL;
}

void qtvUITabDevice::recvServiceChanged(qtvServiceBase* service)
{
	resetService();
	if (service)
	{
		qtvDevice* device = service->_device();
		QImage* icondev = qtvUIResource::getDeviceImage(service->_device()->_chip_info()->chip_module);
		m_chipinfo = qtvPropertyCollection::createChipInformation(device->_chip_info());
		ui->lab_deviceicon->setPixmap(QPixmap::fromImage(icondev->scaled(QSize(DEVICE_ICON_SIZE, DEVICE_ICON_SIZE))));
		ui->txt_devicename->setText(device->_name());
		ui->txt_deviceservice->setText(qtvDevice::getServiceName(device));
		ui->tableinfo->setProperties(qui_table_readonly, m_chipinfo);
	}
	
}

void qtvUITabDevice::recvStatusChanged(qtv_status status)
{
	if (status == qstatus_idle)ui->btn_start->setText("start");
	else ui->btn_start->setText("stop");
	ui->btn_calibration->setEnabled(!(status == qstatus_idle));
	ui->tableinfo->setProperties(qui_table_readonly, m_chipinfo);
}

void qtvUITabDevice::recvRunningUpdate()
{

}
