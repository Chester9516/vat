#ifndef PROGRESSDLG_H
#define PROGRESSDLG_H

#include <QDialog>
#include <QTimer>
#include <storage_device.h>

namespace Ui {
class progressDlg;
}

typedef enum _storage_action
{
	saction_idle,
	saction_open,
	saction_update,
	saction_writeimage,
}storage_action;

class progressDlg : public QDialog
{
    Q_OBJECT

public:
    explicit progressDlg(storageDevice* pdevice,QWidget *parent = nullptr);
    ~progressDlg();

    vatek_result runAction(storage_action action, storageDevice* device);
    vatek_result runAction(storage_action action, Promfile_handle prom, storageDevice* device);


protected:
    storage_action m_action;
    storageDevice* m_device;
    vatek_result m_result;
    Promfile_handle m_romfile;

protected Q_SLOTS:
    void recvTimeout();
    void recvStartAction();
    void recvDeviceStatus();

private:
    Ui::progressDlg *ui;
};

#endif // PROGRESSDLG_H
