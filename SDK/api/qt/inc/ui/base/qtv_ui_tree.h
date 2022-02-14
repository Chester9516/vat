#ifndef QTV_UI_TREE_H
#define QTV_UI_TREE_H

#include <QTreeWidget>
#include <core/qtv_mux.h>
#include <ui/base/qtv_ui_core.h>

typedef enum _qui_tree_mode
{
	qui_tree_normal,
	qui_tree_enumcontext,
}qui_tree_mode;

class QTV_DECL qtvUITreeBase : public QTreeWidget
{
	Q_OBJECT
public:
	explicit qtvUITreeBase(qui_tree_mode mode = qui_tree_normal,QWidget* parent = nullptr);
	virtual ~qtvUITreeBase();
protected:
	qui_tree_mode m_mode;

	void resetTree(qui_tree_mode mode);
	void add_channel(qtvMuxChannel* channel);
	void add_program(qtvMuxProgram* program, QTreeWidgetItem* chnode);

	static Qt::ItemFlags getItemFlags(qui_tree_mode mode);
};

class QTV_DECL qtvUITreeEnumContext : public qtvUITreeBase
{
	Q_OBJECT
public:
	explicit qtvUITreeEnumContext(QWidget* parent = nullptr);
	virtual ~qtvUITreeEnumContext();

	virtual void setEnumContext(qtvMuxChannel* channel);
	virtual bool checkEnumContextValid();
	virtual vatek_result getMuxHandle(mux_spec_mode spec,mux_country_code country,qtvMuxServiceChannel** muxhandle);

Q_SIGNALS:
	void notifyValidChanged(bool isvalid);

protected:
	qtvMuxChannel* m_channel;
	bool m_bvalid;

private Q_SLOTS:
	void recvItemChanged(QTreeWidgetItem* item, int column);
};

class QTV_DECL qtvUITreeContext : public qtvUITreeBase
{
	Q_OBJECT
public:
	explicit qtvUITreeContext(QWidget* parent = nullptr);
	virtual ~qtvUITreeContext();

	virtual void setContext(qtvMuxChannel* channel);
	virtual qtvMuxItem* getSelectMuxItem();

protected:
	qtvMuxChannel* m_channel;
};

#endif
