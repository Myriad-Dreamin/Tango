#ifndef QUERYUSERSSCENE_H
#define QUERYUSERSSCENE_H

#include "scene_common.h"


/*
 * 查询其他用户场景
 */
class QueryUsersScene: Scene
{
    Q_OBJECT
    friend class MainWindow;
public:
    QueryUsersScene(QWidget *parent);
    ~QueryUsersScene();
    void fill_info(const UserFullInfo &query_container);
private:
    MainWindow *parent;
    QGridLayout *lay;

    QPushButton *author_by_id_button;
    QPushButton *author_by_name_button;
    QPushButton *consumer_by_id_button;
    QPushButton *consumer_by_name_button;
    QPushButton *return_button;

    QLineEdit *query_edit;

    QLabel *user_id_key;
    QLabel *name_key;
    QLabel *tango_count_key;
    QLabel *misson_count_key;
    QLabel *level_key;
    QLabel *exp_key;
    QLabel *motto_key;
    
    QLabel *user_id;
    QLabel *name;
    QLabel *tango_count;
    QLabel *misson_count;
    QLabel *level;
    QLabel *exp;
    QLabel *motto;

    inline bool query_author_by_id(int id);
    inline bool query_author_by_name(QString name);
    inline bool query_consumer_by_id(int id);
    inline bool query_consumer_by_name(QString name);
};

#endif // QUERYUSERSSCENE_H
