#ifndef QTV_UI_SERVICE_USB_H
#define QTV_UI_SERVICE_USB_H

#include <core/qtv_service_usb.h>
#include <ui/base/qtv_ui_config.h>
#include <ui/base/qtv_ui_wizard.h>
#include <QDialog>

typedef enum _qtv_ui_usbmuxer_mode
{
	qui_usbmuxer_single = 0,
	qui_usbmuxer_mutil = 1,
}qtv_ui_usbmuxer_mode;

class QTV_DECL qtvUIMainUSBMuxer : public qtvUIMain
{
	Q_OBJECT
public:
	explicit qtvUIMainUSBMuxer(qtv_uimain_mode mainmode,qtv_ui_usbmuxer_mode uimode,qtvServiceUSBMuxer* service);
	virtual ~qtvUIMainUSBMuxer();

	virtual qtv_ui_usbmuxer_mode _usbmuxer_mode() { return m_usbmuxermode; }
	virtual const QList<qtvUIConfigBase*> _configs() { return m_configs; }
	virtual Pqtv_locale_param _local_param();
	virtual qtvCodecVideo* _video_codec() { return m_videocodec; }
	virtual qtvCodecAudio* _audio_codec() { return m_audiocodec; }

	virtual qtvCodecProvider* getVideoProvider() = 0;
	virtual qtvCodecProvider* getAudioProvider() = 0;
	virtual void setDefaultLocal(qtvDTVProperties* locale);

	virtual void attachUSBMuxer(qtvServiceUSBMuxer* usbmuxer);
	virtual void attachVideoCodec(qtvCodecVideo* vcodec);
	virtual void attachAudioCodec(qtvCodecAudio* acodec);

	virtual void refreshConfig();

Q_SIGNALS:
	void notifyServiceChanged();

protected:
	QList<qtvUIConfigBase*> m_configs;
	qtv_ui_usbmuxer_mode m_usbmuxermode;
	qtvDTVProperties* m_locale;
	qtvCodecVideo* m_videocodec;
	qtvCodecAudio* m_audiocodec;
	void clearConfig();
};

class QTV_DECL qtvUIDialogUSBMuxerConfig : public QDialog
{
	Q_OBJECT
public:
	static vatek_result createDialog(qtvServiceUSBMuxer* service, qtvUIMainUSBMuxer* main, qtvUIDialogUSBMuxerConfig** dialog, QWidget* parent = nullptr);
	static void freeDialog(qtvUIDialogUSBMuxerConfig* dialog);

protected:
	explicit qtvUIDialogUSBMuxerConfig(QSize szie, qtvServiceUSBMuxer* service, qtvUIMainUSBMuxer* main, QWidget* parent = nullptr);
	virtual ~qtvUIDialogUSBMuxerConfig();

	qtvUIMainUSBMuxer* m_main;
	qtvUITabConfigs* m_tabbase;

	void refreshConfig();
};

class QTV_DECL qtvUIWizardUSBMuxer : public qtvUIWizardBase
{
	Q_OBJECT
public:
	static vatek_result createWizard(qtvCodecProvider* video,qtvCodecProvider* audio,qtvUIWizardUSBMuxer** wizard, QWidget* parent = nullptr);
	static void freeWizard(qtvUIWizardUSBMuxer* wizard);

	/* qtvUIWizardBase */
	vatek_result startWizard() override;

	void getResult(qtvUIMainUSBMuxer** main);

	/* QWizard */
	int nextId() const override;

protected:
	explicit qtvUIWizardUSBMuxer(qtvUIMainUSBMuxer* main, QWidget* parent = nullptr);
	virtual ~qtvUIWizardUSBMuxer();
	qtvUIMainUSBMuxer* m_usbmuxermain;

protected Q_SLOTS:
	void recvServiceChanged();
};

#endif
