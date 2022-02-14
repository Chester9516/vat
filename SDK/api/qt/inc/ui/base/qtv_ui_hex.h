#ifndef QTV_UI_HEX_H
#define QTV_UI_HEX_H

#include <QAbstractScrollArea>
#include <QSize>
#include <QImage>

typedef enum _qtv_hex_view
{
	qhex_view_raw,
	qhex_view_raw_addr,
}qtv_hex_view;

class qtvUIHexWidget : public QAbstractScrollArea
{
	Q_OBJECT
public:
	explicit qtvUIHexWidget(QWidget* parent = nullptr);
	virtual ~qtvUIHexWidget();

	void setHexView(qtv_hex_view view, int32_t perlinelen);
	void setHexBuffer(uint8_t* pbuf, int32_t len);

protected:
	int32_t m_cWidth;
	int32_t m_cHeight;
	qtv_hex_view m_view;
	int32_t m_prebytes;
	QImage m_baseimage;
	uint8_t* m_ptrbuf;
	int32_t m_buflen;

	void redrawBase();
	void resizeEvent(QResizeEvent* event);

	void paintEvent(QPaintEvent* event);
	void keyPressEvent(QKeyEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);

};

#endif
