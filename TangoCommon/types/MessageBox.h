#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMessageBox>

class MessageBox: public QMessageBox
{
public:
    MessageBox(QWidget *parent);

    static void critical(QWidget *parent, QString title, QString text);
};

#endif // MESSAGEBOX_H
