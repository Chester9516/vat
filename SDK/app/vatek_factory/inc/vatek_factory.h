#ifndef VATEK_FACTORY_H
#define VATEK_FACTORY_H

#include <QMainWindow>
#include <vatek_sdk_storage.h>
#include "./ui_vatek_factory.h"

#include <ui/base/qtv_ui_storage.h>

QT_BEGIN_NAMESPACE
namespace Ui { class vatek_factory; }
QT_END_NAMESPACE

class vatek_factory : public QMainWindow
{
    Q_OBJECT

public:
    vatek_factory(QWidget *parent = nullptr);
    ~vatek_factory();

private:
    hvatek_storage m_hstorage;
    bool m_istempimage;

    vatek_result openRom(QString& floader,QString& fapp);
    vatek_result openRomImage(QString& fimg);
    void closeRom();
    void refreshUIStatus();
    bool isValid();

    static void storage_handler(rom_progress_msg msg, void* progressval, void* param);

    static const char* imageTempFile;

protected Q_SLOTS:
    void recvOpenFile();
    void recvOpenImage();
    void recvCloseFile();
    void recvBuildImage();

private:
    Ui::vatek_factory *ui;
};
#endif // VATEK_FACTORY_H
