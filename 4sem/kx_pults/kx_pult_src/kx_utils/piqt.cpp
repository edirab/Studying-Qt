#include "piqt.h"
#include "qvariantedit.h"


const QVariant PI2QVariant(const PIVariant & v) {
	switch (v.type()) {
	case PIVariant::pivBool: return QVariant(v.toBool());
	case PIVariant::pivChar:
	case PIVariant::pivUChar:
	case PIVariant::pivShort:
	case PIVariant::pivUShort:
	case PIVariant::pivInt: return QVariant(v.toInt());
	case PIVariant::pivUInt: return QVariant(uint(v.toInt()));
	case PIVariant::pivLLong: return QVariant(v.toLLong());
	case PIVariant::pivULLong: return QVariant(ullong(v.toLLong()));
	case PIVariant::pivFloat: return QVariant(v.toFloat());
	case PIVariant::pivDouble: return QVariant(v.toDouble());
	case PIVariant::pivLDouble: return QVariant(v.toDouble());
	case PIVariant::pivByteArray: return QVariant(PI2QByteArray(v.toByteArray()));
	case PIVariant::pivString: return QVariant(PI2QString(v.toString()));
	case PIVariant::pivStringList: return QVariant(PI2QStringList(v.toStringList()));
	case PIVariant::pivTime: return QVariant(PI2QTime(v.toTime()));
	case PIVariant::pivDate: return QVariant(PI2QDate(v.toDate()));
	case PIVariant::pivDateTime: return QVariant(PI2QDateTime(v.toDateTime()));
	//case PIVariant::pivSystemTime: return QVariant(v.to());
	default: return QVariant();
	}
	return QVariant();
}


const PIVariant Q2PIVariant(const QVariant & v) {
	switch (v.type()) {
	case QVariant::Bool: return PIVariant(v.toBool());
	case QVariant::Int: return PIVariant(v.toInt());
	case QVariant::UInt: return PIVariant(v.toUInt());
	case QVariant::LongLong: return PIVariant(v.toLongLong());
	case QVariant::ULongLong: return PIVariant(v.toULongLong());
	case QVariant::Double: return PIVariant(v.toDouble());
	case QVariant::ByteArray: return PIVariant(Q2PIByteArray(v.toByteArray()));
	case QVariant::String: return PIVariant(Q2PIString(v.toString()));
	case QVariant::StringList: return PIVariant(Q2PIStringList(v.toStringList()));
	case QVariant::Time: return PIVariant(Q2PITime(v.toTime()));
	case QVariant::Date: return PIVariant(Q2PIDate(v.toDate()));
	case QVariant::DateTime: return PIVariant(Q2PIDateTime(v.toDateTime()));
	default: return PIVariant();
	}
	return PIVariant();
}
