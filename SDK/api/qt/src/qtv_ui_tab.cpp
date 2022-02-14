#include <ui/base/qtv_ui_tabs.h>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QTextOption>
#include <QPainter>

#define QTABAR_WIDTH			200
#define QTABAR_HEIGH			40

#define QTABAR_LOGO_OFFSET		2
#define QTABAR_LOGO_WIDTH		(QTABAR_WIDTH - (QTABAR_LOGO_OFFSET << 2))
#define QTABAR_LOGO_HEIGHT		(QTABAR_HEIGH)

class qtvTabsLeftButton : public QProxyStyle
{
public:
	QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const;
	void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
};

/* qtvUITabBase */

qtvUITabBase::qtvUITabBase(qtvUIMain* main,const char* name, const char* desc, QWidget* parent) :
	m_main(main),
	m_name(name),
	m_desc(desc),
	m_valid(true),
	QWidget(parent)
{
	
}

qtvUITabBase::qtvUITabBase(qtvUIMain* main, QString& name, QString& desc, QWidget* parent) :
	m_main(main),
	m_name(name),
	m_desc(desc),
	m_valid(true),
	QWidget(parent)
{

}

qtvUITabBase::~qtvUITabBase()
{

}

void qtvUITabBase::setValid(bool valid)
{
	m_valid = valid;
	emit notifyValidChanged(valid);
}

/* qtvUITabsBase */

qtvUITabsBase::qtvUITabsBase(qtv_ui_tabs_mode mode, QWidget* parent) :
	m_mode(mode),
	m_tabmain(NULL),
	m_curtab(NULL),
	QTabWidget(parent)
{
	if (m_mode == qui_tabs_mainleft)
	{
		QPalette newpalette = palette();
		const QImage* piximg = qtvUIResource::getImage(ICON_LOGOBIG);
		setTabPosition(QTabWidget::West);
		tabBar()->setStyle(new qtvTabsLeftButton());
		m_logo = piximg->scaled(QSize(QTABAR_LOGO_WIDTH, QTABAR_LOGO_HEIGHT), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		setStyleSheet("QTabWidget::tab-bar {top: 50px;}");
		setAutoFillBackground(true);
		newpalette.setColor(QPalette::Window, Qt::white);
		setPalette(newpalette);
	}
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(recvTabChanged(int)));
}

qtvUITabsBase::~qtvUITabsBase()
{
	disconnect(this, SIGNAL(currentChanged(int)), this, SLOT(recvTabChanged(int)));
	detachUIMainTab();
}

void qtvUITabsBase::attachUIMainTab(qtvUIMainTabBase* main)
{
	if (main)
	{
		m_tabmain = main;
		refreshTabs();
	}
}

void qtvUITabsBase::detachUIMainTab()
{
	if (m_tabmain)clear();
	m_tabmain = NULL;
}

void qtvUITabsBase::paintEvent(QPaintEvent* paint)
{
	if (m_mode == qui_tabs_mainleft)
	{
		QPainter* painter = new QPainter(this);
		painter->save();
		int32_t pth = rect().height() - QTABAR_LOGO_HEIGHT - (QTABAR_LOGO_OFFSET << 2);
		painter->drawImage(QPoint(QTABAR_LOGO_OFFSET, pth), m_logo);
		painter->restore();
		delete painter;
	}
}

void qtvUITabsBase::pushTab(qtvUITabBase* tab)
{
	insertTab(count(), tab, tab->_name());
}

void qtvUITabsBase::recvTabChanged(int index)
{
	if (m_curtab)
	{
		m_curtab->tabLeave();
		m_curtab = NULL;
	}

	if (index != -1)
	{
		qtvUITabBase* tabpage = qobject_cast<qtvUITabBase*>(widget(index));
		if (tabpage)
		{
			m_curtab = tabpage;
			m_curtab->tabEnter();
		}
	}
}

/* qtvTabsLeftButton */

QSize qtvTabsLeftButton::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
	QSize tabsize = QProxyStyle::sizeFromContents(type, option, size, widget);
	if (type == QStyle::CT_TabBarTab)
	{
		tabsize.transpose();
		tabsize.rwidth() = QTABAR_WIDTH;
		tabsize.rheight() = QTABAR_HEIGH;
	}
	return tabsize;
}

void qtvTabsLeftButton::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
	if (element == CE_TabBarTabLabel)
	{
		if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option))
		{
			QRect allRect = tab->rect;
			QTextOption option;

			painter->save();
			if (tab->state & QStyle::State_Selected)
			{
				painter->setPen(0x555555);
				painter->setBrush(QBrush(0x282828));
				painter->drawRect(allRect.adjusted(5, 5, 0, 0));
			}
			option.setAlignment(Qt::AlignCenter);
			if (tab->state & QStyle::State_Selected)
				painter->setPen(0xf8fcff);
			else painter->setPen(0x5d5d5d);
			painter->drawText(allRect, tab->text, option);
			painter->restore();
			return;
		}
	}

	if (element == CE_TabBarTab)
		QProxyStyle::drawControl(element, option, painter, widget);
}
