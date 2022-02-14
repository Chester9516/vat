#include <ui/base/qtv_ui_tab.h>

#include "ui_qtvUITabSimple.h"

qtvUITabSimple::qtvUITabSimple(qtvUIMain* main, const char* name, const char* desc, QWidget* parent) :
	qtvUITabBase(main,name,desc,parent),
	ui(new Ui::qtvUITabSimple)
{
	ui->setupUi(this);
}

qtvUITabSimple::~qtvUITabSimple()
{
	delete ui;
}

void qtvUITabSimple::putWidget(QWidget* widget, int stretch, Qt::Alignment alignment)
{
	ui->content->addWidget(widget, stretch, alignment);
}