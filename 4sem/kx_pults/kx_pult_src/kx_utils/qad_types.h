#ifndef QAD_TYPES_H
#define QAD_TYPES_H

#include <QPointF>
#include <QRectF>
#include <QLineF>
#include <QVariant>
#include <QStringList>
#include <QDebug>
#include "chunkstream.h"


namespace QAD {

	struct Enumerator {
		Enumerator(int v = 0, const QString & n = QString()): value(v), name(n) {}
		int value;
		QString name;
	};

	struct Enum {
		Enum(const QString & n = QString()): enum_name(n) {}
		QString toString() const {return selected;} // obsolete
		int selectedValue() const;
		QString selectedName() const {return selected;}
		bool selectValue(int v);
		bool selectName(const QString & n);
		int value(const QString & n) const;
		QString name(int v) const;
		QList<int> values() const;
		QStringList names() const;
		QString enum_name;
		QString selected;
		QList<Enumerator> enum_list;
		Enum & operator <<(const QString & v);
		Enum & operator <<(const QStringList & v);
	};

	struct File {
		File(const QString & p = QString(), const QString & f = QString(), bool abs = false): file(p), filter(f), is_abs(abs) {}
		QString toString() const {return file;}
		QString file;
		QString filter;
		bool is_abs;
	};

	struct Dir {
		Dir(const QString & d = QString(), bool abs = false): dir(d), is_abs(abs) {}
		QString toString() const {return dir;}
		QString dir;
		bool is_abs;
	};

}

Q_DECLARE_METATYPE(QAD::Enumerator)
inline QDataStream & operator <<(QDataStream & s, const QAD::Enumerator & v) {s << v.value << v.name; return s;}
inline QDataStream & operator >>(QDataStream & s, QAD::Enumerator & v) {s >> v.value >> v.name; return s;}
inline QDebug operator <<(QDebug s, const QAD::Enumerator & v) {s.nospace() << v.name << "(" << v.value << ")"; return s.space();}

Q_DECLARE_METATYPE(QAD::Enum)
inline QDataStream & operator <<(QDataStream & s, const QAD::Enum & v) {s << v.enum_name << v.selected << v.enum_list; return s;}
inline QDataStream & operator >>(QDataStream & s, QAD::Enum & v) {s >> v.enum_name >> v.selected >> v.enum_list; return s;}
inline QDebug operator <<(QDebug s, const QAD::Enum & v) {s.nospace() << v.selected; return s.space();}

Q_DECLARE_METATYPE(QAD::File)
inline QDataStream & operator <<(QDataStream & s, const QAD::File & v) {s << v.file << v.filter << v.is_abs; return s;}
inline QDataStream & operator >>(QDataStream & s, QAD::File & v) {s >> v.file >> v.filter >> v.is_abs; return s;}
inline QDebug operator <<(QDebug s, const QAD::File & v) {s.nospace() << v.file; return s.space();}

Q_DECLARE_METATYPE(QAD::Dir)
inline QDataStream & operator <<(QDataStream & s, const QAD::Dir & v) {s << v.dir << v.is_abs; return s;}
inline QDataStream & operator >>(QDataStream & s, QAD::Dir & v) {s >> v.dir >> v.is_abs; return s;}
inline QDebug operator <<(QDebug s, const QAD::Dir & v) {s.nospace() << v.dir; return s.space();}

class __QADTypesRegistrator__ {
public:
	__QADTypesRegistrator__() {
		qRegisterMetaType<QAD::Enumerator>("QAD::Enumerator");
		qRegisterMetaTypeStreamOperators<QAD::Enumerator>("QAD::Enumerator");

		qRegisterMetaType<QAD::Enum>("QAD::Enum");
		qRegisterMetaTypeStreamOperators<QAD::Enum>("QAD::Enum");

		qRegisterMetaType<QAD::File>("QAD::File");
		qRegisterMetaTypeStreamOperators<QAD::File>("QAD::File");

		qRegisterMetaType<QAD::Dir>("QAD::Dir");
		qRegisterMetaTypeStreamOperators<QAD::Dir>("QAD::Dir");
	}
};


inline qreal quantize(qreal x, qreal q = 10.f) {return qRound(x / q) * q;}
inline QPointF quantize(QPointF x, qreal q = 10.f) {return QPointF(quantize(x.x(), q), quantize(x.y(), q));}

inline qreal distPointToLine(const QPointF & lp0, const QPointF & lp1, const QPointF & p) {
	QLineF a(lp0, lp1), b(lp0, p), c(lp1, p);
	qreal f = qAbs(a.dx()*b.dy() - a.dy()*b.dx()) / a.length(), s = b.length() + c.length() - a.length();
	return qMax(f, s);
}
inline QPointF nearestPointOnLine(const QPointF & lp0, const QPointF & lp1, const QPointF & p) {
	QLineF a(lp0, lp1), b(lp0, p);
	return a.pointAt(b.length() / a.length());
}
inline QRectF enlargedRect(const QRectF & r, qreal dx, qreal dy, qreal v) {
	return QRectF(r.left() - v + dx, r.top() - v + dy, r.width() + v+v, r.height() + v+v);
}

QVariant::Type typeFromLetter(const QString & l);
QString uniqueName(QString n, const QStringList & names);


#endif // QAD_TYPES_H
