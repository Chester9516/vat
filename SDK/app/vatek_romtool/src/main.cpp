#include <QApplication>
#include "vatek_romtool.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
    QApplication a(argc, argv);
    vatek_romtool w;

#ifdef WIN32
	QFont font("Aria");
	font.setStyleHint(QFont::Monospace);
	font.setPixelSize(11);
	QApplication::setFont(font);
#endif
    w.show();
    return a.exec();
}
