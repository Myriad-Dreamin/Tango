
#include <functional>

#include <vector>

#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QDebug>
#include <QMessageBox>

#include "../mainwindow.h"
#include "../types/TangoPair.h"
#include "CreationScene.h"
#include "SelectingScene.h"
#include "CreationTableItem.h"

CreationScene::CreationScene(QWidget *parent): Scene(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);
    creation_table_row = 0;

    this->create_header();
    this->create_table();

    auto submit_lay = new QHBoxLayout;
    auto submit_button = new QPushButton("提交");
    auto submit_reset_button = new QPushButton("重置");
    auto submit_cancel_button = new QPushButton("取消");

    connect(submit_button, &QPushButton::clicked, [this](){
        this->try_submit_tangos();
    });

    connect(submit_reset_button, &QPushButton::clicked, [this](){
        this->reset_table();
    });

    connect(submit_cancel_button, &QPushButton::clicked, [this](){
        this->parent->switch_scene(this->parent->selecting_scene);
        this->reset_table();
    });


    submit_lay->addWidget(submit_button, 1);
    submit_lay->addWidget(submit_reset_button, 1);
    submit_lay->addWidget(submit_cancel_button, 1);




    lay = new QGridLayout;
    lay->setColumnStretch(0, 1);
    lay->addLayout(header_lay, 1, 1, 1, 1, Qt::Alignment(Qt::AlignmentFlag::AlignLeft));
    lay->addLayout(table_name_lay, 1, 2, 1, 1);
    lay->addLayout(creation_table, 2, 1, 1, 2);
    lay->addLayout(submit_lay, 3, 1, 1, 2);
    lay->setColumnStretch(4, 1);
    lay->setRowStretch(4, 1);
    lay->setRowStretch(0, 1);

    setLayout(lay);
}

CreationScene::~CreationScene()
{
    qDebug() << "creation scene deleted";
}

CreationTableItem *CreationScene::make_creation_table_item()
{
    auto item = new CreationTableItem;
    item->set_delete_this_event([this, item](){
        this->creation_table_row --;
        qDebug() << "deleted";
        item->hide();
        item->deleteLater();
    });
    return item;
}

bool CreationScene::create_header()
{
    this->header_lay = new QHBoxLayout;

    header_lay->addStretch(1);
    auto header = new QLabel("Creation Space");
    header_lay->addWidget(header, 1);

    this->table_name_lay = new QHBoxLayout;

    auto table_name_header = new QLabel("表名");
    table_name_lay->addWidget(table_name_header);
    table_name_edit = new QLineEdit;
    table_name_lay->addWidget(table_name_edit, 1);

    return true;
}


bool CreationScene::create_table()
{
    creation_table = new QVBoxLayout;

    auto insert_button = new QPushButton("+");
    connect(insert_button, &QPushButton::clicked, [this]() mutable {
        this->insert_back_item(this->make_creation_table_item());
    });
    auto insert_lay = new QHBoxLayout;
    insert_lay->addStretch(1);
    insert_lay->addWidget(insert_button);
    insert_lay->addStretch(1);

    auto insert_it_widget = new QWidget;
    insert_it_widget->setLayout(insert_lay);
    this->creation_table->addWidget(insert_it_widget);
    this->creation_table_row = 1;

    this->reset_table();
    return true;
}


void CreationScene::reset_table()
{
    if (creation_table_row > 1) {
        for (creation_table_row -= 2;creation_table_row >= 0;creation_table_row--) {
            auto item = this->creation_table->itemAt(creation_table_row)->widget();
            item->hide();
            item->deleteLater();
        }
        creation_table_row = 1;
    }
    for (int i = 0; i < DEFAULT_CREATION_TABLE_ITEMS_COUNT; i++) {
        this->insert_back_item(this->make_creation_table_item());
    }
}


void CreationScene::insert_back_item(QWidget *row_widget)
{
    qDebug() << "creating item" << row_widget;
    this->creation_table->insertWidget(creation_table_row - 1, row_widget, 1);
    creation_table_row++;
}

void CreationScene::try_submit_tangos()
{
    qDebug() << "try submitting";
    if (creation_table_row <= 1) {
        QMessageBox::critical(this->parent, tr("错误"), "不能提交空的表格", QMessageBox::Ok);
        return ;
    }

    if (this->table_name_edit->text().isEmpty()) {
        QMessageBox::critical(this->parent, tr("错误"), "表格名不能为空", QMessageBox::Ok);
        return ;
    }


    for (int i = creation_table_row - 2; i >= 0; i--) {
        qDebug() << "try getting" << i;
        auto item = dynamic_cast<CreationTableItem*>(this->creation_table->itemAt(i)->widget());
        qDebug() << "getting" << item;
        if (item->first->text().isEmpty() || item->second->text().isEmpty()) {
            QMessageBox::critical(this->parent, tr("错误"), "有空的单词格未填写", QMessageBox::Ok);
            return ;
        }
    }

    std::vector<TangoPair> tango_list;
    tango_list.reserve(static_cast<size_t>(creation_table_row - 1));
    for (int i = creation_table_row - 2; i >= 0; i--) {
        qDebug() << "try getting" << i;
        auto item = dynamic_cast<CreationTableItem*>(this->creation_table->itemAt(i)->widget());

        tango_list.emplace_back(TangoPair(item->first->text(), item->second->text()));
    }
    if (this->parent->submit_creation_table(tango_list)) {
        this->reset_table();
    }
}
