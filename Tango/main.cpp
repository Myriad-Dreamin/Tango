
#include <QApplication>

#include "mainwindow.h"

/* 程序开始的地方 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    w.show();

    return a.exec();
}
