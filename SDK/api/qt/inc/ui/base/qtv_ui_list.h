#ifndef QTV_UI_LIST_H
#define QTV_UI_LIST_H

#include <QListWidget>
#include <ui/base/qtv_ui_core.h>

typedef enum _qtv_ui_list_mode
{
	qui_list_normal,
	qui_list_logo,
	qui_list_logo_title_1,
	qui_list_logo_title_2,
}qtv_ui_list_mode;

class QTV_DECL qtvUIListBase : public QListWidget
{
	Q_OBJECT
public:
	explicit qtvUIListBase(QWidget* parent = nullptr);
};

class QTV_DECL qtvUIListItem : public QObject
{
	Q_OBJECT
public:
	explicit qtvUIListItem(uint32_t logo, int32_t sizelogo, const char* title0, const char* title1);
	explicit qtvUIListItem(uint32_t logo, int32_t sizelogo, QString& title0, QString& title1);
	explicit qtvUIListItem(uint32_t logo, int32_t sizelogo, QString& title0);

	QString& _title_0() { return m_title0; }
	QString& _title_1() { return m_title1; }
	uint32_t _logo() { return m_logo; }
	QPixmap& _logo_pixmap() { return m_pixmap; }
	QWidget* _widget() { return m_widget; }

	void setWidget(QWidget* widget);

protected:
	uint32_t m_logo;
	int32_t m_sizelogo;
	QPixmap m_pixmap;
	QString m_title0;
	QString m_title1;
	QWidget* m_widget;

	void refreshLogo();
};

class QTV_DECL qtvUIListConfig : public qtvUIListBase
{
	Q_OBJECT
public:
	explicit qtvUIListConfig(QWidget* parent = nullptr);
	virtual ~qtvUIListConfig();

	virtual void resetList(qtv_ui_list_mode mode,int32_t iconsize);
	virtual void insertItem(qtvUIListItem* item);
	virtual qtvUIListItem* getSelectConfig();
	virtual void setSelectConfig(int32_t idx);

//Q_SIGNALS:
//	void notifyItemsChanged();

protected:
	QList<qtvUIListItem*> m_items;
};


#endif
