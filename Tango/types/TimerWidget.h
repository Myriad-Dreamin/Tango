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
    std::function<void(int)> set_timer_slotter();
    void set_timer(int expire_time);
    std::function<void()> stop_timer_slotter();
    void stop_timer();

    std::function<void (int)> set_timer_slotter(const std::function<void ()> &shot_func);
private:
    QTimer *timer, *show_timer;
    QLabel *text_show;
    std::function<void()> timeout_slotter;
    void make_timeout_slotter();
};

#endif // TIMERWIDGET_H
