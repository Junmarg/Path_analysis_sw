#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

#if 0
    QString sModulePath = QCoreApplication::applicationDirPath();
    sModulePath += "\\test.tmp";
    qDebug() << sModulePath;
#endif

}
