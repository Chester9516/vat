#ifndef OPENFILEDLG_H
#define OPENFILEDLG_H

#include <QDialog>

namespace Ui {
class openfiledlg;
}

class openfiledlg : public QDialog
{
    Q_OBJECT

public:
    explicit openfiledlg(QWidget *parent = nullptr);
    ~openfiledlg();

    QString getAppFile();
    QString getLoaderFile();
    QString getRFTableFile();

    static QString& getDefaultDir();
    static void setDefaultDir(QString fullpath);

protected Q_SLOTS:
    void recvClickLoader(bool checked);
    void recvClickApp(bool checked);
    void recvClickRF(bool checked);
    void recvClickOK(bool checked);

protected:
    static QString m_pathdir;

    bool checkValid();

private:
    Ui::openfiledlg *ui;
};

#endif // OPENFILEDLG_H
