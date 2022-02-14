#ifndef QTV_UI_TABS_H
#define QTV_UI_TABS_H

#include <ui/base/qtv_ui_tab.h>

typedef enum _qtv_ui_tabs_mode
{
	qui_tabs_normal,
	qui_tabs_mainleft,
}qtv_ui_tabs_mode;

class QTV_DECL qtvUITabsBase : public QTabWidget
{
	Q_OBJECT
public:
	explicit qtvUITabsBase(qtv_ui_tabs_mode mode = qui_tabs_normal, QWidget* parent = nullptr);
	virtual ~qtvUITabsBase();

	qtvUIMainTabBase* _tabs_main() { return m_tabmain; }

	virtual void attachUIMainTab(qtvUIMainTabBase* main);
	virtual void detachUIMainTab();

protected:
	qtv_ui_tabs_mode m_mode;
	QImage m_logo;
	qtvUIMainTabBase* m_tabmain;
	qtvUITabBase* m_curtab;

	virtual void refreshTabs() = 0;
	void paintEvent(QPaintEvent* paint) override;
	void pushTab(qtvUITabBase* tab);

protected Q_SLOTS:
	void recvTabChanged(int index);
};


#endif
