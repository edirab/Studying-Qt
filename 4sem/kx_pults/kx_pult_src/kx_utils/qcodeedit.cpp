#include "qcodeedit.h"
#include <QLayout>
#include <QBoxLayout>
#include <QScrollBar>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextOption>
#include <QTextCursor>
#include <QTextBlock>
#include <QAction>
#include <QApplication>
#include <QHeaderView>


QCodeEdit::QCodeEdit(QWidget * parent): QWidget(parent) {
	prev_lc = auto_comp_pl = -1;
	textCode = textLines = 0;
	
	timer = 0;
	_ignore_focus_out = _destructor = false;
	_first = true;
	es_line.format.setBackground(QColor(240, 245, 240));
	es_line.format.setProperty(QTextFormat::FullWidthSelection, true);
	es_cursor.format.setBackground(QColor(220, 255, 200));
	widget_help = new QFrame();
	widget_help->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
	widget_help->setFocusPolicy(Qt::NoFocus);
	widget_help->setFrameShadow(QFrame::Sunken);
	widget_help->setFrameShape(QFrame::StyledPanel);
	widget_help->setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
	widget_help->layout()->setContentsMargins(0, 0, 0, 0);
	for (int i = 0; i < 2; ++i) {
		lbl_help[i] = new IconedLabel();
		lbl_help[i]->setFrameShadow(QFrame::Plain);
		lbl_help[i]->setFrameShape(QFrame::NoFrame);
		lbl_help[i]->setDirection(IconedLabel::RightToLeft);
		widget_help->layout()->addWidget(lbl_help[i]);
	}
	lbl_help[1]->setIcon(QIcon(":/icons/f1.png"));
	lbl_help[1]->setText(trUtf8("Press F1 for details"));
	completer = new QTreeWidget();
	completer->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
	completer->setFocusPolicy(Qt::NoFocus);
	completer->setColumnCount(2);
	completer->setRootIsDecorated(false);
	completer->setHeaderHidden(true);
	completer->header()->setDefaultAlignment(Qt::AlignCenter);
	completer->header()->
#if (QT_VERSION >= 0x050000)
			setSectionResizeMode
#else
			setResizeMode
#endif
				(QHeaderView::ResizeToContents);
	completer->header()->setStretchLastSection(true);
	//completer->setColumnWidth(0, 180);
	completer->resize(500, 200);
	textCode = new QPlainTextEdit();
	textLines = new QPlainTextEdit();
	textCode->setFrameShadow(QFrame::Plain);
	textCode->setFrameShape(QFrame::NoFrame);
	textCode->setLineWrapMode(QPlainTextEdit::NoWrap);
	textCode->setTabChangesFocus(false);
	textLines->setFrameShadow(QFrame::Plain);
	textLines->setFrameShape(QFrame::NoFrame);
	textLines->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	textLines->setFocusPolicy(Qt::NoFocus);
	textLines->setTextInteractionFlags(Qt::NoTextInteraction);
	textLines->setLineWrapMode(QPlainTextEdit::NoWrap);
	textLines->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	textLines->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	textLines->viewport()->setAutoFillBackground(false);
	textLines->viewport()->setCursor(Qt::ArrowCursor);
	textLines->setFixedWidth(textLines->fontMetrics().width("  "));
	setLayout(new QBoxLayout(QBoxLayout::BottomToTop));
	layout()->setContentsMargins(0, 0, 0, 0);
	QFrame * frame = new QFrame();
	frame->setFrameShadow(QFrame::Sunken);
	frame->setFrameShape(QFrame::StyledPanel);
	frame->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));
	frame->layout()->setContentsMargins(0, 0, 0, 0);
	frame->layout()->setSpacing(0);
	frame->layout()->addWidget(textLines);
	frame->layout()->addWidget(textCode);
	layout()->addWidget(frame);
	
	QAction * a = new QAction(this);
	a->setShortcut(QKeySequence("Shift+Tab"));
	a->setShortcutContext(Qt::WidgetShortcut);
	connect(a, SIGNAL(triggered(bool)), this, SLOT(deindent()));
	textCode->addAction(a);
	a = new QAction(this);
	a->setShortcut(QKeySequence("Ctrl+D"));
	a->setShortcutContext(Qt::WidgetShortcut);
	connect(a, SIGNAL(triggered(bool)), this, SLOT(deleteLine()));
	textCode->addAction(a);
	frame->setFocusProxy(textCode);
	QTextOption to = textLines->document()->defaultTextOption();
	to.setAlignment(Qt::AlignTop | Qt::AlignRight);
	textLines->document()->setDefaultTextOption(to);
	setShowSpaces(true);
	
	connect(completer, SIGNAL(itemDoubleClicked(QTreeWidgetItem * ,int)), this, SLOT(commitCompletition()));
	connect(textCode->verticalScrollBar(), SIGNAL(valueChanged(int)), textLines->verticalScrollBar(), SLOT(setValue(int)));
	connect(textCode, SIGNAL(textChanged()), this, SLOT(updateLines()));
	connect(textCode, SIGNAL(textChanged()), this, SIGNAL(textChanged()));
	connect(textCode, SIGNAL(cursorPositionChanged()), this, SLOT(textEdit_cursorPositionChanged()));
	connect(textCode, SIGNAL(selectionChanged()), this, SLOT(textEdit_selectionChanged()));
	updateLines();
	
	registerAutoCompletitionClass(-1, QCodeEdit::Keyword, "Words", QIcon(":/icons/code-word.png"));
}


QCodeEdit::~QCodeEdit() {
	_destructor = true;
	delete textCode;
	delete textLines;
	delete completer;
	//for (int i = 0; i < 2; ++i)
	//	delete lbl_help[i];
	delete widget_help;
}


int QCodeEdit::skipRange(const QString & s, int pos, QChar oc, QChar cc, QChar sc) {
	int cnt = 0;
	bool skip = false;
	for (int i = pos - 1; i >= 0; --i) {
		QChar c = s[i];
		if (skip) {skip = false; continue;}
		if (c == sc) {skip = true; continue;}
		if (c == cc) {cnt++; continue;}
		if (c == oc) {cnt--; if (cnt == 0) return i;}
	}
	return -1;
}


int QCodeEdit::skipCWord(const QString & s, int pos) {
	QChar pc(0), c(0);
	for (int i = pos - 1; i >= 0; --i) {
		pc = c;
		c = s[i];
		if (c.isLetterOrNumber() || (c.toLatin1() == '_')) continue;
		if (pc.isLetter() || (pc.toLatin1() == '_')) return i + 1;
		return -1;
	}
	return -1;
}


bool QCodeEdit::matchWritten(QString s, QString w) {
	if (s.isEmpty() || w.isEmpty()) return true;
	if (s.contains(w, Qt::CaseInsensitive)) return true;
	int sp(0);
	for (int i = 0; i < w.size(); ++i, ++sp) {
		if (sp >= s.size()) return false;
		QChar wc(w[i].toLower());
		bool ns = false, bl = true;
		while (sp < s.size()) {
			if (ns || s[sp].toLatin1() == '_') {
				if (s[sp].toLatin1() == '_') {sp++; bl = false; continue;}
				if (s[sp].isLower() && bl) {sp++; continue;}
				if (s[sp].toLower() != wc) return false;
			}
			if (s[sp].toLower() == wc) break;
			ns = true;
			sp++;
		}
		if (sp >= s.size()) return false;
	}
	return true;
}


QChar QCodeEdit::pairChar(QChar c) {
	switch (c.toLatin1()) {
	case '\"': return '\"';
	case '(': return ')';
	case ')': return '(';
	case '[': return ']';
	case ']': return '[';
	default: break;
	}
	return QChar();
}


bool QCodeEdit::eventFilter(QObject * o, QEvent * e) {
	if (_destructor) return QWidget::eventFilter(o, e);
	if (e->type() == QEvent::Destroy) {
		completer->removeEventFilter(this);
		textCode->removeEventFilter(this);
		textCode->viewport()->removeEventFilter(this);
		textLines->viewport()->removeEventFilter(this);
		return QWidget::eventFilter(o, e);
	}
	if (textLines) {
		if (o == textLines->viewport()) {
			if (e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonRelease ||
				e->type() == QEvent::MouseMove || e->type() == QEvent::MouseButtonDblClick) {
#if (QT_VERSION < 0x050000)
				const_cast<QPoint&>(((QMouseEvent*)e)->pos()) = QPoint(0, ((QMouseEvent*)e)->pos().y());
#endif
				QApplication::sendEvent(textCode->viewport(), e);
				return true;
			}
			if (e->type() == QEvent::Wheel) {
				QApplication::sendEvent(textCode->viewport(), e);
				return true;
			}
		}
	}
	if (o == completer) {
		//qDebug() << o << e;
		if (e->type() == QEvent::WindowActivate)
			_ignore_focus_out = true;
		//qDebug() << e;
		return QWidget::eventFilter(o, e);
	}
	if (textCode) {
		if (o == textCode->viewport()) {
			if (e->type() == QEvent::MouseButtonPress) {
				completer->hide();
				hideHelp();
			}
			if (e->type() == QEvent::ToolTip) {
				QTextCursor tc = textCode->cursorForPosition(((QHelpEvent*)e)->pos());
				tc.select(QTextCursor::WordUnderCursor);
				raiseHelp(tc);
			}
			return QWidget::eventFilter(o, e);
		}
		if (o == textCode) {
			//qDebug() << o << e;
			QMetaObject::invokeMethod(this, "syncScrolls", Qt::QueuedConnection);
			QKeyEvent * ke;
			QChar kc(0);
			switch (e->type()) {
			case QEvent::KeyPress:
				ke = (QKeyEvent * )e;
				switch (ke->key()) {
				case Qt::Key_Space:
					if (ke->modifiers().testFlag(Qt::ControlModifier)) {
						invokeAutoCompletition(true);
						return true;
					}
					break;
				case Qt::Key_Escape:
					completer->hide();
					hideHelp();
					break;
				case Qt::Key_Up:
					if (completer->isVisible()) {
						previousCompletition();
						return true;
					}
					completer->hide();
					hideHelp();
					if (ke->modifiers().testFlag(Qt::AltModifier)) {
						copyLineUp();
						return true;
					}
					if (ke->modifiers().testFlag(Qt::ControlModifier) && ke->modifiers().testFlag(Qt::ShiftModifier)) {
						moveLineUp();
						return true;
					}
					break;
				case Qt::Key_Down:
					if (completer->isVisible()) {
						nextCompletition();
						return true;
					}
					completer->hide();
					hideHelp();
					if (ke->modifiers().testFlag(Qt::AltModifier)) {
						copyLineDown();
						return true;
					}
					if (ke->modifiers().testFlag(Qt::ControlModifier) && ke->modifiers().testFlag(Qt::ShiftModifier)) {
						moveLineDown();
						return true;
					}
					break;
				case Qt::Key_Home:
				case Qt::Key_End:
				case Qt::Key_PageUp:
				case Qt::Key_PageDown:
					if (completer->isVisible()) {
						qApp->sendEvent(completer, new QKeyEvent(e->type(), ke->key(), ke->modifiers()));
						return true;
					}
					break;
				case Qt::Key_Left:
				case Qt::Key_Right:
				case Qt::Key_Backspace:
				case Qt::Key_Delete:
					if (completer->isVisible())
						QMetaObject::invokeMethod(this, "invokeAutoCompletition", Qt::QueuedConnection, Q_ARG(bool, false));
					break;
				case Qt::Key_Return:
					if (completer->isVisible()) {
						commitCompletition();
						completer->hide();
						return true;
					}
					if (textCode->textCursor().selectedText().isEmpty())
						QMetaObject::invokeMethod(this, "autoIndent", Qt::QueuedConnection);
					break;
				case Qt::Key_Tab:
					if (!textCode->textCursor().selectedText().isEmpty()) {
						if (ke->modifiers().testFlag(Qt::ShiftModifier))
							deindent();
						else
							indent();
						return true;
					}
					break;
				case Qt::Key_D:
					if (ke->modifiers().testFlag(Qt::ControlModifier)) {
						completer->hide();
						return true;
					}
					break;
				default: break;
				}
				if (!ke->text().isEmpty())
					kc = ke->text()[0];
				if (kc == '.') {
					completer->hide();
					QMetaObject::invokeMethod(this, "invokeAutoCompletition", Qt::QueuedConnection, Q_ARG(bool, false));
				} else {
					if ((kc.isLetterOrNumber() || kc.toLatin1() == '_') && completer->isVisible())
						QMetaObject::invokeMethod(this, "invokeAutoCompletition", Qt::QueuedConnection, Q_ARG(bool, false));
				}
				break;
			case QEvent::FocusOut:
				if (_ignore_focus_out) {
					_ignore_focus_out = false;
					break;
				}
			case QEvent::Hide:
			case QEvent::HideToParent:
			case QEvent::MouseButtonPress:
				//qDebug() << e;
				completer->hide();
				hideHelp();
			default: break;
			}
		}
	}
	return QWidget::eventFilter(o, e);
}


void QCodeEdit::showEvent(QShowEvent * ) {
	if (!_first) return;
	_first = false;
	completer->installEventFilter(this);
	textCode->installEventFilter(this);
	textCode->viewport()->installEventFilter(this);
	textLines->viewport()->installEventFilter(this);
}


void QCodeEdit::timerEvent(QTimerEvent * ) {
	parse();
	emit parseRequest();
	killTimer(timer);
	timer = 0;
}


void QCodeEdit::applyExtraSelection() {
	textCode->setExtraSelections(QList<QTextEdit::ExtraSelection>() << es_line << es_selected << es_custom << es_cursor);
}


void QCodeEdit::nextCompletition() {
	int ci = completer->currentIndex().row();
	if (ci >= completer->topLevelItemCount() - 1) return;
	if (completer->topLevelItem(ci + 1)->flags().testFlag(Qt::ItemIsSelectable))
		completer->setCurrentItem(completer->topLevelItem(ci + 1));
	else {
		if (ci >= completer->topLevelItemCount() - 2) return;
		completer->setCurrentItem(completer->topLevelItem(ci + 2));
	}
}


void QCodeEdit::previousCompletition() {
	int ci = completer->currentIndex().row();
	if (ci <= 0) return;
	if (completer->topLevelItem(ci - 1)->flags().testFlag(Qt::ItemIsSelectable))
		completer->setCurrentItem(completer->topLevelItem(ci - 1));
	else {
		if (ci <= 1) return;
		completer->setCurrentItem(completer->topLevelItem(ci - 2));
	}
}


void QCodeEdit::syncScrolls() {
	textLines->verticalScrollBar()->setValue(textCode->verticalScrollBar()->value());
}


void QCodeEdit::deleteLine() {
	QTextCursor tc = textCode->textCursor();
	tc.movePosition(QTextCursor::EndOfLine);
	tc.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
	bool md = true;
	if (!tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor)) {
		tc.movePosition(QTextCursor::StartOfLine);
		tc.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
		tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
		md = false;
	}
	tc.removeSelectedText();
	tc.movePosition(QTextCursor::StartOfLine);
	if (md) tc.movePosition(QTextCursor::Down);
	textCode->setTextCursor(tc);
}


void QCodeEdit::copyLineUp() {
	QTextCursor tc = textCode->textCursor();
	int ss = tc.selectionStart(), ss_ = ss, se = tc.selectionEnd(), se_ = se;
	QString st_ = tc.selection().toPlainText();
	if (st_.endsWith("\n")) {
		st_.chop(1);
		se--; se_--;
	}
	tc.setPosition(ss); tc.movePosition(QTextCursor::StartOfLine); ss = tc.position();
	tc.setPosition(se); tc.movePosition(QTextCursor::EndOfLine); se = tc.position();
	tc.setPosition(ss); tc.setPosition(se, QTextCursor::KeepAnchor);
	bool ins_nl = false;
	if (!tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor))
		ins_nl = true;
	QString l = tc.selectedText();
	tc.beginEditBlock();
	tc.setPosition(ss);
	if (ins_nl)
		l.append("\n");
	tc.insertText(l);
	tc.setPosition(ss_);
	tc.setPosition(se_, QTextCursor::KeepAnchor);
	tc.endEditBlock();
	textCode->setTextCursor(tc);
}


void QCodeEdit::copyLineDown() {
	QTextCursor tc = textCode->textCursor();
	int ss = tc.selectionStart(), ss_ = ss, se = tc.selectionEnd(), se_ = se;
	QString st_ = tc.selection().toPlainText();
	if (st_.endsWith("\n")) {
		st_.chop(1);
		se--; se_--;
	}
	tc.setPosition(ss); tc.movePosition(QTextCursor::StartOfLine); ss = tc.position();
	tc.setPosition(se); tc.movePosition(QTextCursor::EndOfLine); se = tc.position();
	tc.setPosition(ss); tc.setPosition(se, QTextCursor::KeepAnchor);
	bool ins_nl = false;
	if (!tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor))
		ins_nl = true;
	QString l = tc.selectedText();
	tc.beginEditBlock();
	tc.setPosition(ss);
	ss_ += l.size(); se_ += l.size();
	if (ins_nl) {
		l.append("\n");
		ss_++; se_++;
	}
	tc.insertText(l);
	tc.setPosition(ss_);
	tc.setPosition(se_, QTextCursor::KeepAnchor);
	tc.endEditBlock();
	textCode->setTextCursor(tc);
}


void QCodeEdit::moveLineUp() {
	QTextCursor tc = textCode->textCursor();
	int ss = tc.selectionStart(), ss_ = ss, se = tc.selectionEnd(), se_ = se;
	QString st_ = tc.selection().toPlainText();
	if (st_.endsWith("\n")) {
		st_.chop(1);
		se--; se_--;
	}
	tc.setPosition(ss); tc.movePosition(QTextCursor::StartOfLine); ss = tc.position();
	tc.setPosition(se); tc.movePosition(QTextCursor::EndOfLine); se = tc.position();
	tc.setPosition(ss);
	if (!tc.movePosition(QTextCursor::Up))
		return;
	tc.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor);
	QString l = tc.selectedText();
	ss -= l.size(); se -= l.size();
	ss_ -= l.size(); se_ -= l.size();
	tc.beginEditBlock();
	tc.removeSelectedText();
	tc.setPosition(se);
	bool de = false;
	if (!tc.movePosition(QTextCursor::Right)) {
		l.prepend("\n");
		de = true;
	}
	tc.insertText(l);
	if (de) {
		tc.movePosition(QTextCursor::End);
		tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
		tc.removeSelectedText();
	}
	tc.setPosition(ss_);
	tc.setPosition(se_, QTextCursor::KeepAnchor);
	tc.endEditBlock();
	textCode->setTextCursor(tc);
}


void QCodeEdit::moveLineDown() {
	QTextCursor tc = textCode->textCursor();
	int ss = tc.selectionStart(), ss_ = ss, se = tc.selectionEnd(), se_ = se;
	QString st_ = tc.selection().toPlainText();
	if (st_.endsWith("\n")) {
		st_.chop(1);
		se--; se_--;
	}
	tc.setPosition(ss); tc.movePosition(QTextCursor::StartOfLine); ss = tc.position();
	tc.setPosition(se); tc.movePosition(QTextCursor::EndOfLine); se = tc.position();
	tc.setPosition(se);
	if (!tc.movePosition(QTextCursor::Right))
		return;
	bool de = false;
	if (!tc.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor)) {
		tc.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
		de = true;
	}
	QString l = tc.selectedText();
	tc.beginEditBlock();
	tc.removeSelectedText();
	tc.setPosition(ss);
	if (de) l += "\n";
	tc.insertText(l);
	if (de) {
		tc.movePosition(QTextCursor::End);
		tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
		tc.removeSelectedText();
	}
	ss += l.size(); se += l.size();
	ss_ += l.size(); se_ += l.size();
	tc.setPosition(ss_);
	tc.setPosition(se_, QTextCursor::KeepAnchor);
	tc.endEditBlock();
	textCode->setTextCursor(tc);
}


void QCodeEdit::indent() {
	QTextCursor tc = textCode->textCursor();
	int ss = tc.selectionStart(), ss_ = ss, se = tc.selectionEnd(), se_ = se;
	QString st_ = tc.selection().toPlainText();
	if (st_.endsWith("\n")) {
		st_.chop(1);
		se--; se_--;
	}
	tc.setPosition(ss); tc.movePosition(QTextCursor::StartOfLine); ss = tc.position();
	tc.setPosition(se); tc.movePosition(QTextCursor::EndOfLine); se = tc.position();
	tc.setPosition(ss);
	while (tc.position() < se_) {
		tc.insertText("\t");
		se_++;
		tc.movePosition(QTextCursor::StartOfLine);
		if (!tc.movePosition(QTextCursor::Down))
			break;
	}
	tc.setPosition(ss_ + 1);
	tc.setPosition(se_, QTextCursor::KeepAnchor);
	textCode->setTextCursor(tc);
}


void QCodeEdit::deindent() {
	QTextCursor tc = textCode->textCursor();
	int ss = tc.selectionStart(), ss_ = ss, se = tc.selectionEnd(), se_ = se;
	QString st_ = tc.selection().toPlainText();
	if (st_.endsWith("\n")) {
		st_.chop(1);
		se--; se_--;
	}
	tc.setPosition(ss); tc.movePosition(QTextCursor::StartOfLine); ss = tc.position();
	tc.setPosition(se); tc.movePosition(QTextCursor::EndOfLine); se = tc.position();
	tc.setPosition(ss);
	bool first = true;
	while (tc.position() < se_) {
		tc.movePosition(QTextCursor::StartOfLine);
		tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
		int rs = 0;
		if (tc.selectedText() == "\t") {
			tc.removeSelectedText();
			rs = 1;
		} else {
			for (int i = 0; i < 4; ++i) {
				tc.movePosition(QTextCursor::StartOfLine);
				tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
				if (tc.selectedText() == " ") {
					tc.removeSelectedText();
					rs++;
				}
			}
		}
		if (first) {
			first = false;
			ss_ -= rs;
		}
		se_ -= rs;
		tc.movePosition(QTextCursor::StartOfLine);
		if (!tc.movePosition(QTextCursor::Down))
			break;
	}
	tc.setPosition(ss_);
	tc.setPosition(se_, QTextCursor::KeepAnchor);
	textCode->setTextCursor(tc);
}


void QCodeEdit::autoIndent() {
	QTextCursor tc = textCode->textCursor(), stc = tc;
	tc.movePosition(QTextCursor::StartOfLine);
	if (!tc.movePosition(QTextCursor::Up)) return;
	tc.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor);
	QString line = tc.selectedText(), tabs;
	int i = 0;
	for (; i < line.size(); ++i)
		if (!line[i].isSpace()) {
			tabs = line.left(i);
			break;
		}
	if (i >= line.size())
		tabs = line.left(line.size() - 1);
	int nt = qMax<int>(0, line.count(QChar('{')) - line.count(QChar('}')));
	tabs.append(QString("\t").repeated(nt));
	if (tabs.isEmpty()) return;
	stc.insertText(tabs);
	textCode->setTextCursor(stc);
}


void QCodeEdit::scrollToTop() {
	prev_lc = -1;
	updateLines();
	textCode->verticalScrollBar()->setValue(0);
	textLines->verticalScrollBar()->setValue(0);
}


void QCodeEdit::updateLines() {
	if (timer > 0) killTimer(timer);
	timer = startTimer(500);
	textCode->setTabStopWidth(textCode->fontMetrics().width("    "));
	int lc = textCode->document()->lineCount();
	if (prev_lc == lc) return;
	prev_lc = lc;
	textLines->setFixedWidth(textLines->fontMetrics().width(QString(" %1").arg(lc)));
	textLines->clear();
	for (int i = 1; i <= lc; ++i)
		textLines->appendPlainText(QString("%1").arg(i));
	textLines->verticalScrollBar()->setValue(textCode->verticalScrollBar()->value());
}


QString QCodeEdit::selectArg(QString s, int arg) {
	if (!s.contains('(') || arg < 0) return s;
	QString ss = s.left(s.indexOf('('));
	s.remove(0, ss.size());
	if (s.startsWith('(')) s.remove(0, 1);
	if (s.endsWith(')')) s.chop(1);
	QStringList al = s.split(",");
	QString ret = ss + "(";
	for (int i = 0; i < al.size(); ++i) {
		if (i > 0) ret += ", ";
		if (i == arg) ret += "<span style=\"font-weight:600;\">";
		ret += al[i].trimmed();
		if (i == arg) ret += "</span>";
	}
	ret += ")";
	return ret;
}


void QCodeEdit::raiseHelp(QTextCursor tc, int arg) {
	bool ok;
	QPair<QStringList, QString> scope = getScope(tc, &ok);
	//qDebug() << "help" << scope;
	QString st = tc.selectedText();
	if (arg >= 0) st = scope.second;
	if (!ok || st.isEmpty()) {
		hideHelp();
		return;
	}
	ok = false;
	ACList acl(autoCompletitionList(scope.first, scope.second));
	foreach (const ACPair & i, acl) {
		foreach (const StringsPair & s, i.second) {
			QString ts = s.second;
			//qDebug() << ts << st;
			if (ts != st) {
				if (ts.startsWith(st)) {
					ts.remove(0, st.size());
					ts = ts.trimmed();
					if (!ts.isEmpty()) {
						if (ts[0] != '(')
							continue;
					}
				} else
					continue;
			}
			//qDebug() << s.second << st;
			ACClass acc = ac_classes.value(i.first);
			lbl_help[0]->setIcon(acc.icon);
			lbl_help[0]->setText(QString("<html><body>[%1]  %2  %3</html></body>").arg(acc.name, s.first, selectArg(s.second, arg)));
			ok = true;
			break;
		}
		if (ok) break;
	}
	if (!ok) {
		hideHelp();
		return;
	}
	es_cursor.cursor = tc;
	applyExtraSelection();
	tc.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, st.size());
	lbl_help[0]->setFont(font());
	widget_help->resize(widget_help->sizeHint());
	widget_help->move(textCode->mapToGlobal(textCode->cursorRect(tc).topLeft() - QPoint(0, widget_help->height() + 8)));
	widget_help->show();
	cursor_scope = scope.first;
	cursor_scope << scope.second;
	//qDebug() << "tooltip" << st;
}


void QCodeEdit::hideHelp() {
	widget_help->hide();
	es_cursor.cursor = QTextCursor();
	cursor_scope.clear();
	applyExtraSelection();
}


QTextCursor QCodeEdit::functionStart(QTextCursor tc, int * arg) {
	QString doc = textCode->toPlainText();
	int bcnt = 0, a = 0, i = -1;
	for (i = tc.position() - 1; i >= 0; --i) {
		if (doc[i] == ')') bcnt++;
		if (doc[i] == '(') {
			if (bcnt == 0)
				break;
			else
				bcnt--;
		}
		//if (doc[i] == '(') bcnt--;
		if (doc[i] == ',' && bcnt == 0) a++;
	}
	if (i < 0) return QTextCursor();
	if (arg) *arg = a;
	QTextCursor ret(textCode->document());
	ret.setPosition(i);
	//qDebug() << "found" << i << a;
	return ret;
}


QCodeEdit::ACList QCodeEdit::wordsCompletitionList(const QString & written) const {
	QCodeEdit::ACList ret;
	if (!written.isEmpty()) {
		QTextCursor tc = QTextCursor(textCode->document()->begin()), stc;
		QStringList acwl;
		tc = QTextCursor(textCode->document()->begin());
		while (true) {
			tc = textCode->document()->find(written, tc);
			if (tc.isNull()) break;
			stc = tc;
			stc.movePosition(QTextCursor::Left);
			stc.select(QTextCursor::WordUnderCursor);
			if (!stc.selectedText().isEmpty() && stc.selectedText().trimmed() != written)
				acwl << stc.selectedText();
		}
		acwl.removeDuplicates();
		ACPair acl;
		acl.first = -1;
		foreach (const QString & s, acwl)
			acl.second << StringsPair("", s);
		ret << acl;
	}
	return ret;
}


QPair<QStringList, QString> QCodeEdit::getScope(QTextCursor tc, bool * ok) {
	QPair<QStringList, QString> ret;
	QTextCursor stc = tc;
	if (tc.isNull()) {
		completer->hide();
		if (ok) *ok = false;
		return ret;
	}
	int line = tc.block().firstLineNumber();
	if (completer->isVisible()) {
		if (auto_comp_pl != line) {
			completer->hide();
			auto_comp_pl = line;
			if (ok) *ok = false;
			return ret;
		}
	}
	QString doc = textCode->toPlainText();
	auto_comp_pl = line;
	completer->clear();
	int spos = tc.position(), cpos = spos;
	tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
	QStringList scope;
	QString written = tc.selectedText().trimmed();
	//qDebug() << "\n*** invokeAutoCompletition ***";
	if (written != "_" && !written.leftJustified(1)[0].isLetterOrNumber()) {
		written.clear();
	} else {
		cpos = skipCWord(doc, spos);
		if (cpos >= 0)
			written = doc.mid(cpos, spos - cpos).trimmed();
	}
	while (cpos >= 0) {
		cpos--;
		//qDebug() << "char =" << doc.mid(cpos, 1);
		if (doc.mid(cpos, 1) != ".") break;
		QChar c = doc.mid(cpos - 1, 1).leftJustified(1)[0];
		int ppos = cpos;
		if (c == '\"' || c == ')' || c == ']') {
			cpos = skipRange(doc, cpos, pairChar(c), c, '\\');
			//qDebug() << "range" << cpos;
			if (cpos < 0) break;
		}
		int npos = skipCWord(doc, cpos);
		if (npos < 0) break;
		scope.push_front(doc.mid(npos, ppos - npos));
		cpos = npos;
	}
	ret.first = scope;
	ret.second = written;
	if (ok) *ok = true;
	return ret;
}


void QCodeEdit::invokeAutoCompletition(bool force) {
	int arg = -1;
	QTextCursor htc = functionStart(textCode->textCursor(), &arg);
	if (!htc.isNull()) {
		//qDebug() << "raise";
		raiseHelp(htc, arg);
	}
	bool ok;
	QPair<QStringList, QString> scope = getScope(textCode->textCursor(), &ok);
	if (!ok) return;
	ACList acl(autoCompletitionList(scope.first, scope.second));
	//qDebug() << written << scope << acl.size();
	if (scope.first.isEmpty() && scope.second.isEmpty() && !force) {
		completer->hide();
		hideHelp();
		return;
	}
	acl << wordsCompletitionList(scope.second);
	QFont bf(font());
	bf.setBold(true);
	foreach (const ACPair & ac, acl) {
		if (ac.second.isEmpty()) continue;
		ACClass acc = ac_classes.value(ac.first);
		QTreeWidgetItem * gi = new QTreeWidgetItem();
		gi->setText(0, acc.name);
		gi->setTextAlignment(0, Qt::AlignCenter);
		gi->setTextAlignment(1, Qt::AlignCenter);
		gi->setFont(0, bf);
		gi->setBackgroundColor(0, Qt::lightGray);
		gi->setFlags(Qt::ItemIsEnabled);
		completer->addTopLevelItem(gi);
		gi->setFirstColumnSpanned(true);
		foreach (const StringsPair & s, ac.second) {
			QTreeWidgetItem * ni = new QTreeWidgetItem();
			ni->setIcon(0, acc.icon);
			ni->setText(0, s.first);
			ni->setText(1, s.second);
			completer->addTopLevelItem(ni);
		}
	}
	if (completer->topLevelItemCount() > 1)
		completer->setCurrentItem(completer->topLevelItem(1));
	if (completer->isHidden())
		completer->move(textCode->mapToGlobal(textCode->cursorRect().bottomRight()));
	completer->setVisible(completer->topLevelItemCount() > 0);
}


void QCodeEdit::commitCompletition() {
	if (completer->currentItem() == 0) return;
	if (!completer->currentItem()->flags().testFlag(Qt::ItemIsSelectable)) return;
	QString ins = completer->currentItem()->text(1), ret = completer->currentItem()->text(0);
	QTextCursor tc = textCode->textCursor(), stc = tc;
	tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
	bool ins_br = true, shifted = false;
	if (!tc.selectedText().isEmpty()) {
		if (!tc.selectedText()[0].isLetterOrNumber() && !tc.selectedText()[0].isSpace()) {
			stc.movePosition(QTextCursor::Left);
			shifted = true;
		} else {
			tc.movePosition(QTextCursor::Left);
			tc.movePosition(QTextCursor::EndOfWord);
			tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
		}
		if (!tc.selectedText().isEmpty())
			if (tc.selectedText()[0].toLatin1() == '(')
				ins_br = false;
	}
	if (ins.contains("("))
		ins = ins.left(ins.indexOf("(")) + "()";
	if (!ins_br && ins.endsWith("()"))
		ins.chop(2);
	tc = stc;
	tc.select(QTextCursor::WordUnderCursor);
	if (!tc.selectedText().leftJustified(1)[0].isLetterOrNumber()) {
		tc = stc;
		if (shifted)
			tc.movePosition(QTextCursor::Right);
	}
	textCode->setTextCursor(tc);
	textCode->textCursor().insertText(ins);
	tc = textCode->textCursor();
	if (ins_br) {
		if (ret == "void") {
			tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
			if (tc.selectedText() != ";") {
				textCode->textCursor().insertText(";");
				tc.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 2);
			}
		}
		if (ins.endsWith(")") && !completer->currentItem()->text(1).endsWith("()")) {
			tc.movePosition(QTextCursor::Left);
			textCode->setTextCursor(tc);
		}
	} else {
		if (completer->currentItem()->text(1).endsWith(")")) {
			tc.movePosition(QTextCursor::Right);
			textCode->setTextCursor(tc);
		}
		if (completer->currentItem()->text(1).endsWith("()")) {
			tc.movePosition(QTextCursor::Right);
			textCode->setTextCursor(tc);
		}
	}
	completer->hide();
}


void QCodeEdit::textEdit_cursorPositionChanged() {
	es_line.cursor = textCode->textCursor();
	es_line.cursor.select(QTextCursor::LineUnderCursor);
	es_line.cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
	applyExtraSelection();
}


void QCodeEdit::textEdit_textChanged() {
	updateLines();
}


void QCodeEdit::textEdit_selectionChanged() {
	es_selected.clear();
	QString sf = textCode->textCursor().selectedText();
	if (sf.trimmed().isEmpty() || sf.contains("\n")) {
		applyExtraSelection();
		return;
	}
	QTextCursor tc(textCode->document()->begin());
	QTextEdit::ExtraSelection es;
	es.format.setBackground(QColor(251, 250, 150));
	while (true) {
		tc = textCode->document()->find(sf, tc, QTextDocument::FindCaseSensitively | QTextDocument::FindWholeWords);
		if (tc.isNull()) break;
		es.cursor = tc;
		es_selected << es;
	}
	applyExtraSelection();
}


void QCodeEdit::setShowSpaces(bool yes) {
	spaces_ = yes;
	QTextOption to = textCode->document()->defaultTextOption();
	to.setFlags(yes ? QTextOption::ShowTabsAndSpaces : (QTextOption::Flags)0);
	textCode->document()->setDefaultTextOption(to);
}
