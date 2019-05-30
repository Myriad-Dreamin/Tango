#ifndef QUERYUSERSSCENE_H
#define QUERYUSERSSCENE_H

#include "Scene.h"

class MainWindow;
class QGridLayout;
class QPushButton;
class QLineEdit;
class QLabel;
class UserFullInfo;

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
    QLabel *user_id_key, *name_key, *tango_count_key, *misson_count_key, *level_key, *exp_key, *motto_key;
    QLabel *user_id, *name, *tango_count, *misson_count, *level, *exp, *motto;

    QLineEdit *query_edit;
    QPushButton *author_by_id_button, *author_by_name_button;
    QPushButton *consumer_by_id_button, *consumer_by_name_button;
    QPushButton *return_button;


    inline bool query_author_by_id(int id);
    inline bool query_author_by_name(QString name);
    inline bool query_consumer_by_id(int id);
    inline bool query_consumer_by_name(QString name);
};

#endif // QUERYUSERSSCENE_H
