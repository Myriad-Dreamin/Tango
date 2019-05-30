
#include "PairTableItem.h"

/* 标准库 */
#include <functional>

/* 工具库 */
#include <QDebug>

/* 控件库 */
#include <QLayout>

#include <QLabel>
#include <QLineEdit>

#include <QPushButton>


PairTableItem::PairTableItem(QString first_label, QString second_label, QWidget *parent):
    QWidget(parent)
{
    auto lay = new QHBoxLayout(this);

    this->delete_this = new QPushButton("-", this);
    this->first = new QLineEdit(this);
    this->second = new QLineEdit(this);

    lay->addWidget(this->delete_this);
    lay->addWidget(new QLabel(first_label, this));
    lay->addWidget(this->first, 1);
    lay->addWidget(new QLabel(second_label, this));
    lay->addWidget(this->second, 1);

    this->setLayout(lay);
}

PairTableItem::~PairTableItem()
{
}


void PairTableItem::set_delete_this_event(std::function<void()> ev)
{
    connect(this->delete_this, &QPushButton::clicked, ev);
}

