#ifndef RANKINGAUTHORSSCENE_H
#define RANKINGAUTHORSSCENE_H

#include "scene_common.h"


/*
 * 作者排行榜场景
 */
class RankingAuthorsScene : public Scene
{
    Q_OBJECT
    friend class MainWindow;
public:
    RankingAuthorsScene(QWidget *parent=nullptr);
    ~RankingAuthorsScene();
    void set_page_contain(const std::vector<UserBriefInfo> &info_query);
private:
    int page_limit = DEFAULT_AUTHOR_PAGE_LIMIT;
    int ranking_table_row;
    std::vector<UserBriefInfo> info_query;
    MainWindow* parent;
    QLabel *title;
    QVBoxLayout *ranking;
    QLabel *cur_page;
    QPushButton *return_button, *next_button, *last_button;
    QPushButton *switch_button;
    QLineEdit *switch_edit;
    QHBoxLayout *lay;
    void switch_page(int page_count);
    void clear_table();
};

#endif // RANKINGAUTHORSSCENE_H
