#ifndef AUTHOR_H
#define AUTHOR_H

#include "Player.h"

#include "../types/UserFullInfo.h"
class QSqlError;

/* 作者类 */
class Author : public Player
{
    Q_OBJECT
public:
    Author(QSqlDatabase db_handler, QObject *parent = nullptr);
    Author(QObject *parent = nullptr);
    Author(const Author &rau);
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

    UserFullInfo user_info;

private:
    QString _last_error;
};

#endif // AUTHOR_H
