#ifndef PLAYSETTLESCENE_H
#define PLAYSETTLESCENE_H

#include "Scene.h"

class QLabel;
class QPushButton;
class QGridLayout;
class MainWindow;


/*
 * 游玩结算场景
 */
class PlaySettleScene : public Scene
{
    Q_OBJECT
    friend class PlayingScene;
public:
    PlaySettleScene(QWidget *parent=nullptr);
    ~PlaySettleScene();
public:
    MainWindow* parent;
    QLabel *from_exp, *to_exp;
    QLabel *from_level, *to_level;
    QLabel *level_flag;
    QGridLayout *lay;
    QPushButton *confirm_button;
};

#endif // PLAYSETTLESCENE_H
