
#ifndef ABSTRACT_SCENE_H
#define ABSTRACT_SCENE_H

#include <QWidget>

class Scene: public QWidget
{
    Q_OBJECT
public:
    Scene(QWidget* parent=nullptr);
    ~Scene();

    virtual void on_incoming();
    virtual void on_exiting();
};


#endif
