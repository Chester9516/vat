#include <ui/base/qtv_ui_tab.h>
#include <QLabel>
#include <QProgressBar>
#include <ui/qtv_ui_service_usb.h>

#define UITAB_NODEVICE_NAME		"Enumerate"
#define UITAB_NODEVICE_DESC		"detect supported devices"

#define NODEVOCE_SIZE		200
#define NODEVICE_STEP_MS	200
#define NODEVICE_STEP		5

qtvUITabNoDevice::qtvUITabNoDevice(qtvUIMain* main, QWidget* parent) :
	qtvUITabSimple(main, UITAB_NODEVICE_NAME, UITAB_NODEVICE_DESC, parent),
	m_progress(NULL)
{
	QLabel* newimg = new QLabel(this);
	m_progress = new QProgressBar(this);
	QImage* imgsur = qtvUIResource::getImage(ICON_NODEVICE);
	QImage imglogo = imgsur->scaled(QSize(NODEVOCE_SIZE, NODEVOCE_SIZE), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	
	newimg->setFixedWidth(NODEVOCE_SIZE);
	newimg->setFixedHeight(NODEVOCE_SIZE);
	newimg->setPixmap(QPixmap::fromImage(imglogo));

	m_progress->setFixedWidth(newimg->width());
	m_progress->setTextVisible(false);
	putWidget(newimg);
	putWidget(m_progress);
	setValid(false);
}

qtvUITabNoDevice::~qtvUITabNoDevice()
{
	stopTimer();
	m_progress = nullptr;
}

void qtvUITabNoDevice::tabEnter()
{
	startTimer();
}

void qtvUITabNoDevice::tabFinish()
{
	stopTimer();
}

void qtvUITabNoDevice::tabLeave()
{
	stopTimer();
}

void qtvUITabNoDevice::startTimer()
{
	if (!m_timer.isActive())
	{
		m_progress->setValue(0);
		connect(&m_timer, SIGNAL(timeout()), this, SLOT(recvTimeOut()));
		m_timer.start(NODEVICE_STEP_MS);
	}
}

void qtvUITabNoDevice::stopTimer()
{
	if (m_timer.isActive())
	{
		m_timer.stop();
		disconnect(&m_timer, SIGNAL(timeout()), this, SLOT(recvTimeOut()));
	}
}

void qtvUITabNoDevice::recvTimeOut()
{
	int32_t value = m_progress->value() + NODEVICE_STEP;
	if (value >= 100)value = 0;

	if (!_valid())
	{
		qtvUIMainUSBMuxer* umuxmain = qobject_cast<qtvUIMainUSBMuxer*>(m_main);
		if (umuxmain)
		{
			vatek_result nres = qtvDeviceManage::enumDevices(DEVICE_BUS_USB, service_transform);
			if (nres > vatek_success)
			{
				qtvDevice* device = NULL;
				nres = qtvDeviceManage::openDevice(0, &device);
				if (is_vatek_success(nres))
				{
					qtvServiceUSBMuxer* newservice = NULL;
					nres = qtvServiceUSBMuxer::createService(device, &newservice);
					if (is_vatek_success(nres))umuxmain->attachUSBMuxer(newservice);
				}
			}
		}
		if (m_main->_service())setValid(true);
	}
	m_progress->setValue(value);
}
