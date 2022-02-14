#ifndef QTV_UI_TABLE_H
#define QTV_UI_TABLE_H

#include <ui/base/qtv_ui_core.h>
#include <QTableWidget>
#include <QResizeEvent>
#include <QStyledItemDelegate.h>

#define QTV_UI_TABLE_WIDTH_UNIT		20

typedef enum _qui_table_mode
{
	qui_table_normal,
	qui_table_readonly,
}qui_table_mode;

typedef struct _qui_table_field
{
	const char* title;
	int32_t size;
}qui_table_field, * Pqui_table_field;

static const qui_table_field qtable_normal_fields[] =
{
	{"descriptor",9,},
	{"type",3,},
	{"value",7,},
};

#define QTABLE_NORMAL_FIELDS		(sizeof(qtable_normal_fields)/sizeof(qui_table_field))

static const qui_table_field qtable_readonly_fields[] =
{
	{"descriptor",9,},
	{"value",10,},
};

#define QTABLE_READONLY_FIELDS		(sizeof(qtable_readonly_fields)/sizeof(qui_table_field))

class QTV_DECL qtvUITableBase : public QTableWidget
{
	Q_OBJECT
public:
	explicit qtvUITableBase(QWidget* parent = nullptr);
	virtual ~qtvUITableBase();

	virtual qtvPropertyCollection* _properties() { return m_properties; }
	virtual void setProperties(qui_table_mode mode,qtvPropertyCollection* properties);

	static const char* getPropertyTypeName(qtvProperty* prop);
 protected:
	qtvPropertyCollection* m_properties;
	qtvUIMain* m_main;
	qui_table_mode m_mode;
	QSize m_size;
	void insertRowNormal(int32_t idx, qtvProperty* prop);
	void insertRowReadOnly(int32_t idx, qtvProperty* prop);
	void insertRowEmpty(int32_t idx);

	void resizeColumn();

	void attachProperties(qtvPropertyCollection* properties);
	void detachProperties();

	/* QTableWidget */
	void resizeEvent(QResizeEvent* event) override;

protected Q_SLOTS:
	void refreshProperties();
};

class QTV_DECL qtvUITableDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit qtvUITableDelegate(qtvUITableBase* table);
	virtual ~qtvUITableDelegate();

	/* QStyledItemDelegate */
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

protected:
	qtvUITableBase* m_table;

	static qtvProperty* getProperty(const qtvUITableDelegate* editor,const QModelIndex& index);
};

#endif
