#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Client;
class TangoPair;
class TimerWidget;

class MainScene;
class PlayingScene;
class RegisterScene;
class CreationScene;
class SelectingScene;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
// property:
    Q_OBJECT
    friend class MainScene;
    friend class PlayingScene;
    friend class RegisterScene;
    friend class CreationScene;
    friend class SelectingScene;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void switch_scene(QWidget *to_set);
private:

    TimerWidget *timer;
    QString caughted_error;

    QWidget *cur_scene;

    bool client_connected;
    Client *client;

    MainScene *main_scene;
    PlayingScene *playing_scene;
    RegisterScene *register_scene;
    CreationScene *creation_scene;
    SelectingScene *selecting_scene;

    bool init_client();

    bool init_main_scene();
    bool init_playing_scene();
    bool init_creation_scene();
    bool init_register_scene();
    bool init_selecting_scene();

    bool author_sign_in(QString account, QString password);
    bool author_sign_up(QString account, QString password);

    bool consumer_sign_in(QString account, QString password);
    bool consumer_sign_up(QString account, QString password);

    bool submit_creation_table(const std::vector<TangoPair> &tango_pairs);
};

#endif // MAINWINDOW_H
