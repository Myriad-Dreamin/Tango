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
    MainScene(QWidget *parent=nullptr);
    ~MainScene();

    UserStatus user_selecting_status;

    QPushButton *sign_in_button；
    QPushButton *cancel_button;
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
    void set_cancel_button_event(const std::function<void ()> &ev);
    void set_role_button_event(const std::function<void ()> &ev);
private:
    MainWindow *parent;
    QHBoxLayout *lay;
};

#endif // MAINSCENE_H
