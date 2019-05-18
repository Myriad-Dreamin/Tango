#ifndef RANKINGAUTHORSSCENE_H
#define RANKINGAUTHORSSCENE_H

#include "Scene.h"

const int DEFAULT_AUTHOR_PAGE_LIMIT = 10;

class MainWindow;
class QTableWidget;
class QLabel;
class QPushButton;
class QLineEdit;
class QGridLayout;
class UserBriefInfo;

class RankingAuthorsScene : public Scene
{
    Q_OBJECT
    friend class MainWindow;
public:
    RankingAuthorsScene(QWidget *parent=nullptr);
    ~RankingAuthorsScene();
private:
    int page_limit = DEFAULT_AUTHOR_PAGE_LIMIT;
    std::vector<UserBriefInfo> info_query;
    MainWindow* parent;
    QLabel *title;
    QTableWidget *ranking;
    QLabel *cur_page;
    QPushButton *return_button, *next_button, *last_button;
    QPushButton *switch_button;
    QLineEdit *switch_edit;
    QGridLayout *lay;
    void switch_page(int page_count);
};

#endif // RANKINGAUTHORSSCENE_H
