#include "qad_types.h"


__QADTypesRegistrator__ __registrator__;


int QAD::Enum::selectedValue() const {
	foreach (const Enumerator & e, enum_list)
		if (e.name == selected)
			return e.value;
	return 0;
}


bool QAD::Enum::selectValue(int v) {
	foreach (const Enumerator & e, enum_list)
		if (e.value == v) {
			selected = e.name;
			return true;
		}
	return false;
}


bool QAD::Enum::selectName(const QString & n) {
	foreach (const Enumerator & e, enum_list)
		if (e.name == n) {
			selected = e.name;
			return true;
		}
	return false;
}


int QAD::Enum::value(const QString & n) const {
	foreach (const Enumerator & e, enum_list)
		if (e.name == n)
			return e.value;
	return 0;
}


QString QAD::Enum::name(int v) const {
	foreach (const Enumerator & e, enum_list)
		if (e.value == v)
			return e.name;
	return QString();
}


QList<int> QAD::Enum::values() const {
	QList<int> ret;
	foreach (const Enumerator & e, enum_list)
		ret << e.value;
	return ret;
}


QStringList QAD::Enum::names() const {
	QStringList ret;
	foreach (const Enumerator & e, enum_list)
		ret << e.name;
	return ret;
}


QAD::Enum & QAD::Enum::operator <<(const QString & v) {
	enum_list << Enumerator(enum_list.size(), v);
	return *this;
}


QAD::Enum & QAD::Enum::operator <<(const QStringList & v) {
	foreach (const QString & s, v)
		(*this) << s;
	return *this;
}




QVariant::Type typeFromLetter(const QString & l) {
	if (l.isEmpty()) return QVariant::String;
	QString ft = l.left(1);
	if (ft == "l") return QVariant::StringList;
	if (ft == "b") return QVariant::Bool;
	if (ft == "n") return QVariant::Int;
	if (ft == "f") return QVariant::Double;
	if (ft == "c") return QVariant::Color;
	if (ft == "r") return QVariant::Rect;
	if (ft == "a") return QVariant::RectF;
	if (ft == "p") return QVariant::Point;
	if (ft == "v") return QVariant::PointF;
	if (ft == "e") return (QVariant::Type)qMetaTypeId<QAD::Enum>();
	if (ft == "F") return (QVariant::Type)qMetaTypeId<QAD::File>();
	if (ft == "D") return (QVariant::Type)qMetaTypeId<QAD::Dir>();
	return QVariant::String;
}


QString uniqueName(QString n, const QStringList & names) {
	if (!names.contains(n))
		return n;
	QString num;
	while (!n.isEmpty()) {
		if (n.right(1)[0].isDigit()) {
			num.push_front(n.right(1));
			n.chop(1);
		} else break;
	}
	if (!n.endsWith('_')) n += '_';
	int in = num.toInt() + 1;
	QString nn = n + QString::number(in).rightJustified(3, '0');
	while (names.contains(nn))
		nn = n + QString::number(++in).rightJustified(3, '0');
	return nn;
}
