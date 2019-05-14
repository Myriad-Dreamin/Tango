#ifndef REGISTERSCENE_H
#define REGISTERSCENE_H

#include "Scene.h"

namespace std
{
    template<typename function_signature>
    class function;
}

class QGridLayout;
class QPushButton;
class QRadioButton;
class QLineEdit;

class RegisterScene : public Scene
{
    Q_OBJECT
private:
    QGridLayout *lay;
public:
    QPushButton *confirm_button, *cancel_button, *return_button;
    QRadioButton *remote_button;
    QLineEdit *account_edit, *password_edit, *confirm_edit, *network_edit, *port_edit;

    RegisterScene(QWidget *parent=nullptr);
    ~RegisterScene();
    void set_confirm_button_event(std::function<void()>);
    void set_cancel_button_event(std::function<void()>);
    void set_return_button_event(std::function<void()>);
};

#endif // REGISTERSCENE_H
