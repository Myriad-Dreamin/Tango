#ifndef RANKINGCONSUMERSSCENE_H
#define RANKINGCONSUMERSSCENE_H

#include "Scene.h"

const int DEFAULT_CONSUMER_PAGE_LIMIT = 10;

class MainWindow;
class QVBoxLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class QHBoxLayout;
class UserBriefInfo;

class RankingConsumersScene : public Scene
{
    Q_OBJECT
    friend class MainWindow;
public:
    RankingConsumersScene(QWidget *parent=nullptr);
    ~RankingConsumersScene();
private:
    int page_limit = DEFAULT_CONSUMER_PAGE_LIMIT;
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

#endif // RANKINGCONSUMERSSCENE_H
