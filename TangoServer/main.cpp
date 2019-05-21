#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto tango_sql = QSqlDatabase::addDatabase("QMYSQL");
    tango_sql.setHostName("localhost");
    tango_sql.setDatabaseName("tango");
    tango_sql.setUserName("tangosql");
    tango_sql.setPassword("123456");

    if (!tango_sql.open()) {
        qDebug() << "cant open database" << tango_sql.lastError().text();
        QWidget sh;
        QMessageBox::critical(&sh, QString("错误"), tango_sql.lastError().text());
        return a.exec();
    }

    MainWindow w(tango_sql);
    w.show();

    return a.exec();
}
