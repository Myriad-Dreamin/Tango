#ifndef SELECTINGSCENE_H
#define SELECTINGSCENE_H

#include "Scene.h"

class QPushButton;
namespace std
{
    template<typename function_signature>
    class function;
}

class QGridLayout;

class SelectingScene : public Scene
{
    Q_OBJECT
private:
    QGridLayout *lay;
public:
    QPushButton *creation_button;

    SelectingScene(QWidget *parent=nullptr);
    ~SelectingScene();

// c++11 public slots:
    void set_creation_button_event(std::function<void ()> ev);
};

#endif // SELECTINGSCENE_H
