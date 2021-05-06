#ifndef KX_PULT_H
#define KX_PULT_H

#include <QDialog>
#include <QSpinBox>
#include <QFont>
#include <QRect>
#include <QFile>
#include <QVector>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QBoxLayout>
#include <QCheckBox>
#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QMenu>
#include <QMetaObject>
#include <QTreeWidgetItem>
#include <QLabel>
#include "kx_coeffs.h"
#include "kx_protocol_x.h"
#include "kx_protocol_c.h"
#include "piqt.h"
#include "session_manager.h"
#include "qpievaluator.h"


class XCheck: public QWidget {
	Q_OBJECT
public:
	explicit XCheck(int index);
	QCheckBox check;
	QSpinBox spin;
private:
	bool eventFilter(QObject * o, QEvent * e);
	int index_;
private slots:
	void spinChanged(int value) {check.setChecked(true); emit valueChanged(index_, value);}
	void checkChanged_(bool value) {emit checkChanged(index_, value);}
signals:
	void valueChanged(int index, int value);
	void checkChanged(int index, bool on);
};


namespace Ui {
	class KX_Pult;
};

class KX_Pult: public QMainWindow, public PIObject
{
	Q_OBJECT
	PIOBJECT(KX_Pult)
public:
	KX_Pult();
	~KX_Pult();

private:
	struct KDesc {
		KDesc() {index = -1;}
		int index;
		QString name;
		QString type;
		QString comment;
		QString value;
	};
	
	bool eventFilter(QObject * o, QEvent * e);
	void timerEvent(QTimerEvent * );
	void setControlsEnable(bool enable);
	void addToList(const QString & s, const QColor & c);
	QString getNewFileName(int inc);
	QColor invertColor(QColor col) {return QColor(255 - col.red(), 255 - col.green(), 255 - col.blue());}
	void stopWait();
	bool calculateExpression(int i, QVector<int> trace);
	void markError(QTreeWidgetItem * item, const QString & tool_tip = QString());
	void markNormal(QTreeWidgetItem * item);
	void progress(int val, int max);
	void clearSelected();
	QString typeName(const QString & n) const;
	int parseHeader(const QString & file, QMap<int, KDesc> & map);
	
	EVENT_HANDLER1(void, received, bool, ok);
	EVENT_HANDLER(void, pip_sendFailed) {emit q_k_sendFailed();}
	EVENT_HANDLER(void, pip_sendSucceed) {emit q_k_sendSucceed();}
	EVENT_HANDLER(void, pip_receiveFailed) {emit q_k_receiveFailed();}
	EVENT_HANDLER(void, pip_receiveSucceed) {emit q_k_receiveSucceed();}

	QVector<XCheck * > checks;
	QVector<QLabel * > values;
	
	Ui::KX_Pult * ui;
	PIString config_, name_x, name_c;
	QDir dir;
	QString outdir, kdesc_file, cdesc_file;
	QFile file;
	QTime tm, ctm;
	QIcon icon_record, icon_stop;
	QTextStream stream;
	QTimer timer_diag;
	QMap<int, KDesc> kdesc, cdesc;
	QMap<QString, int> knames;
	QSet<int> calculated;
	QStringList knames_sort;
	QPIEvaluator eval;
	SessionManager session;
	QPIConfig config;
	QMenu log_menu;
	//QVector<float> k, x;
	KX_Coefficients coeffs;
	__KX_Protocol_X * prot_x;
	__KX_Protocol_C * prot_c;
	int csize, wcnt, timer, timer_update, clear_target;
	bool needWrite, isPause, need_update, show_x;

private slots:
	void loading(QPIConfig & conf);
	void saving(QPIConfig & conf);
    void setX(const KX_X_Data & data);
    void updateGraph();
	void updateDiag();
	void updateKDesc(bool ask_move = false);
	void updateCDesc();
	void updateTree(bool move = false);
	void updateCommands();
	void filterTree();
	void calculate();
	void renew(bool write = true);
	void toggledX(int index, bool on);
	void changedX(int index, int num);
	void commandClicked();
	void k_sendFailed();
	void k_sendSucceed();
	void k_receiveFailed();
	void k_receiveSucceed();
	void on_spinBuffer_editingFinished();
	void on_buttonSendK_clicked();
	void on_buttonReceiveK_clicked();
	void on_buttonShowAll_clicked();
	void on_buttonHideAll_clicked();
	void on_buttonRead_clicked();
	void on_buttonWrite_clicked();
	void on_buttonResize_clicked();
	void on_buttonSetKDesc_clicked();
	void on_buttonReparseKDesc_clicked() {updateKDesc(true);}
	void on_buttonSetCDesc_clicked();
	void on_buttonReparseCDesc_clicked() {updateCDesc();}
	void on_buttonCalculate_clicked() {calculate();}
	void on_buttonApply_clicked() {renew();}
	void on_spinSize_valueChanged(int);
	void on_buttonPause_toggled(bool on) {isPause = on;}
	void on_buttonRecord_clicked();
	void on_treeK_itemClicked(QTreeWidgetItem * item, int column);
	void on_treeK_itemChanged(QTreeWidgetItem * item, int column);
	void on_actionClear_triggered();

signals:
	void q_k_sendFailed();
	void q_k_sendSucceed();
	void q_k_receiveFailed();
	void q_k_receiveSucceed();
	void recordStarted(const QString & fileName);
	void recordStopped(const QString & fileName);

};

#endif // KX_PULT_H

