#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QTextEdit>

class TcpServer;



class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QSqlDatabase tango_sql;
    TcpServer *server;
    QTextEdit *text_edit, *address_edit, *port_edit;
    void make_layers();
private slots:
    // void newCon();
    void dealData();
public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(QSqlDatabase &tango_sql, QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
