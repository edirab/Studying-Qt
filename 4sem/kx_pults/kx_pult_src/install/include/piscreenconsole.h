#ifndef PISCREENCONSOLE_H
#define PISCREENCONSOLE_H

#include "piscreentiles.h"

class PIP_EXPORT TileVars: public PIScreenTile {
public:
	TileVars(const PIString & n = PIString());
protected:
	struct Variable {
		Variable() {nx = ny = type = offset = bitFrom = bitCount = size = 0; format = PIScreenTypes::CellFormat(); ptr = 0;}
		bool isEmpty() const {return (ptr == 0);}
		PIString name;
		PIScreenTypes::CellFormat format;
		int nx;
		int ny;
		int type;
		int offset;
		int bitFrom;
		int bitCount;
		int size;
		const void * ptr;
		/*void operator =(const Variable & src) {
			name = src.name;
			format = src.format;
			nx = src.nx;
			ny = src.ny;
			type = src.type;
			offset = src.offset;
			bitFrom = src.bitFrom;
			bitCount = src.bitCount;
			size = src.size;
			ptr = src.ptr;
		}*/
	};
	PIVector<Variable> variables;
	PIScreenTypes::Alignment alignment;
	void sizeHint(int & w, int & h) const;
	void drawEvent(PIScreenDrawer * d);
};



class PIScreenConsole : public PIScreenTile
{
public:
	PIScreenConsole();
};

#endif // PISCREENCONSOLE_H
