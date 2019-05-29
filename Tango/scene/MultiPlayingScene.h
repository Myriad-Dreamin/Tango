#ifndef MULTIPLAYINGSCENE_H
#define MULTIPLAYINGSCENE_H

#include "Scene.h"
#include "../mainwindow.h"
#include <QLayout>
#include <QScrollArea>
#include "../TangoCommon/types/UserFullInfo.h"

/*
 * 多人对战场景
 */
class MultiPlayingScene : public Scene
{
    Q_OBJECT

public:
    MultiPlayingScene(QWidget *parent=nullptr);
    ~MultiPlayingScene();

    void clear_table();
    void set_page_contain(std::vector<UserFullInfo> &authors_list, std::vector<UserFullInfo> &consumers_list, std::vector<long long> &socket_list);
private:
    std::vector<UserFullInfo> a_list;
    std::vector<UserFullInfo> c_list;
    std::vector<long long> s_list;
    MainWindow *parent;
    QHBoxLayout *lay;
    QVBoxLayout *player_list;
    int ranking_table_row;
};

#endif // MULTIPLAYINGSCENE_H
