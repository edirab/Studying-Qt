#include <QApplication>
#include "kx_pult.h"


int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	KX_Pult w;
	w.show();
	return a.exec();
}
