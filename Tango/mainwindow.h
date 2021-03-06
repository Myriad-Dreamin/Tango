#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Client;
class Logger;
class TangoPair;
class TimerWidget;


class MainScene;
class PlayingScene;
class PlaySubScene;
class RegisterScene;
class CreationScene;
class SelectingScene;
class PlaySettleScene;
class RankingAuthorsScene;
class RankingConsumersScene;
class QueryUsersScene;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
// property:
    Q_OBJECT
    friend class MainScene;
    friend class PlayingScene;
    friend class PlaySubScene;
    friend class RegisterScene;
    friend class CreationScene;
    friend class SelectingScene;
    friend class PlaySettleScene;
    friend class RankingAuthorsScene;
    friend class RankingConsumersScene;
    friend class QueryUsersScene;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void switch_scene(QWidget *to_set);
private:

    Logger *logger;
    TimerWidget *timer;
    QString caughted_error;

    QWidget *cur_scene;

    bool client_connected;
    Client *client;

    MainScene *main_scene;
    PlayingScene *playing_scene;
    RegisterScene *register_scene;
    CreationScene *creation_scene;
    PlaySubScene *playsub_scene;
    SelectingScene *selecting_scene;
    PlaySettleScene *playset_scene;
    RankingAuthorsScene *ranking_authors_scene;
    RankingConsumersScene *ranking_consumers_scene;
    QueryUsersScene *query_users_scene;

    inline bool init_client();
    inline bool init_menubar();
    inline bool init_statusbar();

    inline bool init_main_scene();
    inline bool init_playing_scene();
    inline bool init_playsub_scene();
    inline bool init_creation_scene();
    inline bool init_register_scene();
    inline bool init_selecting_scene();
    inline bool init_playset_scene();
    inline bool init_ranking_authors_scene();
    inline bool init_ranking_consumers_scene();
    inline bool init_query_users_scene();

    bool author_sign_in(QString account, QString password);
    bool author_sign_up(QString account, QString password);

    bool consumer_sign_in(QString account, QString password);
    bool consumer_sign_up(QString account, QString password);

    bool submit_creation_table(const std::vector<TangoPair> &tango_pairs);
};

#endif // MAINWINDOW_H
