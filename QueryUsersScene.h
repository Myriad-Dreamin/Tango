#ifndef QUERYUSERSSCENE_H
#define QUERYUSERSSCENE_H

#include "Scene.h"

class MainWindow;
class QGridLayout;
class QPushButton;
class QLineEdit;

class QueryUsersScene: Scene
{
    Q_OBJECT
    friend class MainWindow;
public:
    QueryUsersScene(QWidget *parent);
    ~QueryUsersScene();
private:
    MainWindow *parent;
    QGridLayout *lay;


    void query_author_by_id();
    void query_author_by_name();
    void query_consumer_by_id();
    void query_consumer_by_name();
};

#endif // QUERYUSERSSCENE_H
