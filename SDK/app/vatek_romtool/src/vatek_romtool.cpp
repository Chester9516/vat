#include <vatek_romtool.h>
#include <ui/base/qtv_ui_storage.h>
#include "ui_vatek_romtool.h"

#include <QMessageBox>
#include <QFileDialog>
#include "progressdlg.h"

vatek_romtool::vatek_romtool(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::vatek_romtool),
    m_rstatus(romtool_idle),
    m_device(NULL)
{
    ui->setupUi(this);
    ui->listorage->attachStacked(ui->stcontext);
	setFixedSize(QSize(640,400));
	statusBar()->setSizeGripEnabled(false);
    connect(ui->actionDevice, SIGNAL(triggered()), this, SLOT(recvOpenDevice()));
    connect(ui->actionImage, SIGNAL(triggered()), this, SLOT(recvOpenImage()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(recvCloseHandle()));
    connect(ui->actionUpdate, SIGNAL(triggered()), this, SLOT(recvUpdateResource()));
    connect(ui->actionExport, SIGNAL(triggered()), this, SLOT(recvExport()));
    connect(ui->actionUpdateDevice, SIGNAL(triggered()), this, SLOT(recvWriteImage()));
    setStatus(m_rstatus);
}

vatek_romtool::~vatek_romtool()
{
    recvCloseHandle();
    disconnect(ui->actionDevice, SIGNAL(triggered()), this, SLOT(recvOpenDevice()));
    disconnect(ui->actionImage, SIGNAL(triggered()), this, SLOT(recvOpenImage()));
    disconnect(ui->actionClose, SIGNAL(triggered()), this, SLOT(recvCloseHandle()));
    disconnect(ui->actionUpdate, SIGNAL(triggered()), this, SLOT(recvUpdateResource()));
    disconnect(ui->actionExport, SIGNAL(triggered()), this, SLOT(recvExport()));
    disconnect(ui->actionUpdateDevice, SIGNAL(triggered()), this, SLOT(recvWriteImage()));
    delete ui;
}

void vatek_romtool::recvOpenDevice()
{
    if (!m_device)
    {
        m_device = storageDevice::openStorageDevice(1);
        if (!m_device)QMessageBox::warning(this, "vatek_romtool", "can not found valid device.");
        else
        {
            progressDlg newprog(m_device,this);
            vatek_result nres = newprog.runAction(saction_open, m_device);
            if (is_vatek_success(nres))
            {
                ui->listorage->refreshStorage(m_device->_hstorage());
                setStatus(romtool_device);
            }
            else
            {
                QMessageBox::warning(this, "vatek_romtool", QString::asprintf("open device fail : %d", nres));
                storageDevice::closeStorageDevice(m_device);
                m_device = NULL;
            }
        }
    }
}

void vatek_romtool::recvOpenImage()
{

}

void vatek_romtool::recvUpdateResource()
{

}

void vatek_romtool::recvCloseHandle()
{
    if (m_device)
    {
        ui->listorage->refreshStorage(NULL);
        storageDevice::closeStorageDevice(m_device);
        m_device = NULL;
        setStatus(romtool_idle);
    }
}

void vatek_romtool::recvExport()
{

}

void vatek_romtool::recvWriteImage()
{
    QString szimg = QFileDialog::getOpenFileName(this, "Open Image File", ".", "v2Image (*.v2img);");
    if (!szimg.isEmpty())
    {
		QByteArray ba = szimg.toLocal8Bit();
		const char* c_str2 = ba.data();
        Promfile_handle prom = NULL;
        vatek_result nres = vatek_storage_romfile_create(ba.data(), &prom);
        if (is_vatek_success(nres))
        {
			progressDlg newprog(m_device, this);
			nres = newprog.runAction(saction_writeimage, prom,m_device);
            vatek_storage_romfile_free(prom);
        }
        
        if (!is_vatek_success(nres))
            QMessageBox::warning(this, "vatek_tomtool", QString::asprintf("write image fail : %d", nres));
        else recvCloseHandle();
    }
}

void vatek_romtool::setStatus(romtool_status status)
{
	ui->actionClose->setEnabled(status != romtool_idle);
	ui->actionImage->setEnabled(status == romtool_idle);
	ui->actionDevice->setEnabled(status == romtool_idle);
	ui->actionUpdate->setEnabled(status != romtool_idle);
	ui->actionExport->setEnabled(status != romtool_idle);
	ui->actionUpdateDevice->setEnabled(status == romtool_device);
}

