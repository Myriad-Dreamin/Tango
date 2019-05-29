#ifndef PLAYSETTLESCENE_H
#define PLAYSETTLESCENE_H

#include "scene_common.h"


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
