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
    std::function<void ()> single_round(const std::function<GameAutomation *()> &moder);

    std::function<GameAutomation *()> default_automate();
    std::function<GameAutomation *()> easy_increment_automate();
};

#endif // PLAYINGSCENE_H
