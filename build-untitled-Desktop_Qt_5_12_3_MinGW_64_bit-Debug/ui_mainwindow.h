/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *start_up;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *left_space;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *up_space;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_account;
    QLineEdit *line_account;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_password;
    QLineEdit *line_password;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *sign_in_button;
    QPushButton *button_cancel;
    QPushButton *button_sign_up;
    QSpacerItem *down_space;
    QSpacerItem *right_space;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1493, 813);
        start_up = new QWidget(MainWindow);
        start_up->setObjectName(QString::fromUtf8("start_up"));
        start_up->setAcceptDrops(false);
        start_up->setAutoFillBackground(false);
        widget = new QWidget(start_up);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 1061, 461));
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        left_space = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(left_space);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        up_space = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(up_space);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_account = new QLabel(widget);
        label_account->setObjectName(QString::fromUtf8("label_account"));

        horizontalLayout->addWidget(label_account);

        line_account = new QLineEdit(widget);
        line_account->setObjectName(QString::fromUtf8("line_account"));

        horizontalLayout->addWidget(line_account);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_password = new QLabel(widget);
        label_password->setObjectName(QString::fromUtf8("label_password"));

        horizontalLayout_2->addWidget(label_password);

        line_password = new QLineEdit(widget);
        line_password->setObjectName(QString::fromUtf8("line_password"));
        line_password->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(line_password);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        sign_in_button = new QPushButton(widget);
        sign_in_button->setObjectName(QString::fromUtf8("sign_in_button"));

        horizontalLayout_3->addWidget(sign_in_button);

        button_cancel = new QPushButton(widget);
        button_cancel->setObjectName(QString::fromUtf8("button_cancel"));

        horizontalLayout_3->addWidget(button_cancel);

        button_sign_up = new QPushButton(widget);
        button_sign_up->setObjectName(QString::fromUtf8("button_sign_up"));

        horizontalLayout_3->addWidget(button_sign_up);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addLayout(verticalLayout);

        down_space = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(down_space);


        horizontalLayout_4->addLayout(verticalLayout_2);

        right_space = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(right_space);

        MainWindow->setCentralWidget(start_up);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1493, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label_account->setText(QApplication::translate("MainWindow", "\350\264\246\345\217\267\357\274\232", nullptr));
        label_password->setText(QApplication::translate("MainWindow", "\345\257\206\347\240\201\357\274\232", nullptr));
        sign_in_button->setText(QApplication::translate("MainWindow", "\347\231\273\345\275\225", nullptr));
        button_cancel->setText(QApplication::translate("MainWindow", "\345\217\226\346\266\210", nullptr));
        button_sign_up->setText(QApplication::translate("MainWindow", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
