#ifndef SELECTINGSCENE_H
#define SELECTINGSCENE_H

#include "Scene.h"

class QPushButton;
namespace std
{
    template<typename function_signature>
    class function;
}

class QGridLayout;
class MainWindow;

class SelectingScene : public Scene
{
    Q_OBJECT

public:
    SelectingScene(QWidget *parent=nullptr);
    ~SelectingScene();

    QPushButton *creation_button, *play_button, *multi_play_button;
    QPushButton *player_list_button, *ranking_button;

public:
// c++11 public slots:
    void set_play_button_event(const std::function<void ()> &ev);
    void set_ranking_button_event(const std::function<void ()> &ev);
    void set_multi_play_button_event(const std::function<void ()> &ev);
    void set_player_list_button_event(const std::function<void ()> &ev);
    void set_creation_button_event(const std::function<void ()> &ev);

private:
    MainWindow *parent;
    QGridLayout *lay;
};

#endif // SELECTINGSCENE_H
