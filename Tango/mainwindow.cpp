
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QAction>
// toolbar, statusbar

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    // ui(new Ui::MainWindow)
{
    qDebug() << "I...";

    cur_scene = nullptr;

    auto main_menubar = menuBar();
    auto menu_item = main_menubar->addMenu("File(&F)");

    menu_item->addAction("&Open", this, SLOT(close()), QKeySequence::Open);
    menu_item->setGeometry(0,0,this->width(),30);
    main_menubar->addMenu(menu_item);

    this->init_main_scene();
    this->init_register_scene();

    this->switch_scene(main_scene);


    // hlay->setParent(&w);
    this->setWindowTitle("Tango!");
}

void MainWindow::slotOpen()
{
    qDebug() << "...";
}

bool MainWindow::init_main_scene()
{
    main_scene = new MainScene;
    main_scene->set_sign_in_button_event([this]() mutable {
        QString account_text = this->main_scene->account_edit->text();
        QString password_text = this->main_scene->password_edit->text();
        qDebug() << "clicked confirm button" << account_text;
        qDebug() << "clicked confirm button" << password_text;


        if (account_text == "") {
            QMessageBox::critical(this, tr("错误"), tr("用户名不能为空"), QMessageBox::Ok);
        }

        if (password_text == "") {
            QMessageBox::critical(this, tr("错误"), tr("密码不能为空"), QMessageBox::Ok);
        }

    });

    main_scene->set_cancel_button_event([this]() mutable {
        qDebug() << "clicked cancel button";
        this->close();
    });
    main_scene->set_sign_up_button_event([this]() mutable {
         this->switch_scene(this->register_scene);
    });

    return true;
}

bool MainWindow::init_register_scene()
{
    register_scene = new RegisterScene;

    register_scene->set_confirm_button_event([this]() mutable {
        QString account_text = this->register_scene->account_edit->text();
        QString password_text = this->register_scene->password_edit->text();
        QString confirm_password_text = this->register_scene->confirm_edit->text();
        qDebug() << "clicked confirm button" << account_text;
        qDebug() << "clicked confirm button" << password_text;
        qDebug() << "clicked confirm button" << confirm_password_text;

        if (account_text == "") {
            QMessageBox::critical(this, tr("错误"), tr("用户名不能为空"), QMessageBox::Ok);
        }

        if (password_text == "") {
            QMessageBox::critical(this, tr("错误"), tr("密码不能为空"), QMessageBox::Ok);
        }

        if (password_text != confirm_password_text) {
            qDebug() << "not equal";
            QMessageBox::critical(this, tr("错误"), tr("两次输入密码不一致"), QMessageBox::Ok);
        }

    });

    register_scene->set_cancel_button_event([this]() mutable {
        qDebug() << "clicked cancel button";
        this->close();
    });

    register_scene->set_return_button_event([this]() mutable {
        qDebug() << "clicked return button" << this->main_scene;
        this->switch_scene(this->main_scene);
    });

    return true;
}

void MainWindow::switch_scene(QWidget *to_set)
{
    qDebug() << this->cur_scene << to_set;
    if (this->cur_scene) {
        this->cur_scene->hide();
    }
    this->takeCentralWidget();
    this->setCentralWidget(to_set);
    this->cur_scene = to_set;
    this->cur_scene->show();
    return;
}

//bool MainWindow::signal_switch_scene(QWidget *to_set)
//{
//    return this->switch_scene(to_set);
//}



void MainWindow::makeLayout()
{
}

MainWindow::~MainWindow()
{
    // delete ui;
}
