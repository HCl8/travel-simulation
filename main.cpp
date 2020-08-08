#include "Qt10.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Qt10 w;
	w.show();
	w.setWindowTitle(QString::fromLocal8Bit("×ßÄã"));
	int kk = a.exec();
	w.CloseLog();
	return kk;
}