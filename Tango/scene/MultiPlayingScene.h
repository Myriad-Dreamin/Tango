#ifndef MULTIPLAYINGSCENE_H
#define MULTIPLAYINGSCENE_H

#include "scene_common.h"

/*
 * 多人对战场景
 */
class MultiPlayingScene : public Scene
{
    Q_OBJECT

public:
    MultiPlayingScene(MainWindow *parent=nullptr);
    ~MultiPlayingScene();
    
    void clear_table();
    void set_page_contain(std::vector<UserFullInfo> &authors_list, std::vector<UserFullInfo> &consumers_list, std::vector<long long> &socket_list);
    void on_incoming();
private:
    std::vector<UserFullInfo> a_list;
    std::vector<UserFullInfo> c_list;
    std::vector<long long> s_list;
    MainWindow *parent;
    QHBoxLayout *lay;
    QVBoxLayout *player_list;
    int ranking_table_row;
    Logger *logger;
};

#endif // MULTIPLAYINGSCENE_H
