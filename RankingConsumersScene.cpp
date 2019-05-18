
#include "RankingConsumersScene.h"

#include <QLayout>
#include <QStringList>
#include <QHeaderView>
#include <QScrollBar>
#include <QLabel>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include "../mainwindow.h"
#include "../client/Client.h"
#include "../types/UserBriefInfo.h"

#include "SelectingScene.h"

RankingConsumersScene::RankingConsumersScene(QWidget *parent): Scene(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);

    auto center_lay = new QVBoxLayout;

    title = new QLabel("读者排名");

    ranking = new QTableWidget;
    QStringList header;
    header << "id" << "昵称" << "等级";
    ranking->setHorizontalHeaderLabels(header);

    auto pagebar_lay = new QHBoxLayout;
    last_button = new QPushButton("<");

    connect(last_button, &QPushButton::clicked, [this]() mutable{
        if (cur_page->text().toInt() > 1) {
            this->switch_page(cur_page->text().toInt() - 1);
        }
    });

    pagebar_lay->addWidget(last_button, 1);
    cur_page = new QLabel("1");
    pagebar_lay->addWidget(cur_page, 1);
    next_button = new QPushButton(">");

    connect(next_button, &QPushButton::clicked, [this]() mutable{
        this->switch_page(cur_page->text().toInt() + 1);
    });

    pagebar_lay->addWidget(next_button, 1);
    switch_button = new QPushButton("->");
    pagebar_lay->addWidget(switch_button, 1);
    switch_edit = new QLineEdit;
    pagebar_lay->addWidget(switch_edit, 1);

    connect(switch_button, &QPushButton::clicked, [this]() mutable {
        bool ok = false;
        int k = this->switch_edit->text().toInt(&ok);
        if (ok && k > 0) {
            this->switch_page(k);
        }
    });

    return_button = new QPushButton("返回");

    connect(return_button, &QPushButton::clicked, [this]() mutable {
        this->parent->switch_scene(this->parent->selecting_scene);
    });

    center_lay->addWidget(title, 1);
    center_lay->addWidget(ranking, 1);
    center_lay->addLayout(pagebar_lay, 1);
    center_lay->addWidget(return_button, 1);

    this->ranking->horizontalScrollBar()->setEnabled(false);
    this->ranking->horizontalScrollBar()->setVisible(false);
    this->ranking->horizontalHeader()->setStretchLastSection(true);
    this->ranking->verticalHeader()->setStretchLastSection(true);
    lay = new QGridLayout;
    lay->addLayout(center_lay, 1, 1, 3, 3);
    lay->setRowStretch(0, 1);
    lay->setRowStretch(4, 1);
    lay->setColumnStretch(0, 1);
    lay->setColumnStretch(4, 1);
    this->setLayout(lay);
}

void RankingConsumersScene::switch_page(int page_count)
{
    this->cur_page->setNum(page_count);

    this->ranking->clearContents();
    this->info_query.clear();
    this->parent->client->query_authors_brief_info(
        info_query, (page_count - 1) * this->page_limit, page_count * this->page_limit
    );
    this->ranking->setRowCount(10);
    this->ranking->setColumnCount(3);
    for (unsigned int i = 0; i < info_query.size(); i++) {
        QLabel *idL = new QLabel, *nameL = new QLabel, *levelL = new QLabel;
        idL->setNum(info_query[i].id);
        nameL->setText(info_query[i].name);
        levelL->setNum(info_query[i].level);
        this->ranking->setCellWidget(static_cast<int>(i), 0, idL);
        this->ranking->setCellWidget(static_cast<int>(i), 1, nameL);
        this->ranking->setCellWidget(static_cast<int>(i), 2, levelL);
    }
}


RankingConsumersScene::~RankingConsumersScene()
{

}
