/*
    Peri4 Paint
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

#include "session_manager.h"


void SessionManager::save() {
	if (file_.isEmpty()) return;
	QPIConfig sr(file_);
	for (int i = 0; i < mwindows.size(); ++i) {
		sr.setValue(mwindows[i].first + " state", mwindows[i].second->saveState(), false);
		sr.setValue(mwindows[i].first + " window state", (int)mwindows[i].second->windowState(), false);
		sr.setValue(mwindows[i].first + " geometry " + QString::number((int)mwindows[i].second->windowState()), mwindows[i].second->saveGeometry(), false);
		QList<QSplitter * > sp = mwindows[i].second->findChildren<QSplitter * >();
		foreach (QSplitter * s, sp)
			sr.setValue(mwindows[i].first + " splitter " + s->objectName(), s->saveState(), false);
	}
	for (int i = 0; i < widgets.size(); ++i) {
		sr.setValue(widgets[i].first + " geometry " + QString::number((int)widgets[i].second->windowState()), widgets[i].second->saveGeometry(), false);
		sr.setValue(widgets[i].first + " window state", (int)widgets[i].second->windowState(), false);
	}
	for (int i = 0; i < checks.size(); ++i)
		sr.setValue(checks[i].first, checks[i].second->isChecked(), false);
	for (int i = 0; i < lines.size(); ++i)
		sr.setValue(lines[i].first, lines[i].second->text(), "s", false);
	for (int i = 0; i < combos.size(); ++i)
		sr.setValue(combos[i].first, combos[i].second->currentIndex(), false);
	for (int i = 0; i < dspins.size(); ++i)
		sr.setValue(dspins[i].first, dspins[i].second->value(), false);
	for (int i = 0; i < spins.size(); ++i)
		sr.setValue(spins[i].first, spins[i].second->value(), false);
	for (int i = 0; i < spinsliders.size(); ++i)
		sr.setValue(spinsliders[i].first, spinsliders[i].second->value(), false);
	for (int i = 0; i < tabs.size(); ++i)
		sr.setValue(tabs[i].first, tabs[i].second->currentIndex(), false);
	for (int i = 0; i < actions.size(); ++i)
		sr.setValue(actions[i].first, actions[i].second->isChecked(), false);
	for (int i = 0; i < stringlists.size(); ++i)
		sr.setValue(stringlists[i].first, *stringlists[i].second, false);
	for (int i = 0; i < strings.size(); ++i)
		sr.setValue(strings[i].first, *strings[i].second, "s", false);
	for (int i = 0; i < colors.size(); ++i)
		sr.setValue(colors[i].first, *colors[i].second, false);
	for (int i = 0; i < bools.size(); ++i)
		sr.setValue(bools[i].first, *bools[i].second, false);
	for (int i = 0; i < ints.size(); ++i)
		sr.setValue(ints[i].first, *ints[i].second, false);
	for (int i = 0; i < floats.size(); ++i)
		sr.setValue(floats[i].first, *floats[i].second, false);
	sr.writeAll();
	emit saving(sr);
}


void SessionManager::load(bool onlyMainwindow) {
	if (file_.isEmpty()) return;
	QPIConfig sr(file_);
	for (int i = 0; i < mwindows.size(); ++i) {
		mwindows[i].second->restoreState(sr.getValue(mwindows[i].first + " state", QByteArray()));
		mwindows[i].second->restoreGeometry(sr.getValue(mwindows[i].first + " geometry " + QString::number((int)mwindows[i].second->windowState()), QByteArray()));
		mwindows[i].second->setWindowState((Qt::WindowState)(int)sr.getValue(mwindows[i].first + " window state", 0));
		QList<QSplitter * > sp = mwindows[i].second->findChildren<QSplitter * >();
		foreach (QSplitter * s, sp)
			s->restoreState(sr.getValue(mwindows[i].first + " splitter " + s->objectName(), QByteArray()));
	}
	for (int i = 0; i < widgets.size(); ++i) {
		widgets[i].second->restoreGeometry(sr.getValue(widgets[i].first + " geometry " + QString::number((int)widgets[i].second->windowState()), QByteArray()));
		widgets[i].second->setWindowState((Qt::WindowState)(int)sr.getValue(widgets[i].first + " window state", 0));
	}
	if (onlyMainwindow) return;
	for (int i = 0; i < checks.size(); ++i)
		checks[i].second->setChecked(sr.getValue(checks[i].first, checks[i].second->isChecked()));
	for (int i = 0; i < lines.size(); ++i)
		lines[i].second->setText(sr.getValue(lines[i].first, lines[i].second->text()));
	for (int i = 0; i < combos.size(); ++i)
		combos[i].second->setCurrentIndex(sr.getValue(combos[i].first, combos[i].second->currentIndex()));
	for (int i = 0; i < dspins.size(); ++i)
		dspins[i].second->setValue(sr.getValue(dspins[i].first, dspins[i].second->value()));
	for (int i = 0; i < spins.size(); ++i)
		spins[i].second->setValue(sr.getValue(spins[i].first, spins[i].second->value()));
	for (int i = 0; i < spinsliders.size(); ++i)
		spinsliders[i].second->setValue(sr.getValue(spinsliders[i].first, spinsliders[i].second->value()));
	for (int i = 0; i < tabs.size(); ++i)
		tabs[i].second->setCurrentIndex(sr.getValue(tabs[i].first, tabs[i].second->currentIndex()));
	for (int i = 0; i < actions.size(); ++i)
		actions[i].second->setChecked(sr.getValue(actions[i].first, actions[i].second->isChecked()));
	for (int i = 0; i < stringlists.size(); ++i)
		*stringlists[i].second = sr.getValue(stringlists[i].first, *stringlists[i].second);
	for (int i = 0; i < strings.size(); ++i)
		*strings[i].second = sr.getValue(strings[i].first, *strings[i].second).stringValue();
	for (int i = 0; i < colors.size(); ++i)
		*colors[i].second = sr.getValue(colors[i].first, *colors[i].second);
	for (int i = 0; i < bools.size(); ++i)
		*bools[i].second = sr.getValue(bools[i].first, *bools[i].second);
	for (int i = 0; i < ints.size(); ++i)
		*ints[i].second = sr.getValue(ints[i].first, *ints[i].second);
	for (int i = 0; i < floats.size(); ++i)
		*floats[i].second = sr.getValue(floats[i].first, *floats[i].second);
	emit loading(sr);
}
