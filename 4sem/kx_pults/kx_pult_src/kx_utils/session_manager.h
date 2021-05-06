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

#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <QPair>
#include <QMainWindow>
#include <QCheckBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QAction>
#include <QComboBox>
#include <QTabWidget>
#include <QSplitter>
#include "spinslider.h"
#include "qpiconfig.h"

class SessionManager: public QObject
{
	Q_OBJECT
public:
	SessionManager(const QString & file = QString()) {setFile(file);}
	~SessionManager() {;}

	inline void setFile(const QString & file) {file_ = file;}
	
	inline void addEntry(QMainWindow * e) {mwindows.push_back(QPair<QString, QMainWindow * >(e->objectName(), e));}
	inline void addEntry(QCheckBox * e) {checks.push_back(QPair<QString, QCheckBox * >(e->objectName(), e));}
	inline void addEntry(QLineEdit * e) {lines.push_back(QPair<QString, QLineEdit * >(e->objectName(), e));}
	inline void addEntry(QComboBox * e) {combos.push_back(QPair<QString, QComboBox * >(e->objectName(), e));}
	inline void addEntry(QDoubleSpinBox * e) {dspins.push_back(QPair<QString, QDoubleSpinBox * >(e->objectName(), e));}
	inline void addEntry(QSpinBox * e) {spins.push_back(QPair<QString, QSpinBox * >(e->objectName(), e));}
	inline void addEntry(SpinSlider * e) {spinsliders.push_back(QPair<QString, SpinSlider * >(e->objectName(), e));}
	inline void addEntry(QTabWidget * e) {tabs.push_back(QPair<QString, QTabWidget * >(e->objectName(), e));}
	inline void addEntry(QAction * e) {actions.push_back(QPair<QString, QAction * >(e->objectName(), e));}
	inline void addMainWidget(QWidget * e) {widgets.push_back(QPair<QString, QWidget * >(e->objectName(), e));}
	
	inline void addEntry(const QString & name, QMainWindow * e) {mwindows.push_back(QPair<QString, QMainWindow * >(name, e));}
	inline void addEntry(const QString & name, QCheckBox * e) {checks.push_back(QPair<QString, QCheckBox * >(name, e));}
	inline void addEntry(const QString & name, QLineEdit * e) {lines.push_back(QPair<QString, QLineEdit * >(name, e));}
	inline void addEntry(const QString & name, QComboBox * e) {combos.push_back(QPair<QString, QComboBox * >(name, e));}
	inline void addEntry(const QString & name, QDoubleSpinBox * e) {dspins.push_back(QPair<QString, QDoubleSpinBox * >(name, e));}
	inline void addEntry(const QString & name, QSpinBox * e) {spins.push_back(QPair<QString, QSpinBox * >(name, e));}
	inline void addEntry(const QString & name, SpinSlider * e) {spinsliders.push_back(QPair<QString, SpinSlider * >(name, e));}
	inline void addEntry(const QString & name, QTabWidget * e) {tabs.push_back(QPair<QString, QTabWidget * >(name, e));}
	inline void addEntry(const QString & name, QAction * e) {actions.push_back(QPair<QString, QAction * >(name, e));}
	inline void addEntry(const QString & name, QStringList * e) {stringlists.push_back(QPair<QString, QStringList * >(name, e));}
	inline void addEntry(const QString & name, QString * e) {strings.push_back(QPair<QString, QString * >(name, e));}
	inline void addEntry(const QString & name, QColor * e) {colors.push_back(QPair<QString, QColor * >(name, e));}
	inline void addEntry(const QString & name, bool * e) {bools.push_back(QPair<QString, bool * >(name, e));}
	inline void addEntry(const QString & name, int * e) {ints.push_back(QPair<QString, int * >(name, e));}
	inline void addEntry(const QString & name, float * e) {floats.push_back(QPair<QString, float * >(name, e));}
	inline void addMainWidget(const QString & name, QWidget * e) {widgets.push_back(QPair<QString, QWidget * >(name, e));}

	void save();
	void load(bool onlyMainwindow = false);

private:
	QVector<QPair<QString, QMainWindow * > > mwindows;
	QVector<QPair<QString, QWidget * > > widgets;
	QVector<QPair<QString, QCheckBox * > > checks;
	QVector<QPair<QString, QLineEdit * > > lines;
	QVector<QPair<QString, QComboBox * > > combos;
	QVector<QPair<QString, QDoubleSpinBox * > > dspins;
	QVector<QPair<QString, QSpinBox * > > spins;
	QVector<QPair<QString, SpinSlider * > > spinsliders;
	QVector<QPair<QString, QTabWidget * > > tabs;
	QVector<QPair<QString, QAction * > > actions;
	QVector<QPair<QString, QStringList * > > stringlists;
	QVector<QPair<QString, QString * > > strings;
	QVector<QPair<QString, QColor * > > colors;
	QVector<QPair<QString, bool * > > bools;
	QVector<QPair<QString, int * > > ints;
	QVector<QPair<QString, float * > > floats;
	QString file_;

signals:
	void loading(QPIConfig & );
	void saving(QPIConfig & );

};

#endif // SESSION_MANAGER_H
