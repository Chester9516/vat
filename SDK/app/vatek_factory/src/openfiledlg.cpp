#include "openfiledlg.h"
#include "ui_openfiledlg.h"

#include <QFileDialog>
#include <QDir>

QString openfiledlg::m_pathdir;

openfiledlg::openfiledlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::openfiledlg)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    connect(ui->btn_loader, SIGNAL(clicked(bool)), this, SLOT(recvClickLoader(bool)));
    connect(ui->btn_app, SIGNAL(clicked(bool)), this, SLOT(recvClickApp(bool)));
    connect(ui->btn_rf, SIGNAL(clicked(bool)), this, SLOT(recvClickRF(bool)));
    connect(ui->btn_ok, SIGNAL(clicked(bool)), this, SLOT(recvClickOK(bool)));

    ui->btn_ok->setEnabled(checkValid());
    if(m_pathdir.isEmpty())
        m_pathdir = QDir::currentPath();
}
openfiledlg::~openfiledlg()
{
    delete ui;
}

void openfiledlg::recvClickLoader(bool checked)
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open Loader", m_pathdir, "Loader Files (*.v2loader *.v2lic)");
    if (!fileName.isEmpty())
    {
        m_pathdir = QFileInfo(fileName).absoluteFilePath();
        ui->txt_loader->setText(fileName);
    }
	ui->btn_ok->setEnabled(checkValid());
}

void openfiledlg::recvClickApp(bool checked)
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open App", m_pathdir, "App Files (*.v2app)");
    if (!fileName.isEmpty())
    {
        setDefaultDir(fileName);
        ui->txt_app->setText(fileName);
    }
	ui->btn_ok->setEnabled(checkValid());
}

void openfiledlg::recvClickRF(bool checked)
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open RF Table", m_pathdir, "RF Table Files (*.r2tune)");
    if (!fileName.isEmpty())
    {
        setDefaultDir(fileName);
        ui->txt_rf->setText(fileName);
    }
	ui->btn_ok->setEnabled(checkValid());
}

void openfiledlg::recvClickOK(bool checked)
{
    if (QFileInfo::exists(ui->txt_app->text()) &&
        QFileInfo::exists(ui->txt_loader->text()) &&
        QFileInfo::exists(ui->txt_rf->text()))accept();
}

bool openfiledlg::checkValid()
{
    if (!ui->txt_app->text().isEmpty() &&
        !ui->txt_loader->text().isEmpty() &&
        !ui->txt_rf->text().isEmpty())
    {
        return true;
    }
    return false;
}

QString openfiledlg::getAppFile()
{
    return ui->txt_app->text();
}

QString openfiledlg::getLoaderFile()
{
    return ui->txt_loader->text();
}

QString openfiledlg::getRFTableFile()
{
    return ui->txt_rf->text();
}

QString& openfiledlg::getDefaultDir()
{
    return m_pathdir;
}

void openfiledlg::setDefaultDir(QString fullpath)
{
    m_pathdir = QFileInfo(fullpath).absoluteFilePath();;
}
