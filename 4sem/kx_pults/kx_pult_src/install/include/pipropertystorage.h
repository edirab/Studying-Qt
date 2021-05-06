#ifndef PIPROPERTYSTORAGE_H
#define PIPROPERTYSTORAGE_H

#include "pivariant.h"

class PIPropertyStorage {
public:
	PIPropertyStorage() {}

	struct Property {
		Property(const PIString & n = PIString(), const PIString & c = PIString(), const PIVariant & v = PIVariant(), int f = 0):
			name(n), comment(c), value(v), flags(f) {}
		PIString name;
		PIString comment;
		PIVariant value;
		int flags;
	};

	PIPropertyStorage(const PIVector<Property> & pl) {props = pl;}

	typedef PIVector<Property>::const_iterator const_iterator;
	typedef PIVector<Property>::iterator iterator;
	typedef Property value_type;

	iterator begin() {return props.begin();}
	const_iterator begin() const {return props.begin();}
	iterator end() {return props.end();}
	const_iterator end() const {return props.end();}

	int length() const {return props.length();}
	int size() const {return props.size();}
	bool isEmpty() const {return props.isEmpty();}
	Property & front() {return props.front();}
	const Property & front() const {return props.front();}
	Property & back() {return props.back();}
	const Property & back() const {return props.back();}
	void removeAt(int i) {props.remove(i);}
	void clear() {props.clear();}

	PIPropertyStorage copy() const {return PIPropertyStorage(*this);}
	int propertiesCount() const {return props.size();}
	PIVector<Property> & properties() {return props;}
	const PIVector<Property> & properties() const {return props;}
	const PIPropertyStorage & propertyStorage() const {return *this;}
	bool isPropertyExists(const PIString & _name) const;
	void clearProperties() {props.clear();}
	void addProperty(const Property & p);
	void addProperty(const PIString & _name, const PIVariant & _def_value, const PIString & _comment = PIString(), int _flags = 0) {addProperty(Property(_name, _comment, _def_value, _flags));}
	void removeProperty(const PIString & _name);
	void removePropertiesByFlag(int flag);
	void updateProperties(const PIVector<Property> & properties_, int flag_ignore = 0);
	Property propertyByName(const PIString & name) const;
	PIVariant propertyValueByName(const PIString & name) const;
	void setPropertyValue(const PIString & name, const PIVariant & value);
	void setPropertyComment(const PIString & name, const PIString & comment);
	void setPropertyFlags(const PIString & name, int flags);

	PIPropertyStorage & operator <<(const PIPropertyStorage::Property & p) {props << p; return *this;}
	PIPropertyStorage & operator <<(const PIVector<Property> & p) {props << p; return *this;}
	PIPropertyStorage & operator <<(const PIPropertyStorage & p) {props << p.props; return *this;}
	Property & operator[](int i) {return props[i];}
	const Property & operator[](int i) const {return props[i];}
	Property & operator[](const PIString & name);
	const Property operator[](const PIString & name) const;

	static Property parsePropertyLine(PIString l);

protected:
	PIVector<Property> props;

};


inline PIByteArray & operator <<(PIByteArray & s, const PIPropertyStorage::Property & v) {s << v.name << v.value << v.comment << v.flags; return s;}
inline PIByteArray & operator >>(PIByteArray & s, PIPropertyStorage::Property & v) {s >> v.name >> v.value >> v.comment >> v.flags; return s;}

inline PIByteArray & operator <<(PIByteArray & s, const PIPropertyStorage & v) {s << v.properties(); return s;}
inline PIByteArray & operator >>(PIByteArray & s, PIPropertyStorage & v) {s >> v.properties(); return s;}

#endif // PIPROPERTYSTORAGE_H
