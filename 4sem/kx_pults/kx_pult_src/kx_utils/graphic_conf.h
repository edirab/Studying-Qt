#ifndef GRAPHIC_CONF_H
#define GRAPHIC_CONF_H

#include <QDialog>
#include <QCheckBox>
#include <QPen>
#include <QPainter>


namespace Ui {
	class GraphicConf;
};


struct GraphicType {
	GraphicType(QString name_ = "y(x)", QColor color = Qt::red, Qt::PenStyle style = Qt::SolidLine, double width = 0., bool visible_ = true) {
		pen.setColor(color);
		pen.setStyle(style);
		lines = true;
		points = false;
		fill = false;
		fill_color = Qt::yellow;
		if (qRound(width) == width) pen.setWidth(qRound(width));
		else pen.setWidthF(width);
		pen.setWidth(1);
		pen.setCosmetic(true);
		max_x = 0.;
		name = name_;
		visible = visible_;
		pointWidth = 2.;
		pb = new QCheckBox(name);
	}
	//~GraphicType() {delete pb;}
	QString name;
	QPolygonF polyline;
	QPolygonF polyline_pause;
	QPen pen;
	QColor fill_color;
	bool lines;
	bool points;
	bool fill;
	double pointWidth;
	double max_x;
	double max_x_pause;
	QCheckBox * pb;
	QIcon icon;
	bool visible;
};


inline QDataStream & operator <<(QDataStream & s, const GraphicType & v) {s << v.name << v.pen << v.fill_color << v.lines << v.points << v.fill << v.pointWidth << v.visible; return s;}
inline QDataStream & operator >>(QDataStream & s, GraphicType & v) {s >> v.name >> v.pen >> v.fill_color >> v.lines >> v.points >> v.fill >> v.pointWidth >> v.visible; return s;}


class GraphicConf: public QDialog
{
	Q_OBJECT
	friend class Graphic;
public:
	explicit GraphicConf(QVector<GraphicType> & graphics_, QWidget * parent = 0);

	struct GraphicItem {
		QString name;
		QIcon icon;
	};

	void readParams();

	QVector<GraphicType> & graphics;
	QVector<GraphicItem> graphicItems;

protected:
	void changeEvent(QEvent * e);
	
	Ui::GraphicConf * ui;

private slots:
	void on_cbGraphicNames_currentIndexChanged(int index);
	void on_colorGraphic_colorChanged(const QColor &);
	void on_colorFill_colorChanged(const QColor &);
	void on_comboStyleGraphic_currentIndexChanged(int index);
	void on_spinLineWidthGraphic_valueChanged(double value);
	void on_spinPointWidthGraphic_valueChanged(double value);
	void on_checkLines_toggled(bool on);
	void on_checkPoints_toggled(bool on);
	void on_checkFill_toggled(bool on);

};

#endif // GRAPHIC_CONF_H
