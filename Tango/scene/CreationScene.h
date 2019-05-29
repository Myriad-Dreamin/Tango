#ifndef CREATIONSCENE_H
#define CREATIONSCENE_H

#include "scene_common.h"

/*
 * 提交单词的创造场景
 */
class CreationScene : public Scene
{
    Q_OBJECT
public:
    class CreationTableItem;
public:
    CreationScene(QWidget *parent=nullptr);
    ~CreationScene();

private:
    MainWindow *parent;
    Logger *logger;
    QGridLayout *lay;
    
    /* 表格 */
    int         creation_table_row;
    QLineEdit   *table_name_edit;
    QVBoxLayout *creation_table;
    QHBoxLayout *header_lay;
    QHBoxLayout *table_name_lay;

    CreationTableItem *make_creation_table_item();
    void insert_back_item(QWidget *row_widget);
    bool create_table();
    void reset_table();
    bool create_header();
    void try_submit_tangos();
public:
    class CreationTableItem : public PairTableItem
    {
    public:
        CreationTableItem(QWidget *parent = nullptr);
    };
};

#endif // CREATIONSCENE_H
