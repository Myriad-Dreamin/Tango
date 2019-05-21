#ifndef ABSTRACTCLIENT_H
#define ABSTRACTCLIENT_H

#include <QHostAddress>
#include <QSqlDatabase>

#include "../types/UserStatus.h"
#include "../types/RetriveMode.h"

class QTcpSocket;
class QSqlDatabase;

class TangoPair;
class UserBriefInfo;
class UserFullInfo;

class GameConfig;
class AbstractGameAutomation;

class AbstractClient
{
public:
    AbstractClient();
    virtual ~AbstractClient() = 0;

    /* 作者登录 */
    virtual bool author_sign_in(QString account, QString password) = 0;
    /* 作者注册 */
    virtual bool author_sign_up(QString account, QString password) = 0;
    /* 读者登录 */
    virtual bool consumer_sign_in(QString account, QString password) = 0;
    /* 读者注册 */
    virtual bool consumer_sign_up(QString account, QString password) = 0;

    /* 登出 */
    virtual bool logout() = 0;
    /* 本地同步用户状态 */
    virtual bool sync_status() = 0;

    /* 提交一组单词 */
    virtual bool submit_tango_items(const std::vector<TangoPair> &tango_list) = 0;

    /*
     * 开始一场游戏
     * @game_config: 游戏参数设置
     * @mode: 单词fetch_mode
     *
     * 返回nullptr如果失败, 否则返回一个可以执行游戏的自动机.
     */
    virtual AbstractGameAutomation *start_game_event(const GameConfig *game_config, int n, RetriveMode mode) = 0;

    /* 结算一场游戏 */
    virtual bool settle_game_event(const AbstractGameAutomation *automate) = 0;

    /*
     * 查询作者简略信息
     * @info_list: 查询结果载体
     * @l: 左端
     * @r: 右端
     * 向infolist中push_back一组读者信息, 插入[l, r)的所有读者信息
     */
    virtual bool query_authors_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r) = 0;

    /*
     * 查询读者简略信息
     * @info_list: 查询结果载体
     * @l: 左端
     * @r: 右端
     * 向infolist中push_back一组读者信息, 插入[l, r)的所有读者信息
     */
    virtual bool query_consumers_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r) = 0;

    /*
     * 根据作者id查询
     * @query_container: 查询结果载体
     * 返回是否成功
     */
    virtual bool query_authors_by_id(UserFullInfo &query_container, int id) = 0;

    /*
     * 根据作者name查询
     * @query_container: 查询结果载体
     * 返回是否成功
     */
    virtual bool query_authors_by_name(UserFullInfo &query_container, QString name) = 0;

    /*
     * 根据读者id查询
     * @query_container: 查询结果载体
     * 返回是否成功
     */
    virtual bool query_consumers_by_id(UserFullInfo &query_container, int id) = 0;

    /*
     * 根据读者name查询
     * @query_container: 查询结果载体
     * 返回是否成功
     */
    virtual bool query_consumers_by_name(UserFullInfo &query_container, QString name) = 0;

    /*
     * 查询玩家总量
     * @query_count: 查询结果载体
     *
     */
    virtual bool query_users(int &query_count) = 0;

    /*
     * 返回最后错误
     */
    virtual const QString last_error() = 0;

    /* 是否连接 */
    virtual bool is_connected() = 0;

    /* 是否登录 */
    virtual bool consumer_logining() = 0;
    virtual bool author_logining() = 0;

    /* 读者经验 */
    virtual int consumer_exp() = 0;
    /* 读者等级 */
    virtual int consumer_level() = 0;

    /* 读者信息 */
    virtual const UserFullInfo &consumer_info() = 0;
    virtual const UserFullInfo &author_info() = 0;
};

#endif // ABSTRACTCLIENT_H
