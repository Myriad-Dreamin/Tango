#include "QueryUsersScene.h"

#include <QLayout>
#include "../mainwindow.h"

QueryUsersScene::QueryUsersScene(QWidget *parent): Scene(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);

    auto center_lay = new QVBoxLayout;


    lay = new QGridLayout;
    lay->addLayout(center_lay, 1, 1, 3, 3);
    lay->setRowStretch(0, 1);
    lay->setRowStretch(4, 1);
    lay->setColumnStretch(0, 1);
    lay->setColumnStretch(4, 1);


    this->setLayout(lay);
}

QueryUsersScene::~QueryUsersScene()
{

}
