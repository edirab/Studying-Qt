#include "graphic_conf.h"
#include "ui_graphic_conf.h"


GraphicConf::GraphicConf(QVector<GraphicType> & graphics_, QWidget * parent): QDialog(parent), graphics(graphics_) {
	ui = new Ui::GraphicConf();
	ui->setupUi(this);
	QStringList styles;
	styles << tr("NoPen") << tr("Solid") << tr("Dash")
	       << tr("Dot") << tr("Dash-Dot") << tr("Dash-Dot-Dot");
	ui->comboStyleGrid->setIconSize(QSize(60, 22));
	ui->comboStyleGraphic->setIconSize(QSize(60, 22));
	ui->cbGraphicNames->setIconSize(QSize(60, 22));
	for (int i = 0; i < 6; i++) {
		QPixmap pix(60, 22);
		pix.fill();
		QPainter p(&pix);
		p.setPen(QPen(Qt::black, 1, (Qt::PenStyle)i));
		p.drawLine(0, pix.height() / 2, pix.width(), pix.height() / 2);
		p.end();
		ui->comboStyleGraphic->addItem(QIcon(pix), styles[i]);
		ui->comboStyleGrid->addItem(QIcon(pix), styles[i]);
	}
}


void GraphicConf::changeEvent(QEvent * e) {
	if (e->type() == QEvent::LanguageChange) {
		ui->retranslateUi(this);
		return;
	}
	QDialog::changeEvent(e);
}


void GraphicConf::readParams() {
	ui->cbGraphicNames->clear();
	for (int i = 0; i < graphicItems.size(); i++)
		ui->cbGraphicNames->addItem(graphicItems[i].icon, graphicItems[i].name);
}


void GraphicConf::on_cbGraphicNames_currentIndexChanged(int i) {
	if (i < 0) return;
	if (graphicItems.isEmpty()) return;
	ui->comboStyleGraphic->setCurrentIndex((int)graphics[i].pen.style());
	ui->colorGraphic->setColor(graphics[i].pen.color());
	ui->colorFill->setColor(graphics[i].fill_color);
	ui->spinLineWidthGraphic->setValue(graphics[i].pen.widthF());
	ui->spinPointWidthGraphic->setValue(graphics[i].pointWidth);
	ui->checkLines->setChecked(graphics[i].lines);
	ui->checkPoints->setChecked(graphics[i].points);
	ui->checkFill->setChecked(graphics[i].fill);
}


void GraphicConf::on_colorGraphic_colorChanged(const QColor & c) {
	if (graphicItems.isEmpty()) return;
	graphics[ui->cbGraphicNames->currentIndex()].pen.setColor(c);
}


void GraphicConf::on_comboStyleGraphic_currentIndexChanged(int index) {
	if (graphicItems.isEmpty()) return;
	graphics[ui->cbGraphicNames->currentIndex()].pen.setStyle((Qt::PenStyle)index);
}


void GraphicConf::on_spinLineWidthGraphic_valueChanged(double value) {
	if (graphicItems.isEmpty()) return;
	if (qRound(value) == value) graphics[ui->cbGraphicNames->currentIndex()].pen.setWidth(qRound(value));
	else graphics[ui->cbGraphicNames->currentIndex()].pen.setWidthF(value);
}


void GraphicConf::on_spinPointWidthGraphic_valueChanged(double value) {
	if (graphicItems.isEmpty()) return;
	graphics[ui->cbGraphicNames->currentIndex()].pointWidth = value;
}


void GraphicConf::on_checkLines_toggled(bool on) {
	if (graphicItems.isEmpty()) return;
	graphics[ui->cbGraphicNames->currentIndex()].lines = on;
}


void GraphicConf::on_checkPoints_toggled(bool on) {
	if (graphicItems.isEmpty()) return;
	graphics[ui->cbGraphicNames->currentIndex()].points = on;
}


void GraphicConf::on_checkFill_toggled(bool on) {
	if (graphicItems.isEmpty()) return;
	graphics[ui->cbGraphicNames->currentIndex()].fill = on;
}


void GraphicConf::on_colorFill_colorChanged(const QColor & color) {
	if (graphicItems.isEmpty()) return;
	graphics[ui->cbGraphicNames->currentIndex()].fill_color = color;
}
