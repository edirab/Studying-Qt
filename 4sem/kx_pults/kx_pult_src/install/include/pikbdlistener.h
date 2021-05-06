/*! \file pikbdlistener.h
 * \brief Keyboard console input listener
*/
/*
	PIP - Platform Independent Primitives
	Keyboard grabber for console
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

#ifndef PIKBDLISTENER_H
#define PIKBDLISTENER_H

#include "pithread.h"

#define WAIT_FOR_EXIT while (!PIKbdListener::exiting) piMSleep(5);


class PIP_EXPORT PIKbdListener: public PIThread
{
	PIOBJECT_SUBCLASS(PIKbdListener, PIThread)
	friend class PIConsole;
	friend class PITerminal;
public:

	//! Special keyboard keys
	enum SpecialKey {
		Tab /** Tab key */ = 0x09,
		Return /** Enter key */ = 0x0a,
		Esc /** Escape key */ = 0x1b,
		Space /** Space key */ = 0x20,
		Backspace /** Backspace key */ = 0x7f,
		UpArrow /** Up arrow key */ = -1,
		DownArrow /** Down arrow key */ = -2,
		RightArrow /** Right arrow key */ = -3,
		LeftArrow /** Left arrow key */ = -4,
		Home /** Home key */ = -5,
		End /** End key */ = -6,
		PageUp /** Page up key */ = -7,
		PageDown /** Page down key */ = -8,
		Insert /** Delete key */ = -9,
		Delete /** Delete key */ = -10,
		F1  /** F1 key */ = -11,
		F2  /** F2 key */ = -12,
		F3  /** F3 key */ = -13,
		F4  /** F4 key */ = -14,
		F5  /** F5 key */ = -15,
		F6  /** F6 key */ = -16,
		F7  /** F7 key */ = -17,
		F8  /** F8 key */ = -18,
		F9  /** F9 key */ = -19,
		F10 /** F10 key */ = -20,
		F11 /** F11 key */ = -21,
		F12 /** F12 key */ = -22
	};

	//! Keyboard modifiers
	enum KeyModifier {
		Ctrl /** Control key */ = 0x1,
		Shift /** Shift key */ = 0x2,
		Alt /** Alt key */ = 0x4
		//Meta /** Meta (windows) key */ = 0x8
	};

	typedef PIFlags<KeyModifier> KeyModifiers;

	//! This struct contains information about pressed keyboard key
	struct KeyEvent {
		KeyEvent(int k = 0, KeyModifiers m = 0) {key = k; modifiers = m;}

		//! Pressed key. It can be simple \b char or special key (see PIKbdListener::SpecialKey)
		int key;

		//! Active keyboard modifiers. It contains PIKbdListener::KeyModifier bitfields
		KeyModifiers modifiers;
	};

	typedef void (*KBFunc)(KeyEvent, void * );


	//! Constructs keyboard listener with external function "slot" and custom data "data"
	explicit PIKbdListener(KBFunc slot = 0, void * data = 0, bool startNow = true);

	~PIKbdListener();


	//! Returns custom data
	void * data() {return kbddata_;}

	//! Set custom data to "_data"
	void setData(void * _data) {kbddata_ = _data;}

	//! Set external function to "slot"
	void setSlot(KBFunc slot) {ret_func = slot;}

	//! Returns if exit key if awaiting
	bool exitCaptured() const {return exit_enabled;}

	//! Returns exit key, default 'Q'
	int exitKey() const {return exit_key;}

	void readKeyboard();

	//! Returns if keyboard listening is active (not running!)
	bool isActive() {return is_active;}

	EVENT_HANDLER( void, enableExitCapture) {enableExitCapture('Q');}
	EVENT_HANDLER1(void, enableExitCapture, int, key) {exit_enabled = true; exit_key = key;}
	EVENT_HANDLER(void, disableExitCapture) {exit_enabled = false;}
	EVENT_HANDLER(void, setActive) {setActive(true);}
	EVENT_HANDLER1(void, setActive, bool, yes);

	EVENT2(keyPressed, PIKbdListener::KeyEvent, key, void * , data)

//! \handlers
//! \{

	//! \fn void enableExitCapture(int key = 'Q')
	//! \brief Enable exit key "key" awaiting

	//! \fn void disableExitCapture()
	//! \brief Disable exit key awaiting

	//! \fn void setActive(bool yes = true)
	//! \brief Set keyboard listening is active or not

//! \}
//! \events
//! \{

	//! \fn void keyPressed(PIKbdListener::KeyEvent key, void * data)
	//! \brief Raise on key "key" pressed, "data" is custom data

//! \}

	static bool exiting;
	static PIKbdListener * instance() {return _object;}

private:
	void begin();
	void run() {readKeyboard();}
	void end();

#ifndef WINDOWS
	struct EscSeq {
		const char * seq;
		int key;
		int mod;
		int vt;
		int flags;
		// 1 - shift
		// 2 - alt
		// 4 - ctrl
	};

	enum VTType {
		vt_none,
		vt_xterm = 0x1,
		vt_linux = 0x2,
		vt_all = 0xFF
	};

	static const EscSeq esc_seq[];
#endif

	PRIVATE_DECLARATION
#ifdef WINDOWS
	DWORD
#else
	int
#endif
		ret;
	KBFunc ret_func;
	int exit_key;
	bool exit_enabled, is_active;
	void * kbddata_;
	char rc[8];
	KeyEvent ke;
	static PIKbdListener * _object;

};


inline PIByteArray & operator <<(PIByteArray & s, const PIKbdListener::KeyEvent & v) {s << v.key << int(v.modifiers); return s;}

inline PIByteArray & operator >>(PIByteArray & s, PIKbdListener::KeyEvent & v) {int m(0); s >> v.key >> m; v.modifiers = m; return s;}


#endif // PIKBDLISTENER_H
