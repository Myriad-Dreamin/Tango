#ifndef REGISTERSCENE_H
#define REGISTERSCENE_H

#include "Scene.h"
#include "../types/UserStatus.h"

namespace std
{
    template<typename function_signature>
    class function;
}

class MainWindow;

class QLineEdit;
class QHBoxLayout;
class QPushButton;
class QRadioButton;

class RegisterScene : public Scene
{
    Q_OBJECT
public:
    RegisterScene(QWidget *parent=nullptr);
    ~RegisterScene();

    UserStatus user_selecting_status;

    QPushButton *confirm_button, *cancel_button, *return_button, *role_button;
    QRadioButton *remote_button;
    QLineEdit *account_edit, *password_edit, *confirm_edit, *network_edit, *port_edit;

public:
// c++11 public slots:
    void set_role_button_event(const std::function<void ()> &ev);
    void set_cancel_button_event(const std::function<void ()> &ev);
    void set_return_button_event(const std::function<void ()> &ev);
    void set_confirm_button_event(const std::function<void ()> &ev);

private:
    MainWindow *parent;
    QHBoxLayout *lay;
};

#endif // REGISTERSCENE_H
