#include "graphic.h"
#include "ui_graphic.h"
#include "ui_graphic_conf.h"

#include <QDebug>

__GraphicRegistrator__ __graphic_registrator__;


ELineEdit::ELineEdit(QWidget * parent): CLineEdit(parent) {
	last_ret = complexd_0;
	is_auto = false;
	is_reset = true;
	connect(this, SIGNAL(editingFinished()), this, SLOT(calculate()));
	connect(this, SIGNAL(cleared()), this, SLOT(toDefaultClicked()));
}


void ELineEdit::wheelEvent(QWheelEvent * e) {
	double mul = 1.1;
	if (e->delta() < 0) mul = 0.9;
	CLineEdit::setText(QString::number(last_ret.real() * mul).toUpper());
	calculate();
}


void ELineEdit::calculate() {
	evaluator.check(text().replace(",", "."));
	if (!evaluator.isCorrect()) return;
	is_reset = false;
	last_ret = evaluator.evaluate();
	CLineEdit::setText(QString::number(last_ret.real()).toUpper());
	emit valueChanged(last_ret.real());
}



Graphic::Graphic(QWidget * parent): QFrame(parent), line_x_min(this), line_x_max(this), line_y_min(this), line_y_max(this) {
	QTranslator * trans = new QTranslator();
	trans->load(":/lang/qad_graphic_" + QLocale::system().name().left(2));
	if (trans->isEmpty())
		trans->load("lang/qad_graphic_" + QLocale::system().name().left(2));
	qApp->installTranslator(trans);
	leg_update = true;
	visible_update = fullscr = false;
	ui = new Ui::Graphic();
	ui->setupUi(this);
	/*line_x_min.resize(70, 22);
	line_x_max.resize(70, 22);
	line_y_min.resize(70, 22);
	line_y_max.resize(70, 22);*/
	line_x_min.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	line_x_max.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
//    qDebug() << "Layout addres for LY " << ui->widgetLY->layout();
//    qDebug() << "Layout addres for LX " << ui->widgetLX->layout();
    dynamic_cast<QBoxLayout*>(ui->widgetLY->layout())->insertWidget(0, &line_y_min);
    dynamic_cast<QBoxLayout*>(ui->widgetLY->layout())->addWidget(&line_y_max);
    dynamic_cast<QBoxLayout*>(ui->widgetLX->layout())->insertWidget(0, &line_x_min);
    dynamic_cast<QBoxLayout*>(ui->widgetLX->layout())->addWidget(&line_x_max);
 /*
	((QBoxLayout * )ui->widgetLY->layout())->insertWidget(0, &line_y_min);
	((QBoxLayout * )ui->widgetLY->layout())->addWidget(&line_y_max);
	((QBoxLayout * )ui->widgetLX->layout())->insertWidget(0, &line_x_min);
    ((QBoxLayout * )ui->widgetLX->layout())->addWidget(&line_x_max);*/
	tm.restart();
	grid_numbers_x = grid_numbers_y = 1;
	LN2 = qLn(2.);
	LN5 = qLn(5.);
	LN10 = qLn(10.);
	connect(&line_x_min, SIGNAL(valueChanged(double)), this, SLOT(lineXMinChanged(double)));
	connect(&line_x_max, SIGNAL(valueChanged(double)), this, SLOT(lineXMaxChanged(double)));
	connect(&line_y_min, SIGNAL(valueChanged(double)), this, SLOT(lineYMinChanged(double)));
	connect(&line_y_max, SIGNAL(valueChanged(double)), this, SLOT(lineYMaxChanged(double)));
	connect(ui->canvas_raster, SIGNAL(paintEvent(QPaintEvent * )), this, SLOT(canvasPaintEvent(QPaintEvent * )));
	connect(ui->canvas_raster, SIGNAL(mouseMoveEvent(QMouseEvent * )), this, SLOT(canvasMouseMoveEvent(QMouseEvent * )));
	connect(ui->canvas_raster, SIGNAL(mousePressEvent(QMouseEvent * )), this, SLOT(canvasMousePressEvent(QMouseEvent * )));
	connect(ui->canvas_raster, SIGNAL(mouseReleaseEvent(QMouseEvent * )), this, SLOT(canvasMouseReleaseEvent(QMouseEvent * )));
	connect(ui->canvas_raster, SIGNAL(mouseDoubleClickEvent(QMouseEvent*)), this, SLOT(canvasMouseDoubleClickEvent(QMouseEvent * )));
	connect(ui->canvas_raster, SIGNAL(wheelEvent(QWheelEvent * )), this, SLOT(canvasWheelEvent(QWheelEvent * )));
	connect(ui->canvas_raster, SIGNAL(leaveEvent(QEvent * )), this, SLOT(canvasLeaveEvent(QEvent * )));
	connect(ui->canvas_raster, SIGNAL(keyPressEvent(QKeyEvent * )), this, SLOT(canvasKeyPressEvent(QKeyEvent * )));
	connect(ui->canvas_gl, SIGNAL(paintEvent(QPaintEvent * )), this, SLOT(canvasPaintEvent(QPaintEvent * )));
	connect(ui->canvas_gl, SIGNAL(mouseMoveEvent(QMouseEvent * )), this, SLOT(canvasMouseMoveEvent(QMouseEvent * )));
	connect(ui->canvas_gl, SIGNAL(mousePressEvent(QMouseEvent * )), this, SLOT(canvasMousePressEvent(QMouseEvent * )));
	connect(ui->canvas_gl, SIGNAL(mouseReleaseEvent(QMouseEvent * )), this, SLOT(canvasMouseReleaseEvent(QMouseEvent * )));
	connect(ui->canvas_gl, SIGNAL(mouseDoubleClickEvent(QMouseEvent*)), this, SLOT(canvasMouseDoubleClickEvent(QMouseEvent * )));
	connect(ui->canvas_gl, SIGNAL(wheelEvent(QWheelEvent * )), this, SLOT(canvasWheelEvent(QWheelEvent * )));
	connect(ui->canvas_gl, SIGNAL(leaveEvent(QEvent * )), this, SLOT(canvasLeaveEvent(QEvent * )));
	connect(ui->canvas_gl, SIGNAL(keyPressEvent(QKeyEvent * )), this, SLOT(canvasKeyPressEvent(QKeyEvent * )));
	ui->canvas_raster->grabGesture(Qt::PinchGesture);
	ui->canvas_raster->grabGesture(Qt::PanGesture);
	ui->canvas_raster->installEventFilter(this);
	ui->canvas_gl->grabGesture(Qt::PinchGesture);
	ui->canvas_gl->grabGesture(Qt::PanGesture);
	ui->canvas_gl->installEventFilter(this);
	icon_exp_x  = QIcon(":/icons/expand_x.png");
	icon_exp_y  = QIcon(":/icons/expand_y.png");
	icon_exp_sx = QIcon(":/icons/expand_s_x.png");
	icon_exp_sy = QIcon(":/icons/expand_s_y.png");
	icon_pause_b = QImage(":/icons/pause-back.png");
	icon_pause_f = QImage(":/icons/pause-front.png");
	aupdate = grid = isFit = isEmpty = navigation = true;
	aalias = mupdate = bufferActive = isOGL = cancel = isPrinting = guides = hasLblX = hasLblY = isHover = false;
	pause_ = only_expand_x = only_expand_y = false;
	//qDebug() << -DBL_MAX/2. << DBL_MAX/2. << DBL_MIN;
	limit_.setCoords(-DBL_MAX, -DBL_MAX, DBL_MAX, DBL_MAX);
	eminx = eminy = DBL_MAX;
	emaxx = emaxy = DBL_MIN;
	grad_x = grad_y = Auto;
	axis_type_x = Numeric;
	min_repaint_int = 25;
	inc_x = 1.;
	legy = 0;
	buffer = 0;
	gridx = gridy = 1.;
	history = 5.;
	min_int = 1;
	max_int = 200;
	mdm = 10.;
	visible_time = -1.;
	pause_phase = 0.;
	selrect.setRect(0., 0., 1., 1.);
	def_rect = selrect;
	margins_.setRect(4, 4, 4, 4);
	curaction = gaMove;
	selbrush.setStyle(Qt::SolidPattern);
	selbrush.setColor(QColor(60, 175, 255, 100));
	text_color = Qt::black;
	grid_pen = QPen(Qt::gray, 0., Qt::DotLine);
	//graph_pen = QPen(Qt::red);
	//graph_pen.setCosmetic(true);
	graphics.append(GraphicType());
	curGraphic = 0;
	selpen = QPen(Qt::black);
	selpen.setStyle(Qt::DashLine);
	back_color = Qt::white;
	buttons_ = AllButtons;
	setButtonsPosition(Graphic::Left);
	setOpenGL(false);
	setAntialiasing(false);
	setCaption("");
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	ui->layoutButtons->update();
	updateLegend();
	setRectToLines();
	conf = new GraphicConf(graphics, this);
}


Graphic::~Graphic() {
	delete conf;
	if (buffer != 0) delete buffer;
}


void Graphic::changeEvent(QEvent * e) {
	if (e->type() == QEvent::LanguageChange) {
		ui->retranslateUi(this);
		return;
	}
	QFrame::changeEvent(e);
}


void Graphic::timerEvent(QTimerEvent * ) {
	pause_phase += 0.02;
	if (pause_phase > 1.)
		pause_phase -= 1.;
	update();
}


bool Graphic::eventFilter(QObject * o, QEvent * e) {
	//qDebug() << "event" << o << e;
	if (o == canvas) {
		switch (e->type()) {
		case QEvent::Gesture:
			 foreach (QGesture * g, ((QGestureEvent*)e)->gestures())
				procGesture(g);
			break;
		default: break;
		}
	}
	return QFrame::eventFilter(o, e);
}


void Graphic::procGesture(QGesture * g) {
	if (!g) return;
	qDebug() << g;
}


void Graphic::canvasPaintEvent(QPaintEvent * ) {
	if (is_lines_update) return;
	QMutexLocker ml(&mutex_);
	//static int pwid = 0, phei = 0;
	int wid = canvas->width(), hei = canvas->height();
	lastw = wid;
	lasth = hei;
	font_sz = fontMetrics().size(0, "0");
	font_sz.setHeight(font_sz.height() * 1.);
	font_sz.setWidth(font_sz.width() * 8);
	thick = qMax<int>(qRound(font_sz.height() / 15.), 1);
	if (buffer != 0) if (buffer->width() != wid || buffer->height() != hei) {delete buffer; buffer = 0;}
	if (buffer == 0) buffer = new QImage(wid, hei, QImage::Format_RGB32);
	if (bufferActive) {
		QPainter p(canvas);
		p.drawImage(0, 0, *buffer);
		painter = &p;
		if (curpos != startpos) drawAction();
		drawGuides();
		return;
	}
	//if (!aupdate && !mupdate && pwid == wid && phei == hei) return;
	/*if (pwid != wid || phei != hei) {
		line_x_min.move(0, hei - 35);
		line_x_max.move(0, 0);
		line_y_min.move(70, hei - line_x_min.height());
		line_y_max.move(wid - line_y_max.width(), hei - line_x_min.height());
	}
	line_x_min.setVisible(grid);
	line_x_max.setVisible(grid);
	line_y_min.setVisible(grid);
	line_y_max.setVisible(grid);*/
	//pwid = wid;
	//phei = hei;
	QPainter p;
	if (isOGL) {
		glClearColor(0.f, 0.f, 0.f, 0.f);
		p.begin(canvas);
	} else p.begin(buffer);
	p.fillRect(canvas->rect(), back_color);
	painter = &p;
	p.setFont(font());
	gridborder = QPoint(5, 5);
	if (grid) {
		gridborder += QPoint(font_sz.width(), font_sz.height());
		if (hasLblY) gridborder += QPoint(font_sz.height(), 0);
		if (hasLblX) gridborder += QPoint(0, font_sz.height());
		drawGrid();
	}
	p.setRenderHint(QPainter::Antialiasing, aalias);
	if (isOGL) {
		if (aalias) glEnable(GL_MULTISAMPLE);
		else glDisable(GL_MULTISAMPLE);
	}
	//p.setRenderHint(QPainter::HighQualityAntialiasing, aalias);
	if (!aalias) p.translate(-0.5, -0.5);
	drawGraphics();
	drawGuides();
	if (pause_) drawPause();
	emit graphicPaintEvent(&p);
	p.end();
	if (isOGL) return;
	p.begin(canvas);
	p.drawImage(0, 0, *buffer);
	p.end();
}


void Graphic::canvasMouseMoveEvent(QMouseEvent * e) {
	isHover = true;
	curpos = e->pos();
	QPointF rp = canvas2real(QPointF(e->pos())), srp = canvas2real(startpos), crp = canvas2real(curpos), dp;
	QString cursorstr = tr("Cursor: ") + pointCoords(rp);
	emit graphicMouseMoveEvent(rp, e->buttons());
	if (e->buttons() == Qt::NoButton) {
		ui->status->setText(cursorstr);
		if (guides) update();
		return;
	}
	if (!navigation) return;
	if (curaction != gaMove && (e->buttons() & Qt::RightButton) == Qt::RightButton) return;
	switch (curaction) {
	case gaZoomInRect:
		ui->status->setText(tr("Selection") + ": " + pointCoords(srp) + " -> " +
						pointCoords(crp) + ",  " + tr("Size") + ": " + pointCoords(absPoint(crp - srp)));
		repaintCanvas(true);
		break;
	case gaZoomRangeX:
		ui->status->setText(tr("Range") + ": " + QString::number(srp.x(), 'f', 3) +
						" -> " + QString::number(crp.x(), 'f', 3) + ",  " + tr("Length") + ": " +
						QString::number(qAbs(crp.x() - srp.x()), 'f', 3));
		repaintCanvas(true);
		break;
	case gaZoomRangeY:
		ui->status->setText(tr("Range") + ": " + QString::number(srp.y(), 'f', 3) +
						" -> " + QString::number(crp.y(), 'f', 3) + ",  " + tr("Length") + ": " +
						QString::number(qAbs(crp.y() - srp.y()), 'f', 3));
		repaintCanvas(true);
		break;
	case gaMove:
		dp = e->pos() - prevpos;
		dp.rx() *= selrect.width() / double(gridborder.x() + 5 - lastw);
		dp.ry() *= selrect.height() / double(lasth - legy - gridborder.y() - 5);
		selrect.translate(dp);
		isFit = false;
		emit visualRectChanged();
		update(true);
		setRectToLines();
		break;
	default: break;
	}
	prevpos = e->pos();
}


void Graphic::canvasMousePressEvent(QMouseEvent * e) {
	emit graphicMousePressEvent(canvas2real(QPointF(e->pos())), e->buttons());
	if (!navigation) return;
	ui->canvas_gl->setCursor(guides ? Qt::BlankCursor : Qt::ArrowCursor);
	ui->canvas_raster->setCursor(guides ? Qt::BlankCursor : Qt::ArrowCursor);
	prevpos = e->pos();
	startpos = prevpos;
	if (cancel) return;
	if (e->button() == Qt::MidButton) curaction = gaMove;
	if (e->button() == Qt::RightButton) {
		if (bufferActive) {
			curpos = startpos;
			repaintCanvas(true);
			swapToNormal();
			cancel = true;
			return;
		} else {
			prevaction = curaction;
			curaction = gaMove;
			return;
		}
	}
	if (e->button() == Qt::LeftButton) {
		if (e->modifiers() == Qt::ControlModifier) curaction = gaZoomRangeX;
		else if (e->modifiers() == Qt::ShiftModifier) curaction = gaZoomRangeY;
		else curaction = gaZoomInRect;
		switch (curaction) {
		case gaZoomInRect:
		case gaZoomRangeX:
		case gaZoomRangeY:
			swapToBuffer();
			break;
		default: break;
		}
	}
	setCurrentAction(curaction);
}


void Graphic::canvasMouseReleaseEvent(QMouseEvent * e) {
	emit graphicMouseReleaseEvent(canvas2real(QPointF(e->pos())), e->buttons());
	if (!navigation) return;
	ui->canvas_gl->setCursor(guides ? Qt::BlankCursor : Qt::ArrowCursor);
	ui->canvas_raster->setCursor(guides ? Qt::BlankCursor : Qt::ArrowCursor);
	QPointF tlp, brp;
	QRect sr;
	sr = QRect(startpos, curpos).normalized();
	if (cancel) {
		if (e->buttons() == Qt::NoButton) cancel = false;
		return;
	}
	if (e->button() == Qt::RightButton && curaction == gaMove) {
		curaction = prevaction;
		return;
	}
	if (e->button() == Qt::LeftButton && (e->buttons() & Qt::RightButton) != Qt::RightButton) {
		if (curpos != startpos) {
			tlp = canvas2real(sr.topLeft());
			brp = canvas2real(sr.bottomRight());
			isFit = false;
			switch (curaction) {
			case gaZoomInRect:
				if (sr.width() <= 1 || sr.height() <= 1) break;
				selrect.setCoords(tlp.x(), brp.y(), brp.x(), tlp.y());
				setRectToLines();
				break;
			case gaZoomRangeX:
				if (sr.width() <= 1) break;
				findGraphicsRect(tlp.x(), brp.x());
				break;
			case gaZoomRangeY:
				if (sr.height() <= 1) break;
				findGraphicsRect(0., 0., brp.y(), tlp.y());
				break;
			default: return;
			}
		}
		swapToNormal();
		update(true);
	}
	QPointF rp = canvas2real(QPointF(e->pos()));
	ui->status->setText(tr("Cursor") + ": " + pointCoords(rp));
	emit visualRectChanged();
}


void Graphic::canvasMouseDoubleClickEvent(QMouseEvent * ) {
	autofit();
}


void Graphic::canvasWheelEvent(QWheelEvent * e) {
	//if (curaction != gaMove) return;
	emit graphicWheelEvent(canvas2real(QPointF(e->pos())), e->delta());
	if (!navigation) return;
	double scl, wid = canvas->width() - gridborder.x() - margins_.width() - margins_.left(), hei = canvas->height() - gridborder.y() - margins_.height() - margins_.top();
	double px = e->pos().x() - gridborder.x() - margins_.left(), py = hei - e->pos().y() + margins_.height();
	px = px / wid * selrect.width() + selrect.x();
	py = py / hei * selrect.height() + selrect.y();
	if (e->delta() > 0) scl = 1. / 1.2;
	else scl = 1.2;
	if (e->modifiers() == Qt::NoModifier)
		selrect.setRect(px - (px - selrect.x()) * scl, py - (py - selrect.y()) * scl, selrect.width() * scl, selrect.height() * scl);
	else {
		if (e->modifiers() == Qt::ControlModifier)
			selrect.setRect(px - (px - selrect.x()) * scl, selrect.y(), selrect.width() * scl, selrect.height());
		if (e->modifiers() == Qt::ShiftModifier)
			selrect.setRect(selrect.x(), py - (py - selrect.y()) * scl, selrect.width(), selrect.height() * scl);
	}
	isFit = false;
	update(true);
	setRectToLines();
	emit visualRectChanged();
}


void Graphic::zoom(float factor) {
	double wid = canvas->width() - gridborder.x() - margins_.width() - margins_.left(), hei = canvas->height() - gridborder.y() - margins_.height() - margins_.top();
	double px = wid / 2, py = hei / 2;
	px = px / wid * selrect.width() + selrect.x();
	py = py / hei * selrect.height() + selrect.y();
	selrect.setRect(px - (px - selrect.x()) * factor, py - (py - selrect.y()) * factor, selrect.width() * factor, selrect.height() * factor);
	isFit = false;
	update(true);
	setRectToLines();
}


void Graphic::fullscreen() {
	if (fullscr) leaveFullscreen();
	else enterFullscreen();
}


void Graphic::canvasLeaveEvent(QEvent * ) {
	isHover = false;
	if (guides) update(true);
	ui->status->setText(tr("Cursor") + ": ( ; )");
	leaveFullscreen();
}


void Graphic::canvasKeyPressEvent(QKeyEvent * e) {
	switch (e->key()) {
	case Qt::Key_Escape: leaveFullscreen();
	default: break;
	};
}


void Graphic::clear() {
	//cout << "clear" << endl;
	for (int i = 0; i < graphics.size(); ++i) {
		graphics[i].polyline.clear();
		graphics[i].polyline_pause.clear();
		graphics[i].max_x = 0.;
	}
	on_buttonAutofit_clicked();
}


void Graphic::setAntialiasing(bool enabled) {
	if (aalias == enabled) return;
	aalias = enabled;
	/*QGLFormat f = ui->canvas_gl->format();
	f.setSampleBuffers(enabled);
	ui->canvas_gl->setFormat(f);*/
	update();
}


void Graphic::setPaused(bool yes) {
	pause_ = yes;
	ui->checkPause->blockSignals(true);
	ui->checkPause->setChecked(yes);
	ui->checkPause->blockSignals(false);
	if (!pause_) {
		killTimer(timer_pause);
		timer_pause = 0;
		update(true);
		return;
	}
	for (int i = 0; i < graphics.size(); ++i) {
		graphics[i].polyline_pause = graphics[i].polyline;
		graphics[i].polyline_pause.detach();
		graphics[i].max_x_pause = graphics[i].max_x;
	}
	timer_pause = startTimer(40);
}


void Graphic::setHistorySize(double val) {
	history = val;
	double x;
	for (int i = 0; i < graphics.size(); ++i) {
		QPolygonF & pol(graphics[i].polyline);
		if (pol.isEmpty() || history <= 0.) continue;
		x = pol.back().x() - history;
		for (int j = pol.size() - 2; j >= 0 ; --j)
			if (pol[j].x() < x) {
				//qDebug() << pol.size() << j;
				pol.erase(pol.begin(), pol.begin() + j);
				break;
			}
	}
}


void Graphic::setOnlyExpandY(bool yes) {
	only_expand_y = yes;
	ui->checkExpandY->blockSignals(true);
	ui->checkExpandY->setCheckable(yes);
	ui->checkExpandY->blockSignals(false);
}


void Graphic::setOnlyExpandX(bool yes) {
	only_expand_x = yes;
	ui->checkExpandX->blockSignals(true);
	ui->checkExpandX->setCheckable(yes);
	ui->checkExpandX->blockSignals(false);
}


Graphic::GraphicsData Graphic::graphicsData() const {
	GraphicsData ret;
	ret.resize(graphics.size());
	for (int i = 0; i < graphics.size(); ++i)
		ret[i] = graphics[i].polyline;
	return ret;
}


void Graphic::setGraphicsData(const Graphic::GraphicsData & gd) {
	setGraphicsCount(gd.size());
	for (int i = 0; i < gd.size(); ++i)
		setGraphicData(gd[i], i, false);
	updateGraphics();
}


void Graphic::setButtons(Graphic::Buttons b) {
	buttons_ = b;
	ui->buttonAutofit->setVisible(b.testFlag(Autofit));
	ui->checkGrid->setVisible(b.testFlag(Grid));
	ui->checkGuides->setVisible(b.testFlag(CursorAxis));
	ui->checkExpandY->setVisible(b.testFlag(OnlyExpandY));
	ui->checkExpandX->setVisible(b.testFlag(OnlyExpandX));
	ui->buttonFullscreen->setVisible(b.testFlag(Fullscreen));
	ui->checkBorderInputs->setVisible(b.testFlag(BorderInputs));
	ui->checkLegend->setVisible(b.testFlag(Legend));
	ui->buttonClear->setVisible(b.testFlag(Clear));
	ui->buttonConfigure->setVisible(b.testFlag(Configure));
	ui->buttonSave->setVisible(b.testFlag(Save));
	ui->buttonClose->setVisible(b.testFlag(Close));
	ui->checkPause->setVisible(b.testFlag(Pause));
	if (ui->buttonAutofit->isVisible() || ui->checkGrid->isVisible() || ui->checkGuides->isVisible() ||
		ui->buttonConfigure->isVisible() || ui->buttonSave->isVisible() || ui->checkPause->isVisible())
		ui->verticalSpacer->changeSize(0, 30, QSizePolicy::Preferred, QSizePolicy::Preferred);
	else
		ui->verticalSpacer->changeSize(0, 0, QSizePolicy::Preferred, QSizePolicy::Preferred);
	ui->layoutButtons->update();
}


void Graphic::setButtonsPosition(Graphic::Alignment a) {
	align = a;
	ui->widgetLeft->hide();
	ui->widgetRight->hide();
	switch (a) {
	case Graphic::Left:
		ui->widgetLeft->setLayout(ui->layoutButtons);
		ui->widgetLeft->show();
		break;
	case Graphic::Right:
		ui->widgetRight->setLayout(ui->layoutButtons);
		ui->widgetRight->show();
		break;
	}
}


void Graphic::addPoint(const QPointF & p, int graphic, bool update_) {
	if (graphic >= graphics.size() || graphic < 0) return;
	if (graphics.at(graphic).polyline.size() == 0) graphics[graphic].max_x = p.x();
	graphics[graphic].polyline << p;
	if (graphics.at(graphic).max_x < p.x()) graphics[graphic].max_x = p.x();
	tick(graphic, true, update_);
}


void Graphic::setGraphicData(const QVector<QPointF> & g, int graphic, bool update_) {
	if (graphic >= graphics.size() || graphic < 0) return;
	graphics[graphic].polyline.clear();
	graphics[graphic].polyline = g;
	if (graphics.at(graphic).polyline.size() == 0) {
		graphics[graphic].max_x = 0.;
		tick(graphic, false, update_);
		return;
	}
	graphics[graphic].max_x = graphics.at(graphic).polyline[0].x();
	for (int i = 1; i < graphics.at(graphic).polyline.size(); ++i)
		if (graphics.at(graphic).max_x < graphics.at(graphic).polyline[i].x())
			graphics[graphic].max_x = graphics.at(graphic).polyline[i].x();
	tick(graphic, false, update_);
}


void Graphic::setGraphicProperties(int graphic, const QString & name, const QColor& color, Qt::PenStyle style, double width, bool visible) {
	if (graphic < 0 || graphic >= graphics.size()) return;
	graphics[graphic].name = name;
	graphics[graphic].pen.setColor(color);
	graphics[graphic].pen.setStyle(style);
	graphics[graphic].pen.setWidth(width);
	graphics[graphic].visible = visible;
	updateLegend();
}


void Graphic::addGraphic(const QString & name, const QColor & color, Qt::PenStyle style, double width, bool visible) {
	graphics << GraphicType(name, color, style, width, visible);
	updateLegend();
}


void Graphic::setVisualRect(const QRectF & rect) {
	selrect = rect;
	isFit = false;
	update();
}


void Graphic::setDefaultRect(const QRectF & rect) {
	def_rect = rect;
	if (isFit) autofit();
}


void Graphic::saveImage() {
	QString str = QFileDialog::getSaveFileName(this, tr("Save Image"), ppath, "PNG(*.png);;JPEG(*.jpg *.jpeg);;BMP(*.bmp);;TIFF(*.tiff *.tif);;PPM(*.ppm)");
	if (str == "") return;
	ppath = str;
	QPixmap im(canvas->size());
	mupdate = true;
	canvas->render(&im);
	mupdate = false;
	im.save(ppath);
	update(true);
}


void Graphic::setOpenGL(bool on) {
	isOGL = on;
	if (on) {
		ui->canvas_raster->hide();
		ui->canvas_gl->show();
		canvas = ui->canvas_gl;
	} else {
		ui->canvas_gl->hide();
		ui->canvas_raster->show();
		canvas = ui->canvas_raster;
	}
	/*line_x_min.setParent(canvas);
	line_x_max.setParent(canvas);
	line_y_min.setParent(canvas);
	line_y_max.setParent(canvas);
	line_x_min.show();
	line_x_max.show();
	line_y_min.show();
	line_y_max.show();*/
	update();
}


void Graphic::update(bool force) {
	mupdate = true;
	repaintCanvas(force);
	mupdate = false;
}


void Graphic::setGraphicsCount(int arg, bool update) {
	if (arg < 0) return;
	while (graphics.size() < arg)
		graphics.append(GraphicType(tr("y(x)"), QColor::fromHsv((graphics.size() * 55) % 360, 255, 255 - qrand() % 115)));
	while (graphics.size() > arg) {
		delete graphics.back().pb;
		graphics.pop_back();
	}
	if (update) updateLegend();
}


void Graphic::setHistogramData(const QVector<float> & g, int graphic) {
    graphics[graphic].polyline.clear();
    if (g.isEmpty()) {
		return;
	}
	QVector<float> data = g;
	QVector<int> hist;
	int ic = max_int, ci;
	double md, cd, min, max, range, cx;
	qSort(data);
	md = DBL_MAX;
	min = max = data[0];
	for (int i = 1; i < data.size(); ++i) {
		if (min > data[i]) min = data[i];
		if (max < data[i]) max = data[i];
		cd = qAbs<float>(data[i] - data[i - 1]);
		if (md > cd && cd != 0.) md = cd;
	}
	range = max - min;
	md = mdm;
	//qDebug() << md << range << ic;
	if (md != 0.)
		ic = qRound(qMax<double>(qMin<double>(double(ic), range / md), double(min_int)));
	md = range / ic;
	hist.resize(ic);
	foreach (const float & i, data) {
		ci = qRound((i - min) / range * double(ic - 1));
		//if (ci < 0) ci = 0;
		//if (ci >= ic) ci = ic - 1;
		hist[ci]++;
	}
	QPolygonF & cpol(graphics[graphic].polyline);
	if (hist.size() == 1 || range == 0.) {
		cpol << QPointF(min - 0.5, 0.) << QPointF(min - 0.25, 0.);
		cpol << QPointF(min - 0.25, hist[0]) << QPointF(min + 0.25, hist[0]);
		cpol << QPointF(min + 0.25, 0.) << QPointF(min + 0.5, 0.);
	} else {
		cpol << QPointF(min, 0.);
		for (int i = 0; i < hist.size(); ++i) {
			cx = i * range / ic + min;
			cpol << QPointF(cx, hist[i]) << QPointF(cx + md, hist[i]);
		}
		cpol << QPointF(range + min, 0.);
	}
	updateGraphics();
}


void Graphic::findGraphicsRect(double start_x, double end_x, double start_y, double end_y) {
	double cx, cy, maxX, minX, maxY, minY, vx = DBL_MIN;
	bool isRangeX = (start_x != end_x), isRangeY = (start_y != end_y);
	bool isEmpty = true, anyVisible = false, isTimeLimit = (visible_time > 0.) && !(isRangeX || isRangeY);
	foreach (const GraphicType & t, graphics) {
		const QPolygonF & pol(pause_ ? t.polyline_pause : t.polyline);
		if (!pol.isEmpty()) {
			isEmpty = false;
			break;
		}
	}
	if (isEmpty) {
		grect = def_rect;
		setRectToLines();
		return;
	}
	minY = minX = DBL_MAX;
	maxY = maxX = -DBL_MAX;
	foreach (const GraphicType & t, graphics) {
		if (!t.visible) continue;
		if (vx < (pause_ ? t.max_x_pause : t.max_x)) vx = (pause_ ? t.max_x_pause : t.max_x);
	}
	vx -= visible_time;
	foreach (const GraphicType & t, graphics) {
		if (!t.visible) continue;
		const QPolygonF & pol(pause_ ? t.polyline_pause : t.polyline);
		for (int i = 0; i < pol.size(); i++) {
			cx = pol[i].x();
			cy = pol[i].y();
			if ((start_x > cx || end_x < cx) && isRangeX) continue;
			if ((start_y > cy || end_y < cy) && isRangeY) continue;
			if ((cx < vx) && isTimeLimit) continue;
			if (maxY < cy) maxY = cy;
			if (minY > cy) minY = cy;
			if (maxX < cx) maxX = cx;
			if (minX > cx) minX = cx;
		}
		if (!pol.isEmpty()) anyVisible = true;
	}
	if (!anyVisible) {
		grect.setRect(0., 0., 1., 1.);
		setRectToLines();
		return;
	}
	if (maxX > limit_.right()) maxX = limit_.right();
	if (minX > limit_.right()) minX = limit_.right();
	if (minX < limit_.left()) minX = limit_.left();
	if (maxX < limit_.left()) maxX = limit_.left();
	if (maxY > limit_.bottom()) maxY = limit_.bottom();
	if (minY > limit_.bottom()) minY = limit_.bottom();
	if (minY < limit_.top()) minY = limit_.top();
	if (maxY < limit_.top()) maxY = limit_.top();
	if (minX > maxX) qSwap<double>(minX, maxX);
	if (minY > maxY) qSwap<double>(minY, maxY);
	if (qAbs<double>(minX - maxX) < 1E-60) {minX -= 1E-20; maxX += 1E-20;}
	if (qAbs<double>(minY - maxY) < 1E-60) {minY -= 1E-20; maxY += 1E-20;}
	if (only_expand_x) {
		if (minX > eminx) minX = eminx;
		if (maxX < emaxx) maxX = emaxx;
	}
	if (only_expand_y) {
		if (minY > eminy) minY = eminy;
		if (maxY < emaxy) maxY = emaxy;
	}
	eminx = minX; emaxx = maxX;
	eminy = minY; emaxy = maxY;
	if (isRangeX) selrect.setRect(start_x, minY, end_x - start_x, maxY - minY);
	else if (isRangeY) selrect.setRect(minX, start_y, maxX - minX, end_y - start_y);
	else grect.setRect(minX, minY, maxX - minX, maxY - minY);
	grect = grect.normalized();
	if (isFit)/* || isRangeX || isRangeY)*/ selrect = grect;
	setRectToLines();
}


void Graphic::drawAction() {
	//qDebug() << "draw action";
	int wid = canvas->width(), hei = canvas->height() - gridborder.y(), sx = startpos.x(), sy = startpos.y(), cx = curpos.x(), cy = curpos.y();
	painter->setPen(selpen);
	painter->setBrush(selbrush);
	switch (curaction) {
	case gaZoomInRect:
		painter->drawRect(QRect(startpos, curpos));
		break;
	case gaZoomRangeX:
		painter->drawLine(sx, hei, sx, 0);
		painter->drawLine(cx, hei, cx, 0);
		painter->fillRect(sx, 0, cx - sx, hei, selbrush);
		break;
	case gaZoomRangeY:
		painter->drawLine(gridborder.x(), sy, wid, sy);
		painter->drawLine(gridborder.x(), cy, wid, cy);
		painter->fillRect(gridborder.x(), sy, wid - gridborder.x(), cy - sy, selbrush);
		break;
	default: break;
	}
}


void Graphic::drawGrid() {
	int gbx = gridborder.x(), gby = gridborder.y(), cwid = painter->viewport().width(), chei = painter->viewport().height() - legy;
	double px, py, range, step, start;
	int wid = cwid - gbx - 5, hei = chei - gby - 5, cx, cy, cnt;
	QRect rect;
	QPair<QString, QString> str;

	range = selrect.bottom() - selrect.top();
	if (grad_y == Graphic::Auto) step = splitRange(range, hei / gridy / font_sz.height() / 1.4);
	else step = gridy;//range / hei * gridy;
	start = roundTo(canvas2realY(-hei), step) - step;
	py = start + step;
	cy = 0;
	cx = gbx - 5;
	grid_pen.setWidth(qMax<int>(qRound(thick / 1.4), 1));
    QFont sf = font();
	QFont nf = sf;
	sf.setPointSizeF(qMax<qreal>(sf.pointSizeF() / 1.6, 7.));
	QFontMetrics fm(nf), sfm(sf);
	if (step > 0.) {
		cnt = 1000;
		while (cnt-- > 0) {
			py -= step;
			if (fabs(py) < step * .5) py = 0.;
			cy = real2canvasY(py);
			if (cy < 0) continue;
			if (cy > hei + 5) break;
			painter->setPen(grid_pen);
			painter->drawLine(gbx, cy, cwid, cy);
			str = gridMark(py * grid_numbers_y);
			painter->setPen(text_color);
			cy += font_sz.height() / 4.;
			int dx = font_sz.height() / 8.;
			if (!str.second.isEmpty()) {
				rect = sfm.boundingRect(str.second);
				painter->setFont(sf);
				painter->drawText(cx - rect.width() - dx, cy - font_sz.height() / 2.5, str.second);
				dx += rect.width() + font_sz.height() / 6.;
			}
			rect = fm.boundingRect(str.first);
			painter->setFont(nf);
			painter->drawText(cx - rect.width() - dx, cy, str.first);
		}
	}
	cy = real2canvasY(0.);
	if (cy >= 0 && cy <= (hei + 5)) {
		QPen _p(grid_pen);
		_p.setStyle(Qt::SolidLine);
		painter->setPen(_p);
		painter->drawLine(gbx, cy, cwid, cy);
	}
	if (hasLblY) {
		painter->setPen(text_color);
		painter->save();
		painter->translate(5, hei);
		painter->rotate(-90.);
		painter->drawText(0, 0, hei, font_sz.height(), Qt::AlignCenter, label_y);
		painter->restore();
	}

	cy = chei - font_sz.height() / 4;
	if (hasLblX) cy -= font_sz.height();
	range = selrect.right() - selrect.left();
	QString df;
	if (axis_type_x == Graphic::Numeric) {
		if (grad_x == Graphic::Auto) step = splitRange(range, wid / gridx / font_sz.width() * 1.4);
		else step = gridx;//range / wid * gridx;
		start = roundTo(canvas2realX(wid), step) + step;
		px = start + step;
		if (step > 0.) {
			cnt = 1000;
			while (cnt-- > 0) {
				px -= step;
				if (fabs(px) < step * .5) px = 0.;
				cx = real2canvasX(px);
				if (cx > cwid) continue;
				if (cx < gbx) break;
				painter->setPen(grid_pen);
				painter->drawLine(cx, hei + 5, cx, 0);
				painter->setPen(text_color);
				int dx = -font_sz.height() / 4.;
				painter->setFont(nf);
				str = gridMark(px * grid_numbers_x);
				rect = fm.boundingRect(str.first);
				painter->drawText(cx + dx, cy, str.first);
				dx += rect.width() + font_sz.height() / 6.;
				if (!str.second.isEmpty()) {
					rect = sfm.boundingRect(str.second);
					painter->setFont(sf);
					painter->drawText(cx + dx, cy - font_sz.height() / 2.5, str.second);
				}
			}
		}
		cx = real2canvasX(0.);
		if (cx <= cwid && cx >= gbx) {
			QPen _p(grid_pen);
			_p.setStyle(Qt::SolidLine);
			painter->setPen(_p);
			painter->drawLine(cx, hei + 5, cx, 0);
		}
	} else {
		int cur_scl[7] = {0,0,0,0,0,0,0};
		step = splitRangeDate(range, wid / gridx / font_sz.width() * 1.4, &df, cur_scl);
		start = roundTo(canvas2realX(wid), step) + step;
		px = start + step;
		QDateTime cd = QDateTime::fromMSecsSinceEpoch(px * grid_numbers_x);
		//qDebug() << "*** start" << cd << step;
		roundDateTime(cd, cur_scl);
		//qDebug() << "*** round" << cd;
		addDateTime(cd, cur_scl);
		//qDebug() << "***   add" << cd;
		//qDebug() << "*** cur" << cur_scl[0] << cur_scl[1] << cur_scl[2] << cur_scl[3] << cur_scl[4] << cur_scl[5] << cur_scl[6];
		if (step > 0.) {
			cnt = 1000;
			while (cnt-- > 0) {
				addDateTime(cd, cur_scl, -1);
				//roundDateTime(cd, cur_scl);
				//qDebug() << "next" << cd;
				cx = real2canvasX(cd.toMSecsSinceEpoch() / grid_numbers_x);
				if (cx > cwid) continue;
				if (cx < gbx) {/*qDebug() << cx << "<" << gbx;*/ break;}
				painter->setPen(grid_pen);
				painter->drawLine(cx, hei + 5, cx, 0);
				painter->setPen(text_color);
				int dx = -font_sz.height() / 4.;
				painter->setFont(nf);
				str.first = cd.toString(df);
				painter->drawText(cx + dx, cy, str.first);
			}
		}
	}
	painter->setPen(text_color);
	painter->setFont(nf);
	if (hasLblX) {
		painter->setPen(text_color);
		painter->drawText(gbx, chei - font_sz.height(), wid, font_sz.height(), Qt::AlignCenter, label_x);
	}

	painter->setPen(QPen(grid_pen.color(), thick));
	painter->drawRect(gbx, -1, wid + 6, hei + 6);
}


QPair<QString, QString> Graphic::gridMark(double v) const {
	QPair<QString, QString> ret;
	if ((qAbs(v) >= 1E+4 || qAbs(v) <= 1E-4) && v != 0.) {
		int p = qFloor(qLn(qAbs(v)) / LN10);
		v /= qPow(10., p);
		if (v == 10.) {
			v = 1.;
			p += 1;
		}
		ret.first = QString::fromUtf8("%1·10").arg(v);
		ret.second = QString::number(p);
	} else
		ret.first = QString::number(v);
	return ret;
}


void Graphic::drawGraphics() {
	if (isHover)
		ui->status->setText(tr("Cursor: ") + pointCoords(canvas2real(QPointF(curpos))));
	QPointF srp = -selrect.topLeft(), cp;
	double sclx, scly, wid = painter->viewport().width(), hei = painter->viewport().height() - legy, pw;
	sclx = (wid - gridborder.x() - margins_.left() - margins_.width()) / selrect.width();
	scly = (hei - gridborder.y() - margins_.top() - margins_.height()) / selrect.height();
	painter->setClipping(true);
	painter->setClipRect(QRect(gridborder.x(), 0, wid - gridborder.x(), hei - gridborder.y()));
	painter->translate(gridborder.x() + margins_.left(), hei - gridborder.y() - margins_.top());
	//if (isOGL && aalias) pen.setWidthF(1.5f);
	painter->scale(sclx, -scly);
	painter->translate(srp);
	QTransform mat = painter->transform();
	painter->resetTransform();
	painter->setWorldMatrixEnabled(false);
	QPolygonF cpol;
	for (int i = 0; i < graphics.size(); ++i) {
		GraphicType & t(graphics[i]);
		QPolygonF & rpol(pause_ ? t.polyline_pause : t.polyline);
		if (t.visible && !rpol.isEmpty()) {
			pw = t.pen.widthF();
			if (t.lines) {
				t.pen.setCosmetic(true);
				painter->setPen(t.pen);
				if (t.fill) {
					cpol = rpol;
					painter->setBrush(t.fill_color);
					//cpol.push_front(QPointF(cpol.front().x(), 0.));
					//cpol.push_back(QPointF(cpol.back().x(), 0.));
					painter->drawPolygon(mat.map(cpol));
				} else
					painter->drawPolyline(mat.map(rpol));
			}
			if (t.points) {
				if (qRound(t.pointWidth) == t.pointWidth) t.pen.setWidth(qRound(t.pointWidth));
				else t.pen.setWidthF(t.pointWidth);
				t.pen.setCosmetic(true);
				painter->setPen(t.pen);
				painter->drawPoints(mat.map(rpol));
				if (qRound(pw) == pw) t.pen.setWidth(qRound(pw));
				else t.pen.setWidthF(pw);
			}
		}
	}
	painter->setWorldMatrixEnabled(true);
}


QString Graphic::pointCoords(QPointF point) {
	if (axis_type_x == Numeric)
		return "(" + QString::number(point.x(), 'f', 3) + " ; " + QString::number(point.y(), 'f', 3) + ")";
	return "(" + QDateTime::fromMSecsSinceEpoch(point.x()).toString() + " ; " + QString::number(point.y(), 'f', 3) + ")";
}


void Graphic::drawGuides() {
	if (!guides || !isHover) return;
	int wid = canvas->width(), hei = canvas->height();
	painter->setRenderHint(QPainter::Antialiasing, false);
	painter->setPen(QPen(grid_pen.color(), qMax<int>(qRound(thick / 1.4), 1)));
	painter->resetTransform();
	painter->setClipping(true);
	painter->setClipRect(QRect(gridborder.x(), 0, wid - gridborder.x(), hei - gridborder.y()));
	painter->drawLine(0, curpos.y(), wid, curpos.y());
	painter->drawLine(curpos.x(), 0, curpos.x(), hei);
	QString str = pointCoords(canvas2real(curpos));
	QFontMetrics fm(font());
	QRect r = fm.boundingRect(str);
	QPoint p = curpos + QPoint(font_sz.height() / 4., -font_sz.height() / 4.);
	if (r.width() + curpos.x() > wid - font_sz.height() / 2.) p.setX(curpos.x() - r.width() - font_sz.height() / 4.);
	if (curpos.y() - r.height() < font_sz.height() / 8.) p.setY(curpos.y() + r.height() - font_sz.height() / 8.);
	painter->setPen(text_color);
	painter->drawText(p, str);
}


void Graphic::drawPause() {
	painter->setClipping(false);
	painter->save();
	painter->resetMatrix();
	painter->translate(canvas->width() - icon_pause_b.width() - 6, 6);
	double o = (0.5 - pause_phase) * 2;
	painter->setOpacity(o*o);
	painter->drawImage(0, 0, icon_pause_b);
	painter->setOpacity(1.);
	painter->drawImage(0, 0, icon_pause_f);
	painter->restore();
	painter->setClipping(true);
}


double Graphic::splitRange(double range, int count) {
	double digits, step, tln;
	range = qAbs<double>(range);
	tln = qFloor(qLn(range) / LN10);
	for (int i = 0; i <= 5; ++i) {
		digits = qPow(10., tln - i);
		step = qRound(range / count / digits);
		if (step > 0.) {
			digits = qPow(10., tln - i - 1);
			step = qRound(range / count / digits);
			break;
		}
	}
	double step5 = qRound(step / 5.) * 5., step10 = qRound(step / 10.) * 10.;
	double err5 = qAbs<double>(step - step5), err10 = qAbs<double>(step - step10);
	step = (err5 < err10 ? step5 : step10) * digits;
	return step;
}


double Graphic::splitRangeDate(double range, int count, QString * format, int step[7]) {
	double ret = splitRange(range, count);
	//qDebug() << "ret =" << ret << getScaleX();
	if (ret < 1000. * 1) {*format = "ss.zzz"; step[0] = ret;}
	else if (ret < 1000. * 60) {*format = "h:m:ss"; step[1] = qRound(ret / 1000);}
	else if (ret < 1000. * 60 * 60) {*format = "h:mm"; step[2] = qRound(ret / 1000 / 60);}
	else if (ret < 1000. * 60 * 60 * 24) {*format = "dd(ddd) hh"; step[3] = qRound(ret / 1000 / 60 / 60);}
	else if (ret < 1000. * 60 * 60 * 24 * 30) {*format = "MMM dd"; step[4] = qRound(ret / 1000 / 60 / 60 / 24);}
	else if (ret < 1000. * 60 * 60 * 24 * 30 * 12) {*format = "yyyy MMM"; step[5] = qRound(ret / 1000 / 60 / 60 / 24 / 30);}
	else {*format = "yyyy"; step[6] = qRound(ret / 1000 / 60 / 60 / 24 / 30 / 12);}
	return ret;
}


double Graphic::roundTo(double value, double round_to) {
	if (round_to == 0.) return value;
	return qRound(value / round_to) * round_to;
}


void Graphic::roundDateTime(QDateTime & dt, int c[7]) {
	QDate d(dt.date()); QTime t(dt.time());
	//if (c[0] != 0) t.setHMS(t.hour(), t.minute(), t.second(), 0);
	if (c[1] != 0) t.setHMS(t.hour(), t.minute(), t.second());
	if (c[2] != 0) t.setHMS(t.hour(), t.minute(), 0);
	if (c[3] != 0) t.setHMS(t.hour(), 0, 0);
	if (c[4] != 0) {t.setHMS(0, 0, 0); d.setDate(d.year(), d.month(), d.day());}
	if (c[5] != 0) {t.setHMS(0, 0, 0); d.setDate(d.year(), d.month(), 1);}
	if (c[6] != 0) {t.setHMS(0, 0, 0); d.setDate(d.year(), 1, 1);}
	dt = QDateTime(d, t);
}


void Graphic::addDateTime(QDateTime & dt, int c[7], int mul) {
	if (c[0] != 0) dt = dt.addMSecs(mul * c[0]);
	if (c[1] != 0) dt = dt.addSecs(mul * c[1]);
	if (c[2] != 0) dt = dt.addSecs(mul * c[2] * 60);
	if (c[3] != 0) dt = dt.addSecs(mul * c[3] * 60 * 60);
	if (c[4] != 0) dt = dt.addDays(mul * c[4]);
	if (c[5] != 0) dt = dt.addMonths(mul * c[5]);
	if (c[6] != 0) dt = dt.addYears(mul * c[6]);
}


double Graphic::canvas2realX(double px) const {
	int gbx = gridborder.x() + margins_.left(), cwid = lastw, wid = cwid - gbx - margins_.width();
	double cx = px - gbx, sclx = selrect.width() / (double)wid;
	return cx * sclx + selrect.x();
}


double Graphic::canvas2realY(double py) const {
	int gby = gridborder.y() + margins_.top(), chei = lasth - legy, hei = chei - gby - margins_.height();
	double cy = chei - py - gby, scly = selrect.height() / (double)hei;
	return cy * scly + selrect.y();
}


double Graphic::real2canvasX(double px) const {
	int gbx = gridborder.x() + margins_.left(), cwid = lastw, wid = cwid - gbx - margins_.width();
	double sclx = selrect.width() / (double)wid;
	return (px - selrect.x()) / sclx + gbx;
}


double Graphic::real2canvasY(double py) const {
	int gby = gridborder.y() + margins_.top(), chei = lasth - legy, hei = chei - gby - margins_.height();
	double scly = selrect.height() / (double)hei;
	return chei - gby - (py - selrect.y()) / scly;
}


void Graphic::setCurrentAction(GraphicAction action) {
	curaction = action;
	switch (action) {
	case gaNone:
		if (guides) setCanvasCursor(Qt::BlankCursor);
		else setCanvasCursor(Qt::ArrowCursor);
		break;
	case gaZoomInRect:
		setCanvasCursor(Qt::CrossCursor);
		break;
	case gaZoomRangeX:
		setCanvasCursor(Qt::SplitHCursor);
		break;
	case gaZoomRangeY:
		setCanvasCursor(Qt::SplitVCursor);
		break;
	case gaMove:
		setCanvasCursor(Qt::SizeAllCursor);
		break;
	}
}


void Graphic::setCanvasCursor(QCursor cursor) {
	ui->canvas_raster->setCursor(cursor);
	ui->canvas_gl->setCursor(cursor);
}


void Graphic::swapToBuffer() {
	QImage timg;
	//qDebug() << "render start";
	if (isOGL) {
		timg = ui->canvas_gl->grabFrameBuffer();
		QPainter p(buffer);
		p.drawImage(0, 0, timg);
		p.end();
	}
	//qDebug() << "render finish";
	bufferActive = true;
}


void Graphic::setRectToLines() {
	is_lines_update = true;
	line_x_min.is_auto = line_x_max.is_auto = line_y_min.is_auto = line_y_max.is_auto = true;
	//qDebug() << "set to lines" << selrect;
	line_x_min.is_reset = line_x_max.is_reset = line_y_min.is_reset = line_y_max.is_reset = isFit;
	if (!line_x_min.hasFocus()) line_x_min.setValue(selrect.left());
	if (!line_x_max.hasFocus()) line_x_max.setValue(selrect.right());
	if (!line_y_min.hasFocus()) line_y_min.setValue(selrect.bottom());
	if (!line_y_max.hasFocus()) line_y_max.setValue(selrect.top());
	if (!isFit) {
		line_x_min.setDefaultText(QString::number(grect.left()).toUpper());
		line_x_max.setDefaultText(QString::number(grect.right()).toUpper());
		line_y_min.setDefaultText(QString::number(grect.bottom()).toUpper());
		line_y_max.setDefaultText(QString::number(grect.top()).toUpper());
	}
	line_x_min.is_auto = line_x_max.is_auto = line_y_min.is_auto = line_y_max.is_auto = false;
	is_lines_update = false;
}


void Graphic::checkLines() {
	isFit = (line_x_min.isDefault() && line_x_max.isDefault() && line_y_min.isDefault() && line_y_max.isDefault());
	update(true);
}


void Graphic::tick(int index, bool slide, bool update_) {
	if (slide) {
		mutex.lock();
		GraphicType & t(graphics[index]);
		if (history > 0.)
			while (t.polyline.size() > 1) {
				if (fabs(t.polyline.back().x() - t.polyline.front().x()) <= history) break;
				t.polyline.pop_front();
			}
	}
	if (!update_) {
		if (isFit) findGraphicsRect();
		mutex.unlock();
		return;
	}
	//polyline.push_back(QPointF(brick->time_, brick->output(port)));
	//cout << polyline.size() << endl;
	if (isFit) findGraphicsRect();
	if (!slide) {
		if (aupdate) update();
		return;
	}
	mutex.unlock();
	if (aupdate) update();
}


void Graphic::on_buttonAutofit_clicked() {
	isFit = true;
	bool isEmpty = true;
	foreach (const GraphicType & t, graphics) {
		const QPolygonF & pol(pause_ ? t.polyline_pause : t.polyline);
		if (!pol.isEmpty()) {
			isEmpty = false;
			break;
		}
	}
	if (isEmpty) grect = def_rect;
	selrect = grect;
	findGraphicsRect();
	update();
}


void Graphic::on_buttonConfigure_clicked() {
	conf->graphicItems.clear();
	for (int i = 0; i < graphics.size(); i++) {
		GraphicConf::GraphicItem item;
		item.icon = graphics[i].icon;
		item.name = graphics[i].name;
		conf->graphicItems.append(item);
	}
	conf->ui->colorGrid->setColor(grid_pen.color());
	conf->ui->comboStyleGrid->setCurrentIndex((int)grid_pen.style());
	conf->ui->spinWidthGrid->setValue(grid_pen.widthF());
	conf->ui->checkOGL->setChecked(isOGL);
	conf->ui->checkAAlias->setChecked(aalias);
	conf->ui->checkInputs->setChecked(borderInputsVisible());
	conf->ui->checkStatus->setChecked(statusVisible());
	conf->ui->checkLegend->setChecked(legendVisible());
	conf->ui->checkGridAutoX->setChecked(grad_x == Auto);
	conf->ui->checkGridAutoY->setChecked(grad_y == Auto);
	conf->ui->colorBackground->setColor(back_color);
	conf->ui->colorText->setColor(text_color);
	conf->ui->spinGridStepX->setValue(gridx);
	conf->ui->spinGridStepY->setValue(gridy);
	conf->ui->spinMarginL->setValue(margins_.left());
	conf->ui->spinMarginT->setValue(margins_.height());
	conf->ui->spinMarginR->setValue(margins_.width());
	conf->ui->spinMarginB->setValue(margins_.top());
	conf->readParams();
	if (conf->exec() == QDialog::Rejected) return;
	grid_pen = QPen(conf->ui->colorGrid->color(), conf->ui->spinWidthGrid->value(), (Qt::PenStyle)conf->ui->comboStyleGrid->currentIndex());
	back_color = conf->ui->colorBackground->color();
	text_color = conf->ui->colorText->color();
	grad_x = conf->ui->checkGridAutoX->isChecked() ? Auto : Fixed;
	grad_y = conf->ui->checkGridAutoY->isChecked() ? Auto : Fixed;
	gridx = conf->ui->spinGridStepX->value();
	gridy = conf->ui->spinGridStepY->value();
	setOpenGL(conf->ui->checkOGL->isChecked());
	setAntialiasing(conf->ui->checkAAlias->isChecked());
	setBorderInputsVisible(conf->ui->checkInputs->isChecked());
	setStatusVisible(conf->ui->checkStatus->isChecked());
	setLegendVisible(conf->ui->checkLegend->isChecked());
	setMargins(conf->ui->spinMarginL->value(), conf->ui->spinMarginR->value(), conf->ui->spinMarginT->value(), conf->ui->spinMarginB->value());
	updateLegend();
	update();
}


void Graphic::on_checkGuides_toggled(bool checked) {
	guides = checked;
	if (guides) setCanvasCursor(Qt::BlankCursor);
	else setCanvasCursor(Qt::ArrowCursor);
	update();
}


void Graphic::updateLegend() {
	if (!ui->widgetLegend->isVisible()) return;
	leg_update = false;
	int ps = 100;
	for (int i = 0; i < graphics.size(); i++) {
		while (!graphics[i].pb->actions().isEmpty()) graphics[i].pb->removeAction(graphics[i].pb->actions()[0]);
		delete graphics[i].pb;
		QPixmap pix(60, 22);
		pix.fill(back_color);
		QPainter p(&pix);
		p.setPen(graphics[i].pen);
		p.drawLine(0, pix.height() / 2, pix.width(), pix.height() / 2);
		p.end();
		graphics[i].icon = QIcon(pix);
		graphics[i].pb = new QCheckBox(graphics[i].name);
		graphics[i].pb->setIconSize(pix.size());
		//graphics[i].pb->setFlat(true);
		graphics[i].pb->setIcon(graphics[i].icon);
		graphics[i].pb->setChecked(graphics[i].visible);
		graphics[i].pb->setProperty("graphic_num", i);
		graphics[i].pb->setContextMenuPolicy(Qt::ActionsContextMenu);
		//qDebug() << graphics[i].pb->actions();
		QAction * act = new QAction(trUtf8("Check all"), 0);
		act->setCheckable(true);
		act->setChecked(true);
		graphics[i].pb->addAction(act);
		connect(act, SIGNAL(triggered(bool)), this, SLOT(graphicAllVisibleChange(bool)));
		connect(graphics[i].pb, SIGNAL(toggled(bool)), this, SLOT(graphicVisibleChange(bool)));
		int cps = graphics[i].pb->sizeHint().width() + 4;
		if (cps > ps) ps = cps;
	}
	int maxcol = qMax<int>(ui->widgetLegend->width() / ps - 1, 1);
	int row = 0, col = 0;
	bool lv = ui->widgetLegend->isVisible();
	ui->widgetLegend->hide();
	for (int i = 0; i < graphics.size(); i++) {
		ui->layoutLegend->addWidget(graphics[i].pb,row,col);
		col++;
		if (col > maxcol) {col = 0; row++;}
	}
	ui->widgetLegend->setVisible(lv);
	leg_update = true;
}


void Graphic::graphicVisibleChange(bool checked) {
	if (visible_update) return;
	QCheckBox * cb = qobject_cast<QCheckBox*>(sender());
	int i = cb->property("graphic_num").toInt();
	graphics[i].visible = checked;
	if (isFit) on_buttonAutofit_clicked();
	else update();
//    update();
}


void Graphic::graphicAllVisibleChange(bool checked) {
	visible_update = true;
	for (int i=0; i<graphics.size(); i++) {
		graphics[i].visible = checked;
		graphics[i].pb->setChecked(checked);
	}
	visible_update = false;
	if (isFit) on_buttonAutofit_clicked();
	else update();
}


void Graphic::enterFullscreen() {
	if (fullscr) return;
	ui->layoutCanvas->removeWidget(canvas);
	canvas->setParent(0);
	canvas->showFullScreen();
	canvas->setFocus();
	canvas->raise();
	fullscr = true;
}


void Graphic::leaveFullscreen() {
	if (!fullscr) return;
	canvas->setWindowFlags(canvas->windowFlags() & ~Qt::WindowFullScreen);
	ui->layoutCanvas->addWidget(canvas);
	canvas->show();
	fullscr = false;
}


QString Graphic::caption() const {
	return ui->labelCaption->text();
}


bool Graphic::borderInputsVisible() const {
	return ui->widgetLX->isVisible();
}


bool Graphic::statusVisible() const {
	return ui->status->isVisible();
}


bool Graphic::legendVisible() const {
    return ui->widgetLegend->isVisible();
}


QByteArray Graphic::save() {
    QByteArray ba;
    QDataStream s(&ba, QIODevice::ReadWrite);
    s << openGL() << antialiasing() << borderInputsVisible() << statusVisible() << legendVisible();
    s << graphics;
    return ba;
}


void Graphic::load(QByteArray ba) {
	if (ba.isEmpty()) return;
    QDataStream s(ba);
	bool a;
    s >> a; setOpenGL(a);
    s >> a; setAntialiasing(a);
    s >> a; setBorderInputsVisible(a);
    s >> a; setStatusVisible(a);
    s >> a; setLegendVisible(a);
    s >> graphics;
}


void Graphic::setCaption(const QString & str) {
	ui->labelCaption->setText(str);
	ui->labelCaption->setVisible(str.length() > 0);
	if (aupdate) update();
}


void Graphic::setGridEnabled(bool enabled) {
	ui->checkGrid->setChecked(enabled);
}


void Graphic::setBorderInputsVisible(bool visible) {
	ui->widgetLX->setVisible(visible);
	ui->widgetLY->setVisible(visible);
	ui->checkBorderInputs->setChecked(visible);
}


void Graphic::setStatusVisible(bool visible) {
	ui->status->setVisible(visible);
}


void Graphic::setLegendVisible(bool visible) {
	ui->widgetLegend->setVisible(visible);
	ui->checkLegend->setChecked(visible);
	updateLegend();
}


void Graphic::on_checkExpandY_toggled(bool checked) {
	only_expand_y = checked;
	ui->checkExpandY->setIcon(checked ? icon_exp_y : icon_exp_sy);
}


void Graphic::on_checkExpandX_toggled(bool checked) {
	only_expand_x = checked;
	ui->checkExpandX->setIcon(checked ? icon_exp_x : icon_exp_sx);
}
