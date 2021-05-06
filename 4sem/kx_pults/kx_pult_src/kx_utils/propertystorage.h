#ifndef PROPERTYSTORAGE_H
#define PROPERTYSTORAGE_H

#include "qad_types.h"


class PropertyStorage {
public:
	PropertyStorage() {}
	
	struct Property {
		Property(const QString & n = QString(), const QString & c = QString(), const QVariant & v = QVariant(), int f = 0):
			name(n), comment(c), value(v), flags(f) {}
		QString name;
		QString comment;
		QVariant value;
		int flags;
	};
	
	PropertyStorage(const QList<Property> & pl) {props = pl;}
	
	typedef QList<Property>::const_iterator const_iterator;
	typedef QList<Property>::iterator iterator;
	
	iterator begin() {return props.begin();}
	const_iterator begin() const {return props.begin();}
	const_iterator constBegin() const {return props.constBegin();}
	iterator end() {return props.end();}
	const_iterator end() const {return props.end();}
	const_iterator constEnd() const {return props.constEnd();}
	
	int count() const {return props.count();}
	int length() const {return props.length();}
	int size() const {return props.size();}
	bool isEmpty() const {return props.isEmpty();}
	Property & first() {return props.first();}
	const Property & first() const {return props.first();}
	Property & front() {return props.front();}
	const Property & front() const {return props.front();}
	Property & last() {return props.last();}
	const Property & last() const {return props.last();}
	Property & back() {return props.back();}
	const Property & back() const {return props.back();}
	void removeFirst() {props.removeFirst();}
	void removeLast() {props.removeLast();}
	void removeAt(int i) {props.removeAt(i);}
	Property value(int i) const {return props.value(i);}
	Property value(int i, const Property & defaultValue) const {return props.value(i, defaultValue);}
	void clear() {props.clear();}
	
	PropertyStorage copy() const {return PropertyStorage(*this);}
	int propertiesCount() const {return props.size();}
	QList<Property> & properties() {return props;}
	const QList<Property> & properties() const {return props;}
	const PropertyStorage & propertyStorage() const {return *this;}
	bool isPropertyExists(const QString & _name) const;
	void clearProperties() {props.clear();}
	void addProperty(const Property & p);
	void addProperty(const QString & _name, const QVariant & _def_value, const QString & _comment = QString(), int _flags = 0) {addProperty(Property(_name, _comment, _def_value, _flags));}
	void removeProperty(const QString & _name);
	void removePropertiesByFlag(int flag);
	void updateProperties(const QList<Property> & properties_, int flag_ignore = 0);
	Property propertyByName(const QString & name) const;
	QVariant propertyValueByName(const QString & name) const;
	void setPropertyValue(const QString & name, const QVariant & value);
	void setPropertyComment(const QString & name, const QString & comment);
	void setPropertyFlags(const QString & name, int flags);
	
	PropertyStorage & operator <<(const PropertyStorage::Property & p) {props << p; return *this;}
	PropertyStorage & operator <<(const QList<Property> & p) {props << p; return *this;}
	PropertyStorage & operator <<(const PropertyStorage & p) {props << p.props; return *this;}
	Property & operator[](int i) {return props[i];}
	const Property & operator[](int i) const {return props[i];}
	
	static Property parsePropertyLine(QString l);
	
protected:
	QList<Property> props;
	
};

inline QDebug operator <<(QDebug s, const PropertyStorage::Property & p) {s.nospace() << p.name << " (0x" << QString::number(p.flags, 16) << ") = " << p.value; return s.space();}

inline QDataStream & operator <<(QDataStream & s, const PropertyStorage & p) {s << p.properties(); return s;}
inline QDataStream & operator >>(QDataStream & s, PropertyStorage & p) {s >> p.properties(); return s;}

inline QDataStream & operator <<(QDataStream & s, const PropertyStorage::Property & p) {
	ChunkStream cs;
	cs << cs.chunk(1, p.name) << cs.chunk(2, p.comment) << cs.chunk(3, p.value) << cs.chunk(4, p.flags);
	s << cs.data();
	return s;
}
inline QDataStream & operator >>(QDataStream & s, PropertyStorage::Property & p) {
	ChunkStream cs(s);
	while (!cs.atEnd()) {
		switch (cs.read()) {
		case 1: cs.get(p.name); break;
		case 2: cs.get(p.comment); break;
		case 3: cs.get(p.value); break;
		case 4: cs.get(p.flags); break;
		}
	}
	return s;
}


#endif // PROPERTYSTORAGE_H
