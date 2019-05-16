
#include "TimerWidget.h"

#include <QDebug>
#include <QTimer>

#include <QLabel>
#include <QLayout>

TimerWidget::TimerWidget(QWidget *parent) : QWidget(parent)
{
    this->timer = new QTimer(this);
    this->show_timer = new QTimer(this);
    this->make_timeout_slotter();

    auto lay = new QHBoxLayout(this);

    text_show = new QLabel("0.0", this);
    lay->addWidget(text_show);

    this->setLayout(lay);

}

void TimerWidget::make_timeout_slotter()
{
    this->timeout_slotter = [this]() mutable {
        QString text_timer;
        text_timer.sprintf("%.1f", this->timer->remainingTime()/1000.0);
        this->text_show->setText(text_timer);
    };
}

std::function<void (int)> TimerWidget::set_timer_slotter()
{
    return [this](int expire_time) mutable {

        connect(this->timer, &QTimer::timeout, this->stop_timer_slotter());
        this->timer->start(expire_time);

        connect(this->show_timer, &QTimer::timeout, this->timeout_slotter);
        this->show_timer->start(100);
    };
}

std::function<void (int)> TimerWidget::set_timer_slotter(const std::function<void()> &shot_func)
{
    return [this, shot_func](int expire_time) mutable {

        connect(this->timer, &QTimer::timeout, [this, shot_func]() mutable {
            this->stop_timer();
            shot_func();
        });
        this->timer->start(expire_time);

        connect(this->show_timer, &QTimer::timeout, this->timeout_slotter);
        this->show_timer->start(100);
    };
}

void TimerWidget::set_timer(int expire_time)
{
    connect(this->timer, &QTimer::timeout, this->stop_timer_slotter());
    this->timer->start(expire_time);

    connect(this->show_timer, &QTimer::timeout, this->timeout_slotter);
    this->show_timer->start(100);
}

std::function<void ()> TimerWidget::stop_timer_slotter()
{
    return [this]() mutable {
        this->timer->stop();
        this->show_timer->stop();

        disconnect(this->timer, nullptr, nullptr, nullptr);
        this->text_show->setText("0.0");
    };
}

void TimerWidget::stop_timer()
{
    this->timer->stop();
    this->show_timer->stop();

    disconnect(this->timer, nullptr, nullptr, nullptr);
    this->text_show->setText("0.0");
}
