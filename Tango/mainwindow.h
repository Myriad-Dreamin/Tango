#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QTcpSocket>
#include <QHostAddress>

#include "scene/MainScene.h"
#include "scene/RegisterScene.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void makeLayout();
    ~MainWindow();

private slots:

    void switch_scene(QWidget *to_set);

    void client_on_connected();
    void client_on_closed();
private:

    bool client_connected;
    QHostAddress connected_server_address;
    quint16 connected_server_port;
    QTcpSocket *client;



    QWidget *cur_scene;

    MainScene *main_scene;
    RegisterScene *register_scene;

    bool init_client();

    bool init_main_scene();
    bool init_register_scene();

    // Ui::MainWindow *ui;
    bool try_connect_to_server(QHostAddress host_address, quint16 server_host);
};

#endif // MAINWINDOW_H
