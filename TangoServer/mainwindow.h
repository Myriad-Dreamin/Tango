#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTcpServer *server;
    QTextEdit *text_edit, *address_edit, *port_edit;
private slots:
    void newCon();
    void dealData();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
