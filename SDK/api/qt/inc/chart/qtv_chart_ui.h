#ifndef QTV_CHART_UI_H
#define QTV_CHART_UI_H

#include <chart/qtv_chart_define.h>
#include <core/qtv_service.h>

#include <QGraphicsView>
#include <QResizeEvent>
#include <QPaintEvent>

#define QTVCHART_RANGE_OFFECT		4
#define QTVCHART_FONT_MAXSIZE		20

class QTV_DECL qtvChartPattern : public QObject
{
	Q_OBJECT
public:
	explicit qtvChartPattern(qtvChartAxisBase* x, qtvChartAxisBase* y);
	~qtvChartPattern();

	virtual void setSize(QSize& size) = 0;
	virtual qtvChartAxisBase* _x_axis() { return m_xaxis; }
	virtual qtvChartAxisBase* _y_axis() { return m_yaxis; }

Q_SIGNALS :
	void notifyViewUdpate();

protected:
	qtvChartAxisBase* m_xaxis;
	qtvChartAxisBase* m_yaxis;

	static void drawChartBase(QPainter& p, QRect& rtchart,qtvChartAxisBase* axisx,qtvChartAxisBase* axisy);
	static void setFontSizeH(QPainter& p, int32_t h, int32_t maxfont = QTVCHART_FONT_MAXSIZE);
};


class qtvChartUIBase;
class QTV_DECL qtvChartUIProvider : public QObject
{
	Q_OBJECT
public:
	explicit qtvChartUIProvider(const char* name, qtvChartPattern* pattern) : 
		m_name(name),m_pattern(pattern),m_uiattach(0) {}
	~qtvChartUIProvider() { delete m_pattern; }

	virtual QString& _name() { return m_name; }
	virtual qtvChartPattern* _pattern() { return m_pattern; }

	virtual void attachUI() { m_uiattach = 1; }
	virtual void detachUI() { m_uiattach = 0; }
	virtual void refreshSize(QSize& size) { m_pattern->setSize(size); }
	virtual void drawWidget(QPainter& p) = 0;

Q_SIGNALS : 
	void notifyDataUpdate();

protected:
	QString m_name;
	qtvChartPattern* m_pattern;
	int32_t m_uiattach;
};

class QTV_DECL qtvChartUIProviderService : public qtvChartUIProvider
{
	Q_OBJECT
public:

	virtual void connectChart(qtvServiceBase* service) = 0;
	virtual void disconnectChart() = 0;

	static qtvChartUIProviderService* createServiceProvider(qtvServiceBase* service);

protected:
	explicit qtvChartUIProviderService(const char* name, qtvChartPattern* pattern) :
		qtvChartUIProvider(name, pattern) {}
};

class QTV_DECL qtvChartUIBase : public QWidget
{
	Q_OBJECT
public:
	explicit qtvChartUIBase(QWidget* parent = nullptr);
	~qtvChartUIBase();

	void resizeEvent(QResizeEvent* event) override;
	void paintEvent(QPaintEvent* event);

	void attachProvider(qtvChartUIProvider* provider);
	void detachProvider();

protected:
	QSize m_size;
	qtvChartUIProvider* m_provider;

protected Q_SLOTS:
	void recvProviderUpdate();
};

#endif