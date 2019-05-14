#ifndef CREATIONSCENE_H
#define CREATIONSCENE_H

#include "Scene.h"

namespace std
{
    template<typename function_signature>
    class function;
}

class QGridLayout;

class CreationScene : public Scene
{
    Q_OBJECT
private:
    QGridLayout *lay;
public:
    CreationScene(QWidget *parent=nullptr);
    ~CreationScene();
};

#endif // CREATIONSCENE_H
