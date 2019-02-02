#include <QApplication>
#include <QIcon>
#include <QTranslator>
#include <QFileInfo>
#include <QDir>

#include <random>
#include <ctime>
#include <memory>

#include "MainDialog.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	qApp->setWindowIcon(QIcon(":/logo.png"));

	// try to determine language id from OS
	QString langId = QLocale::system().name();
	int pos = langId.indexOf('_');
	if (pos != -1)
		langId = langId.left(pos);

	// nothing to be done for english
	if (langId != "en") {
		// load application translator
		std::unique_ptr<QTranslator> applicationTranslator(new QTranslator);
		if (applicationTranslator->load(":/translations/Rechenmeistertrainer_"+langId+".qm")) {
			qApp->installTranslator(applicationTranslator.release());
			// also try to load the system translator
			// install system translator
			std::unique_ptr<QTranslator> systemTranslator(new QTranslator);

			// system translator first
			if (systemTranslator->load(":/translations/qt_"+langId+".qm")) {
				qApp->installTranslator(systemTranslator.release());
			}
		}
	}

	// randomize
	qsrand( (uint)time(NULL));

	// create main dialog
	MainDialog w;
	w.show();

	return a.exec();
}
