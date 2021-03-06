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
    QPushButton *player_list_button, *ranking_authors_button, *ranking_consumers_button;
    QPushButton *return_button;

public:
// c++11 public slots:

    bool set_visble_buttons();
    void set_play_button_event(const std::function<void ()> &ev);
    void set_multi_play_button_event(const std::function<void ()> &ev);
    void set_player_list_button_event(const std::function<void ()> &ev);
    void set_creation_button_event(const std::function<void ()> &ev);
    void set_ranking_authors_button_event(const std::function<void ()> &ev);
    void set_ranking_consumers_button_event(const std::function<void ()> &ev);
    void set_return_button_event(const std::function<void ()> &ev);

private:
    MainWindow *parent;
    QGridLayout *lay;
};

#endif // SELECTINGSCENE_H
