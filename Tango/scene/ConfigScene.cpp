#include "ConfigScene.h"
#include "MainScene.h"
#include "mainwindow.h"
#include <QSplitter>
#include <QListView>

ConfigScene::ConfigScene(MainWindow *parent): Scene(parent)
{
    auto item_margin = 5;
    auto center_item_lab_minimum_heigth = 32;
    auto center_item_lab_maximum_heigth = 50;
    auto base_item_margin_ratio = 1;
    auto base_item_lab_ratio = 3;
    auto base_item_edit_ratio = 10;


    this->logger = Logger::get_logger("main");
    this->parent = parent;

    auto center_lay = new QVBoxLayout;
    center_lay->setMargin(20);

    auto display_config_lab = new QLabel("显示设定");
    display_config_lab->setMinimumHeight(center_item_lab_minimum_heigth);
    display_config_lab->setMaximumHeight(center_item_lab_maximum_heigth);
    display_config_lab->setAlignment(Qt::AlignmentFlag::AlignCenter);
    center_lay->addWidget(display_config_lab);

    auto displaystyle_lay = new QHBoxLayout;
    auto displaystyle_lab = new QLabel("屏幕分辨率");
    displaystyle_edit = new QComboBox;
    displaystyle_edit->setEditable(false);
    QStringList displaystyle_list;
    displaystyle_list << "800x600" << "960x540" << "1280x720" << "1920x1080" << "2560x1440" << "1366x768" <<
                         "800x480" << "1024x600" << "1280x800" << "1440x900" << "1680x1050" << "1920x1200" << "2560x1600";
    displaystyle_edit->addItems(displaystyle_list);
    displaystyle_edit->setView(new QListView());

    displaystyle_lay->addStretch(base_item_margin_ratio);
    displaystyle_lay->addWidget(displaystyle_lab, base_item_lab_ratio);
    displaystyle_lay->addStretch(base_item_margin_ratio);
    displaystyle_lay->addWidget(displaystyle_edit, base_item_edit_ratio);
    displaystyle_lay->addStretch(base_item_margin_ratio);

    center_lay->addLayout(displaystyle_lay);
    displaystyle_lay->setMargin(item_margin);


    auto fullscreen_lay = new QHBoxLayout;
    fullscreen_button = new QPushButton;
    connect(fullscreen_button, &QPushButton::clicked, [this]() {
        if (fullscreen_button->text() == "全屏") {
            fullscreen_button->setText("普通");
        } else {
            fullscreen_button->setText("全屏");
        }
    });
    connect(this->parent, &MainWindow::screen_size_changed, [this]() {
        if (this->parent->isFullScreen()) {
            fullscreen_button->setText("全屏");
        } else {
            fullscreen_button->setText("普通");
        }
    });

    fullscreen_lay->addStretch(base_item_margin_ratio);
    fullscreen_lay->addWidget(fullscreen_button, base_item_edit_ratio);
    fullscreen_lay->addStretch(base_item_margin_ratio);

    center_lay->addLayout(fullscreen_lay);
    fullscreen_lay->setMargin(item_margin);

    auto limit_config_lab = new QLabel("限制设定");
    limit_config_lab->setMinimumHeight(center_item_lab_minimum_heigth);
    limit_config_lab->setMaximumHeight(center_item_lab_maximum_heigth);
    limit_config_lab->setAlignment(Qt::AlignmentFlag::AlignCenter);
    center_lay->addWidget(limit_config_lab);

    auto limit__default_creation_table_items_count_lay = new QHBoxLayout;
    auto limit__default_creation_table_items_count_lab = new QLabel("默认创造表格项数目");
    limit__default_creation_table_items_count_edit = new QLineEdit;
    limit__default_creation_table_items_count_lay->addStretch(base_item_margin_ratio);
    limit__default_creation_table_items_count_lay->addWidget(limit__default_creation_table_items_count_lab, base_item_lab_ratio);
    limit__default_creation_table_items_count_lay->addStretch(base_item_margin_ratio);
    limit__default_creation_table_items_count_lay->addWidget(limit__default_creation_table_items_count_edit, base_item_edit_ratio);
    limit__default_creation_table_items_count_lay->addStretch(base_item_margin_ratio);

    center_lay->addLayout(limit__default_creation_table_items_count_lay);
    limit__default_creation_table_items_count_lay->setMargin(item_margin);


    auto database_config_lab = new QLabel("数据库设定");
    database_config_lab->setMinimumHeight(center_item_lab_minimum_heigth);
    database_config_lab->setMaximumHeight(center_item_lab_maximum_heigth);
    database_config_lab->setAlignment(Qt::AlignmentFlag::AlignCenter);
    center_lay->addWidget(database_config_lab);

    auto mysql__host_lay = new QHBoxLayout;
    auto mysql__host_lab = new QLabel("mysql服务器地址");
    mysql__host_edit = new QLineEdit;
    mysql__host_lay->addStretch(base_item_margin_ratio);
    mysql__host_lay->addWidget(mysql__host_lab, base_item_lab_ratio);
    mysql__host_lay->addStretch(base_item_margin_ratio);
    mysql__host_lay->addWidget(mysql__host_edit, base_item_edit_ratio);
    mysql__host_lay->addStretch(base_item_margin_ratio);

    center_lay->addLayout(mysql__host_lay);
    mysql__host_lay->setMargin(item_margin);

    auto mysql__basename_lay = new QHBoxLayout;
    auto mysql__basename_lab = new QLabel("mysql数据库名");
    mysql__basename_edit = new QLineEdit;
    mysql__basename_lay->addStretch(base_item_margin_ratio);
    mysql__basename_lay->addWidget(mysql__basename_lab, base_item_lab_ratio);
    mysql__basename_lay->addStretch(base_item_margin_ratio);
    mysql__basename_lay->addWidget(mysql__basename_edit, base_item_edit_ratio);
    mysql__basename_lay->addStretch(base_item_margin_ratio);

    center_lay->addLayout(mysql__basename_lay);
    mysql__basename_lay->setMargin(item_margin);

    auto mysql__user_lay = new QHBoxLayout;
    auto mysql__user_lab = new QLabel("mysql登入用户名");
    mysql__user_edit = new QLineEdit;
    mysql__user_lay->addStretch(base_item_margin_ratio);
    mysql__user_lay->addWidget(mysql__user_lab, base_item_lab_ratio);
    mysql__user_lay->addStretch(base_item_margin_ratio);
    mysql__user_lay->addWidget(mysql__user_edit, base_item_edit_ratio);
    mysql__user_lay->addStretch(base_item_margin_ratio);

    center_lay->addLayout(mysql__user_lay);
    mysql__user_lay->setMargin(item_margin);

    auto mysql__password_lay = new QHBoxLayout;
    auto mysql__password_lab = new QLabel("mysql登入密码");
    mysql__password_edit = new QLineEdit;
    mysql__password_lay->addStretch(base_item_margin_ratio);
    mysql__password_lay->addWidget(mysql__password_lab, base_item_lab_ratio);
    mysql__password_lay->addStretch(base_item_margin_ratio);
    mysql__password_lay->addWidget(mysql__password_edit, base_item_edit_ratio);
    mysql__password_lay->addStretch(base_item_margin_ratio);

    center_lay->addLayout(mysql__password_lay);
    mysql__password_lay->setMargin(item_margin);

    center_lay->addStretch(1);

    auto button_lay = new QHBoxLayout;
    save_button = new QPushButton("保存");
    reset_button = new QPushButton("重设");
    return_button = new QPushButton("返回");

    button_lay->addWidget(save_button);
    button_lay->addWidget(reset_button);
    button_lay->addWidget(return_button);

    center_lay->addLayout(button_lay);
    button_lay->setMargin(item_margin);

    auto center_lay_area = new QScrollArea;

    auto policy = center_lay_area->sizePolicy();
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    policy.setHorizontalPolicy(QSizePolicy::Preferred);
    center_lay_area->setSizePolicy(policy);

    auto center_lay_widget = new QWidget;
    center_lay_widget->setLayout(center_lay);
    auto policyx = center_lay_widget->sizePolicy();
    policyx.setHorizontalPolicy(QSizePolicy::Preferred);
    policyx.setVerticalPolicy(QSizePolicy::Preferred);
    center_lay_widget->setSizePolicy(policyx);
    center_lay_widget->setMinimumWidth(400);
    QPalette palette = center_lay_widget->palette();
    palette.setBrush(QPalette::Window,QBrush(QColor(50, 50, 50)));
    center_lay_widget->setPalette(palette);


    center_lay_area->setSizePolicy(policy);

    center_lay_widget->setMinimumHeight(300);
    center_lay_area->setWidget(center_lay_widget);
    center_lay_area->setWidgetResizable(true);
    center_lay_area->setMinimumWidth(400);
    center_lay_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    lay = new QHBoxLayout;

    lay->addStretch(1);
    lay->addWidget(center_lay_area, 8);
    lay->addStretch(1);

    this->setLayout(lay);
    set_button_events();
}

void ConfigScene::refill()
{
    auto mconfig = this->parent->qconfig;
    this->displaystyle_edit->setCurrentText(mconfig->at("display/style").toString());
    if (mconfig->at("display/fullscreen_mode").toString() == "yes") {
        this->fullscreen_button->setText("全屏");
    } else {
        this->fullscreen_button->setText("普通");
    }

    this->limit__default_creation_table_items_count_edit->setText(mconfig->at("limit/default_creation_table_items_count").toString());
    this->mysql__host_edit->setText(mconfig->at("mysql/host").toString());
    this->mysql__basename_edit->setText(mconfig->at("mysql/basename").toString());
    this->mysql__user_edit->setText(mconfig->at("mysql/user").toString());
    this->mysql__password_edit->setText(mconfig->at("mysql/password").toString());
}

void ConfigScene::on_incoming()
{
    this->refill();
}


void ConfigScene::set_button_events()
{
    connect(save_button, &QPushButton::clicked, [this]() mutable {
        logger->debug() << "trigger save button";
        bool ok;
        auto limit__default_creation_table_items_count = this->limit__default_creation_table_items_count_edit->text().toInt(&ok);
        if (!ok) {
            MessageBox::critical(this, "错误", "不能将默认创造表格项数目变为整数");
            return;
        }

        auto display_style = this->displaystyle_edit->currentText();

        auto mysql__host = this->mysql__host_edit->text();
        auto mysql__basename = this->mysql__basename_edit->text();
        auto mysql__user = this->mysql__user_edit->text();
        auto mysql__password = this->mysql__password_edit->text();

        auto mconfig = this->parent->qconfig;
        mconfig->setValue("limit/default_creation_table_items_count", limit__default_creation_table_items_count);
        mconfig->setValue("mysql/host", mysql__host);
        mconfig->setValue("mysql/basename", mysql__basename);
        mconfig->setValue("mysql/user", mysql__user);
        mconfig->setValue("mysql/password", mysql__password);
        mconfig->setValue("display/style", display_style);
        if (this->fullscreen_button->text() == "全屏") {
            mconfig->setValue("display/fullscreen_mode", "yes");
        } else {
            mconfig->setValue("display/fullscreen_mode", "no");
        }

        this->parent->reset_screen_size();
    });

    connect(reset_button, &QPushButton::clicked, [this]() mutable {
        logger->debug() << "trigger reset button";

        auto mconfig = this->parent->qconfig;
        mconfig->reset();
        this->refill();

        this->parent->reset_screen_size();
    });

    connect(return_button, &QPushButton::clicked, [this]() mutable {
        logger->debug() << "trigger return button";
        this->parent->switch_scene(this->parent->main_scene);
    });
}
