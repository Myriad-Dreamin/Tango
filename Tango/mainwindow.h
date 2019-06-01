#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Client;
class Logger;
class TangoPair;
class TimerWidget;

class Scene;
class ConfigScene;
class MainScene;
class LoginScene;
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

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Client *client;
    ConfigSet *qconfig;
    Logger *logger;
    TimerWidget *timer;

    Scene *cur_scene;
    MainScene *main_scene;
    ConfigScene *config_scene;
    LoginScene *login_scene;
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
    void reset_screen_size();
signals:
    void screen_size_changed();
private:
    inline bool load_configs(const char *file_path="config.ini");
    bool load_configs(QString file_path);
    bool set_default_configs();
    inline bool init_client();
    inline bool init_menubar();
    inline bool init_statusbar();

    inline bool init_main_scene();
    inline bool init_login_scene();
    inline bool init_config_scene();
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
