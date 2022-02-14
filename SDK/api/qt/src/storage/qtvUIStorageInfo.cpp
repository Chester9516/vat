#include <ui/base/qtv_ui_storage.h>
#include "ui_qtvUIStorageInfo.h"

#include <QFrame>
#include <QLabel>
#include <QTableWidget>


/* qtvUIStorageInfo */

qtvUIStorageInfo::qtvUIStorageInfo(qtvUIMain* main, QWidget* parent) :
	qtvUITabBase(main, "Information", "Storage Information", parent),
	ui(new Ui::qtvUIStorageInfo)
{
	QImage* imglogo = qtvUIResource::getImage(ICON_LOGOBIG);
	ui->setupUi(this);
	ui->lbimage->setFrameShape(QFrame::Panel);
	ui->lbimage->setFrameShadow(QFrame::Sunken);
	ui->lbimage->setLineWidth(2);
	ui->lbimage->setPixmap(QPixmap::fromImage(imglogo->scaled(QSize(300, 60), Qt::KeepAspectRatio,Qt::SmoothTransformation)));
}

qtvUIStorageInfo::~qtvUIStorageInfo()
{
	delete ui;
}

void qtvUIStorageInfo::tabEnter()
{
	static const sinfo_item sinfo_loader[] =
	{
		{"loader_magic",smode_hex,(int32_t)offsetof(loader_header,magic_tag)},
		{"loader_major",smode_hex,(int32_t)offsetof(loader_header,major_version)},
		{"loader_minor",smode_hex,(int32_t)offsetof(loader_header,minor_version)},
		{"loader_arch",smode_arch,(int32_t)offsetof(loader_header,chip_arch)},
	};

	#define SINFO_LOADER_NUMS	(sizeof(sinfo_loader)/sizeof(sinfo_item))

	static const sinfo_item sinfo_app[] =
	{
		{"app_binsize",smode_hex,(int32_t)offsetof(app_header,bin_size)},
		{"app_magic",smode_hex,(int32_t)offsetof(app_header,magic_tag)},
		{"app_major",smode_hex,(int32_t)offsetof(app_header,major_version)},
		{"app_minor",smode_hex,(int32_t)offsetof(app_header,minor_version)},
		{"app_arch",smode_arch,(int32_t)offsetof(app_header,chip_arch)},
		{"app_service",smode_service,(int32_t)offsetof(app_header,service_mode)},
		{"app_crc32",smode_hex,(int32_t)offsetof(app_header,crc32)},
	};

	#define SINFO_APP_NUMS	(sizeof(sinfo_app)/sizeof(sinfo_item))
	#define SINFO_TOTAL_NUMS	(SINFO_LOADER_NUMS + SINFO_APP_NUMS)

	resetTable(ui->tableinfo);
	if (qtvUIMain::is_uimain_storage(m_main->_mode()))
	{
		qtvUIMainStorage* storage = qobject_cast<qtvUIMainStorage*>(m_main);
		Papp_header papp = NULL;
		Ploader_header ploader = NULL;
		hvatek_storage hstorage = storage->_storage_handle();
		if (hstorage)
		{
			vatek_result nres = vatek_storage_get_loader(hstorage, &ploader);
			if (is_vatek_success(nres))
				nres = vatek_storage_get_app(hstorage, &papp);

			if (is_vatek_success(nres))
			{
				int32_t i = 0;
				int32_t pos = 0;
				ui->tableinfo->setRowCount(SINFO_TOTAL_NUMS + 3);
				for (i = 0; i < SINFO_LOADER_NUMS; i++)
					insertItem(pos++, (const Psinfo_item)&sinfo_loader[i],(uint8_t*)ploader);

				for (i = 0; i < SINFO_APP_NUMS; i++)
					insertItem(pos++, (const Psinfo_item)&sinfo_app[i],(uint8_t*)papp);
			}
		}
	}
}

void qtvUIStorageInfo::tabFinish()
{

}

void qtvUIStorageInfo::tabLeave()
{

}

void qtvUIStorageInfo::insertItem(int32_t pos, const Psinfo_item sitem,uint8_t* raw)
{
	QTableWidgetItem* newitem = new QTableWidgetItem(sitem->name);
	newitem->setFlags(newitem->flags() & ~Qt::ItemIsEditable);
	ui->tableinfo->setItem(pos, 0, newitem);

	newitem = NULL;
	if (sitem->mode == smode_hex)
		newitem = new QTableWidgetItem(QString::asprintf("0x%08x", *(uint32_t*)&raw[sitem->offect]));
	else if (sitem->mode == smode_arch)
	{
		binary_chip_arch arch = (binary_chip_arch)*(uint32_t*)&raw[sitem->offect];
		const char* szarch = "chip_unknown";
		if (arch == chip_arch_ax)szarch = "chip_arch_ax";
		else if (arch == chip_arch_bx)szarch = "chip_arch_bx";
		else if(arch == chip_arch_a3)szarch = "chip_arch_a3";
		newitem = new QTableWidgetItem(szarch);
	}
	else if (sitem->mode == smode_service)
	{
		hal_service_mode service = (hal_service_mode)*(uint32_t*)&raw[sitem->offect];
		const char* szservice = "unknown";
		if (service == service_broadcast)szservice = "broadcast";
		else if (service == service_transform)szservice = "transform";
		else if (service == service_rescue)szservice = "rescue";
		newitem = new QTableWidgetItem(szservice);
	}

	if (newitem)
	{
		newitem->setFlags(newitem->flags() & ~Qt::ItemIsEditable);
		ui->tableinfo->setItem(pos, 1, newitem);
	}
}

void qtvUIStorageInfo::resetTable(QTableWidget* table)
{
	int32_t nw = table->viewport()->width() / QTV_UI_TABLE_WIDTH_UNIT;
	QStringList sztitles;

	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->verticalHeader()->setStretchLastSection(true);
	table->horizontalHeader()->setStretchLastSection(true);

	table->setColumnCount(QTABLE_READONLY_FIELDS);
	for (int32_t i = 0; i < (int32_t)QTABLE_READONLY_FIELDS; i++)
	{
		table->setColumnWidth(i, nw * qtable_readonly_fields[i].size);
		sztitles.append(QString(qtable_readonly_fields[i].title));
	}

	table->setHorizontalHeaderLabels(sztitles);
	table->verticalHeader()->hide();
}
