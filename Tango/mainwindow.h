#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>

#include "scene/MainScene.h"
#include "scene/RegisterScene.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void makeLayout();
    ~MainWindow();

private slots:
    void slotOpen();

    void switch_scene(QWidget *to_set);

    // bool signal_switch_scene(QWidget *to_set);
//    void on_sign_in_button_clicked();

//    void on_button_cancel_clicked();

private:
    QWidget *cur_scene;

    MainScene *main_scene;
    RegisterScene *register_scene;

    bool init_main_scene();
    bool init_register_scene();

    // Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
