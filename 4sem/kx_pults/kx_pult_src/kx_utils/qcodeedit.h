#ifndef QCODEEDIT_H
#define QCODEEDIT_H

#include <QDebug>
#include <QPlainTextEdit>
#include <QTreeWidget>
#include <QScrollBar>
#include "iconedlabel.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QCodeEdit: public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QString text READ text WRITE setText)
	Q_PROPERTY(bool showSpaces READ showSpaces WRITE setShowSpaces)
	Q_PROPERTY(bool showLineNumbers READ showLineNumbers WRITE setShowLineNumbers)
	
public:
	QCodeEdit(QWidget * parent = 0);
	~QCodeEdit();
	
	enum ACClassType {
		Keyword,
		Function,
		Namespace
	};
	
	QTextCursor textCursor() const {return textCode->textCursor();}
	QTextDocument * document() const {return textCode->document();}
	void setTextCursor(const QTextCursor & c) {textCode->setTextCursor(c);}
	void centerCursor() {textCode->centerCursor(); updateLines();}
	void insertText(const QString & text) {textCode->insertPlainText(text); updateLines();}
	void appendText(const QString & text) {textCode->appendPlainText(text); updateLines();}
	void setCustomExtraSelection(const QList<QTextEdit::ExtraSelection> & es) {es_custom = es; applyExtraSelection();}
	QRect cursorRect() const {return textCode->cursorRect();}
	QRect cursorRect(const QTextCursor & cursor) const {return textCode->cursorRect(cursor);}
	QString text() const {return textCode->toPlainText();}
	QStringList cursorScope() const {return cursor_scope;}
	bool showSpaces() const {return spaces_;}
	bool showLineNumbers() const {return textLines->isVisible();}
	
	QPlainTextEdit * textEdit() const {return textCode;}
	
	void registerAutoCompletitionClass(int id, ACClassType ac_class, const QString & name, const QIcon & icon = QIcon()) {ac_classes[id] = ACClass(id, ac_class, name, icon);}
	
protected:
	typedef QPair<QString, QString> StringsPair;
	typedef QPair<int, QList<StringsPair> > ACPair;
	typedef QList<ACPair> ACList;
	
	virtual ACList autoCompletitionList(const QStringList & scope, const QString & written) const {return ACList();}
	virtual void parse() {}
	QString selectArg(QString s, int arg);
	void raiseHelp(QTextCursor tc, int arg = -1);
	void hideHelp();
	QTextCursor functionStart(QTextCursor tc, int * arg);
	ACList wordsCompletitionList(const QString & written) const;
	QPair<QStringList, QString> getScope(QTextCursor tc, bool * ok = 0);
	static int skipRange(const QString & s, int pos, QChar oc, QChar cc, QChar sc = QChar());
	static int skipCWord(const QString & s, int pos);
	static bool matchWritten(QString s, QString w);
	static QChar pairChar(QChar c);
	
private:
	struct ACClass {
		ACClass(int i = -2, ACClassType c = QCodeEdit::Keyword, const QString & n = QString(), const QIcon & ic = QIcon()): id(i), class_(c), name(n), icon(ic) {}
		int id;
		ACClassType class_;
		QString name;
		QIcon icon;
	};
	
	QPlainTextEdit * textCode, * textLines;
	QTreeWidget * completer;
	IconedLabel * lbl_help[2];
	QFrame * widget_help;
	QTextEdit::ExtraSelection es_line, es_cursor;
	QList<QTextEdit::ExtraSelection> es_selected, es_custom;
	QMap<int, ACClass> ac_classes;
	QStringList cursor_scope;
	int prev_lc, auto_comp_pl, timer;
	bool spaces_, _ignore_focus_out, _first, _destructor;
	
	bool eventFilter(QObject * o, QEvent * e);
	void showEvent(QShowEvent * );
	void timerEvent(QTimerEvent * );
	void applyExtraSelection();
	void nextCompletition();
	void previousCompletition();
	
private slots:
	void syncScrolls();
	void deleteLine();
	void copyLineUp();
	void copyLineDown();
	void moveLineUp();
	void moveLineDown();
	void indent();
	void deindent();
	void autoIndent();
	void invokeAutoCompletition(bool force = false);
	void commitCompletition();
	void textEdit_cursorPositionChanged();
	void textEdit_textChanged();
	void textEdit_selectionChanged();
	
public slots:
	void updateLines();
	void scrollToTop();
	void setFocus() {textCode->setFocus();}
	void setText(const QString & t) {textCode->setPlainText(t);}
	void setShowSpaces(bool yes);
	void setShowLineNumbers(bool yes) {textLines->setVisible(yes);}
	
signals:
	void textChanged();
	void parseRequest();
	
};

//Q_DECLARE_OPERATORS_FOR_FLAGS(QPIConsole::Formats)

QT_END_NAMESPACE

QT_END_HEADER

#endif // QCODEEDIT_H
