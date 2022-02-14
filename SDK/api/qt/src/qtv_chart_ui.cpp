#include <chart/qtv_chart_ui.h>
#include <QPainterPath>
#include <QtMath>

/* qtvChartUIBase */

qtvChartUIBase::qtvChartUIBase(QWidget* parent) :
	m_provider(NULL)
{

}

qtvChartUIBase::~qtvChartUIBase()
{
	detachProvider();
}

void qtvChartUIBase::resizeEvent(QResizeEvent* event)
{
	if (event->size() != m_size)
	{
		m_size = event->size();
		if (m_provider)
			m_provider->refreshSize(m_size);
	}
}

void qtvChartUIBase::paintEvent(QPaintEvent* event)
{
	QPainter g(this);
	if (m_provider)m_provider->drawWidget(g);
		//g.drawImage(0, 0, m_provider->_pattern()->_chart_base());
	else g.fillRect(0, 0, m_size.width(), m_size.height(), Qt::black);
}

void qtvChartUIBase::attachProvider(qtvChartUIProvider* provider)
{
	if (m_provider)detachProvider();

	if (provider)
	{
		provider->refreshSize(m_size);
		connect(provider->_pattern(), SIGNAL(notifyViewUdpate()), this, SLOT(recvProviderUpdate()));
		provider->attachUI();
		m_provider = provider;
		QWidget::update();
	}
}

void qtvChartUIBase::detachProvider()
{
	if (m_provider)
	{
		disconnect(m_provider->_pattern(), SIGNAL(notifyViewUdpate()), this, SLOT(recvProviderUpdate()));
		m_provider->detachUI();
		m_provider = NULL;
		QWidget::update();
	}
}

void qtvChartUIBase::recvProviderUpdate()
{
	update();
}

/* qtvChartPattern */

qtvChartPattern::qtvChartPattern(qtvChartAxisBase* x, qtvChartAxisBase* y) :
	m_xaxis(x),
	m_yaxis(y)
{

}

qtvChartPattern::~qtvChartPattern()
{
	if (m_xaxis)delete m_xaxis;
	if (m_yaxis)delete m_yaxis;
}

void qtvChartPattern::drawChartBase(QPainter& p, QRect& rtchart, qtvChartAxisBase* axisx, qtvChartAxisBase* axisy)
{
	int32_t i = 0;
	for (i = 0; i < axisx->_range()->_step_length() + 1; i++)
	{
		qreal px = rtchart.left() + axisx->_range()->convertValue(i * axisx->_range()->_step());
		p.drawLine(QPointF(px, rtchart.top()), QPointF(px, rtchart.bottom()));
	}
	for (i = 0; i < axisy->_range()->_step_length() + 1; i++)
	{
		qreal py = rtchart.top() + axisy->_range()->convertValue(i * axisy->_range()->_step());
		p.drawLine(QPointF(rtchart.left(), py), QPointF(rtchart.right(), py));
	}
}

void qtvChartPattern::setFontSizeH(QPainter& p, int32_t h, int32_t maxfont)
{
	int32_t ftsize = 0;
	QFont newfont = p.font();
	newfont.setPixelSize(maxfont);
	ftsize = newfont.pixelSize();

	while (ftsize)
	{
		QFontMetrics ftm(newfont);
		if (ftm.height() > h)
		{
			ftsize--;
			newfont.setPixelSize(ftsize);
		}
		else break;
	}
	p.setFont(newfont);
}
