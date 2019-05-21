#include "LocalClient.h"

#include <random>
#include <QSqlError>
#include <QSqlQuery>
#include "../players/Author.h"
#include "../players/Consumer.h"
#include "../types/TangoPair.h"
#include "../types/UserBriefInfo.h"
#include "../types/UserFullInfo.h"
#include "../types/UserStatus.h"
#include "GameAutomation.h"
#include "GameConfig.h"

LocalClient::LocalClient(QObject *parent): QObject (parent), AbstractClient ()
{
    this->user_author = nullptr;
    this->user_consumer = nullptr;
    this->user_status = UserStatus::None;

    handler = QSqlDatabase::addDatabase("QMYSQL");
    handler.setHostName("localhost");
    handler.setDatabaseName("tango");
    handler.setUserName("tangosql");
    handler.setPassword("123456");

    this->ready = false;
}

LocalClient::~LocalClient()
{

}

bool LocalClient::setup_connection()
{

    if (this->ready == false) {

        if (!handler.open()) {
            qDebug() << "db open error:" << handler.lastError().text();
            _last_error = handler.lastError().text();
            return false;
        }

        if (!this->create_tables()) {
            return false;
        }

        this->ready = true;
        return true;
    }

    qDebug() << "might be open" << this->handler.isOpen();
    return true;
}

bool LocalClient::stop_connection()
{
    if (this->ready) {
        handler.close();

        return true;
    }

    return true;
}

bool LocalClient::author_sign_in(QString account, QString password)
{
    if (user_status_util::has_author_status(this->user_status)) {
        qDebug() << "user_doesn't logout";
        user_author->deleteLater();
        user_author = nullptr;
        user_status_util::remove_author_status(this->user_status);
    }
    return this->_author_sign_in(account, password);
}

bool LocalClient::_author_sign_in(QString account, QString password)
{
    user_author = new class Author(this->handler);
    if (user_author->sign_in_local(account, password)) {
        user_status_util::add_author_status(this->user_status);

        return true;
    }

    _last_error = user_author->last_error();
    user_author->deleteLater();
    user_author = nullptr;

    return false;
}

bool LocalClient::author_sign_up(QString account, QString password)
{
    if (user_status_util::has_author_status(this->user_status)) {
        qDebug() << "author doesn't logout";
        user_author->deleteLater();
        user_author = nullptr;
        user_status_util::remove_author_status(this->user_status);
    }
    return this->_author_sign_up(account, password);
}

bool LocalClient::_author_sign_up(QString account, QString password)
{
    user_author = new class Author(this->handler);
    if (user_author->sign_up_local(account, password)) {
        user_status_util::add_author_status(this->user_status);

        return true;
    }

    _last_error = user_author->last_error();
    user_author->deleteLater();
    user_author = nullptr;

    return false;
}

bool LocalClient::consumer_sign_in(QString account, QString password)
{
    if (user_status_util::has_consumer_status(this->user_status)) {
        qDebug() << "consumer doesn't logout";
        user_consumer->deleteLater();
        user_consumer = nullptr;
        user_status_util::remove_consumer_status(this->user_status);
    }
    return this->_consumer_sign_in(account, password);
}

bool LocalClient::_consumer_sign_in(QString account, QString password)
{
    user_consumer = new class Consumer(this->handler);
    if (user_consumer->sign_in_local(account, password)) {
        user_status_util::add_consumer_status(this->user_status);

        return true;
    }

    _last_error = user_consumer->last_error();
    user_consumer->deleteLater();
    user_consumer = nullptr;

    return false;

}

bool LocalClient::consumer_sign_up(QString account, QString password)
{
    if (user_status_util::has_consumer_status(this->user_status)) {
        qDebug() << "consumer doesn't logout";
        user_consumer->deleteLater();
        user_consumer = nullptr;
        user_status_util::remove_consumer_status(this->user_status);
    }
    return this->_consumer_sign_up(account, password);
}

bool LocalClient::logout()
{
    qDebug() << "logouting";
    if (user_status_util::has_author_status(this->user_status)) {
        qDebug() << "logout author";
        if (user_author->login_out_local()) {
            user_author->deleteLater();
            user_author = nullptr;
            user_status_util::remove_author_status(this->user_status);
        } else {
            _last_error = user_author->last_error();
            qDebug() << "error occured" << _last_error;
            return false;
        }
    }
    if (user_status_util::has_consumer_status(this->user_status)) {
        qDebug() << "logout consumer" << this->user_consumer->user_info.tango_count;
        if (user_consumer->login_out_local()) {
            user_consumer->deleteLater();
            user_consumer = nullptr;
            user_status_util::remove_consumer_status(this->user_status);
        } else {
            _last_error = user_consumer->last_error();
            qDebug() << "error occured" << _last_error;
            return false;
        }
    }

    return true;
}

bool LocalClient::sync_status()
{
    qDebug() << "logouting";
    if (user_status_util::has_author_status(this->user_status)) {
        qDebug() << "logout author";
        if (!user_author->update_full_info_local()) {
            _last_error = user_author->last_error();
            qDebug() << "error occured" << _last_error;
            return false;
        }
    }
    if (user_status_util::has_consumer_status(this->user_status)) {
        qDebug() << "logout consumer" << this->user_consumer->user_info.tango_count;
        if (!user_consumer->update_full_info_local()) {
            _last_error = user_consumer->last_error();
            qDebug() << "error occured" << _last_error;
            return false;
        }
    }

    return true;
}

bool LocalClient::init_default_tangos()
{
    static const char *tangos_commands[] = {
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('appraisal', '估计', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('appraise', '评价', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('allot', '分配', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('alloy', '合金', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('apt', '适当的;恰当的', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('apprentice', '学徒;学弟', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('approach', '走进;方法', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('archieve', '档案', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('arctic', '北极', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('ardent', '热心的;热情的', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('banana', '香蕉', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('strawberry', '草莓', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('pineapple', '菠萝', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('onion', '洋葱', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('garlic', '大蒜', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('cabbage', '包菜', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('peach', '桃子', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('orange', '橘子', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('tomato', '西红柿', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('potato', '土豆', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('apple', '苹果', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('altitude', '高度', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('ambassador', '大使', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('ambiguity', '含糊不清', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('ambiguous', '含糊不清的', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('ambition', '雄心', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('ambitious', '有抱负的', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('ambush', '埋伏', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('amount', '数量', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('ample', '充足的', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('amplify', '放大', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('amuse', '逗乐', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('analogy', '相似', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('analyst', '分析者', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('analytic', '分析的', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('analytical', '分析的', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('anew', '又;再一', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('ancestry', '血统,门第', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('anguish', '极度痛苦;剧痛', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('animate', '活的', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('annex', '附加;添加', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('announce', '宣布', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('annoyance', '烦恼;困扰', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('annual', '每年的', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('anonymity', '匿名', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('anonymous', '匿名的', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('antagonism', '敌对;对抗', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('antagonist', '对抗者', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('anticipate', '预料', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('antique', '古代的;古式的', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('antibiotic', '抗生素', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('antiquity', '古代,古老', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('antonym', '反义词', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('apparatus', '仪器', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('appease', '平息;抚慰', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('appendix', '附件', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('applicable', '适当的;合适的', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('apply', '申请;请求', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('appoint', '任命', 'admin');",
        "insert into `tangos` (`key`, `value`, `last_submit`) values ('apprehend', '逮捕;拘押', 'admin');"
    };

    QSqlQuery query(this->handler);
    query.exec("set names 'utf8'");
    for (auto tango_command: tangos_commands) {
        if (!query.exec(tango_command)) {
            _last_error = query.lastError().text();
            if (_last_error[0] == 'D' && _last_error[1] == 'u' && _last_error[2] == 'p') {
                continue;
            }
            qDebug() << "init consumers table failed" << query.lastError().text();
            return false;
        }
    }
    return true;
}

bool LocalClient::submit_tango_items(const std::vector<TangoPair> &tango_list)
{
    if (!user_status_util::has_author_status(this->user_status)) {
        this->_last_error = "author doesn't sign in";
        return false;
    }
    return this->_submit_tango_items(tango_list);
}

AbstractGameAutomation *LocalClient::start_game_event(const GameConfig *game_config, int n, RetriveMode mode)
{
    if (!user_status_util::has_consumer_status(this->user_status)) {
        this->_last_error = "consumer doesn't sign in";
        return nullptr;
    }
    return _start_game_event(game_config, n, mode);
}

bool LocalClient::settle_game_event(const AbstractGameAutomation *automate)
{
    qDebug() << this->user_consumer << this->user_consumer->user_info.misson_count << this->user_consumer->user_info.exp << this->user_consumer->user_info.tango_count << this->user_consumer->user_info.level;
    if (static_cast<unsigned int>(automate->success_count) < automate->tango_pool->size()) {
        this->user_consumer->user_info.misson_count++;
    }
    this->user_consumer->user_info.exp += automate->exp;
    this->user_consumer->user_info.tango_count += automate->success_count;
    while (this->user_consumer->user_info.level * 10 + 10 <= this->user_consumer->user_info.exp) {
        this->user_consumer->user_info.exp -= this->user_consumer->user_info.level * 10 + 10;
        this->user_consumer->user_info.level++;
    }
    qDebug() << this->user_consumer << this->user_consumer->user_info.misson_count << this->user_consumer->user_info.exp << this->user_consumer->user_info.tango_count << this->user_consumer->user_info.level;
    return true;
}

// 假定 l <= r
bool LocalClient::query_authors_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r)
{
    static const char *query_command =
        "select `id`, `name`, `level` from `authors`"
        "     order by `level` desc, `exp` desc limit :kth, :ntimes";

    QSqlQuery query(this->handler);
    query.prepare(query_command);
    query.bindValue(":kth", l);
    query.bindValue(":ntimes", r-l+1);
    qDebug() << "k, n" << l << " " << r-l+1;
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }

    while (query.next()) {
        info_list.emplace_back(UserBriefInfo(query.value(0).toInt(), query.value(1).toString(), query.value(2).toInt()));
        qDebug() << "fetched " << query.value(0).toInt() << query.value(1).toString() << query.value(2).toInt();
    }

    return true;
}

bool LocalClient::query_authors_by_id(UserFullInfo &query_container, int id)
{
    static const char *query_command = "select * from `authors` where `id` = :id";

    QSqlQuery query(this->handler);
    query.prepare(query_command);
    query.bindValue(":id", id);
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }
    if (!query.first()) {
        _last_error = query.lastError().text();
        qDebug() << "fetch first element error occured: " << _last_error;

        return false;
    }

    query_container.user_id = query.value(0).toInt();
    query_container.name = query.value(1).toString();
    query_container.tango_count = query.value(5).toInt();
    query_container.misson_count = query.value(6).toInt();
    query_container.exp = query.value(3).toInt();
    query_container.level = query.value(4).toInt();
    query_container.motto = query.value(7).toString();

    return true;
}

bool LocalClient::query_consumers_by_id(UserFullInfo &query_container, int id)
{
    static const char *query_command = "select * from `consumers` where `id` = :id";

    QSqlQuery query(this->handler);
    query.prepare(query_command);
    query.bindValue(":id", id);
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }
    if (!query.first()) {
        _last_error = query.lastError().text();
        qDebug() << "fetch first element error occured: " << _last_error;

        return false;
    }

    query_container.user_id = query.value(0).toInt();
    query_container.name = query.value(1).toString();
    query_container.tango_count = query.value(5).toInt();
    query_container.misson_count = query.value(6).toInt();
    query_container.exp = query.value(3).toInt();
    query_container.level = query.value(4).toInt();
    query_container.motto = query.value(7).toString();

    return true;
}

bool LocalClient::query_authors_by_name(UserFullInfo &query_container, QString name)
{
    static const char *query_command = "select * from `authors` where `name` = :name";

    QSqlQuery query(this->handler);
    query.prepare(query_command);
    query.bindValue(":name", name);
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }
    if (!query.first()) {
        _last_error = query.lastError().text();
        qDebug() << "fetch first element error occured: " << _last_error;

        return false;
    }

    query_container.user_id = query.value(0).toInt();
    query_container.name = query.value(1).toString();
    query_container.tango_count = query.value(5).toInt();
    query_container.misson_count = query.value(6).toInt();
    query_container.exp = query.value(3).toInt();
    query_container.level = query.value(4).toInt();
    query_container.motto = query.value(7).toString();

    return true;
}

bool LocalClient::query_consumers_by_name(UserFullInfo &query_container, QString name)
{
    static const char *query_command = "select * from `consumers` where `name` = :name";

    QSqlQuery query(this->handler);
    query.prepare(query_command);
    query.bindValue(":name", name);
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }
    if (!query.first()) {
        _last_error = query.lastError().text();
        qDebug() << "fetch first element error occured: " << _last_error;

        return false;
    }

    query_container.user_id = query.value(0).toInt();
    query_container.name = query.value(1).toString();
    query_container.tango_count = query.value(5).toInt();
    query_container.misson_count = query.value(6).toInt();
    query_container.exp = query.value(3).toInt();
    query_container.level = query.value(4).toInt();
    query_container.motto = query.value(7).toString();

    return true;
}

// 假定 l <= r
bool LocalClient::query_consumers_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r)
{
    static const char *query_command =
        "select `id`, `name`, `level` from `consumers`"
        "     order by `level` desc, `exp` desc limit :kth, :ntimes";

    QSqlQuery query(this->handler);
    query.prepare(query_command);
    query.bindValue(":kth", l);
    query.bindValue(":ntimes", r-l+1);
    qDebug() << "k, n" << l << " " << r-l+1;
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }

    while (query.next()) {
        info_list.emplace_back(UserBriefInfo(query.value(0).toInt(), query.value(1).toString(), query.value(2).toInt()));
        qDebug() << "fetched " << query.value(0).toInt() << query.value(1).toString() << query.value(2).toInt();
    }

    return true;
}


bool LocalClient::query_users(int &query_count)
{
    static const char *query_command_aut = "select count(*) from `authors`";
    static const char *query_command_con = "select count(*) from `consumers`";
    query_count = 0;
    QSqlQuery query(this->handler);
    if (!query.exec(query_command_aut)) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;
        return false;
    }
    if (!query.first()) {
        _last_error = "first fetch error";
        qDebug() << "error occured: " << _last_error;
        return false;
    }

    query_count += query.value(0).toInt();
    qDebug() << "tot_length " << query.value(0).toInt();

    if (!query.exec(query_command_con)) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;
        return false;
    }
    if (!query.first()) {
        _last_error = "first fetch error";
        qDebug() << "error occured: " << _last_error;
        return false;
    }

    query_count += query.value(0).toInt();
    qDebug() << "tot_length " << query.value(0).toInt();
    return true;
}

const QString LocalClient::last_error()
{
    return _last_error;
}

bool LocalClient::is_connected()
{
    return this->ready;
}

bool LocalClient::consumer_logining()
{
    return user_status_util::has_consumer_status(user_status);
}

bool LocalClient::author_logining()
{
    return user_status_util::has_author_status(user_status);
}


int LocalClient::consumer_exp()
{
    if (user_status_util::has_consumer_status(this->user_status)) {
        return this->user_consumer->user_info.exp;
    }
    qDebug() << "consumer exp=0";
    return -1;
}

int LocalClient::consumer_level()
{
    if (user_status_util::has_consumer_status(this->user_status)) {
        return this->user_consumer->user_info.level;
    }
    return -1;
}

const UserFullInfo LocalClient::consumer_info()
{
    return this->user_consumer->user_info;
}

const UserFullInfo LocalClient::author_info()
{
    return this->user_author->user_info;
}

AbstractGameAutomation *LocalClient::_start_game_event(const GameConfig *game_config, int n, RetriveMode mode)
{
    std::vector<TangoPair> tango_list;
    tango_list.reserve(static_cast<unsigned int>(n));
    qDebug() << "want " << n;
    int temp_n = n;
    if (!this->retrive_tango_items(tango_list, temp_n, mode)) {
        return nullptr;
    }

    auto automate = new GameAutomation(game_config);
    if (!automate->prepare_start(tango_list, static_cast<unsigned int>(temp_n))) {
        _last_error = automate->last_error();
        automate->deleteLater();
        return nullptr;
    }
    return automate;
}

bool LocalClient::_submit_tango_items(const std::vector<TangoPair> &tango_list)
{
    static const char *insert_command =
        "insert into `tangos` ( `key`, `value`, `last_submit`) "
        "               values (:key,  :value,  :ls)";

    qDebug() << "tango list" << tango_list;

    QSqlQuery query(this->handler);
    if (!query.exec("start transaction")) {
        qDebug() << "start transaction error" << query.lastError().text();
        _last_error = query.lastError().text();
        return false;
    }

    query.prepare(insert_command);
    query.bindValue(":ls", this->user_author->user_info.name);

    for (unsigned int i = 0; i < tango_list.size(); i++) {
        query.bindValue(":key", tango_list[i].first);
        query.bindValue(":value", tango_list[i].second);

        if (!query.exec()) {
             qDebug() << "exec error" << query.lastError().text();
             _last_error = query.lastError().text();
             if (!query.exec("rollback")) {
                 qDebug() << "rollback error" << query.lastError().text();
                 _last_error = query.lastError().text();
                 return false;
             }
             return false;
        }
    }

    if (!query.exec("commit")) {
        qDebug() << "commit error" << query.lastError().text();
        _last_error = query.lastError().text();
        return false;
    }

    this->settle_creation_event(tango_list);
    return true;
}

// 假定author已上线
bool LocalClient::settle_creation_event(const std::vector<TangoPair> &tango_list)
{
    this->user_author->user_info.misson_count++;
    for (unsigned int i = 0; i < tango_list.size(); i++) {
        this->user_author->user_info.exp += tango_list[i].first.length() + tango_list[i].second.length();
    }
    this->user_author->user_info.tango_count += tango_list.size();
    while (this->user_author->user_info.level * 10 + 10 <= this->user_author->user_info.exp) {
        this->user_author->user_info.exp -= this->user_author->user_info.level * 10 + 10;
        this->user_author->user_info.level++;
    }
    return true;
}

bool LocalClient::_consumer_sign_up(QString account, QString password)
{
    user_consumer = new class Consumer(this->handler);
    if (user_consumer->sign_up_local(account, password)) {
        user_status_util::add_consumer_status(this->user_status);

        return true;
    }

    _last_error = user_consumer->last_error();
    user_consumer->deleteLater();
    user_consumer = nullptr;

    return false;
}


bool LocalClient::create_author_table()
{
    static const char *create_command =
        "create table if not exists `authors` ("
        "    `id` int(11) NOT NULL AUTO_INCREMENT,"
        "    `name` varchar(45) NOT NULL,"
        "    `password` varchar(45) NOT NULL,"
        "    `exp` int(10) unsigned NOT NULL DEFAULT '0',"
        "    `level` int(10) unsigned NOT NULL DEFAULT '0',"
        "    `tango_count` int(10) unsigned NOT NULL DEFAULT '0',"
        "    `misson_count` int(10) unsigned NOT NULL DEFAULT '0',"
        "    `motto` longtext,"
        "     PRIMARY KEY (`id`),"
        "     UNIQUE KEY `name_UNIQUE` (`name`)"
        " ) engine=InnoDB default charset=utf8 comment='authors that produce good tango list for users'";

    QSqlQuery query(this->handler);

    query.exec("set names 'utf8'");
    if (!query.exec(create_command)) {
        qDebug() << "create authors table failed" << query.lastError().text();
        _last_error = query.lastError().text();

        return false;
    }

    return true;
}

bool LocalClient::create_consumer_table()
{
    static const char *create_command =
        "create table if not exists `consumers` ("
        "    `id` int(11) NOT NULL AUTO_INCREMENT,"
        "    `name` varchar(45) NOT NULL,"
        "    `password` varchar(45) NOT NULL,"
        "    `exp` int(10) unsigned NOT NULL DEFAULT '0',"
        "    `level` int(10) unsigned NOT NULL DEFAULT '0',"
        "    `tango_count` int(10) unsigned NOT NULL DEFAULT '0',"
        "    `misson_count` int(10) unsigned NOT NULL DEFAULT '0',"
        "    `motto` longtext,"
        "     PRIMARY KEY (`id`),"
        "     UNIQUE KEY `name_UNIQUE` (`name`)"
        " ) engine=InnoDB default charset=utf8 comment='consumers that enjoy the happiness the tango world'";

    QSqlQuery query(this->handler);

    query.exec("set names 'utf8'");
    if (!query.exec(create_command)) {
        qDebug() << "create consumers table failed" << query.lastError().text();
        _last_error = query.lastError().text();

        return false;
    }

    return true;
}

inline bool LocalClient::create_tangos_table()
{
    static const char *create_command =
        "create table if not exists `tangos` ("
        "    `id` int(11) NOT NULL AUTO_INCREMENT,"
        "    `key` varchar(50) NOT NULL,"
        "    `value` longtext NOT NULL,"
        "    `last_submit` varchar(45) DEFAULT NULL,"
        "    `last_submit_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,"
        "    PRIMARY KEY (`id`),"
        "    UNIQUE KEY `key_UNIQUE` (`key`),"
        "    UNIQUE KEY `id_UNIQUE` (`id`)"
        " ) engine=InnoDB default charset=utf8 comment='tango list'";

    QSqlQuery query(this->handler);

    query.exec("set names 'utf8'");
    if (!query.exec(create_command)) {
        qDebug() << "create authors table failed" << query.lastError().text();
        _last_error = query.lastError().text();
        return false;
    }

    return true;
}


bool LocalClient::create_tables()
{

    if (!this->create_author_table()) {
        return false;
    }
    if (!this->create_consumer_table()) {
        return false;
    }
    if (!this->create_tangos_table()) {
        return false;
    }

    return true;
}


bool LocalClient::retrive_kth_tango_item(TangoPair &tp, int k) {
    static const char *search_command = "select * from `tangos` order by length(key) limit :kth - 1, 1";

    QSqlQuery query(this->handler);
    query.prepare(search_command);
    query.bindValue(":kth", k);

    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }

    if (query.first()) {
        tp = TangoPair(query.value(1).toString(), query.value(2).toString());

        return true;
    }
    _last_error = "not found";

    return false;
}

int LocalClient::retrive_since_kth_tango_item(std::vector<TangoPair> &tango_list, unsigned int k, int n)
{
    static const char *search_command = "select * from `tangos` order by length(`key`) limit :kth, :ntimes";

    QSqlQuery query(this->handler);
    query.prepare(search_command);
    query.bindValue(":kth", k);
    query.bindValue(":ntimes", n);
    qDebug() << "k, n" << k << " " << n;
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return 0;
    }

    int ret = 0;
    while (query.next()) {
        tango_list.push_back(TangoPair(query.value(1).toString(), query.value(2).toString()));
        qDebug() << "fetched " << TangoPair(query.value(1).toString(), query.value(2).toString());
        ret++;
    }

    return ret;
}


bool LocalClient::retrive_tango_items(std::vector<TangoPair> &tango_list, int &n, RetriveMode mode)
{
    static std::mt19937 mtrand(static_cast<unsigned int>(time(nullptr)));
    static const char *query_count = "select count(*) from `tangos`";
    QSqlQuery query(this->handler);
    if (!query.exec(query_count)) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;
        return false;
    }
    if (!query.first()) {
        _last_error = "first fetch error";
        qDebug() << "error occured: " << _last_error;
        return false;
    }

    int tot_length = query.value(0).toInt();
    qDebug() << "tot_length " << tot_length;

    if (tot_length == 0) {
        _last_error = "empty pool in the tango database";
        return false;
    }

    unsigned int to_fetch;
    switch (mode) {
    case RetriveMode::Easy:
        tot_length = static_cast<int>(0.3 * tot_length);
        if (tot_length < n) {
            n = std::max(tot_length, 1);
        }
        to_fetch = mtrand() % static_cast<unsigned int>(tot_length - n + 1);
        _last_error = "";
        if (!this->retrive_since_kth_tango_item(tango_list, to_fetch, tot_length)) {
            if (_last_error != "") {
                _last_error = "fetch error";
            }
            return false;
        }
        return true;

    case RetriveMode::Normal:
        tot_length = static_cast<int>(0.6 * tot_length);
        if (tot_length < n) {
            n = std::max(tot_length, 1);
        }
        to_fetch = mtrand() % static_cast<unsigned int>(tot_length - n + 1);
        _last_error = "";
        if (!this->retrive_since_kth_tango_item(tango_list, to_fetch, tot_length)) {
            if (_last_error != "") {
                _last_error = "fetch error";
            }
            return false;
        }
        return true;

    case RetriveMode::Hard:
        if (tot_length < n) {
            n = std::max(tot_length, 1);
        }
        to_fetch = mtrand() % static_cast<unsigned int>(tot_length - n + 1);
        _last_error = "";
        if (!this->retrive_since_kth_tango_item(tango_list, to_fetch, tot_length)) {
            if (_last_error != "") {
                _last_error = "fetch error";
            }
            return false;
        }
        return true;
    }

    return true;
}
