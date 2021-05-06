#ifndef CLINEEDIT_H
#define CLINEEDIT_H

#include <QDebug>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>


class CLineEdit: public QLineEdit
{
	Q_OBJECT
	Q_PROPERTY(QString defaultText READ defaultText WRITE setDefaultText)
	
public:
	explicit CLineEdit(QWidget * parent = 0);
	~CLineEdit() {delete cw;}
	
	inline QString defaultText() const {return dt;}
	
protected:
	QWidget * cw;
	QString dt;
	
private:
	bool eventFilter(QObject * o, QEvent * e);
	void resizeEvent(QResizeEvent * );
	void changeEvent(QEvent * e);
	
private slots:
	void clearMouseRelease(QMouseEvent * e) {if (cw->rect().contains(e->pos())) clearClick();}
	void textChanged_(QString text) {cw->setVisible(text != dt);}
	
public slots:
	void clearClick() {if (!isEnabled()) return; setText(dt); emit cleared(); emit textEdited(dt);}
	void setDefaultText(const QString & t, bool set_text = false);
	
signals:
	void cleared();
	
};

#endif // CLINEEDIT_H
