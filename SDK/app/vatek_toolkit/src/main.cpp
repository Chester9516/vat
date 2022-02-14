#include <vatek_toolkit.h>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
	vatek_toolkit w;

#ifdef WIN32
	QFont font("Aria");
	font.setStyleHint(QFont::Monospace);
	font.setPixelSize(11);
	QApplication::setFont(font);
#endif
	w.show();
	return a.exec();
}
