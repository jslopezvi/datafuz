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
	QCoreApplication::setOrganizationName("QtProject");
	QCoreApplication::setApplicationName("Application Example");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);

	/*QDirIterator it(":", QDirIterator::Subdirectories);
	while (it.hasNext()) {
		qDebug() << it.next();
	}
*/
	/*QFile f(":/theme/qdarkstyle/style.qss");
	if (!f.exists())
	{
		printf("Unable to set stylesheet, file not found\n");
	}
	else
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		qApp->setStyleSheet(ts.readAll());
	}*/
	
	Datafuz w;
	w.show();
	return app.exec();
}
