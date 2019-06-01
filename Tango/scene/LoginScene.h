#ifndef LOGINSCENE_H
#define LOGINSCENE_H

#include "scene_common.h"

/*
 * 登录场景
 */
class LoginScene : public Scene
{
    Q_OBJECT

public:
    LoginScene(MainWindow *parent=nullptr);
    ~LoginScene();

    UserStatus user_selecting_status;

    QPushButton *sign_in_button;
    QPushButton *return_button;
    QPushButton *sign_up_button;
    QPushButton *role_button;

    QRadioButton *remote_button;

    QLineEdit *account_edit;
    QLineEdit *password_edit;
    QLineEdit *network_edit;
    QLineEdit *port_edit;

public:
// c++11 public slots:
    void set_sign_in_button_event(const std::function<void ()> &ev);
    void set_sign_up_button_event(const std::function<void ()> &ev);
    void set_return_button_event(const std::function<void ()> &ev);
    void set_role_button_event(const std::function<void ()> &ev);

    virtual void on_incoming();
private:
    MainWindow *parent;
    QHBoxLayout *lay;
    Logger *logger;
    void set_button_events();
};

#endif // LOGINSCENE_H
