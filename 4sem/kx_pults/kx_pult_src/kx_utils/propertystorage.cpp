#include "propertystorage.h"
#include <QColor>


bool PropertyStorage::isPropertyExists(const QString & _name) const {
	for (int i = 0; i < props.size(); ++i)
		if (props[i].name == _name)
			return true;
	return false;
}


void PropertyStorage::addProperty(const PropertyStorage::Property & p) {
	for (int i = 0; i < props.size(); ++i)
		if (props[i].name == p.name) {
			props[i] = p;
			return;
		}
	props << p;
}


void PropertyStorage::removeProperty(const QString & _name) {
	for (int i = 0; i < props.size(); ++i)
		if (props[i].name == _name) {
			props.removeAt(i);
			return;
		}
}


void PropertyStorage::removePropertiesByFlag(int flag) {
	for (int i = 0; i < props.size(); ++i)
		if ((props[i].flags & flag) == flag) {
			props.removeAt(i);
			--i;
		}
}


void PropertyStorage::updateProperties(const QList<PropertyStorage::Property> & properties_, int flag_ignore) {
	QVariantMap values;
	foreach (const PropertyStorage::Property & p, props)
		if (((p.flags & flag_ignore) != flag_ignore) || (flag_ignore == 0))
			values[p.name] = p.value;
	props = properties_;
	for (int i = 0; i < props.size(); ++i) {
		PropertyStorage::Property & p(props[i]);
		if (values.contains(p.name)) {
			QVariant pv = values[p.name];
			if (pv.userType() == p.value.userType())
				p.value = pv;
		}
	}
}


PropertyStorage::Property PropertyStorage::propertyByName(const QString & name) const {
	foreach (const Property & p, props)
		if (p.name == name)
			return p;
	return Property();
}


QVariant PropertyStorage::propertyValueByName(const QString & name) const {
	foreach (const Property & p, props)
		if (p.name == name)
			return p.value;
	return QVariant();
}


void PropertyStorage::setPropertyValue(const QString & name, const QVariant & value) {
	for (int i = 0; i < props.size(); ++i)
		if (props[i].name == name) {
			props[i].value = value;
			return;
		}
}


void PropertyStorage::setPropertyComment(const QString & name, const QString & comment) {
	for (int i = 0; i < props.size(); ++i)
		if (props[i].name == name) {
			props[i].comment = comment;
			return;
		}
}


void PropertyStorage::setPropertyFlags(const QString & name, int flags) {
	for (int i = 0; i < props.size(); ++i)
		if (props[i].name == name) {
			props[i].flags = flags;
			return;
		}
}


PropertyStorage::Property PropertyStorage::parsePropertyLine(QString l) {
	PropertyStorage::Property ret;
	QString pn, pc, pt("s"), pv;
	if (l.contains('#')) {
		int i = l.indexOf('#');
		pn = l.left(i).trimmed();
		pc = l.right(l.length() - i - 1).trimmed();
	} else {
		if (l.contains('(')) {
			int bs = l.indexOf('('), be = l.indexOf(')');
			if (be > 0) {
				pc = l.mid(bs + 1, be - bs - 1).trimmed();
				l.remove(bs, be - bs + 1);
			} else {
				pc = l.right(l.length() - bs - 1).trimmed();
				l = l.left(bs);
			}
		}
		pn = l.trimmed();
	}
	if (!pc.isEmpty()) {
		pt = pc.left(1);
		pc = pc.remove(0, 1).trimmed();
	}
	if (pn.contains('=')) {
		int i = pn.indexOf('=');
		pv = pn.right(pn.length() - i - 1).trimmed();
		pn.truncate(i);
		pn = pn.trimmed();
	}
	ret.name = pn;
	ret.comment = pc;
	ret.value = QVariant(typeFromLetter(pt));
	if (!pv.isEmpty()) {
		//qDebug() << "set value !" << pv;
		switch (ret.value.type()) {
			case QVariant::Bool: pv = pv.toLower(); ret.value = (pv == "on" || pv == "true" || pv == "enable" || pv == "enabled" || pv.toInt() > 0 ? true : false); break;
			case QVariant::Int: ret.value = pv.toInt(); break;
			case QVariant::UInt: ret.value = pv.toUInt(); break;
			case QVariant::LongLong: ret.value = pv.toLongLong(); break;
			case QVariant::ULongLong: ret.value = pv.toULongLong(); break;
			case QVariant::Double: ret.value = pv.toDouble(); break;
			case QVariant::Color: ret.value = QColor(pv); break;
			default: ret.value = pv; break;
		};
	}
	return ret;
}
