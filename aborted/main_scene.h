
#ifndef ABSTRACT_SCENE_H
#define ABSTRACT_SCENE_H

#include <QLayout>
#include <functional>
#include <QWidget>
#include "scene.h"

class MainScene: public QWidget
{
    Q_OBJECT
private:
    QGridLayout *lay;
public:
    MainScene(QWidget *parent=nullptr);
    ~MainScene();
    void init();
    void set_sign_in_button_event(std::function<void()>);
    void set_cancel_button_event(std::function<void()>);
    void set_sign_up_button_event(std::function<void()>);
};

#endif
