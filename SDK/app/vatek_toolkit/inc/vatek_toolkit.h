#pragma once

#include <QtWidgets/QMainWindow>
#include <QDialog>
#include "ui_vatek_toolkit.h"

#include <ui/base/qtv_ui_tab.h>
#include <ui/qtv_ui_app.h>

typedef enum _toolkit_timer
{
	tktimer_null,
	tktimer_waitsource,
	tktimer_update,
}toolkit_timer;

class calibrationDialig : public QDialog
{
	Q_OBJECT
public:
	explicit calibrationDialig(qtvUIMain* main,QWidget* parent = nullptr);
	~calibrationDialig();

protected:
	qtvUITabBase* m_tabbase;
	qtvServiceHandle* m_service;
};

class vatek_toolkit : public QMainWindow
{
    Q_OBJECT
public:
    vatek_toolkit(QWidget *parent = Q_NULLPTR);
    ~vatek_toolkit();

	qtvServiceBase* _service() { return m_uimain._service(); }

protected:

	class UIMainToolKit : public qtvUIMainDeviceTabs
	{
	public:
		explicit UIMainToolKit(vatek_toolkit* toolkit);
		/* qtvUIMainDeviceTabs */
		bool startBroadcast() override;
		bool startUpdate() override;
		void startCalibration() override;
		void stop() override;

	protected:
		vatek_toolkit* m_toolkit;
		QTimer m_timer;
	};

	UIMainToolKit m_uimain;
	QTimer m_timer;
	toolkit_timer m_timemode;
	qtvDevice* m_device;
	qtvSourceBase* m_source;
	qtvServiceChipBase* m_chipservice;
	qtvMuxServiceBase* m_muxhandle;

	void startTimer(toolkit_timer tktimer);
	void stopTimer();

	vatek_result startBroadcast(qtvSourceBase* source,qtvMuxServiceBase* muxhandle);
	void stopBroadcast();

	vatek_result openService(qtvServiceBase* service);
	void closeService();

private Q_SLOTS:
	void recvTimerCheckDevice();
	void recvTimerUpdate();

private:
    Ui::vatek_toolkitClass ui;
};
