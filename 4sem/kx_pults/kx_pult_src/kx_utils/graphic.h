#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QComboBox>
#include <QMutex>
#include <QDebug>
#include <QGridLayout>
#include <QFileDialog>
#include <QTime>
#include <QTranslator>
#include <QGestureEvent>
#include <qmath.h>
#include <float.h>
#include "graphic_conf.h"
#include "clineedit.h"
#include "qpievaluator.h"
#if QT_VERSION >= 0x050100
#  include <QSensorGestureManager>
#endif

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class ELineEdit: public CLineEdit {
	Q_OBJECT
public:
	explicit ELineEdit(QWidget * parent = 0);
	//void setText(const QString & v) {if (!is_auto) is_reset = false; CLineEdit::setText(v); setDefaultText(v, is_reset); last_ret = complexd(v.toDouble(), 0.); emit valueChanged(last_ret.real());}
	void setValue(const int & v) {if (is_reset) setDefaultText(QString::number(v).toUpper(), is_reset); else setText(QString::number(v).toUpper()); last_ret = complexd(v, 0.); if (!is_auto) emit valueChanged(last_ret.real());}
	void setValue(const double & v) {if (is_reset) setDefaultText(QString::number(v).toUpper(), is_reset); else setText(QString::number(v).toUpper()); last_ret = complexd(v, 0.); if (!is_auto) emit valueChanged(last_ret.real());}
	double value() const {return last_ret.real();}
	bool isDefault() const {return !cw->isVisible();}
	bool is_reset, is_auto;
protected:
	void wheelEvent(QWheelEvent * e);
	QPIEvaluator evaluator;
	complexd last_ret;
private slots:
	void toDefaultClicked() {is_reset = true; calculate();}
	void calculate();
signals:
	void valueChanged(double value);
};


namespace Ui {
	class Graphic;
};


class Graphic: public QFrame
{
	Q_OBJECT
	Q_FLAGS(Buttons)
	Q_ENUMS(Alignment Graduation AxisType)

	Q_PROPERTY(QString caption READ caption WRITE setCaption)
	Q_PROPERTY(QString labelX READ labelX WRITE setLabelX)
	Q_PROPERTY(QString labelY READ labelY WRITE setLabelY)
	Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
	Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)

	Q_PROPERTY(int currentGraphic READ currentGraphic WRITE setCurrentGraphic)
	Q_PROPERTY(int graphicsCount READ graphicsCount WRITE setGraphicsCount)
	Q_PROPERTY(QString graphicName READ graphicName WRITE setGraphicName)
	Q_PROPERTY(QPen graphicPen READ graphicPen WRITE setGraphicPen)
	Q_PROPERTY(QColor graphicColor READ graphicColor WRITE setGraphicColor)
	Q_PROPERTY(Qt::PenStyle graphicStyle READ graphicStyle WRITE setGraphicStyle)
	Q_PROPERTY(double graphicLineWidth READ graphicLineWidth WRITE setGraphicLineWidth)
	Q_PROPERTY(double graphicPointWidth READ graphicPointWidth WRITE setGraphicPointWidth)
	Q_PROPERTY(QColor graphicFillColor READ graphicFillColor WRITE setGraphicFillColor)
	Q_PROPERTY(bool graphicLinesEnabled READ graphicLinesEnabled WRITE setGraphicLinesEnabled)
	Q_PROPERTY(bool graphicPointsEnabled READ graphicPointsEnabled WRITE setGraphicPointsEnabled)
	Q_PROPERTY(bool graphicFillEnabled READ graphicFillEnabled WRITE setGraphicFillEnabled)

	Q_PROPERTY(bool gridEnabled READ gridEnabled WRITE setGridEnabled)
	Q_PROPERTY(QPen gridPen READ gridPen WRITE setGridPen)
	Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor)
	Q_PROPERTY(Qt::PenStyle gridStyle READ gridStyle WRITE setGridStyle)

	Q_PROPERTY(QPen selectionPen READ selectionPen WRITE setSelectionPen)
	Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor)
	Q_PROPERTY(Qt::PenStyle selectionStyle READ selectionStyle WRITE setSelectionStyle)
	Q_PROPERTY(QBrush selectionBrush READ selectionBrush WRITE setSelectionBrush)

	Q_PROPERTY(Alignment buttonsPosition READ buttonsPosition WRITE setButtonsPosition)
	Q_PROPERTY(Buttons buttons READ buttons WRITE setButtons)
	Q_PROPERTY(bool navigationEnabled READ navigationEnabled WRITE setNavigationEnabled)
	Q_PROPERTY(bool openGL READ openGL WRITE setOpenGL)
	Q_PROPERTY(bool antialiasing READ antialiasing WRITE setAntialiasing)
	Q_PROPERTY(bool autoUpdate READ autoUpdate WRITE setAutoUpdate)
	Q_PROPERTY(bool borderInputsVisible READ borderInputsVisible WRITE setBorderInputsVisible)
	Q_PROPERTY(bool statusVisible READ statusVisible WRITE setStatusVisible)
	Q_PROPERTY(bool legendVisible READ legendVisible WRITE setLegendVisible)
	Q_PROPERTY(bool paused READ paused WRITE setPaused)
	Q_PROPERTY(bool onlyExpandY READ onlyExpandY WRITE setOnlyExpandY)
	Q_PROPERTY(bool onlyExpandX READ onlyExpandX WRITE setOnlyExpandX)
	Q_PROPERTY(double historySize READ historySize WRITE setHistorySize)
	Q_PROPERTY(double maxVisibleTime READ maxVisibleTime WRITE setMaxVisibleTime)
	Q_PROPERTY(double autoXIncrement READ autoXIncrement WRITE setAutoXIncrement)
	Q_PROPERTY(QRectF limit READ limit WRITE setLimit)
	Q_PROPERTY(QRect margins READ margins WRITE setMargins)
	Q_PROPERTY(QRectF visualRect READ visualRect WRITE setVisualRect)
	Q_PROPERTY(QRectF defaultRect READ defaultRect WRITE setDefaultRect)
	Q_PROPERTY(int minimumRepaintInterval READ minimumRepaintInterval WRITE setMinimumRepaintInterval)
	
	Q_PROPERTY(double gridNumbersMultiplierX READ gridNumbersMultiplierX WRITE setGridNumbersMultiplierX)
	Q_PROPERTY(double gridNumbersMultiplierY READ gridNumbersMultiplierY WRITE setGridNumbersMultiplierY)
	Q_PROPERTY(Graduation graduationX READ graduationX WRITE setGraduationX)
	Q_PROPERTY(Graduation graduationY READ graduationY WRITE setGraduationY)
	Q_PROPERTY(double graduationStepX READ graduationStepX WRITE setGraduationStepX)
	Q_PROPERTY(double graduationStepY READ graduationStepY WRITE setGraduationStepY)
	Q_PROPERTY(AxisType axisType READ axisType WRITE setAxisType)

	Q_PROPERTY(int histogramMinIntervals READ histogramMinIntervals WRITE setHistogramMinIntervals)
	Q_PROPERTY(int histogramMaxIntervals READ histogramMaxIntervals WRITE setHistogramMaxIntervals)
	Q_PROPERTY(double histogramMinDeltaMultiplier READ histogramMinDeltaMultiplier WRITE setHistogramMinDeltaMultiplier)
	
	Q_PROPERTY(Graphic::GraphicsData graphicsData READ graphicsData WRITE setGraphicsData)

public:
	Graphic(QWidget * parent = 0);
	~Graphic();

	typedef QVector<QVector<QPointF> > GraphicsData;
	enum GraphicAction {gaNone, gaZoomInRect, gaZoomRangeX, gaZoomRangeY, gaMove};
	enum Button {NoButtons = 0x0,
				AllButtons = 0xFFFFFFFF,
				Autofit = 0x01,
				Grid = 0x02,
				CursorAxis = 0x04,
				OnlyExpandY = 0x08,
				OnlyExpandX = 0x10,
				Fullscreen = 0x20,
				BorderInputs = 0x40,
				Legend = 0x80,
				Configure = 0x100,
				Save = 0x200,
				Clear = 0x800,
				Close = 0x1000,
				Pause = 0x2000
				};
	enum Alignment {Left, Right};
	enum Graduation {Auto, Fixed};
	enum AxisType {Numeric, DateTime};
	Q_DECLARE_FLAGS(Buttons, Button)

	QString caption() const;
	QString labelX() const {return label_x;}
	QString labelY() const {return label_y;}
	QString graphicName() const {return graphics[curGraphic].name;}
	QColor backgroundColor() const {return back_color;}
	QColor textColor() const {return text_color;}
	QColor graphicColor() const {return graphics[curGraphic].pen.color();}
	QColor gridColor() const {return grid_pen.color();}
	QColor selectionColor() const {return selpen.color();}
	Qt::PenStyle graphicStyle() const {return graphics[curGraphic].pen.style();}
	Qt::PenStyle gridStyle() const {return grid_pen.style();}
	Qt::PenStyle selectionStyle() const {return selpen.style();}
	double graphicLineWidth() const {return graphics[curGraphic].pen.widthF();}
	double graphicPointWidth() const {return graphics[curGraphic].pointWidth;}
	QColor graphicFillColor() const {return graphics[curGraphic].fill_color;}
	bool graphicVisible() const {return graphics[curGraphic].visible;}
	bool graphicLinesEnabled() const {return graphics[curGraphic].lines;}
	bool graphicPointsEnabled() const {return graphics[curGraphic].points;}
	bool graphicFillEnabled() const {return graphics[curGraphic].fill;}
	QPen graphicPen() const {return graphics[curGraphic].pen;}
	QPen gridPen() const {return grid_pen;}
	QPen selectionPen() const {return selpen;}
	QBrush selectionBrush() const {return selbrush;}
	bool navigationEnabled() const {return navigation;}
	bool openGL() const {return isOGL;}
	bool antialiasing() const {return aalias;}
	bool autoUpdate() const {return aupdate;}
	bool gridEnabled() const {return grid;}
	bool borderInputsVisible() const;
	bool statusVisible() const;
	bool legendVisible() const;
	bool paused() const {return pause_;}
	bool onlyExpandY() const {return only_expand_y;}
	bool onlyExpandX() const {return only_expand_x;}
	int currentGraphic() const {return curGraphic;}
	int graphicsCount() const {return graphics.size();}
	Graphic::Buttons buttons() const {return buttons_;}
	Graphic::Alignment buttonsPosition() const {return align;}
	double historySize() const {return history;}
	double maxVisibleTime() const {return visible_time;}
	double autoXIncrement() const {return inc_x;}
	QRectF visualRect() const {return selrect;}
	QRectF defaultRect() const {return def_rect;}
	QRectF limit() const {return limit_;}
	QRect margins() const {return margins_;}
	int minimumRepaintInterval() const {return min_repaint_int;}
	int histogramMinIntervals() const {return min_int;}
	int histogramMaxIntervals() const {return max_int;}
	double histogramMinDeltaMultiplier() const {return mdm;}
	double gridNumbersMultiplierX() const {return grid_numbers_x;}
	double gridNumbersMultiplierY() const {return grid_numbers_y;}
	Graduation graduationX() const {return grad_x;}
	Graduation graduationY() const {return grad_y;}
	double graduationStepX() const {return gridx;}
	double graduationStepY() const {return gridy;}
	AxisType axisType() const {return axis_type_x;}
	QVector<QPointF> graphicData(const int index = 0) const {return graphics[index].polyline;}
	GraphicsData graphicsData() const;
	QWidget * viewport() const {return canvas;}
	QByteArray save();
	void load(QByteArray ba);
	void lock() {mutex_.lock();}
	void unlock() {mutex_.unlock();}

	void reset() {mutex.lock(); clear(); mutex.unlock();}
	
	GraphicType graphic(int arg) {if (arg < 0 || arg >= graphics.size()) return GraphicType(); return graphics[arg];}
	const QVector<GraphicType> & allGraphics() const {return graphics;}
	void setAllGraphics(const QVector<GraphicType> & g, bool update = true) {graphics = g; if (update) updateLegend();}
	void setHistogramData(const QVector<float> & g, int graphic);
	void setHistogramData(const QVector<float> & g) {setHistogramData(g, curGraphic);}
	
	double canvas2realX(double px) const;
	double canvas2realY(double py) const;
	double real2canvasX(double px) const;
	double real2canvasY(double py) const;
	QPointF canvas2real(QPointF canvas_point) const {return QPointF(canvas2realX(canvas_point.x()), canvas2realY(canvas_point.y()));}
	QPointF real2canvas(QPointF real_point) const {return QPointF(real2canvasX(real_point.x()), real2canvasY(real_point.y()));}
	double getScaleX() const {return real2canvasX(1.) - real2canvasX(0.);}
	double getScaleY() const {return real2canvasY(1.) - real2canvasY(0.);}
	QPointF getScale() const {return QPointF(getScaleX(), getScaleY());}

public slots:
	void setCaption(const QString & str);
	void setLabelX(const QString & str) {label_x = str; hasLblX = (str.length() > 0); if (aupdate) update();}
	void setLabelY(const QString & str) {label_y = str; hasLblY = (str.length() > 0); if (aupdate) update();}
	void setGraphicName(const QString & str, int index) {graphics[index].name = str; updateLegend(); if (aupdate) update();}
	void setGraphicName(const QString & str) {graphics[curGraphic].name = str; updateLegend(); if (aupdate) update();}
	void setBackgroundColor(const QColor & color) {back_color = color; if (aupdate) update(); updateLegend();}
	void setTextColor(const QColor & color) {text_color = color; if (aupdate) update();}
	void setGraphicColor(const QColor & color, int index) {graphics[index].pen.setColor(color); updateLegend(); if (aupdate) update();}
	void setGraphicColor(const QColor & color) {setGraphicColor(color, curGraphic);}
	void setGridColor(const QColor & color) {grid_pen.setColor(color); if (aupdate) update();}
	void setSelectionColor(const QColor & color) {selpen.setColor(color);}
	void setGraphicStyle(const Qt::PenStyle & style) {graphics[curGraphic].pen.setStyle(style); updateLegend(); if (aupdate) update();}
	void setGridStyle(const Qt::PenStyle & style) {grid_pen.setStyle(style); if (aupdate) update();}
	void setSelectionStyle(const Qt::PenStyle & style) {selpen.setStyle(style);}
	void setGraphicVisible(bool visible, int index) {graphics[index].visible = visible; updateLegend(); if (aupdate) update();}
	void setGraphicVisible(bool visible) {setGraphicVisible(visible, curGraphic);}
	void setGraphicLineWidth(double w, int index) {if (qRound(w) == w) graphics[index].pen.setWidth(qRound(w)); else graphics[index].pen.setWidthF(w); updateLegend(); if (aupdate) update();}
	void setGraphicLineWidth(double w) {setGraphicLineWidth(w, curGraphic);}
	void setGraphicPointWidth(double w, int index) {graphics[index].pointWidth = w; updateLegend(); if (aupdate) update();}
	void setGraphicPointWidth(double w) {setGraphicPointWidth(w, curGraphic);}
	void setGraphicFillColor(const QColor & w, int index) {graphics[index].fill_color = w; updateLegend(); if (aupdate) update();}
	void setGraphicFillColor(const QColor & w) {setGraphicFillColor(w, curGraphic);}
	void setGraphicLinesEnabled(bool w, int index) {graphics[index].lines = w; updateLegend(); if (aupdate) update();}
	void setGraphicLinesEnabled(bool w) {setGraphicLinesEnabled(w, curGraphic);}
	void setGraphicPointsEnabled(bool w, int index) {graphics[index].points = w; updateLegend(); if (aupdate) update();}
	void setGraphicPointsEnabled(bool w) {setGraphicPointsEnabled(w, curGraphic);}
	void setGraphicFillEnabled(bool w, int index) {graphics[index].fill = w; updateLegend(); if (aupdate) update();}
	void setGraphicFillEnabled(bool w) {setGraphicFillEnabled(w, curGraphic);}
	void setGraphicPen(const QPen & pen, int index) {graphics[index].pen = pen; updateLegend(); if (aupdate) update();}
	void setGraphicPen(const QPen & pen) {setGraphicPen(pen, curGraphic);}
	void setGridPen(const QPen & pen) {grid_pen = pen; if (aupdate) update();}
	void setSelectionPen(const QPen & pen) {selpen = pen;}
	void setSelectionBrush(const QBrush & brush) {selbrush = brush;}
	void setNavigationEnabled(bool on) {navigation = on;}
	void setOpenGL(bool on);
	void setAntialiasing(bool enabled);
	void setAutoUpdate(bool enabled) {aupdate = enabled;}
	void setGridEnabled(bool enabled);
	void setBorderInputsVisible(bool visible);
	void setStatusVisible(bool visible);
	void setLegendVisible(bool visible);
	void setPaused(bool yes);
	void setButtons(Graphic::Buttons b);
	void setButtonsPosition(Graphic::Alignment a);
	void setHistorySize(double val);
	void setMaxVisibleTime(double val) {visible_time = val;}
	void setAutoXIncrement(double val) {inc_x = val;}
	void setLimit(const QRectF & val) {limit_ = val;}
	void setMargins(const QRect & val) {margins_ = val; update();}
	void setMargins(int left_, int right_, int top_, int bottom_) {setMargins(QRect(left_, bottom_, right_, top_));}
	void setLeftMargin(int value) {margins_.moveLeft(value); setMargins(margins_);}
	void setRightMargin(int value) {margins_.setWidth(value); setMargins(margins_);}
	void setTopMargin(int value) {margins_.setHeight(value); setMargins(margins_);}
	void setBottomMargin(int value) {margins_.moveTop(value); setMargins(margins_);}
	void setMinimumRepaintInterval(const int & val) {min_repaint_int = val;}
	void setOnlyExpandY(bool yes);
	void setOnlyExpandX(bool yes);
	void setHistogramMinIntervals(int value) {min_int = value; updateGraphics();}
	void setHistogramMaxIntervals(int value) {max_int = value; updateGraphics();}
	void setHistogramMinDeltaMultiplier(double value) {mdm = value; updateGraphics();}
	void setGraphicsData(const GraphicsData & gd);
	
	void setGridNumbersMultiplierX(double value) {grid_numbers_x = value; updateGraphics();}
	void setGridNumbersMultiplierY(double value) {grid_numbers_y = value; updateGraphics();}
	void setGraduationX(Graduation value) {grad_x = value; if (aupdate) update();;}
	void setGraduationY(Graduation value) {grad_y = value; if (aupdate) update();;}
	void setGraduationStepX(double sx) {gridx = sx; if (aupdate) update();}
	void setGraduationStepY(double sy) {gridy = sy; if (aupdate) update();}
	void setGraduationSteps(double sx, double sy) {gridx = sx; gridy = sy; if (aupdate) update();}
	void setAxisType(AxisType t) {axis_type_x = t; if (aupdate) update();}

	void addPoint(const QPointF & p, int graphic, bool update_ = true);
	void addPoint(const QPointF & p, bool update = true) {addPoint(p, curGraphic, update);}
	void addPoint(double x, double y, int graphic, bool update = true) {addPoint(QPointF(x, y), graphic, update);}
	void addPoint(double x, double y, bool update = true) {addPoint(QPointF(x, y), update);}
	void addPoint(double y, int graphic, bool update = true) {addPoint(QPointF(graphics.at(graphic).max_x + inc_x, y), graphic, update);}
	void addPoint(double y, bool update = true) {addPoint(QPointF(graphics[curGraphic].max_x + inc_x, y), update);}
	void setGraphicData(const QVector<QPointF> & g, int graphic, bool update_ = true);
	void setGraphicData(const QVector<QPointF> & g) {setGraphicData(g, curGraphic);}
	void setGraphicProperties(const QString & name, const QColor & color = Qt::darkRed, Qt::PenStyle style = Qt::SolidLine, double width = 0., bool visible = true) {setGraphicProperties(curGraphic, name, color, style, width, visible);}
	void setGraphicProperties(int graphic, const QString & name, const QColor & color = Qt::darkRed, Qt::PenStyle style = Qt::SolidLine, double width = 0., bool visible = true);
	void addGraphic(const QString & name, const QColor & color = Qt::darkRed, Qt::PenStyle style = Qt::SolidLine, double width = 0., bool visible = true);
	void addGraphic(const GraphicType & gd, bool update = true) {graphics << gd; if (update) updateLegend();}
	void setVisualRect(const QRectF & rect);
	void setDefaultRect(const QRectF & rect);
	void autofit() {on_buttonAutofit_clicked();}
	void saveImage();
	void clear();
	void update(bool force = false);
	void updateGraphics() {findGraphicsRect(); update();}
	void setCurrentGraphic(int arg) {if (arg < 0 || arg >= graphics.size()) return; curGraphic = arg;}
	void setGraphicsCount(int arg, bool update = true);
	
	void zoom(float factor);
	void zoomIn() {zoom(1. / 1.2);}
	void zoomOut() {zoom(1.2);}
	void fullscreen();

protected:
	virtual void changeEvent(QEvent * e);
	virtual void resizeEvent(QResizeEvent * ) {if (leg_update) updateLegend();}
	virtual QSize sizeHint() const {return QSize(400, 300);}
	virtual void timerEvent(QTimerEvent * );
	virtual bool eventFilter(QObject * o, QEvent * e);

	void procGesture(QGesture * g);
	void setCurrentAction(GraphicAction action);
	void findGraphicsRect(double start_x = 0., double end_x = 0., double start_y = 0., double end_y = 0.);
	void tick(int index, bool slide = true, bool update = true);
	void repaintCanvas(bool force = false) {if (tm.elapsed() < min_repaint_int && !force) return; tm.restart(); canvas->update();}
	void drawGraphics();
	void drawGrid();
	void drawGuides();
	void drawPause();
	void drawAction();
	void updateLegend();
	void setCanvasCursor(QCursor cursor);
	void swapToBuffer();
	void swapToNormal() {bufferActive = false;}
	void setRectToLines();
	void checkLines();
	double splitRange(double range, int count = 1);
	double splitRangeDate(double range, int count = 1, QString * format = 0, int step[7] = 0);
	double roundTo(double value, double round_to);
	void roundDateTime(QDateTime & dt, int c[7]);
	void addDateTime(QDateTime & dt, int c[7], int mul = 1);
	QPointF absPoint(QPointF point) {return QPointF(qAbs(point.x()), qAbs(point.y()));}
	QString pointCoords(QPointF point);
	QPair<QString, QString> gridMark(double v) const;

	Ui::Graphic * ui;
	QMutex mutex, mutex_;
	QWidget * canvas;
	QImage * buffer;
	QPainter * painter;
	QBrush selbrush;
	QPen grid_pen, selpen;
	QColor back_color, text_color;
	QVector<GraphicType> graphics;
	int curGraphic;
	GraphicAction curaction, prevaction;
	QRectF grect, rrect, selrect, limit_, def_rect;
	QRect margins_;
	QSize font_sz;
	QPoint startpos, curpos, prevpos, gridborder;
	QString label_x, label_y, ppath;
	Graphic::Buttons buttons_;
	Graphic::Alignment align;
	GraphicConf * conf;
	ELineEdit line_x_min, line_x_max, line_y_min, line_y_max;
	QTime tm;
	QIcon icon_exp_x, icon_exp_y, icon_exp_sx, icon_exp_sy;
	QImage icon_pause_b, icon_pause_f;
	Graduation grad_x, grad_y;
	AxisType axis_type_x;
	double gridx, gridy, history, visible_time, inc_x, mdm, grid_numbers_x, grid_numbers_y, LN2, LN5, LN10;
	double eminx, eminy, emaxx, emaxy, pause_phase;
	int legy, lastw, lasth, min_repaint_int, min_int, max_int, timer_pause, thick;
	bool aalias, aupdate, mupdate, grid, guides, isFit, isEmpty, isOGL, isHover, bufferActive, cancel, pause_, isPrinting;
	bool hasLblX, hasLblY, navigation, only_expand_y, only_expand_x, is_lines_update, leg_update, visible_update, fullscr;

protected slots:
	void canvasPaintEvent(QPaintEvent * );
	void canvasMouseMoveEvent(QMouseEvent * );
	void canvasMousePressEvent(QMouseEvent * );
	void canvasMouseReleaseEvent(QMouseEvent * );
	void canvasMouseDoubleClickEvent(QMouseEvent * );
	void canvasWheelEvent(QWheelEvent * );
	void canvasLeaveEvent(QEvent * );
	void canvasKeyPressEvent(QKeyEvent * );
	void graphicVisibleChange(bool checked);
	void graphicAllVisibleChange(bool checked);
	void lineXMinChanged(double value) {selrect.setLeft(value); checkLines();}
	void lineXMaxChanged(double value) {selrect.setRight(value); checkLines();}
	void lineYMinChanged(double value) {selrect.setBottom(value); checkLines();}
	void lineYMaxChanged(double value) {selrect.setTop(value); checkLines();}
	void on_buttonClose_clicked() {emit closeRequest(this);}
	void on_buttonClear_clicked() {reset(); emit cleared();}
	void on_buttonAutofit_clicked();
	void on_buttonConfigure_clicked();
	void on_buttonFullscreen_clicked() {fullscreen();}
	void on_buttonSave_clicked() {saveImage();}
	void on_checkGrid_toggled(bool checked) {grid = checked; update();}
	void on_checkGuides_toggled(bool checked);
	void on_checkExpandY_toggled(bool checked);
	void on_checkExpandX_toggled(bool checked);
	void on_checkBorderInputs_toggled(bool checked) {setBorderInputsVisible(checked);}
	void on_checkLegend_toggled(bool checked) {setLegendVisible(checked);}
	void on_checkPause_toggled(bool checked) {setPaused(checked);}
	void enterFullscreen();
	void leaveFullscreen();

signals:
	void graphicPaintEvent(QPainter * );
	void graphicMouseMoveEvent(QPointF point, int buttons);
	void graphicMousePressEvent(QPointF point, int buttons);
	void graphicMouseReleaseEvent(QPointF point, int buttons);
	void graphicWheelEvent(QPointF point, int delta);
	void closeRequest(QWidget * );
	void cleared();
	void visualRectChanged();

};

Q_DECLARE_METATYPE(Graphic::GraphicsData)

Q_DECLARE_OPERATORS_FOR_FLAGS(Graphic::Buttons)

//inline QDataStream & operator <<(QDataStream & s, const Graphic::GraphicsData & v) {s << v; return s;}
//inline QDataStream & operator >>(QDataStream & s, Graphic::GraphicsData & v) {s >> v; return s;}

class __GraphicRegistrator__ {
public:
	__GraphicRegistrator__() {
		qRegisterMetaType<Graphic::GraphicsData>("Graphic::GraphicsData");
		qRegisterMetaTypeStreamOperators<Graphic::GraphicsData>("Graphic::GraphicsData");
	}
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // GRAPHIC_H
