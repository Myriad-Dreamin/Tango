#ifndef PLAYINGSCENE_H
#define PLAYINGSCENE_H

#include "Scene.h"

class MainWindow;

class QPushButton;
class QVBoxLayout;
class QGridLayout;
class QLineEdit;
class GameConfig;
class GameAutomation;
class PlayingScene : public Scene
{
    Q_OBJECT

public:
    PlayingScene(QWidget *parent=nullptr);
    ~PlayingScene();

private:
    QPushButton *start_button;
    MainWindow *parent;
    GameConfig *game_config;
    QGridLayout *lay;
    QVBoxLayout *center_lay;
    void settle_game(GameAutomation *automate);
};

#endif // PLAYINGSCENE_H
