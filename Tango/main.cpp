
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    qDebug("---------------");
    QApplication a(argc, argv);

    MainWindow w;

    // exec(), show() 模态, 非模态

    w.show();

    return a.exec();
}
