#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Client;
class Logger;
class TangoPair;
class TimerWidget;

class Scene;
class MainScene;
class PlayingScene;
class MultiPlayingScene;
class PlaySubScene;
class RegisterScene;
class CreationScene;
class SelectingScene;
class PlaySettleScene;
class RankingAuthorsScene;
class RankingConsumersScene;
class QueryUsersScene;

class ConfigSet;
/*
 * 主窗口类
 * 
 * 相当于游戏引擎的Director
 */
class MainWindow : public QMainWindow
{
// property:
    Q_OBJECT
    friend class Client;
    friend class RemoteClient;
    
    friend class MainScene;
    friend class PlayingScene;
    friend class MultiPlayingScene;
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
private:
    Client *client;
    ConfigSet *qconfig;
    Logger *logger;
    TimerWidget *timer;

    Scene *cur_scene;
    MainScene *main_scene;
    PlayingScene *playing_scene;
    MultiPlayingScene *multiplaying_scene;
    RegisterScene *register_scene;
    CreationScene *creation_scene;
    PlaySubScene *playsub_scene;
    SelectingScene *selecting_scene;
    PlaySettleScene *playset_scene;
    RankingAuthorsScene *ranking_authors_scene;
    RankingConsumersScene *ranking_consumers_scene;
    QueryUsersScene *query_users_scene;

    void switch_scene(Scene *to_set);

private:
    inline bool load_configs();
    bool set_default_configs();
    inline bool init_client();
    inline bool init_menubar();
    inline bool init_statusbar();

    inline bool init_main_scene();
    inline bool init_playing_scene();
    inline bool init_multiplaying_scene();
    inline bool init_playsub_scene();
    inline bool init_creation_scene();
    inline bool init_register_scene();
    inline bool init_selecting_scene();
    inline bool init_playset_scene();
    inline bool init_ranking_authors_scene();
    inline bool init_ranking_consumers_scene();
    inline bool init_query_users_scene();
};

#endif // MAINWINDOW_H
