#ifndef VATEK_ROMTOOL_H
#define VATEK_ROMTOOL_H

#include <QMainWindow>
#include <vatek_sdk_storage.h>
#include <vatek_sdk_device.h>
#include <storage_device.h>

namespace Ui {
class vatek_romtool;
}

typedef enum _romtool_status
{
    romtool_idle,
    romtool_device,
    romtool_image,
}romtool_status;

class vatek_romtool : public QMainWindow
{
    Q_OBJECT

public:
    explicit vatek_romtool(QWidget *parent = nullptr);
    ~vatek_romtool();

    void setStatus(romtool_status status);

protected:
    romtool_status m_rstatus;
    storageDevice* m_device;

private:
    Ui::vatek_romtool *ui;

private Q_SLOTS:
    void recvOpenDevice();
    void recvOpenImage();
    void recvWriteImage();
    void recvUpdateResource();
    void recvExport();
    void recvCloseHandle();
};

#endif // VATEK_ROMTOOL_H
