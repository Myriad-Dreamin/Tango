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

class PlaySubScene : public Scene
{
    Q_OBJECT
    friend class PlayingScene;
public:
    PlaySubScene(QWidget *parent=nullptr);
    ~PlaySubScene();

private:
    QPushButton *stop_button, *answer_button;
    MainWindow *parent;
    QGridLayout *lay;
    QLineEdit *user_ret;
    QVBoxLayout *play_lay;
    QLabel *key_label, *value_label;
};

#endif // PLAYSUBSCENE_H
