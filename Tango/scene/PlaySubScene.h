#ifndef PLAYSUBSCENE_H
#define PLAYSUBSCENE_H

#include "Scene.h"

class MainWindow;

class QPushButton;
class QVBoxLayout;
class QGridLayout;
class QLineEdit;
class GameConfig;
class QLabel;
class GameAutomation;


/*
 * 游玩子场景
 */
class PlaySubScene : public Scene
{
    Q_OBJECT
    friend class PlayingScene;
public:
    PlaySubScene(QWidget *parent=nullptr);
    ~PlaySubScene();

private:
    MainWindow *parent;
    QGridLayout *lay;
    QVBoxLayout *play_lay;

    QPushButton *stop_button;
    QPushButton *answer_button;

    QLineEdit *user_ret;
    QLabel *key_label;
    QLabel *value_label;
};


#endif // PLAYSUBSCENE_H
