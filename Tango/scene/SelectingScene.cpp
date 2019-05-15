
#include <functional>

#include <QPushButton>
#include <QLayout>
#include <QDebug>

#include "SelectingScene.h"

SelectingScene::SelectingScene(QWidget *parent): Scene(parent)
{

    auto buttons_lay = new QHBoxLayout;

    buttons_lay->addStretch(1);

    creation_button = new QPushButton;
    creation_button->setText("开始创造");
    buttons_lay->addWidget(creation_button);

    buttons_lay->addStretch(1);

    auto main_center_lay = new QVBoxLayout;
    main_center_lay->addLayout(buttons_lay);

    lay = new QGridLayout;
    lay->addLayout(main_center_lay, 1, 1);
    lay->setColumnStretch(0, 1);
    lay->setColumnStretch(2, 1);
    lay->setRowStretch(2, 1);
    lay->setRowStretch(0, 1);

    setLayout(lay);
}

SelectingScene::~SelectingScene()
{
    qDebug() << "selecting scene deleted";
}

void SelectingScene::set_creation_button_event(std::function<void ()> ev)
{
    connect(creation_button, &QPushButton::clicked, ev);
}
