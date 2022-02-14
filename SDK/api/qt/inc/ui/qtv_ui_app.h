#ifndef QTV_UI_APP_H
#define QTV_UI_APP_H

#include <ui/base/qtv_ui_tabs.h>
#include <chart/qtv_chart_ui.h>

class QTV_DECL qtvUIMainDeviceTabs : public qtvUIMainTabBase
{
	Q_OBJECT
public:
	explicit qtvUIMainDeviceTabs();
	virtual bool startBroadcast() = 0;
	virtual bool startUpdate() = 0;
	virtual void startCalibration() = 0;
	virtual void stop() = 0;
};

class QTV_DECL qtvUITabsDevice : public qtvUITabsBase
{
	Q_OBJECT
public:
	explicit qtvUITabsDevice(QWidget* parent = nullptr);
	virtual ~qtvUITabsDevice();

	void attachUIMainTab(qtvUIMainTabBase* main) override;
	void detachUIMainTab() override;

protected:
	void refreshTabs() override;

protected Q_SLOTS:
	void recvServiceChanged(qtvServiceBase* service);
};

#endif
