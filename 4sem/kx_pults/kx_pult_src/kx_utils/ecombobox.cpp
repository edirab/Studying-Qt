#include <QStandardItemModel>
#include <QApplication>
#include <QDebug>
#include <QHeaderView>
#include "ecombobox.h"


class EModel: public QStandardItemModel {
public:
	EModel(QObject * parent = 0): QStandardItemModel(parent) {
#if QT_VERSION < 0x050000
		setSupportedDragActions(Qt::MoveAction);
#endif
	}
protected:
	virtual Qt::ItemFlags flags(const QModelIndex & index) const {
		Qt::ItemFlags f = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
		if (!index.isValid()) f |= Qt::ItemIsDropEnabled;
		return f;
	}
#if QT_VERSION >= 0x050000
	Qt::DropActions supportedDragActions() const {return Qt::MoveAction;}
#endif
};


EComboBox::EComboBox(QWidget * parent): QComboBox(parent) {
	setView(&iv);
	setModel(new EModel());
	iv.setTextElideMode(Qt::ElideMiddle);
	iv.setRootIsDecorated(false);
	iv.setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	iv.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	iv.setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	iv.setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	iv.setDragDropMode(QAbstractItemView::InternalMove);
	iv.setMinimumHeight(100);
	icon.setPixmap(QPixmap(":/icons/edit-find.png"));
	ifont = nfont = font();
	ifont.setItalic(true);
#if QT_VERSION >= 0x040700
	filter.setPlaceholderText(tr("Filter"));
	filter.setFont(ifont);
#endif
	header.setAutoFillBackground(true);
	header.setLayout(new QBoxLayout(QBoxLayout::LeftToRight));
	header.layout()->setSpacing(2);
	header.layout()->setContentsMargins(2, 0, 0, 0);
	header.layout()->addWidget(&icon);
	header.layout()->addWidget(&filter);
	header.setParent(iv.header());
	connect(&filter, SIGNAL(textChanged(QString)), this, SLOT(filterChanged(QString)));
}


QSize EComboBox::sizeHint() const {
	QSize s = QComboBox::sizeHint();
	s.setWidth(s.width() + 16);
	return s;
}


void EComboBox::showPopup() {
	filterChanged(filter.text(), true);
	QComboBox::showPopup();
	QRect r = iv.header()->rect();
	header.setGeometry(r.x(), r.y(), r.width(), r.height() - 1);
	filter.setFocus();
}


void EComboBox::filterChanged(const QString & text, bool first) {
	if (filter.text().isEmpty()) filter.setFont(ifont);
	else filter.setFont(nfont);
	iv.hide();
	QModelIndex pi = iv.rootIndex();
	if (text.isEmpty()) {
		for (int i = 0; i < iv.model()->rowCount(); ++i) {
			iv.setRowHidden(i, pi, false);
			iv.model()->setData(iv.model()->index(i, 0), iv.model()->index(i, 0, pi).data().toString(), Qt::ToolTipRole);
		}
		iv.show();
		if (first) return;
		hidePopup();
		showPopup();
		qApp->processEvents();
		QRect r = iv.header()->rect();
		header.setGeometry(r.x(), r.y(), r.width(), r.height() - 1);
		return;
	}
	for (int i = 0; i < iv.model()->rowCount(); ++i) {
		iv.setRowHidden(i, pi, !iv.model()->index(i, 0, pi).data().toString().contains(QRegExp(text, Qt::CaseInsensitive)));
		iv.model()->setData(iv.model()->index(i, 0), iv.model()->index(i, 0, pi).data().toString(), Qt::ToolTipRole);
	}
	iv.show();
	qApp->processEvents();
	QRect r = iv.header()->rect();
	header.setGeometry(r.x(), r.y(), r.width(), r.height() - 1);
}
