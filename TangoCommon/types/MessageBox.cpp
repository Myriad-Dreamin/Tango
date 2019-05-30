#include "MessageBox.h"

MessageBox::MessageBox(QWidget *parent): QMessageBox (parent)
{
    this->setStyleSheet("background-color:rgb(72,72,73);");
}

void MessageBox::critical(QWidget *parent, QString title, QString text)
{
    MessageBox *mbox = new MessageBox(parent);
    mbox->setWindowTitle(title);
    mbox->setText(text);
    mbox->setIcon(QMessageBox::Critical);
    mbox->addButton(tr("Yes"), QMessageBox::ActionRole);

    mbox->exec();
    mbox->deleteLater();
}

void MessageBox::info_text(QWidget *parent, QString title, QString text)
{
    MessageBox *mbox = new MessageBox(parent);
    mbox->setWindowTitle(title);
    mbox->setText(text);
    mbox->addButton(tr("Yes"), QMessageBox::ActionRole);

    mbox->exec();
    mbox->deleteLater();
}
