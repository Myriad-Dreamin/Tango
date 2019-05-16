#ifndef CREATIONTABLEITEM_H
#define CREATIONTABLEITEM_H

#include "../types/PairTableItem.h"

class CreationTableItem : public PairTableItem
{
    Q_OBJECT

public:
    CreationTableItem(QWidget *parent = nullptr);
};

#endif // CREATIONTABLEITEM_H
