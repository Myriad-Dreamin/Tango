#include "QueryUsersScene.h"

#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>


#include "../mainwindow.h"
#include "SelectingScene.h"
#include "../../TangoCommon/client/Client.h"
#include "../../TangoCommon/types/MessageBox.h"
#include "../../TangoCommon/types/UserFullInfo.h"

QueryUsersScene::QueryUsersScene(QWidget *parent): Scene(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);

    auto center_lay = new QVBoxLayout;
    QHBoxLayout *lay;

    auto user_info_lay = new QVBoxLayout;
    auto user_id_lay = new QHBoxLayout;
    user_id_key = new QLabel("id:");
    user_id = new QLabel("");
    user_id_lay->addWidget(user_id_key);
    user_id_lay->addWidget(user_id, 7);

    auto name_lay = new QHBoxLayout;
    name_key = new QLabel("昵称:");
    name = new QLabel("");
    name_lay->addWidget(name_key);
    name_lay->addWidget(name, 7);

    auto tango_count_lay = new QHBoxLayout;
    tango_count_key = new QLabel("通过单词数:");
    tango_count = new QLabel("");
    tango_count_lay->addWidget(tango_count_key);
    tango_count_lay->addWidget(tango_count, 7);

    auto misson_count_lay = new QHBoxLayout;
    misson_count_key = new QLabel("闯关数:");
    misson_count = new QLabel("");
    misson_count_lay->addWidget(misson_count_key);
    misson_count_lay->addWidget(misson_count, 7);

    auto level_lay = new QHBoxLayout;
    level_key = new QLabel("等级:");
    level = new QLabel("");
    level_lay->addWidget(level_key);
    level_lay->addWidget(level, 7);

    auto exp_lay = new QHBoxLayout;
    exp_key = new QLabel("经验:");
    exp = new QLabel("");
    exp_lay->addWidget(exp_key);
    exp_lay->addWidget(exp, 7);

    auto motto_lay = new QHBoxLayout;
    motto_key = new QLabel("签名:");
    motto = new QLabel("");
    motto_lay->addWidget(motto_key);
    motto_lay->addWidget(motto, 7);

    center_lay->addStretch(1);
    user_info_lay->addLayout(user_id_lay);
    user_info_lay->addLayout(name_lay);
    user_info_lay->addLayout(tango_count_lay);
    user_info_lay->addLayout(misson_count_lay);
    user_info_lay->addLayout(level_lay);
    user_info_lay->addLayout(exp_lay);
    user_info_lay->addLayout(motto_lay);

    center_lay->addLayout(user_info_lay);
    center_lay->addStretch(1);

    auto query_lay = new QVBoxLayout;

    query_edit = new QLineEdit;
    query_lay->addWidget(query_edit);

    auto author_query_lay = new QHBoxLayout;

    author_by_id_button = new QPushButton("根据id查找作者");
    connect(author_by_id_button, &QPushButton::clicked, [this]() mutable {

        bool ok;
        int id = this->query_edit->text().toInt(&ok);
        if (!ok) {
            MessageBox::critical(this, "错误", "不能将输入转化为整数查询");
            return;
        }

        this->tango_count_key->setText("提交单词数:");
        this->misson_count_key->setText("提交单词表数:");

        this->query_author_by_id(id);
    });

    author_by_name_button = new QPushButton("根据昵称查找作者");
    connect(author_by_name_button, &QPushButton::clicked, [this]() mutable {

        this->tango_count_key->setText("提交单词数:");
        this->misson_count_key->setText("提交单词表数:");

        this->query_author_by_name(this->query_edit->text());
    });

    author_query_lay->addWidget(author_by_id_button, 4);
    author_query_lay->addWidget(author_by_name_button, 4);
    query_lay->addLayout(author_query_lay);

    auto consumer_query_lay = new QHBoxLayout;

    consumer_by_id_button = new QPushButton("根据id查找读者");
    connect(consumer_by_id_button, &QPushButton::clicked, [this]() mutable {

        bool ok;
        int id = this->query_edit->text().toInt(&ok);
        if (!ok) {
            MessageBox::critical(this, "错误", "不能将输入转化为整数查询");
            return;
        }

        this->tango_count_key->setText("通过单词数:");
        this->misson_count_key->setText("闯关数:");

        this->query_consumer_by_id(id);
    });

    consumer_by_name_button = new QPushButton("根据昵称查找读者");
    connect(consumer_by_name_button, &QPushButton::clicked, [this]() mutable {

        this->tango_count_key->setText("通过单词数:");
        this->misson_count_key->setText("闯关数:");

        this->query_consumer_by_name(this->query_edit->text());
    });

    consumer_query_lay->addWidget(consumer_by_id_button, 4);
    consumer_query_lay->addWidget(consumer_by_name_button, 4);

    center_lay->addLayout(query_lay);

    return_button = new QPushButton("返回");
    connect(return_button, &QPushButton::clicked, [this]() mutable {
        this->parent->switch_scene(this->parent->selecting_scene);
    });

    center_lay->addWidget(return_button);
    center_lay->addStretch(1);


    lay = new QHBoxLayout;
    query_lay->addLayout(consumer_query_lay);
    lay->addStretch(1);
    lay->addLayout(center_lay, 8);
    lay->addStretch(1);


    this->setLayout(lay);
}

bool QueryUsersScene::query_author_by_id(int id)
{
    UserFullInfo query_container;
    if (!this->parent->client->query_authors_by_id(query_container, id)) {
        MessageBox::critical(this, "错误", this->parent->client->last_error());
        return false;
    }

    this->user_id->setNum(query_container.user_id);
    this->tango_count->setNum(query_container.tango_count);
    this->misson_count->setNum(query_container.misson_count);
    this->level->setNum(query_container.level);
    this->exp->setNum(query_container.exp);
    this->name->setText(query_container.name);
    this->motto->setText(query_container.motto);

    return true;
}

bool QueryUsersScene::query_consumer_by_id(int id)
{
    UserFullInfo query_container;
    if (!this->parent->client->query_consumers_by_id(query_container, id)) {
        MessageBox::critical(this, "错误", this->parent->client->last_error());
        return false;
    }

    this->user_id->setNum(query_container.user_id);
    this->tango_count->setNum(query_container.tango_count);
    this->misson_count->setNum(query_container.misson_count);
    this->level->setNum(query_container.level);
    this->exp->setNum(query_container.exp);
    this->name->setText(query_container.name);
    this->motto->setText(query_container.motto);

    return true;
}

bool QueryUsersScene::query_author_by_name(QString name)
{
    UserFullInfo query_container;
    if (!this->parent->client->query_authors_by_name(query_container, name)) {
        MessageBox::critical(this, "错误", this->parent->client->last_error());
        return false;
    }

    this->user_id->setNum(query_container.user_id);
    this->tango_count->setNum(query_container.tango_count);
    this->misson_count->setNum(query_container.misson_count);
    this->level->setNum(query_container.level);
    this->exp->setNum(query_container.exp);
    this->name->setText(query_container.name);
    this->motto->setText(query_container.motto);

    return true;
}

bool QueryUsersScene::query_consumer_by_name(QString name)
{
    UserFullInfo query_container;
    if (!this->parent->client->query_consumers_by_name(query_container, name)) {
        MessageBox::critical(this, "错误", this->parent->client->last_error());
        return false;
    }

    this->user_id->setNum(query_container.user_id);
    this->tango_count->setNum(query_container.tango_count);
    this->misson_count->setNum(query_container.misson_count);
    this->level->setNum(query_container.level);
    this->exp->setNum(query_container.exp);
    this->name->setText(query_container.name);
    this->motto->setText(query_container.motto);

    return true;
}

void QueryUsersScene::fill_info(const UserFullInfo &query_container)
{
    this->user_id->setNum(query_container.user_id);
    this->tango_count->setNum(query_container.tango_count);
    this->misson_count->setNum(query_container.misson_count);
    this->level->setNum(query_container.level);
    this->exp->setNum(query_container.exp);
    this->name->setText(query_container.name);
    this->motto->setText(query_container.motto);
}



QueryUsersScene::~QueryUsersScene()
{

}
