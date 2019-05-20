#ifndef PLAYINGSCENE_H
#define PLAYINGSCENE_H

#include "Scene.h"
#include <functional>

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
    QPushButton *trial_button;
    QPushButton *normal_button;
    QPushButton *endless_button;

    MainWindow *parent;
    GameConfig *game_config;
    QGridLayout *lay;
    QVBoxLayout *center_lay;
    void settle_game(GameAutomation *automate);
    GameAutomation *default_automate();
    GameAutomation *more_complex_automate();
    std::function<void ()> single_round(const std::function<GameAutomation *()> &moder);
};

#endif // PLAYINGSCENE_H
