#ifndef CONFIGSCENE_H
#define CONFIGSCENE_H

#include "scene_common.h"


class ConfigScene : public Scene
{
    Q_OBJECT
public:
    ConfigScene(MainWindow *parent=nullptr);
    void refill();
    virtual void on_incoming();
private:
    Logger *logger;
    MainWindow *parent;
    QHBoxLayout *lay;

    QComboBox *displaystyle_edit;
    QPushButton *fullscreen_button;

    QLineEdit *limit__default_creation_table_items_count_edit;

    QLineEdit *mysql__host_edit;
    QLineEdit *mysql__basename_edit;
    QLineEdit *mysql__user_edit;
    QLineEdit *mysql__password_edit;

    QPushButton *save_button;
    QPushButton *reset_button;
    QPushButton *return_button;
    void set_button_events();
};

#endif // CONFIGSCENE_H
