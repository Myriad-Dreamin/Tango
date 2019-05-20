#ifndef CONSUMER_H
#define CONSUMER_H


#include "Player.h"


class Consumer : public Player
{
public:
    Consumer(QSqlDatabase db_handler, QObject *parent = nullptr);
    ~Consumer();

    /* 本地登录 */
    bool sign_in_local(QString account, QString password);
    /* 远程登录 */
    bool sign_in_remote(QString account, QString password);

    /* 本地注册 */
    bool sign_up_local(QString account, QString password);
    /* 远程注册 */
    bool sign_up_remote(QString account, QString password);

    /* 本地登出 */
    bool login_out_local();
    /* 远程登出 */
    bool login_out_remote();

    /* 本地更新信息 */
    bool update_full_info_local();
    /* 远程更新信息 */
    bool update_full_info_remote();

    const QString last_error();

    int user_id;
    QString name;

    int tango_count;
    int misson_count;

    int level;
    int exp;

    QString motto;

private:
    QString _last_error;
};

#endif // CONSUMER_H
