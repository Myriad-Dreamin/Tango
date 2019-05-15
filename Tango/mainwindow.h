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
class Client;
class TangoPair;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class MainScene;
    friend class RegisterScene;
    friend class SelectingScene;
    friend class CreationScene;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void makeLayout();
    ~MainWindow();

private slots:

    void switch_scene(QWidget *to_set);
private:

    QString caughted_error;

    QWidget *cur_scene;

    bool client_connected;
    Client *client;

    MainScene *main_scene;
    RegisterScene *register_scene;
    SelectingScene *selecting_scene;
    CreationScene *creation_scene;

    bool init_client();

    bool init_main_scene();
    bool init_register_scene();
    bool init_selecting_scene();
    bool init_creation_scene();

    bool author_sign_in(QString account, QString password);
    bool author_sign_up(QString account, QString password);
    bool submit_creation_table(const std::vector<TangoPair> &tango_pairs);
    bool consumer_sign_in(QString account, QString password);
    bool consumer_sign_up(QString account, QString password);
};

#endif // MAINWINDOW_H
