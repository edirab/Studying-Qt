/*! \file picout.h
 * \brief Universal output to console class
*/
/*
	PIP - Platform Independent Primitives
	Universal output to console class
	Copyright (C) 2016  Ivan Pelipenko peri4ko@yandex.ru

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PICOUT_H
#define PICOUT_H

#include "piincludes.h"

#ifdef DOXYGEN

//! \brief Macro used for conditional (piDebug) output to PICout
#  define piCout

//! \relatesalso PIObject \brief Macro used for conditional (piDebug and PIObject::debug()) output to PICout for subclasses of PIObject
#  define piCoutObj

#else
#  define piCout if (piDebug) PICout()
#  define piCoutObj if (piDebug && debug()) PICout() << "" << (PIString("[") + className() + " \"" + name() + "\"]")
#endif

extern PIMutex __PICout_mutex__;
extern PIString __PICout_string__;

//! \brief Namespace contains enums controlled PICout
namespace PICoutManipulators {

	//! \brief Enum contains special characters
	enum PIP_EXPORT PICoutSpecialChar {
		Null	/*! Null-character, '\\0' */,
		NewLine	/*! New line character, '\\n' */,
		Tab		/*! Tab character, '\\t' */,
		Esc		/*! Escape character, '\\e' */,
		Quote	/*! Quote character, '"' */
	};

	//! \brief Enum contains immediate action
	enum PIP_EXPORT PICoutAction {
		Flush			/*! Flush the output */,
		Backspace		/*! Remove last symbol */,
		ShowCursor		/*! Show cursor */,
		HideCursor		/*! Hide cursor */,
		ClearLine		/*! Clear current line */,
		ClearScreen		/*! Clear the screen */,
		SaveContol		/*! Save control flags, equivalent to \a saveControl() */,
		RestoreControl	/*! Restore control flags, equivalent to \a restoreControl() */
	};

	//! \brief Enum contains control of PICout
	enum PIP_EXPORT PICoutControl {
		AddNone		/*! No controls */ = 0x0,
		AddSpaces	/*! Spaces will be appear after each output */ = 0x1,
		AddNewLine	/*! New line will be appear after all output */ = 0x2,
		AddQuotes	/*! Each string will be quoted */ = 0x4,
		AddAll		/*! All controls */ = 0xFFFFFFFF
	};

	//! \brief Enum contains output format
	enum PIP_EXPORT PICoutFormat {
		Bin			/*! Binary representation of integers */ = 0x01,
		Oct			/*! Octal representation of integers */ = 0x02,
		Dec			/*! Decimal representation of integers */ = 0x04,
		Hex			/*! Hexadecimal representation of integers */ = 0x08,
		Bold		/*! Bold */ = 0x10,
		Faint		/*!  */ = 0x20,
		Italic		/*!  */ = 0x40,
		Underline	/*! Underline */ = 0x80,
		Blink		/*! Blink */ = 0x100,
		Black		/*! Black font */ = 0x400,
		Red			/*! Red font */ = 0x800,
		Green		/*! Green font */ = 0x1000,
		Blue		/*! Blue font */ = 0x2000,
		Yellow		/*! Yellow font */ = 0x4000,
		Magenta		/*! Magenta font */ = 0x8000,
		Cyan		/*! Cyan font */ = 0x10000,
		White		/*! White font */ = 0x20000,
		BackBlack	/*! Black background */ = 0x40000,
		BackRed		/*! Red background */ = 0x80000,
		BackGreen	/*! Green background */ = 0x100000,
		BackBlue	/*! Blue background */ = 0x200000,
		BackYellow	/*! Yellow background */ = 0x400000,
		BackMagenta	/*! Magenta background */ = 0x800000,
		BackCyan	/*! Cyan background */ = 0x1000000,
		BackWhite	/*! White background */ = 0x2000000,
		Default		/*! Default format */ = 0x4000000
	};

	typedef PIFlags<PICoutControl> PICoutControls;
};

//using namespace PICoutManipulators;


class PIP_EXPORT PICout {
public:
	//! Default constructor with default features (AddSpaces and AddNewLine)
	explicit PICout(PIFlags<PICoutManipulators::PICoutControl> controls = PICoutManipulators::AddSpaces | PICoutManipulators::AddNewLine);

	PICout(const PICout & other);

	~PICout();


	//! Output operator for strings with <tt>"const char * "</tt> type
	PICout operator <<(const char * v);

	//! Output operator for strings with <tt>"std::string"</tt> type
	PICout operator <<(const std::string & v);

	//! Output operator for boolean values
	PICout operator <<(const bool v);

	//! Output operator for <tt>"char"</tt> values
	PICout operator <<(const char v);

	//! Output operator for <tt>"unsigned char"</tt> values
	PICout operator <<(const uchar v);

	//! Output operator for <tt>"short"</tt> values
	PICout operator <<(const short v);

	//! Output operator for <tt>"unsigned short"</tt> values
	PICout operator <<(const ushort v);

	//! Output operator for <tt>"int"</tt> values
	PICout operator <<(const int v);

	//! Output operator for <tt>"unsigned int"</tt> values
	PICout operator <<(const uint v);

	//! Output operator for <tt>"long"</tt> values
	PICout operator <<(const long v);

	//! Output operator for <tt>"unsigned long"</tt> values
	PICout operator <<(const ulong v);

	//! Output operator for <tt>"long long"</tt> values
	PICout operator <<(const llong v);

	//! Output operator for <tt>"unsigned long long"</tt> values
	PICout operator <<(const ullong v);

	//! Output operator for <tt>"float"</tt> values
	PICout operator <<(const float v);

	//! Output operator for <tt>"double"</tt> values
	PICout operator <<(const double v);

	//! Output operator for pointers
	PICout operator <<(const void * v);

	//! Output operator for PIObject and ancestors
	PICout operator <<(const PIObject * v);

	//! Output operator for \a PICoutSpecialChar values
	PICout operator <<(const PICoutManipulators::PICoutSpecialChar v);

	//! Output operator for \a PIFlags<PICoutFormat> values
	PICout operator <<(const PIFlags<PICoutManipulators::PICoutFormat> & v) {
		if (v[PICoutManipulators::Bin]) cnb_ = 2;
		if (v[PICoutManipulators::Oct]) cnb_ = 8;
		if (v[PICoutManipulators::Dec]) cnb_ = 10;
		if (v[PICoutManipulators::Hex]) cnb_ = 16;
		if (v[PICoutManipulators::Bold]) applyFormat(PICoutManipulators::Bold);
		if (v[PICoutManipulators::Faint]) applyFormat(PICoutManipulators::Faint);
		if (v[PICoutManipulators::Italic]) applyFormat(PICoutManipulators::Italic);
		if (v[PICoutManipulators::Underline]) applyFormat(PICoutManipulators::Underline);
		if (v[PICoutManipulators::Blink]) applyFormat(PICoutManipulators::Blink);
		if (v[PICoutManipulators::Black]) applyFormat(PICoutManipulators::Black);
		if (v[PICoutManipulators::Red]) applyFormat(PICoutManipulators::Red);
		if (v[PICoutManipulators::Green]) applyFormat(PICoutManipulators::Green);
		if (v[PICoutManipulators::Blue]) applyFormat(PICoutManipulators::Blue);
		if (v[PICoutManipulators::Yellow]) applyFormat(PICoutManipulators::Yellow);
		if (v[PICoutManipulators::Magenta]) applyFormat(PICoutManipulators::Magenta);
		if (v[PICoutManipulators::Cyan]) applyFormat(PICoutManipulators::Cyan);
		if (v[PICoutManipulators::White]) applyFormat(PICoutManipulators::White);
		if (v[PICoutManipulators::BackBlack]) applyFormat(PICoutManipulators::BackBlack);
		if (v[PICoutManipulators::BackRed]) applyFormat(PICoutManipulators::BackRed);
		if (v[PICoutManipulators::BackGreen]) applyFormat(PICoutManipulators::BackGreen);
		if (v[PICoutManipulators::BackBlue]) applyFormat(PICoutManipulators::BackBlue);
		if (v[PICoutManipulators::BackYellow]) applyFormat(PICoutManipulators::BackYellow);
		if (v[PICoutManipulators::BackMagenta]) applyFormat(PICoutManipulators::BackMagenta);
		if (v[PICoutManipulators::BackCyan]) applyFormat(PICoutManipulators::BackCyan);
		if (v[PICoutManipulators::BackWhite]) applyFormat(PICoutManipulators::BackWhite);
		if (v[PICoutManipulators::Default]) applyFormat(PICoutManipulators::Default);
		return *this;
	}

	//! Output operator for \a PICoutFormat values
	PICout operator <<(const PICoutManipulators::PICoutFormat v) {
		switch (v) {
		case PICoutManipulators::Bin: cnb_ = 2; break;
		case PICoutManipulators::Oct: cnb_ = 8; break;
		case PICoutManipulators::Dec: cnb_ = 10; break;
		case PICoutManipulators::Hex: cnb_ = 16; break;
		default: applyFormat(v);
		};
		return *this;
	}

	//! Do some action
	PICout operator <<(const PICoutManipulators::PICoutAction v);

	//! Set control flag "c" is "on" state
	PICout & setControl(PICoutManipulators::PICoutControl c, bool on = true) {co_.setFlag(c, on); return *this;}

	//! Set control flags "c" and if "save" exec \a saveControl()
	PICout & setControl(PICoutManipulators::PICoutControls c, bool save = false) {if (save) saveControl(); co_ = c; return *this;}

	//! Save control flags to internal stack \sa \a restoreControl()
	PICout & saveControl();

	//! Restore control flags from internal stack \sa \a saveControl()
	PICout & restoreControl();

	/*! \brief Conditional put space character to output
	 * \details If it is not a first output and control \a AddSpaces is set
	 * space character is put \sa \a quote(), \a newLine() */
	PICout & space();

	/*! \brief Conditional put quote character to output
	 * \details If control \a AddQuotes is set
	 * quote character is put \sa \a space(), \a newLine() */
	PICout & quote();

	/*! \brief Conditional put new line character to output
	 * \details If control \a AddNewLine is set
	 * new line character is put \sa \a space(), \a quote() */
	PICout & newLine();

	static bool setBufferActive(bool on, bool clear = false);
	static bool isBufferActive();
	static PIString buffer(bool clear = false);
	static void clearBuffer();

private:
	void applyFormat(PICoutManipulators::PICoutFormat f);

	static bool buffer_;
	PRIVATE_DECLARATION
	bool fo_, cc_, fc_;
	int cnb_, attr_;
	PICoutManipulators::PICoutControls co_;
};

#endif // PICOUT_H
