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
class AbstractGameAutomation;


/*
 * 游玩场景
 */
class PlayingScene : public Scene
{
    Q_OBJECT

public:
    PlayingScene(QWidget *parent=nullptr);
    ~PlayingScene();

private:
    QPushButton *trial_button;
    QPushButton *easy_button;
    QPushButton *normal_button;
    QPushButton *hard_button;
    QPushButton *endless_button;

    MainWindow *parent;
    GameConfig *game_config;
    QGridLayout *lay;
    QVBoxLayout *center_lay;
    void settle_game(AbstractGameAutomation *automate);
    void abort_game(AbstractGameAutomation *automate);
    std::function<void ()> single_round(const std::function<AbstractGameAutomation *()> &moder);
    std::function<void ()> single_round_must_done(const std::function<AbstractGameAutomation *()> &moder);

    std::function<AbstractGameAutomation *()> default_automate();
    std::function<AbstractGameAutomation *()> easy_increment_automate();
    std::function<AbstractGameAutomation *()> normal_increment_automate();
    std::function<AbstractGameAutomation *()> hard_increment_automate();
};

#endif // PLAYINGSCENE_H
