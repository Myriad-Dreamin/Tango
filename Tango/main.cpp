#include "mainwindow.h"
#include <QApplication>

#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

// #include "db/db_connection.h"
#include "db/db_create_tables.h"

bool open_sql_base()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName("localhost");
    db.setDatabaseName("tango");
    db.setUserName("tangosql");
    db.setPassword("123456");
    if (!db.open()) {
        qDebug() << "db open error:" << db.lastError().text();
        QMessageBox::critical(
                    nullptr,
                    QObject::tr("无法打开数据"),
                    "无法创建数据库连接！ ",
                    QMessageBox::Cancel
                    );
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    if (!open_sql_base()) {
        return 1;
    }

    if (!create_authors_table()) {
        return 2;
    }


    /* QObject::connect(QObject.from, SIGNAL(signal()), &QObject.to, SLOT(slot())) */
    // linkActivated
    // ::event(QEvent *v)
    // button->setDefault();

    // exec(), show() 模态, 非模态


    w.show();

    // V:竖直的
    // QVBoxLayout

    // H:竖直的
    // QHBoxLayout
    return a.exec();
}
