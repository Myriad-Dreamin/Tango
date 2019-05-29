
#include <QApplication>

#include "mainwindow.h"
#include "../TangoCommon/types/Logger.h"

/* 程序开始的地方 */
int main(int argc, char *argv[])
{
    Logger::get_logger("main")->set_mode(LoggerFlag::debug_logger_flag());

    QApplication a(argc, argv);

    MainWindow w;

    w.show();

    return a.exec();
}
