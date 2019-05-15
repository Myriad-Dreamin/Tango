#ifndef REGISTERSCENE_H
#define REGISTERSCENE_H

#include "Scene.h"
#include "../types/UserStatus.h"

namespace std
{
    template<typename function_signature>
    class function;
}

class QGridLayout;
class QPushButton;
class QRadioButton;
class QLineEdit;
class MainWindow;

class RegisterScene : public Scene
{
    Q_OBJECT
private:
    MainWindow *parent;
    QGridLayout *lay;
public:
    UserStatus user_selecting_status;

    QPushButton *confirm_button, *cancel_button, *return_button, *role_button;
    QRadioButton *remote_button;
    QLineEdit *account_edit, *password_edit, *confirm_edit, *network_edit, *port_edit;

    RegisterScene(QWidget *parent=nullptr);
    ~RegisterScene();
    void set_confirm_button_event(const std::function<void ()> &ev);
    void set_cancel_button_event(const std::function<void ()> &ev);
    void set_return_button_event(const std::function<void ()> &ev);
    void set_role_button_event(const std::function<void ()> &ev);
};

#endif // REGISTERSCENE_H
