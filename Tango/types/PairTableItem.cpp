
#include <functional>

#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>

#include "PairTableItem.h"


PairTableItem::PairTableItem(QString first_label, QString second_label, QWidget *parent) : QWidget(parent)
{
    auto lay = new QHBoxLayout;

    this->delete_this = new QPushButton("-");
    lay->addWidget(this->delete_this);

    this->first = new QLineEdit;
    this->second = new QLineEdit;
    lay->addWidget(new QLabel(first_label));
    lay->addWidget(this->first, 1);
    lay->addWidget(new QLabel(second_label));
    lay->addWidget(this->second, 1);

    this->setLayout(lay);
}

void PairTableItem::set_delete_this_event(std::function<void()> ev)
{
    connect(this->delete_this, &QPushButton::clicked, ev);
}

PairTableItem::~PairTableItem()
{
    qDebug() << "truely deleted";
}
