#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QTcpSocket>
#include <QHostAddress>

class Author;
class MainScene;
class RegisterScene;
class SelectingScene;
class CreationScene;
class QSqlError;

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

    enum UserStatusType: unsigned char
    {
        None      = 0x0 | 0x0,
        Author    = 0x1 | 0x0,
        Consumer  = 0x0 | 0x2,
        Both      = 0x1 | 0x2
    } user_status;

    class Author *user_author;
    QString caughted_error;

    QWidget *cur_scene;

    MainScene *main_scene;
    RegisterScene *register_scene;
    SelectingScene *selecting_scene;
    CreationScene *creation_scene;

    bool init_client();

    bool init_main_scene();
    bool init_register_scene();
    bool init_selecting_scene();
    bool init_creation_scene();

    // Ui::MainWindow *ui;
    bool try_connect_to_server(QHostAddress host_address, quint16 server_host);

    bool author_sign_in(QString account, QString password);
    bool author_sign_in_remote(QString account, QString password);

    bool author_sign_up(QString account, QString password);
    bool author_sign_up_remote(QString account, QString password);
};

#endif // MAINWINDOW_H
