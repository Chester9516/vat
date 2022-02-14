#ifndef QTV_UI_WIZARD_H
#define QTV_UI_WIZARD_H

#include <QWizard>
#include <QWizardPage>
#include <QShowEvent>
#include <QHideEvent>
#include <ui/base/qtv_ui_tab.h>

#define BCWIZARD_ID_LOCALE			0
#define BCWIZARD_ID_SOURCE			1
#define BCWIZARD_ID_SOURCEVALID		2
#define BCWIZARD_ID_CONTEXT			3
#define BCWIZARD_ID_MODULATION		4

#define BCWIZARD_ID_MUXPARAM		5
#define BCWIZARD_ID_VIDEOCODEC		6
#define BCWIZARD_ID_AUDIOCODEC		7

#define BCWIZARD_ID_NODEVICE		8

class QTV_DECL qtvUIWizardPage : public QWizardPage
{
	Q_OBJECT
public:
	explicit qtvUIWizardPage(qtvUIMain* main,uint32_t id, QWidget* parent = nullptr);
	virtual ~qtvUIWizardPage();

	uint32_t _id() { return m_id; }
	void setWizardButtonEnable(QWizard::WizardButton button, bool isenable);

	/* QWizardPage */
	void initializePage() override;
	void cleanupPage() override;
	bool isComplete()const override;
	bool validatePage() override;

	void tabEnter();
	void tabLeave();

protected:
	qtvUIMain* m_main;
	qtvUITabBase* m_tab;
	uint32_t m_id;

	void setTab(qtvUITabBase* tab);


protected Q_SLOTS:
	void recvValidChanged(bool bvalid);
};

class QTV_DECL qtvUIWizardBase : public QWizard
{
	Q_OBJECT
public:
	explicit qtvUIWizardBase(qtvUIMain* main, QWidget* parent = nullptr);
	virtual ~qtvUIWizardBase();

	virtual vatek_result startWizard() = 0;
	void done(int result) override;

protected:
	vatek_result m_result;
	QList<qtvUIWizardPage*> m_pages;
	qtvUIWizardPage* m_currentpage;
	int32_t m_isdone;
	qtvUIMain* m_main;

	virtual void pushPage(uint32_t id);
	qtvUIWizardPage* getPage(uint32_t id);

protected Q_SLOTS:
	void recvPageChanged(int id);
};

#endif
