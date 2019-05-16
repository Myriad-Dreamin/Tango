#ifndef PLAYINGSCENE_H
#define PLAYINGSCENE_H

#include "Scene.h"


class MainWindow;

class QGridLayout;


class PlayingScene : public Scene
{
    Q_OBJECT

public:
    PlayingScene(QWidget *parent=nullptr);
    ~PlayingScene();

private:
    MainWindow *parent;
    QGridLayout *lay;
};

#endif // PLAYINGSCENE_H
