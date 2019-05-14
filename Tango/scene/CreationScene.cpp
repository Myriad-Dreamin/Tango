
#include <functional>

#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>

#include "CreationScene.h"

CreationScene::CreationScene(QWidget *parent): Scene(parent)
{

    auto header_lay = new QHBoxLayout;

    header_lay->addStretch(1);
    auto header = new QLabel("Creation Space");
    header_lay->addWidget(header);

    lay = new QGridLayout;
    lay->setColumnStretch(0, 1);
    lay->addLayout(header_lay, 1, 1);
    lay->setColumnStretch(10, 1);
    lay->setRowStretch(10, 1);
    lay->setRowStretch(0, 1);

    setLayout(lay);
}

CreationScene::~CreationScene()
{

}
