#ifndef PLAYINGSCENE_H
#define PLAYINGSCENE_H

#include "Scene.h"


class MainWindow;
class QGridLayout;

class PlayingScene : public Scene
{
    Q_OBJECT
private:
    MainWindow *parent;
    QGridLayout *lay;
public:
    PlayingScene(QWidget *parent=nullptr);
    ~PlayingScene();
};

#endif // PLAYINGSCENE_H
