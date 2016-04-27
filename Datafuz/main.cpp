#include "datafuz.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QTextStream>
#include <QDirIterator>
#include <QDebug>

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(datafuz);

	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("Vibrations");
	QCoreApplication::setApplicationName("Datafuz");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	
	Datafuz w;
	w.show();
	return app.exec();
}
