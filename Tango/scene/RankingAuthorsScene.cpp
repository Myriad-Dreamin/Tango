
#include "RankingAuthorsScene.h"


RankingAuthorsScene::RankingAuthorsScene(QWidget *parent): Scene(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);
    this->ranking_table_row = 0;

    auto center_lay = new QVBoxLayout;

    title = new QLabel("作者排名");

    ranking = new QVBoxLayout;


    auto header = new QHBoxLayout;
    header->addStretch(1);
    header->addWidget(new QLabel("排名"), 8, Qt::AlignmentFlag::AlignCenter);
    header->addStretch(1);
    header->addWidget(new QLabel("id"), 8, Qt::AlignmentFlag::AlignCenter);
    header->addStretch(1);
    header->addWidget(new QLabel("昵称"), 8, Qt::AlignmentFlag::AlignCenter);
    header->addStretch(1);
    header->addWidget(new QLabel("等级"), 8, Qt::AlignmentFlag::AlignCenter);
    header->addStretch(1);
    ranking->addLayout(header, 1);
    this->ranking_table_row = 1;

    auto pagebar_lay = new QHBoxLayout;
    last_button = new QPushButton("<");
    last_button->setMinimumHeight(20);
    connect(last_button, &QPushButton::clicked, [this]() mutable{
        if (cur_page->text().toInt() > 1) {
            this->switch_page(cur_page->text().toInt() - 1);
        }
    });
    pagebar_lay->addWidget(last_button, 1);

    cur_page = new QLabel("1");
    pagebar_lay->addWidget(cur_page, 1, Qt::AlignmentFlag::AlignCenter);

    next_button = new QPushButton(">");
    connect(next_button, &QPushButton::clicked, [this]() mutable{
        this->switch_page(cur_page->text().toInt() + 1);
    });
    next_button->setMinimumHeight(20);
    pagebar_lay->addWidget(next_button, 1);


    switch_button = new QPushButton("->");
    connect(switch_button, &QPushButton::clicked, [this]() mutable {
        bool ok = false;
        int k = this->switch_edit->text().toInt(&ok);
        if (ok && k > 0) {
            this->switch_page(k);
        }
    });
    switch_button->setMinimumHeight(20);
    pagebar_lay->addWidget(switch_button, 1);

    switch_edit = new QLineEdit;
    switch_edit->setMinimumHeight(20);
    pagebar_lay->addWidget(switch_edit, 1);

    return_button = new QPushButton("返回");
    connect(return_button, &QPushButton::clicked, [this]() mutable {
        this->parent->switch_scene(this->parent->selecting_scene);
    });
    return_button->setMinimumHeight(20);

    center_lay->addStretch(1);
    center_lay->addWidget(title, 1, Qt::AlignmentFlag::AlignCenter);
    center_lay->addLayout(ranking, 15);
    center_lay->addLayout(pagebar_lay, 1);
    center_lay->addWidget(return_button, 1);
    center_lay->addStretch(1);

    lay = new QHBoxLayout;
    lay->addStretch(1);
    lay->addLayout(center_lay, 8);
    lay->addStretch(1);
    this->setLayout(lay);

}

void RankingAuthorsScene::clear_table()
{
    for (int i = ranking_table_row - 1; i > 0; i--) {
        auto item = this->ranking->itemAt(i)->widget();
        item->hide();
        item->deleteLater();
    }
    this->ranking_table_row = 1;
}

void RankingAuthorsScene::switch_page(int page_count)
{
    this->cur_page->setNum(page_count);

    this->clear_table();
    this->info_query.clear();
    this->parent->client->query_authors_brief_info(
        info_query, (page_count - 1) * this->page_limit, page_count * this->page_limit - 1
    );

    for (unsigned int i = 0; i < info_query.size(); i++) {
        auto item = new QWidget;
        auto item_lay = new QHBoxLayout;
        QLabel * ranking_text = new QLabel, *id_text = new QLabel, *name_text = new QLabel, *level_text = new QLabel;
        ranking_text->setNum(static_cast<int>(i) + page_count * 10 - 9);
        id_text->setNum(info_query[i].id);
        name_text->setText(info_query[i].name);
        level_text->setNum(info_query[i].level);
        item_lay->addStretch(1);
        item_lay->addWidget(ranking_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item_lay->addWidget(id_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item_lay->addWidget(name_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item_lay->addWidget(level_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item->setLayout(item_lay);
        this->ranking->insertWidget(static_cast<int>(i + 1), item, 1);
        this->ranking_table_row++;
    }
    for (int i = static_cast<int>(info_query.size()); i < page_limit; i++) {
        auto item = new QWidget;
        auto item_lay = new QHBoxLayout;
        QLabel * ranking_text = new QLabel, *id_text = new QLabel, *name_text = new QLabel, *level_text = new QLabel;
        ranking_text->setNum(i + page_count * 10 - 9);
        item_lay->addStretch(1);
        item_lay->addWidget(ranking_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item_lay->addWidget(id_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item_lay->addWidget(name_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item_lay->addWidget(level_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item->setLayout(item_lay);
        this->ranking->insertWidget(i + 1, item, 1);
        this->ranking_table_row++;
    }
}

void RankingAuthorsScene::set_page_contain(const std::vector<UserBriefInfo> &info_query)
{
    this->clear_table();

    for (unsigned int i = 0; i < info_query.size(); i++) {
        auto item = new QWidget;
        auto item_lay = new QHBoxLayout;
        QLabel * ranking_text = new QLabel, *id_text = new QLabel, *name_text = new QLabel, *level_text = new QLabel;
        ranking_text->setNum(static_cast<int>(i) + this->cur_page->text().toInt() * 10 - 9);
        id_text->setNum(info_query[i].id);
        name_text->setText(info_query[i].name);
        level_text->setNum(info_query[i].level);
        item_lay->addStretch(1);
        item_lay->addWidget(ranking_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item_lay->addWidget(id_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item_lay->addWidget(name_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item_lay->addWidget(level_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item->setLayout(item_lay);
        this->ranking->insertWidget(static_cast<int>(i + 1), item, 1);
        this->ranking_table_row++;
    }
    for (int i = static_cast<int>(info_query.size()); i < page_limit; i++) {
        auto item = new QWidget;
        auto item_lay = new QHBoxLayout;
        QLabel * ranking_text = new QLabel, *id_text = new QLabel, *name_text = new QLabel, *level_text = new QLabel;
        ranking_text->setNum(i + this->cur_page->text().toInt() * 10 - 9);
        item_lay->addStretch(1);
        item_lay->addWidget(ranking_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item_lay->addWidget(id_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item_lay->addWidget(name_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item_lay->addWidget(level_text, 8, Qt::AlignmentFlag::AlignCenter);
        item_lay->addStretch(1);
        item->setLayout(item_lay);
        this->ranking->insertWidget(i + 1, item, 1);
        this->ranking_table_row++;
    }
}


RankingAuthorsScene::~RankingAuthorsScene()
{

}
