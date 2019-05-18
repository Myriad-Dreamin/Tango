
#include "RankingAuthorsScene.h"

#include <QLayout>
#include <QStringList>
#include <QLabel>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include "../mainwindow.h"
#include "../client/Client.h"
#include "../types/UserBriefInfo.h"

RankingAuthorsScene::RankingAuthorsScene(QWidget *parent): Scene(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);

    auto center_lay = new QVBoxLayout;

    title = new QLabel("作者排名");

    ranking = new QTableWidget;
    QStringList header;
    header << "id" << "昵称" << "等级";
    ranking->setHorizontalHeaderLabels(header);

    auto pagebar_lay = new QHBoxLayout;
    last_button = new QPushButton;
    pagebar_lay->addWidget(last_button, 1);
    cur_page = new QLabel;
    pagebar_lay->addWidget(cur_page, 1);
    next_button = new QPushButton;
    pagebar_lay->addWidget(next_button, 1);
    switch_button = new QPushButton;
    pagebar_lay->addWidget(switch_button, 1);
    switch_edit = new QLineEdit;
    pagebar_lay->addWidget(switch_edit, 1);

    return_button = new QPushButton;

    center_lay->addWidget(title);
    center_lay->addWidget(ranking);
    center_lay->addLayout(pagebar_lay);
    center_lay->addWidget(return_button);

    lay = new QGridLayout;
    lay->addLayout(center_lay, 1, 1, 3, 3);
    lay->setRowStretch(0, 1);
    lay->setRowStretch(4, 1);
    lay->setColumnStretch(0, 1);
    lay->setColumnStretch(4, 1);
    this->setLayout(lay);
}

void RankingAuthorsScene::switch_page(int page_count)
{
    this->ranking->clearContents();
    this->info_query.clear();
    this->parent->client->query_authors_brief_info(
        info_query, page_count * this->page_limit, (page_count + 1) * this->page_limit
    );
    for (unsigned int i = 0; i < info_query.size(); i++) {
        QLabel *idL = new QLabel(this->ranking),
               *nameL = new QLabel(this->ranking),
               *levelL = new QLabel(this->ranking);
        idL->setNum(info_query[i].id);
        nameL->setText(info_query[i].name);
        levelL->setNum(info_query[i].level);
        this->ranking->setCellWidget(static_cast<int>(i), 0, idL);
        this->ranking->setCellWidget(static_cast<int>(i), 0, nameL);
        this->ranking->setCellWidget(static_cast<int>(i), 0, levelL);
    }
}


RankingAuthorsScene::~RankingAuthorsScene()
{

}
