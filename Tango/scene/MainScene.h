#ifndef MAINSCENE_H
#define MAINSCENE_H

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

class MainScene : public Scene
{
    Q_OBJECT
private:
    QGridLayout *lay;
public:
    QPushButton *sign_in_button, *cancel_button, *sign_up_button;
    QRadioButton *remote_button;
    QLineEdit *account_edit, *password_edit, *network_edit, *port_edit;

    MainScene(QWidget *parent=nullptr);
    ~MainScene();
    void set_sign_in_button_event(std::function<void()>);
    void set_cancel_button_event(std::function<void()>);
    void set_sign_up_button_event(std::function<void()>);
};

#endif // MAINSCENE_H
