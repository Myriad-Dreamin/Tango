
#include "PlaySubScene.h"


PlaySubScene::PlaySubScene(QWidget *parent): Scene(parent)
{
    user_ret = new QLineEdit;
    answer_button = new QPushButton;
    answer_button->setText("回答");
    stop_button = new QPushButton;
    stop_button->setText("停止");

    key_label = new QLabel("");
    value_label = new QLabel("");

    play_lay = new QVBoxLayout;
    play_lay->addWidget(key_label);
    play_lay->addWidget(value_label);
    play_lay->addWidget(user_ret);
    play_lay->addWidget(answer_button);
    play_lay->addWidget(stop_button);


    this->lay = new QGridLayout;
    this->lay->addLayout(play_lay, 1, 1, 3, 3);
    this->lay->setColumnStretch(0, 1);
    this->lay->setColumnStretch(4, 1);
    this->lay->setRowStretch(4, 1);
    this->lay->setRowStretch(0, 1);
    this->setLayout(this->lay);
}

PlaySubScene::~PlaySubScene()
{

}
