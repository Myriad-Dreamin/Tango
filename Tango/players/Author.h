#ifndef AUTHOR_H
#define AUTHOR_H

#include "Player.h"


class QSqlError;

/* 作者类 */
class Author : public Player
{
    Q_OBJECT
public:
    Author(QSqlDatabase db_handler, QObject *parent = nullptr);
    ~Author();

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

#endif // AUTHOR_H
