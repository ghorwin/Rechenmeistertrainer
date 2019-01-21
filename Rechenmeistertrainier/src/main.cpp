#include "MainDialog.h"
#include <QApplication>

#include <random>
#include <ctime>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	MainDialog w;
	w.show();

	qsrand( time(NULL));

	return a.exec();
}
