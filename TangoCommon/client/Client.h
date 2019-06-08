#ifndef CLIENT_H
#define CLIENT_H

#include <functional>

#include <QObject>
#include <QHostAddress>
#include <QSqlDatabase>

#include "../types/UserStatus.h"
#include "../types/RetriveMode.h"

class QTcpSocket;
class QSqlDatabase;
class MainWindow;
class Logger;

class TangoPair;
class UserBriefInfo;
class UserFullInfo;

class Author;
class Consumer;

class GameConfig;
class AbstractGameAutomation;

class AbstractClient;
class LocalClient;
class RemoteClient;

/*
 * Tango客户端
 * 
 * 向图形化界面提供客户端的交互服务
 */
class Client: public QObject
{
    Q_OBJECT
public:
    explicit Client(MainWindow *parent = nullptr);
    virtual ~Client() override;

    /* 远程登陆 */
    bool setup_remote_connection(QHostAddress host_address, quint16 server_port);
    /* 本地登陆 */
    bool setup_local_connection();

    /* 断开远程连接 */
    bool stop_remote_connection();
    /* 断开本地连接 */
    bool stop_local_connection();

    /* 作者登录 */
    bool author_sign_in(QString account, QString password);
    /* 作者注册 */
    bool author_sign_up(QString account, QString password);
    /* 读者登录 */
    bool consumer_sign_in(QString account, QString password);
    /* 读者注册 */
    bool consumer_sign_up(QString account, QString password);
    /* 登出 */
    bool logout();
    /* 本地同步用户状态 */
    bool sync_status();

    /* 初始化默认词库 */
    bool init_default_tangos();
    /* QT bug, init_default_tangos不能被自动链接 */
    bool init_default_tangosf();

    /* 提交一组单词 */
    bool submit_tango_items(const std::vector<TangoPair> &tango_list);

    /*
     * 开始一场游戏
     * @game_config: 游戏参数设置
     * @mode: 单词fetch_mode
     *
     * 返回nullptr如果失败, 否则返回一个可以执行游戏的自动机.
     */
    AbstractGameAutomation *start_game_event(const GameConfig *game_config, int n, RetriveMode mode);

    /* 结算一场游戏 */
    bool settle_game_event(const AbstractGameAutomation *automate);

    /*
     * 查询作者简略信息
     * @info_list: 查询结果载体
     * @l: 左端
     * @r: 右端
     * 向infolist中push_back一组读者信息, 插入[l, r)的所有读者信息
     */
    bool query_authors_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r);

    /*
     * 查询读者简略信息
     * @info_list: 查询结果载体
     * @l: 左端
     * @r: 右端
     * 向infolist中push_back一组读者信息, 插入[l, r)的所有读者信息
     */
    bool query_consumers_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r);

    /*
     * 根据作者id查询
     * @query_container: 查询结果载体
     * 返回是否成功
     */
    bool query_authors_by_id(UserFullInfo &query_container, int id);

    /*
     * 根据作者name查询
     * @query_container: 查询结果载体
     * 返回是否成功
     */
    bool query_authors_by_name(UserFullInfo &query_container, QString name);

    /*
     * 根据读者id查询
     * @query_container: 查询结果载体
     * 返回是否成功
     */
    bool query_consumers_by_id(UserFullInfo &query_container, int id);

    /*
     * 根据读者name查询
     * @query_container: 查询结果载体
     * 返回是否成功
     */
    bool query_consumers_by_name(UserFullInfo &query_container, QString name);

    /*
     * 查询玩家总量
     * @query_count: 查询结果载体
     *
     */
    bool query_users(int &query_count);

    /*
     * 查询在线玩家
     *
     */
    bool query_online_users();

    /*
     * 返回最后错误
     */
    const QString last_error();

    /* 是否连接 */
    bool is_connected();
    /* 是否远程连接 */
    bool is_remote_server_connected();
    /* 是否本地连接 */
    bool is_local_handler_connected();

    /* 读者经验 */
    int consumer_exp();
    /* 读者等级 */
    int consumer_level();

    bool consumer_logining();
    bool author_logining();
private:
    MainWindow *parent;
    AbstractClient *handler;
    LocalClient *local_handler;
    RemoteClient *remote_handler;

    /* 最后一个错误 */
    QString _last_error;

    /* 远程连接成功槽, 用于处理连接事件 */
    inline void make_remote_server_on_connected();
    /* 远程连接断开槽, 用于处理失去链接事件 */
    inline void make_remote_server_on_disconnected();

    /* 用户本地登录函数组开始 */
    bool author_sign_in_remote(QString account, QString password);

    bool author_sign_up_remote(QString account, QString password);

    bool consumer_sign_in_remote(QString account, QString password);

    bool consumer_sign_up_remote(QString account, QString password);
    /* 用户本地登录函数组结束 */

    /* 向远程提交一组单词 */
    bool submit_tango_items_remote(const std::vector<TangoPair> &tango_list);

    /* 创建表格 */
    bool create_tables();
signals:
    /* 连接信号 */
    void connected();
    /* 断开信号 */
    void disconnected();
public:
    std::function<void()> disconnected_callback;
private:
    Logger *logger;
};

#endif // CLIENT_H
