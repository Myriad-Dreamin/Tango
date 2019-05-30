
#include "MultiPlayingScene.h"
#include "SelectingScene.h"
#include "../mainwindow.h"



MultiPlayingScene::MultiPlayingScene(MainWindow *parent) : Scene (parent)
{
    this->logger = Logger::get_logger("main");
    this->parent = parent;
    this->ranking_table_row = 0;
    auto main_center_lay = new QVBoxLayout;

    auto ten_button = new QPushButton("刷新");
    connect(ten_button, &QPushButton::clicked, [this]() mutable {
        this->parent->client->query_online_users();
    });

    auto return_button = new QPushButton("返回");
    connect(return_button, &QPushButton::clicked, [this]() mutable {
        this->parent->switch_scene(this->parent->selecting_scene);
    });



    main_center_lay->addWidget(ten_button);
    main_center_lay->addWidget(return_button);

    auto player_list_area = new QScrollArea;
    player_list = new QVBoxLayout;
    player_list->setAlignment(Qt::AlignmentFlag::AlignTop);

    auto policy = player_list_area->sizePolicy();
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    policy.setHorizontalPolicy(QSizePolicy::Preferred);
    player_list_area->setSizePolicy(policy);

    auto play_list_widget = new QWidget;
    play_list_widget->setLayout(player_list);
    auto policyx = play_list_widget->sizePolicy();
    policyx.setHorizontalPolicy(QSizePolicy::Preferred);
    policyx.setVerticalPolicy(QSizePolicy::Preferred);
    play_list_widget->setSizePolicy(policyx);
    play_list_widget->setMinimumWidth(400);
    QPalette palette = play_list_widget->palette();
    palette.setBrush(QPalette::Window,QBrush(QColor(72, 72, 73)));
    play_list_widget->setPalette(palette);


    player_list_area->setSizePolicy(policy);

    play_list_widget->setMinimumHeight(2500);
    player_list_area->setWidget(play_list_widget);
    player_list_area->setWidgetResizable(true);
    player_list_area->setMinimumWidth(400);
    player_list_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    auto header = new QHBoxLayout;
    header->addStretch(1);
    header->addWidget(new QLabel("挑战"), 8, Qt::AlignmentFlag::AlignCenter);
    header->addStretch(1);
    header->addWidget(new QLabel("id"), 8, Qt::AlignmentFlag::AlignCenter);
    header->addStretch(1);
    header->addWidget(new QLabel("昵称"), 8, Qt::AlignmentFlag::AlignCenter);
    header->addStretch(1);
    header->addWidget(new QLabel("等级"), 8, Qt::AlignmentFlag::AlignCenter);
    header->addStretch(1);
    header->addWidget(new QLabel("作者/玩家"), 8, Qt::AlignmentFlag::AlignCenter);
    header->addStretch(1);
    header->addWidget(new QLabel("查看信息"), 8, Qt::AlignmentFlag::AlignCenter);
    header->addStretch(1);
    player_list->addLayout(header);
    this->ranking_table_row = 1;
    main_center_lay->addWidget(player_list_area);


    lay = new QHBoxLayout;
    lay->addStretch(2);
    lay->addLayout(main_center_lay, 7);
    lay->addStretch(2);

    setLayout(lay);
    this->setMinimumSize(400, 400);
}

MultiPlayingScene::~MultiPlayingScene()
{

}

void MultiPlayingScene::on_incoming()
{
    this->clear_table();
}

void MultiPlayingScene::clear_table()
{
    for (int i = ranking_table_row - 1; i > 0; i--) {
        auto item = this->player_list->itemAt(i)->widget();
        item->hide();
        item->deleteLater();
    }
    this->ranking_table_row = 1;
}

void MultiPlayingScene::set_page_contain(std::vector<UserFullInfo> &authors_list, std::vector<UserFullInfo> &consumers_list, std::vector<long long> &socket_list)
{
    this->clear_table();
    a_list.clear();
    a_list.swap(authors_list);
    c_list.clear();
    c_list.swap(consumers_list);
    s_list.clear();
    s_list.swap(socket_list);


    for (unsigned int i = 0; i < a_list.size(); i++) {

        if (a_list[i].user_id != 0) {
            auto item = new QWidget;
            auto item_lay = new QHBoxLayout;
            QPushButton *challenge_button = new QPushButton("挑战"), *seek_button = new QPushButton("详细信息");
            QLabel * info_text = new QLabel, *id_text = new QLabel, *name_text = new QLabel, *level_text = new QLabel;

            id_text->setNum(a_list[i].user_id);
            name_text->setText(a_list[i].name);
            level_text->setNum(a_list[i].level);
            info_text->setText("作者");
            connect(challenge_button, &QPushButton::clicked, [this]() {
                MessageBox::critical(this, tr("错误"), tr("你不能挑战作者"));
            });
            connect(seek_button, &QPushButton::clicked, [=]() {
                static wchar_t x[500];
                QString info;
                auto &uinfo = this->a_list[i];
                uinfo.name.toWCharArray(x);
                MessageBox::info_text(this, tr("用户信息"), info.sprintf(
                    "id: %d\n昵称: %ls\n提交单词量: %d\n提交关卡量: %d\n等级: %d\n经验: %d",
                    uinfo.user_id,
                    x,
                    uinfo.tango_count,
                    uinfo.misson_count,
                    uinfo.level,
                    uinfo.exp
                ));
            });

            item_lay->addStretch(1);
            item_lay->addWidget(challenge_button, 8, Qt::AlignmentFlag::AlignCenter);
            item_lay->addStretch(1);
            item_lay->addWidget(id_text, 8, Qt::AlignmentFlag::AlignCenter);
            item_lay->addStretch(1);
            item_lay->addWidget(name_text, 8, Qt::AlignmentFlag::AlignCenter);
            item_lay->addStretch(1);
            item_lay->addWidget(level_text, 8, Qt::AlignmentFlag::AlignCenter);
            item_lay->addStretch(1);
            item_lay->addWidget(info_text, 8, Qt::AlignmentFlag::AlignCenter);
            item_lay->addStretch(1);
            item_lay->addWidget(seek_button, 8, Qt::AlignmentFlag::AlignCenter);
            item_lay->addStretch(1);
            item->setLayout(item_lay);
            this->player_list->insertWidget(ranking_table_row, item);
            this->ranking_table_row++;
        }

        if (c_list[i].user_id != 0) {

            auto item = new QWidget;
            auto item_lay = new QHBoxLayout;
            QPushButton *challenge_button = new QPushButton("挑战"), *seek_button = new QPushButton("详细信息");
            QLabel * info_text = new QLabel, *id_text = new QLabel, *name_text = new QLabel, *level_text = new QLabel;

            id_text->setNum(c_list[i].user_id);
            name_text->setText(c_list[i].name);
            level_text->setNum(c_list[i].level);
            info_text->setText("玩家");

            connect(challenge_button, &QPushButton::clicked, [this]() {
                MessageBox::critical(this, tr("错误"), tr("TODO"));
            });
            connect(seek_button, &QPushButton::clicked, [=]() {
                static wchar_t x[500];

                QString info;
                auto &uinfo = this->c_list[i];
                uinfo.name.toWCharArray(x);
                MessageBox::info_text(this, tr("用户信息"), info.sprintf(
                    "id: %d\n昵称: %ls\n通过单词量: %d\n通过关卡量: %d\n等级: %d\n经验: %d",
                    uinfo.user_id,
                    x,
                    uinfo.tango_count,
                    uinfo.misson_count,
                    uinfo.level,
                    uinfo.exp
                ));
            });


            item_lay->addStretch(1);
            item_lay->addWidget(challenge_button, 8, Qt::AlignmentFlag::AlignCenter);
            item_lay->addStretch(1);
            item_lay->addWidget(id_text, 8, Qt::AlignmentFlag::AlignCenter);
            item_lay->addStretch(1);
            item_lay->addWidget(name_text, 8, Qt::AlignmentFlag::AlignCenter);
            item_lay->addStretch(1);
            item_lay->addWidget(level_text, 8, Qt::AlignmentFlag::AlignCenter);
            item_lay->addStretch(1);
            item_lay->addWidget(info_text, 8, Qt::AlignmentFlag::AlignCenter);
            item_lay->addStretch(1);
            item_lay->addWidget(seek_button, 8, Qt::AlignmentFlag::AlignCenter);
            item_lay->addStretch(1);
            item->setLayout(item_lay);
            this->player_list->insertWidget(ranking_table_row, item);
            this->ranking_table_row++;
        }

    }
}
