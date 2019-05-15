#include "mainwindow.h"
#include <QApplication>

#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "players/Author.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

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
