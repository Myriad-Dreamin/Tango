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

class TangoPair;
class UserBriefInfo;
class UserFullInfo;

class Author;
class Consumer;

class GameConfig;
class GameAutomation;


/* 客户端 */
class Client: public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    virtual ~Client() override;

    /* 远程登陆 */
    bool setup_remote_connection(QHostAddress host_address, quint16 server_port);
    /* 本地登陆 */
    bool setup_local_connection();

    // TODO: check if mode is valid
    /* 转到远程模式 */
    void switch_remote_mode();
    /* 远程模式槽 */
    std::function<void ()> switch_remote_mode_slottor();
    /* 转到远程模式 */
    void switch_local_mode();
    /* 转到本地模式槽 */
    std::function<void ()> switch_local_mode_slottor();

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


    /* 提交一组单词 */
    bool submit_tango_items(const std::vector<TangoPair> &tango_list);

    /*
     * 开始一场游戏
     * @game_config: 游戏参数设置
     * @mode: 单词fetch_mode
     *
     * 返回nullptr如果失败, 否则返回一个可以执行游戏的自动机.
     */
    GameAutomation *start_game_event(const GameConfig *game_config, int n, RetriveMode mode);

    /* 结算一场游戏 */
    bool settle_game_event(const GameAutomation *automate);

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
private:

    /* 用户状态 */
    UserStatus user_status;

    /* author用户handler */
    class Author *user_author;
    /* consumer用户handler */
    class Consumer *user_consumer;

    /* 远程连接地址 */
    QHostAddress remote_address;
    /* 远程连接端口 */
    quint16 remote_port;
    /* 远程连接handler */
    QTcpSocket *remote_server;
    /* 本地连接handler */
    QSqlDatabase local_handler;

    /* 远程连接是否成功 */
    bool remote_ready;
    /* 本地连接是否成功 */
    bool local_ready;
    /* 最后一个错误 */
    QString _last_error;

    /* 远程连接成功槽, 用于处理连接事件 */
    inline void make_remote_server_on_connected();
    /* 远程连接断开槽, 用于处理失去链接事件 */
    inline void make_remote_server_on_disconnected();

    /* 用户本地登录函数组开始 */
    bool author_sign_in_local(QString account, QString password);
    bool author_sign_in_remote(QString account, QString password);

    bool author_sign_up_local(QString account, QString password);
    bool author_sign_up_remote(QString account, QString password);

    bool consumer_sign_in_local(QString account, QString password);
    bool consumer_sign_up_local(QString account, QString password);

    bool consumer_sign_in_remote(QString account, QString password);
    bool consumer_sign_up_remote(QString account, QString password);
    /* 用户本地登录函数组结束 */

    /* 向本地提交一组单词 */
    bool submit_tango_items_local(const std::vector<TangoPair> &tango_list);
    /* 向远程提交一组单词 */
    bool submit_tango_items_remote(const std::vector<TangoPair> &tango_list);

    /* 本地登出 */
    bool logout_local();

    /* 断开远程连接 */
    bool disconnect_to_remote();
    /* 断开本地连接 */
    bool disconnect_to_local();

    /* 创建表格 */
    bool create_tables();
    inline bool create_author_table();
    inline bool create_tangos_table();
    inline bool create_consumer_table();

    /* 本地开始游戏 */
    GameAutomation *start_game_event_local(const GameConfig *game_config, int n, RetriveMode mode);

    /* 本地获取单词组 [random(), random() + retrive_mode(n) - 1)*/
    bool retrive_tango_items_local(std::vector<TangoPair> &tango_list, int &n, RetriveMode mode);

    /* 本地获取单词组 [k, k+n-1) */
    int retrive_since_kth_tango_item_local(std::vector<TangoPair> &tango_list, unsigned int k, int n);

    /* 本地获取第k个单词 */
    bool retrive_kth_tango_item_local(TangoPair &tp, int k);

    /* 结算游戏 读者经验提升策略 */
    bool settle_game_event_local(const GameAutomation *automate);
    /* 结算创造 作者经验提升策略 */
    bool settle_creation_event_local(const std::vector<TangoPair> &tango_list);

    /* 查询事件 开始 */
    bool query_authors_brief_info_local(std::vector<UserBriefInfo> &info_list, int l, int r);
    bool query_authors_by_id_local(UserFullInfo &query_container, int id);
    bool query_authors_by_name_local(UserFullInfo &query_container, QString name);

    bool query_consumers_brief_info_local(std::vector<UserBriefInfo> &info_list, int l, int r);
    bool query_consumers_by_id_local(UserFullInfo &query_container, int id);
    bool query_consumers_by_name_local(UserFullInfo &query_container, QString name);
    /* 查询事件 结束 */

    /* 当前mode的函数组 开始 */
    std::function<bool(QString,QString)> _author_sign_in;
    std::function<bool(QString,QString)> _author_sign_up;
    std::function<bool(QString,QString)> _consumer_sign_in;
    std::function<bool(QString,QString)> _consumer_sign_up;
    std::function<bool(const std::vector<TangoPair>&)> _submit_tango_items;
    std::function<bool()> _is_connected;
    /* 当前mode的函数组 结束 */

signals:
    /* 连接信号 */
    void connected();
    /* 断开信号 */
    void disconnected();
};

#endif // CLIENT_H
