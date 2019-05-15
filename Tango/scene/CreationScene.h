#ifndef CREATIONSCENE_H
#define CREATIONSCENE_H

#include "Scene.h"

const int DEFAULT_CREATION_TABLE_ITEMS_COUNT = 3;


namespace std
{
    template<typename function_signature>
    class function;
}

class QGridLayout;
class QLineEdit;
class QVBoxLayout;
class QHBoxLayout;
class CreationTableItem;
class MainWindow;



class CreationScene : public Scene
{
    Q_OBJECT
private:
    MainWindow *parent;

    QGridLayout *lay;
    QHBoxLayout *header_lay, *table_name_lay;

    int creation_table_row;
    QVBoxLayout *creation_table;

    CreationTableItem *make_creation_table_item();
    void insert_back_item(QWidget *row_widget);
    bool create_table();
    void reset_table();
    bool create_header();
    void try_submit_tangos();
public:
    QLineEdit *table_name_edit;



    CreationScene(QWidget *parent=nullptr);
    ~CreationScene();
};

#endif // CREATIONSCENE_H
