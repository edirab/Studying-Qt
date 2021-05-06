#ifndef PIQT_H
#define PIQT_H

#include "pistring.h"
#include "pimathmatrix.h"
#include "pitime.h"
#include "pivariant.h"
#include <QString>
#include <QByteArray>
#include <QPointF>
#include <QVector2D>
#include <QVector3D>
#include <QDateTime>
#include <QStringList>
#include <QVariant>


//inline const QString PI2QString(const PIString & v) {return QString::fromLocal8Bit(v.data());}
inline const QString PI2QString(const PIString & v) {return QString::fromUtf8(v.dataUTF8());}

//inline const PIString Q2PIString(const QString & v) {return PIString(v.toLocal8Bit().data());}
inline const PIString Q2PIString(const QString & v) {return PIString::fromUTF8(v.toUtf8().data());}


inline const QStringList PI2QStringList(const PIStringList & v) {QStringList ret; piForeachC (PIString & s, v) ret << PI2QString(s); return ret;}

inline const PIStringList Q2PIStringList(const QStringList & v) {PIStringList ret; foreach (const QString & s, v) ret << Q2PIString(s); return ret;}


inline const QByteArray PI2QByteArray(const PIByteArray & v) {return QByteArray((const char *)(v.data()), v.size_s());}

inline const PIByteArray Q2PIByteArray(const QByteArray & v) {return PIByteArray(v.constData(), v.size());}


inline const QPointF PI2QVector2(const PIMathVectorT2d & v) {return QPointF(v[0], v[1]);}
inline const QVector3D PI2QVector3(const PIMathVectorT3d & v) {return QVector3D(v[0], v[1], v[2]);}

inline const PIMathVectorT2d Q2PIVector2(const QPointF & v) {return PIMathVectorT2d(double(v.x()), double(v.y()), 0.);}
inline const PIMathVectorT3d Q2PIVector3(const QVector3D & v) {return PIMathVectorT3d(double(v.x()), double(v.y()), double(v.z()));}


inline const QDate PI2QDate(const PIDate & v) {return QDate(v.year, v.month, v.day);}
inline const QTime PI2QTime(const PITime & v) {return QTime(v.hours, v.minutes, v.seconds, v.milliseconds);}
inline const QDateTime PI2QDateTime(const PIDateTime & v) {return QDateTime(PI2QDate(v.date()), PI2QTime(v.time()));}

inline const PIDate Q2PIDate(const QDate & v) {return PIDate(v.year(), v.month(), v.day());}
inline const PITime Q2PITime(const QTime & v) {return PITime(v.hour(), v.minute(), v.second(), v.msec());}
inline const PIDateTime Q2PIDateTime(const QDateTime & v) {return PIDateTime(Q2PIDate(v.date()), Q2PITime(v.time()));}

const QVariant PI2QVariant(const PIVariant & v);

const PIVariant Q2PIVariant(const QVariant & v);

//inline const PIVariant QString2PIVariant(const QString & v) {return PIVariant::readFromString(QString2PIString(v));}


template <typename From>
class __PIQtConverter {
public:
	__PIQtConverter(const From & v): val(v) {}
	template <typename To> operator To() {return To();}
	From val;
};
template<typename From> inline __PIQtConverter<From> __PIQtConvert(const From & f) {return __PIQtConverter<From>(f);}

#define _PIQt_CONVERT(ft, tt, ftc, tfc) \
template<> template<> inline __PIQtConverter<ft>::operator tt() {return ftc(val);} \
template<> template<> inline __PIQtConverter<tt>::operator ft() {return tfc(val);}
#define _PIQt_CONVERT_S(t) _PIQt_CONVERT(PI##t, Q##t, PI2Q##t, Q2PI##t)

_PIQt_CONVERT_S(String)
_PIQt_CONVERT_S(ByteArray)
_PIQt_CONVERT_S(Time)
_PIQt_CONVERT_S(Date)
_PIQt_CONVERT_S(DateTime)
_PIQt_CONVERT(PIMathVectorT2d, QPointF, PI2QVector2, Q2PIVector2)
_PIQt_CONVERT(PIMathVectorT3d, QVector3D, PI2QVector3, Q2PIVector3)

#define piqt __PIQtConvert
#define qtpi __PIQtConvert

#endif // PIQT_H
