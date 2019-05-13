
#ifndef ABSTRACT_SCENE_H
#define ABSTRACT_SCENE_H

#include <QWidget>
#include <QLayout>

class Scene: public QWidget
{
    Q_OBJECT
public:
    Scene(QWidget* parent=nullptr);
    ~Scene();
};


#endif
