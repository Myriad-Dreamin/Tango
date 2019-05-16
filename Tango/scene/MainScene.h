#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "Scene.h"
#include "../types/UserStatus.h"

namespace std
{
    template<typename function_signature>
    class function;
}

class MainWindow;

class QLineEdit;
class QGridLayout;

class QPushButton;
class QRadioButton;

class MainScene : public Scene
{
    Q_OBJECT

public:
    MainScene(QWidget *parent=nullptr);
    ~MainScene();

    UserStatus user_selecting_status;

    QPushButton *sign_in_button, *cancel_button, *sign_up_button, *role_button;
    QRadioButton *remote_button;
    QLineEdit *account_edit, *password_edit, *network_edit, *port_edit;

public:
// c++11 public slots:
    void set_sign_in_button_event(const std::function<void ()> &ev);
    void set_sign_up_button_event(const std::function<void ()> &ev);
    void set_cancel_button_event(const std::function<void ()> &ev);
    void set_role_button_event(const std::function<void ()> &ev);
private:
    MainWindow *parent;
    QGridLayout *lay;
};

#endif // MAINSCENE_H
