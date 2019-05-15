#ifndef PAIRTABLEITEM_H
#define PAIRTABLEITEM_H

#include <QWidget>
#include <functional>

namespace std
{
    template<typename function_signature>
    class function;
}
class QPushButton;
class QLineEdit;

class PairTableItem : public QWidget
{
    Q_OBJECT
public:
    PairTableItem(QString first_label, QString second_label, QWidget *parent = nullptr);
    ~PairTableItem();

    QPushButton *delete_this;
    QLineEdit *first, *second;
public:
// c++11 stype public slots:
    void set_delete_this_event(std::function<void()> ev);
};

#endif // PAIRTABLEITEM_H
