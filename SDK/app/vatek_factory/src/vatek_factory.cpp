#include "vatek_factory.h"
#include <openfiledlg.h>


#include <QMessagebox>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QDate>
#include <QStackedWidget>

const char* vatek_factory::imageTempFile = "~tmpimage.vimage";

vatek_factory::vatek_factory(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::vatek_factory),
    m_hstorage(NULL),
    m_istempimage(false)
{
    ui->setupUi(this);
    ui->lisections->attachStacked(ui->stpages);
	setFixedSize(size());
	statusBar()->setSizeGripEnabled(false);
    refreshUIStatus();
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(recvOpenFile()));
    connect(ui->actionOpenImage, SIGNAL(triggered()), this, SLOT(recvOpenImage()));
    connect(ui->actionBuildImage, SIGNAL(triggered()), this, SLOT(recvBuildImage()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(recvCloseFile()));
}

vatek_factory::~vatek_factory()
{
    closeRom();
	disconnect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(recvOpenFile()));
    disconnect(ui->actionOpenImage, SIGNAL(triggered()), this, SLOT(recvOpenImage()));
    disconnect(ui->actionBuildImage, SIGNAL(triggered()), this, SLOT(recvBuildImage()));
    disconnect(ui->actionClose, SIGNAL(triggered()), this, SLOT(recvCloseFile()));
    delete ui;
}

vatek_result vatek_factory::openRomImage(QString& fimg)
{
    std::string szimg = fimg.toStdString();
    Pstorage_handle pstorage = NULL;
    vatek_result nres = vatek_storage_open_file_handle(szimg.c_str(), &pstorage, storage_handler, this);
    if (is_vatek_success(nres))
    {
        hvatek_storage hstorage = NULL;
        nres = vatek_storage_open(pstorage, &hstorage,1);
        if (is_vatek_success(nres))
        {
            closeRom();
            m_hstorage = hstorage;
            m_istempimage = false;
            refreshUIStatus();
        }

        if (!is_vatek_success(nres))
            vatek_storage_free_handle(pstorage);
    }

	if (!is_vatek_success(nres))
		QMessageBox::warning(this, windowTitle(), QString::asprintf("open image fail : %d", nres));
    return nres;
}

vatek_result vatek_factory::openRom(QString& floader, QString& fapp)
{
    std::string szloader = floader.toStdString();
    std::string szapp = fapp.toStdString();
    Pstorage_handle pstorage = NULL;
    vatek_result nres = vatek_storage_create_file_handle(imageTempFile, szloader.c_str(), szapp.c_str(), &pstorage, storage_handler, this);
    if (is_vatek_success(nres))
    {
        hvatek_storage hstorage = NULL;
        nres = vatek_storage_open(pstorage, &hstorage,1);
        if (is_vatek_success(nres))
        {
            closeRom();
            m_hstorage = hstorage;
            m_istempimage = true;
            refreshUIStatus();

        }
        if(!is_vatek_success(nres))
            vatek_storage_free_handle(pstorage);
    }

    if (!is_vatek_success(nres))
        QMessageBox::warning(this, windowTitle(), QString::asprintf("open file fail : %d", nres));
    return nres;
}

void vatek_factory::closeRom()
{
    if (m_hstorage)vatek_storage_close(m_hstorage);
    QFile::remove(imageTempFile);
    m_hstorage = NULL;
    m_istempimage = false;
    refreshUIStatus();
}

void vatek_factory::recvOpenFile()
{
    openfiledlg* filedlg = new openfiledlg(this);
    int32_t nres = filedlg->exec();
    if (nres == QDialog::DialogCode::Accepted)
        openRom(filedlg->getLoaderFile(), filedlg->getAppFile());
    delete filedlg;
}

void vatek_factory::recvOpenImage()
{
    QString szimgfile = QFileDialog::getOpenFileName(this, "Open Image", openfiledlg::getDefaultDir(), "v2Image (*.v2img)");
    if (!szimgfile.isEmpty())
    {
        openfiledlg::setDefaultDir(szimgfile);
        openRomImage(szimgfile);
    }
}

void vatek_factory::recvCloseFile()
{
    closeRom();
}

void vatek_factory::recvBuildImage()
{
    if (m_hstorage)
    {
        QString szfilename;
        Papp_header papp = NULL;
        vatek_result nres = vatek_storage_get_app(m_hstorage, &papp);
        if (is_vatek_success(nres))
        {
            QDateTime t = QDateTime::currentDateTime();
            const char* szapp = "app2_broadcast";
            if (papp->service_mode == service_transform)
                szapp = "app2_transform";
            szfilename = QString::asprintf("%s_%02d%02d%02d_%02d%02d", 
                                           szapp, 
                                           t.date().year() & 0xFF,
                                           t.date().month(),
                                           t.date().day(),
                                           t.time().hour(),
                                           t.time().minute());

            szfilename = QDir::cleanPath(openfiledlg::getDefaultDir() + QDir::separator() + szfilename);
            szfilename = QFileDialog::getSaveFileName(this, "Build Image", szfilename, "v2Image (*.v2img)");
            if (!szfilename.isEmpty())
            {
                std::string szimg = szfilename.toStdString();
                nres = vatek_storage_save(m_hstorage, szimg.c_str());
            }
            else return;

            if (!is_vatek_success(nres))
                QMessageBox::warning(this, windowTitle(), QString::asprintf("Build image fail : %d", nres));
            else QMessageBox::information(this, windowTitle(), QString::asprintf("Build image success!"));
        }
        else QMessageBox::warning(this,windowTitle(), "can not found current application and loader");
    }
}

void vatek_factory::storage_handler(rom_progress_msg msg, void* progressval, void* param)
{
    vatek_factory* pfactory = (vatek_factory*)param;
}

bool vatek_factory::isValid()
{
    return (m_hstorage != NULL);
}

void vatek_factory::refreshUIStatus()
{
	ui->actionBuildImage->setEnabled(isValid());
	ui->actionOpen->setEnabled(!isValid());
	ui->actionClose->setEnabled(isValid());
    ui->actionOpenImage->setEnabled(!isValid());

    ui->lisections->refreshStorage(m_hstorage);
}
