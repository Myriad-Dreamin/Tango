
#include "CreationScene.h"
#include "SelectingScene.h"
#include "../mainwindow.h"


CreationScene::CreationScene(QWidget *parent): Scene(parent)
{
    this->logger = Logger::get_logger("main");
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
    logger->info() << "creation scene deleted";
}


/************************************** check *************************************/

bool is_legal(QCharRef x) {
    return x.isLetter() || x.isSpace();
}

bool tango_en_check(QString tango, QString &err_info) {
    int block_check=0;
    if (tango.length() > 0 && (tango[0] == ' ' || tango[tango.length() - 1] == ' ')) {
        err_info = "首尾不能有空格";
        return false;
    }
    for (int i = 0; i < tango.length(); i++) {
        if (!is_legal(tango[i])) {
            err_info = "存在不合法字符";
            return false;
        }
        if (tango[i] == ' ') {
            block_check++;
        } else {
            block_check = 0;
        }
        if (block_check > 1) {
            err_info = "空格不能连续超过两个";
            return false;
        }
    }
    return true;
}


/************************************** table *************************************/

/* 创建表格 */
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

/* 创建表头 */
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

/* 创建表项 */
CreationScene::CreationTableItem *CreationScene::make_creation_table_item()
{
    auto item = new CreationScene::CreationTableItem;

    item->set_delete_this_event([this, item](){
        this->creation_table_row --;
        logger->info() << "deleted";
        item->hide();
        item->deleteLater();
    });

    return item;
}

/* 重置表格 */
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
    auto DEFAULT_CREATION_TABLE_ITEMS_COUNT = this->parent->config_set[tr("default_creation_table_items_count")].toInt();
    for (int i = 0; i < DEFAULT_CREATION_TABLE_ITEMS_COUNT; i++) {
        this->insert_back_item(this->make_creation_table_item());
    }
}

/* 向creation table之后插入一项 */
void CreationScene::insert_back_item(QWidget *row_widget)
{
    logger->info() << "creating item" << row_widget;
    this->creation_table->insertWidget(creation_table_row - 1, row_widget, 1);
    creation_table_row++;
}

/* 提交单词 */
void CreationScene::try_submit_tangos()
{
    logger->info() << "try submitting";

    if (creation_table_row <= 1) {
        MessageBox::critical(this->parent, tr("错误"), "不能提交空的表格");
        return ;
    }

    if (this->table_name_edit->text().isEmpty()) {
        MessageBox::critical(this->parent, tr("错误"), "表格名不能为空");
        return ;
    }

    for (int i = creation_table_row - 2; i >= 0; i--) {
        logger->info() << "try getting" << i;
        auto item = dynamic_cast<CreationScene::CreationTableItem*>(this->creation_table->itemAt(i)->widget());
        logger->info() << "getting" << item;
        
        if (item->first->text().isEmpty() || item->second->text().isEmpty()) {
            MessageBox::critical(this->parent, tr("错误"), "有空的单词格未填写");
            return ;
        }
        
        QString err;
        if (!tango_en_check(item->first->text(), err)) {
            MessageBox::critical(this->parent, tr("单词合法性检查未通过"), err);
            return ;
        }
    }

    std::vector<TangoPair> tango_list;
    tango_list.reserve(static_cast<size_t>(creation_table_row - 1));
    for (int i = creation_table_row - 2; i >= 0; i--) {
        logger->info() << "try getting" << i;
        auto item = dynamic_cast<CreationScene::CreationTableItem*>(this->creation_table->itemAt(i)->widget());

        tango_list.emplace_back(TangoPair(item->first->text(), item->second->text()));
    }

    if (this->parent->submit_creation_table(tango_list)) {
        this->parent->client->sync_status();
        this->reset_table();
    }
}

/* 内嵌类CrationTableItem构造函数 */
CreationScene::CreationTableItem::CreationTableItem(QWidget *parent):
    PairTableItem("单词", "注释", parent)
{

}
