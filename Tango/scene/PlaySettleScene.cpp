
#include "PlaySettleScene.h"


PlaySettleScene::PlaySettleScene(QWidget *parent): Scene(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);
    from_exp = new QLabel;
    to_exp = new QLabel;
    from_level = new QLabel;
    to_level = new QLabel;
    level_flag = new QLabel;
    confirm_button = new QPushButton("确认");

    connect(this->confirm_button, &QPushButton::clicked, [this]() mutable {
        this->parent->switch_scene(this->parent->selecting_scene);
    });
    lay = new QGridLayout;

    lay->setRowStretch(0, 1);
    lay->setColumnStretch(0, 1);
    lay->addWidget(from_exp, 1, 1);
    lay->addWidget(new QLabel("→"), 1, 2);
    lay->addWidget(to_exp, 1, 3);
    lay->addWidget(from_level, 2, 1);
    lay->addWidget(new QLabel("→"), 2, 2);
    lay->addWidget(to_level, 2, 3);
    lay->addWidget(level_flag, 4, 1, 1, 3);
    lay->addWidget(confirm_button, 6, 1, 1, 3);
    lay->setRowStretch(7, 1);
    lay->setColumnStretch(7, 1);

    this->setLayout(lay);
}

PlaySettleScene::~PlaySettleScene()
{

}
