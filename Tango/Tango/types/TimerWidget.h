#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <QWidget>

#include <functional>

class QTimer;
class QLabel;

class TimerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimerWidget(QWidget *parent = nullptr);

    void set_timer(int expire_time);
    void stop_timer();

public:
// c++ style public slots:
    std::function<void(int)> set_timer_slotter();
    std::function<void(int)> set_timer_slotter(const std::function<void ()> &shot_func);
    std::function<void()> stop_timer_slotter();

private:

    QTimer *timer, *show_timer;
    QLabel *text_show;

    std::function<void()> timeout_slotter;
    void make_timeout_slotter();
};

#endif // TIMERWIDGET_H
