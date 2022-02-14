#ifndef QTV_UI_SERVICE_BASE_H
#define QTV_UI_SERVICE_BASE_H

#include <ui/base/qtv_ui_wizard.h>

class QTV_DECL qtvUIMainWizardBroadcast : public qtvUIMain
{
	Q_OBJECT
public:
	explicit qtvUIMainWizardBroadcast(qtv_uimain_mode mode, qtvServiceChipBase* service);
	virtual ~qtvUIMainWizardBroadcast();

	virtual qtvDTVProperties* _default_locale() { return m_locale; }
	virtual qtvSourceBase* _source() { return m_source; }
	virtual qtvMuxServiceBase* _mux_handle() { return m_muxhandle; }

	virtual void setDefaultLocale(qtvDTVProperties* locale);
	virtual void setMuxHandle(qtvMuxServiceBase* muxhandle);
	virtual void attachSource(qtvSourceBase* source);
	virtual void detachSource();

	virtual void getSource(qtvSourceBase** source);
	virtual void getLocale(qtvDTVProperties** locale);
	virtual void getMuxHandle(qtvMuxServiceBase** muxhandle);

protected:
	qtvDTVProperties* m_locale;
	qtvSourceBase* m_source;
	qtvMuxServiceBase* m_muxhandle;

Q_SIGNALS:
	void notifyLocaleChanged(qtvDTVProperties* locale);
	void notifyMuxHandleChenged(qtvMuxServiceBase* muxhandle);
	void notifySourceAttach(qtvSourceBase* source);
	void notifySourceDetach();
};

class QTV_DECL qtvUIWizardBroadcast : public qtvUIWizardBase
{
	Q_OBJECT
public:
	static vatek_result createWizard(qtvServiceBase* service, qtvUIWizardBroadcast** wizard,QWidget* parent = nullptr);
	static void freeWizard(qtvUIWizardBroadcast* wizard);

	/* qtvUIMainWizardBase */
	vatek_result startWizard() override;
	vatek_result getResult(qtvDTVProperties** locale, qtvSourceBase** source, qtvMuxServiceBase** muxhandle);
	/* QWizard */
	int nextId() const override;

protected:
	explicit qtvUIWizardBroadcast(qtvUIMainWizardBroadcast* main,qtvServiceChipBase* service, QWidget* parent = nullptr);
	~qtvUIWizardBroadcast();

	qtvUIMainWizardBroadcast* m_bcmain;
};

#endif
