#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "scene_common.h"


/*
 * 主场景
 */
class MainScene : public Scene
{
    Q_OBJECT

public:
    MainScene(MainWindow *parent=nullptr);
    ~MainScene();

    UserStatus user_selecting_status;

    QPushButton *login_button;
    QPushButton *register_button;
    QPushButton *setting_button;
    QPushButton *exit_button;
public:
// c++11 public slots:
    void set_login_button_event(const std::function<void ()> &ev);
    void set_setting_button_event(const std::function<void ()> &ev);
    void set_exit_button_event(const std::function<void ()> &ev);
    void set_register_button_event(const std::function<void ()> &ev);

private:
    MainWindow *parent;
    QGridLayout *lay;
    Logger *logger;
    void set_button_events();
};

#endif // MAINSCENE_H
