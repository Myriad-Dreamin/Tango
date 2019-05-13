#ifndef REGISTERSCENE_H
#define REGISTERSCENE_H


#include <QLayout>
#include <functional>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include "Scene.h"

class RegisterScene : public Scene
{
    Q_OBJECT
private:
    QGridLayout *lay;
public:
    QPushButton *confirm_button, *cancel_button, *return_button;
    QLineEdit *account_edit, *password_edit, *confirm_edit, *network_edit, *port_edit;

    RegisterScene(QWidget *parent=nullptr);
    ~RegisterScene();
    void set_confirm_button_event(std::function<void()>);
    void set_cancel_button_event(std::function<void()>);
    void set_return_button_event(std::function<void()>);
};

#endif // REGISTERSCENE_H
