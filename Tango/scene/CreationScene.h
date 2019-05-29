#ifndef CREATIONSCENE_H
#define CREATIONSCENE_H

#include "Scene.h"

const int DEFAULT_CREATION_TABLE_ITEMS_COUNT = 3;


namespace std
{
    template<typename function_signature>
    class function;
}

class MainWindow;

class QLineEdit;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;

class CreationTableItem;


class CreationScene : public Scene
{
    Q_OBJECT

public:
    CreationScene(QWidget *parent=nullptr);
    ~CreationScene();

    QLineEdit *table_name_edit;

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
};

#endif // CREATIONSCENE_H
