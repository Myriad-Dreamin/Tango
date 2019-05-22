
#include "json_rpc.h"
#include <QDebug>

namespace client_rpc {
    QByteArray author_sign_in_request(QString account, QString password)
    {
        QJsonObject request;
        request.insert("id", code::author_sign_in);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "author_sign_in");

        QJsonArray params;
        params.push_back(account);
        params.push_back(password);

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    QByteArray author_sign_in_returns(const UserFullInfo &info)
    {
        QJsonObject request;
        request.insert("id", code::author_sign_in);
        request.insert("jsonrpc", "2.0");

        request.insert("result", UserFullInfo::to_json_array(info));

        return QJsonDocument(request).toJson();
    }

    QByteArray author_sign_up_request(QString account, QString password)
    {
        QJsonObject request;
        request.insert("id", code::author_sign_up);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "author_sign_up");

        QJsonArray params;
        params.push_back(account);
        params.push_back(password);

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    QByteArray author_sign_up_returns(const UserFullInfo &info)
    {
        QJsonObject request;
        request.insert("id", code::author_sign_up);
        request.insert("jsonrpc", "2.0");

        request.insert("result", UserFullInfo::to_json_array(info));

        return QJsonDocument(request).toJson();
    }

    QByteArray consumer_sign_in_request(QString account, QString password)
    {
        QJsonObject request;
        request.insert("id", code::consumer_sign_in);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "consumer_sign_in");

        QJsonArray params;
        params.push_back(account);
        params.push_back(password);

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    QByteArray consumer_sign_in_returns(const UserFullInfo &info)
    {
        QJsonObject request;
        request.insert("id", code::consumer_sign_in);
        request.insert("jsonrpc", "2.0");

        request.insert("result", UserFullInfo::to_json_array(info));

        return QJsonDocument(request).toJson();
    }

    QByteArray consumer_sign_up_request(QString account, QString password)
    {
        QJsonObject request;
        request.insert("id", code::consumer_sign_up);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "consumer_sign_up");

        QJsonArray params;
        params.push_back(account);
        params.push_back(password);

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    QByteArray consumer_sign_up_returns(const UserFullInfo &info)
    {
        QJsonObject request;
        request.insert("id", code::consumer_sign_up);
        request.insert("jsonrpc", "2.0");

        request.insert("result", UserFullInfo::to_json_array(info));

        return QJsonDocument(request).toJson();
    }


    QByteArray logout_requset()
    {
        QJsonObject request;
        request.insert("id", code::logout);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "logout");

        QJsonArray params;

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    QByteArray logout_returns(const UserStatus &info)
    {
        QJsonObject request;
        request.insert("id", code::logout);
        request.insert("jsonrpc", "2.0");

        QJsonArray result;
        result.push_back(info);

        request.insert("result", result);

        return QJsonDocument(request).toJson();
    }


    QByteArray sync_status_request()
    {
        QJsonObject request;
        request.insert("id", code::sync_status);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "sync_status");

        QJsonArray params;

        request.insert("params", params);
        qDebug() << "req" << QJsonDocument(request);
        return QJsonDocument(request).toJson();
    }

    QByteArray sync_status_returns(const UserStatus &status, const UserFullInfo &author_info, const UserFullInfo &consumer_info)
    {
        QJsonObject request;
        request.insert("id", code::logout);
        request.insert("jsonrpc", "2.0");

        QJsonArray result;
        result.push_back(status);
        result.push_back(UserFullInfo::to_json_array(author_info));
        result.push_back(UserFullInfo::to_json_array(consumer_info));

        request.insert("result", result);
        qDebug() << "ret" << status << author_info.user_id << consumer_info.user_id;
        return QJsonDocument(request).toJson();
    }

    QByteArray submit_tango_items_request(const std::vector<TangoPair> &tango_list)
    {
        QJsonObject request;
        request.insert("id", code::submit_tango_items);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "submit_tango_items");

        QJsonArray params;
        for (unsigned int i = 0; i < tango_list.size(); i++) {
            params.push_back(TangoPair::to_json_array(tango_list[i]));
        }

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    QByteArray submit_tango_items_returns(const UserFullInfo &info)
    {
        QJsonObject request;
        request.insert("id", code::submit_tango_items);
        request.insert("jsonrpc", "2.0");

        QJsonArray result;
        result.push_back(UserFullInfo::to_json_array(info));

        request.insert("result", result);

        return QJsonDocument(request).toJson();
    }


    QByteArray start_game_event_request(int n, RetriveMode mode)
    {
        QJsonObject request;
        request.insert("id", code::start_game_event);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "start_game_event");

        QJsonArray params;
        params.push_back(n);
        params.push_back(static_cast<int>(mode));

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    // QByteArray start_game_event_returns() {}

    QByteArray settle_game_event_request()
    {
        QJsonObject request;
        request.insert("id", code::settle_game_event);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "settle_game_event");

        QJsonArray params;

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    QByteArray settle_game_event_returns(const UserFullInfo &info)
    {
        QJsonObject request;
        request.insert("id", code::settle_game_event);
        request.insert("jsonrpc", "2.0");

        QJsonArray result;
        result.push_back(UserFullInfo::to_json_array(info));

        request.insert("result", result);

        return QJsonDocument(request).toJson();
    }

    QByteArray query_authors_brief_info_request(int l, int r)
    {
        QJsonObject request;
        request.insert("id", code::query_authors_brief_info);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "query_authors_brief_info");

        QJsonArray params;
        params.push_back(l);
        params.push_back(r);

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    QByteArray query_authors_brief_info_returns(std::vector<UserBriefInfo> &info_list)
    {
        QJsonObject request;
        request.insert("id", code::submit_tango_items);
        request.insert("jsonrpc", "2.0");

        QJsonArray result;
        for (unsigned int i = 0; i < info_list.size(); i++) {
            result.push_back(UserBriefInfo::to_json_array(info_list[i]));
        }

        request.insert("result", result);

        return QJsonDocument(request).toJson();
    }

    QByteArray query_consumers_brief_info_request(int l, int r)
    {
        QJsonObject request;
        request.insert("id", code::query_consumers_brief_info);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "query_consumers_brief_info");

        QJsonArray params;
        params.push_back(l);
        params.push_back(r);

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    QByteArray query_consumers_brief_info_returns(std::vector<UserBriefInfo> &info_list)
    {
        QJsonObject request;
        request.insert("id", code::query_consumers_brief_info);
        request.insert("jsonrpc", "2.0");

        QJsonArray result;
        for (unsigned int i = 0; i < info_list.size(); i++) {
            result.push_back(UserBriefInfo::to_json_array(info_list[i]));
        }

        request.insert("result", result);

        return QJsonDocument(request).toJson();
    }

    QByteArray query_authors_by_id_request(int id)
    {
        QJsonObject request;
        request.insert("id", code::query_authors_by_id);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "query_authors_by_id");

        QJsonArray params;
        params.push_back(id);

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }
    QByteArray query_authors_by_name_request(QString name)
    {
        QJsonObject request;
        request.insert("id", code::query_authors_by_name);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "query_authors_by_name");

        QJsonArray params;
        params.push_back(name);

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }
    QByteArray query_consumers_by_id_request(int id)
    {
        QJsonObject request;
        request.insert("id", code::query_consumers_by_id);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "query_consumers_by_id");

        QJsonArray params;
        params.push_back(id);

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }
    QByteArray query_consumers_by_name_request(QString name)
    {
        QJsonObject request;
        request.insert("id", code::query_consumers_by_name);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "query_consumers_by_name");

        QJsonArray params;
        params.push_back(name);

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    QByteArray query_users_info_returns(int id, UserFullInfo &query_container)
    {
        QJsonObject request;
        request.insert("id", id);
        request.insert("jsonrpc", "2.0");

        request.insert("result", UserFullInfo::to_json_array(query_container));

        return QJsonDocument(request).toJson();
    }

    QByteArray query_users_request()
    {
        QJsonObject request;
        request.insert("id", code::query_users);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "query_consumers_by_name");

        QJsonArray params;

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }
    QByteArray query_users_returns(int query_count)
    {
        QJsonObject request;
        request.insert("id", code::query_users);
        request.insert("jsonrpc", "2.0");

        QJsonArray returns;

        returns.push_back(query_count);

        request.insert("result", returns);

        return QJsonDocument(request).toJson();
    }

    bool decode_json_params_object(QByteArray bytes_json, QJsonArray &params, int &id, QString &err)
    {
        QJsonParseError decode_err;
        QJsonDocument json_decoder = QJsonDocument::fromJson(bytes_json, &decode_err);;
        qDebug() << "decoding" << bytes_json;
        if (decode_err.error != QJsonParseError::NoError) {
            err = decode_err.errorString();
            return false;
        }
        qDebug() << "decoding" << json_decoder;
        if (!json_decoder.isObject()) {
            err = "not obj";
            return false;
        }
        QJsonObject ret = json_decoder.object();

        if (!ret.contains("jsonrpc") || ret["jsonrpc"].toString() != "2.0") {
            err = "unsupported jsonrpc";
            return false;
        }

        if (!ret.contains("params") || !ret["params"].isArray()) {
            err = "invalid params";
            return false;
        }

        if (ret.length() != 4 || !ret.contains("id") || !ret.contains("method")) {
            err = "mismatch body count";
            return false;
        }

        params = ret["params"].toArray();
        id = ret["id"].toInt();
        return true;
    }

    bool decode_json_rets_object(QByteArray bytes_json, QJsonValue &rets, int &id, QString &err)
    {
        QJsonParseError decode_err;
        QJsonDocument json_decoder = QJsonDocument::fromJson(bytes_json, &decode_err);;
        qDebug() << "decoding" << bytes_json;
        if (decode_err.error != QJsonParseError::NoError) {
            err = decode_err.errorString();
            return false;
        }
        qDebug() << "decoding" << json_decoder;
        if (!json_decoder.isObject()) {
            err = "not obj";
            return false;
        }

        QJsonObject ret = json_decoder.object();

        if (!ret.contains("jsonrpc") || ret["jsonrpc"].toString() != "2.0") {
            err = "unsupported jsonrpc";
            return false;
        }

        if (ret.size() == 3 && ret.contains("id") && ret.contains("result")) {
            rets = ret["result"];
            id = ret["id"].toInt();
            return true;
        }

        if (ret.size() == 4 && ret.contains("id") && ret.contains("error") && ret.contains("code")) {
            err = ret["error"].toString();
            return false;
        }

        err = "mismatch return json";
        return false;
    }


    namespace _maker {
        inline QByteArray _method_not_found()
        {
            QJsonObject result;
            result.insert("id", 0);
            result.insert("jsonrpc", "2.0");
            result.insert("error", "method not found");
            result.insert("code", RPCBaseError::MethodNotFound);
            return QJsonDocument(result).toJson();
        }

        inline QByteArray _invalid_params()
        {
            QJsonObject result;
            result.insert("id", 0);
            result.insert("jsonrpc", "2.0");
            result.insert("error", "invalid params");
            result.insert("code", RPCBaseError::InvalidParams);
            return QJsonDocument(result).toJson();
        }

        inline QByteArray _signal_start_game()
        {
            QJsonObject request;
            request.insert("id", code::signal_start_game);
            request.insert("jsonrpc", "2.0");
            request.insert("method", "signal_start_game");

            QJsonArray params;
            request.insert("params", params);

            return QJsonDocument(request).toJson();
        }

        inline QByteArray _signal_game_success()
        {
            QJsonObject request;

            request.insert("id", code::signal_game_success);
            request.insert("jsonrpc", "2.0");
            request.insert("method", "signal_game_success");
            QJsonArray params;
            request.insert("params", params);
            return QJsonDocument(request).toJson();
        }

        inline QByteArray _signal_game_failed()
        {
            QJsonObject request;
            request.insert("id", code::signal_game_failed);
            request.insert("jsonrpc", "2.0");
            request.insert("method", "signal_game_failed");

            QJsonArray params;
            request.insert("params", params);

            return QJsonDocument(request).toJson();
        }

        inline QByteArray _signal_game_stop()
        {
            QJsonObject request;
            request.insert("id", code::signal_game_stop);
            request.insert("jsonrpc", "2.0");
            request.insert("method", "signal_game_stop");

            QJsonArray params;
            request.insert("params", params);

            return QJsonDocument(request).toJson();
        }
    }

    QByteArray err_invalid_request_result(QString err)
    {
        QJsonObject result;
        result.insert("id", 0);
        result.insert("jsonrpc", "2.0");
        result.insert("error", err);
        result.insert("code", RPCBaseError::InvalidRequest);
        return QJsonDocument(result).toJson();
    }


    QByteArray err_exec_error(int id, QString err)
    {
        QJsonObject result;
        result.insert("id", id);
        result.insert("jsonrpc", "2.0");
        result.insert("error", err);
        result.insert("code", RPCBaseError::InternalError);
        return QJsonDocument(result).toJson();
    }

    QByteArray err_method_not_found()
    {
        static QByteArray ret = _maker::_method_not_found();
        return ret;
    }

    QByteArray err_invalid_params()
    {
        static QByteArray ret = _maker::_invalid_params();
        return ret;
    }

    QByteArray success(int16_t id)
    {
        QJsonObject result;
        result.insert("id", id);
        result.insert("jsonrpc", "2.0");
        result.insert("result", true);
        return QJsonDocument(result).toJson();
    }

    QByteArray signal_start_game_request()
    {
        static QByteArray ret = _maker::_signal_start_game();
        return ret;
    }

    QByteArray signal_new_tango_request(const TangoPair &tango, int fade_time)
    {
        QJsonObject request;

        request.insert("id", code::signal_new_tango);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "signal_new_tango");

        QJsonArray params;
        params.push_back(TangoPair::to_json_array(tango));
        params.push_back(fade_time);

        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    QByteArray signal_tango_faded_request(int answer_time)
    {
        QJsonObject request;
        request.insert("id", code::signal_tango_faded);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "signal_tango_faded");

        QJsonArray params;
        params.push_back(answer_time);
        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }

    QByteArray signal_game_success_request()
    {
        static QByteArray ret = _maker::_signal_game_success();
        return ret;
    }

    QByteArray signal_game_failed_request()
    {
        static QByteArray ret = _maker::_signal_game_failed();
        return ret;
    }

    QByteArray signal_game_stop_request()
    {
        static QByteArray ret = _maker::_signal_game_stop();
        return ret;
    }

    QByteArray signal_game_answer_request(QString tango)
    {
        QJsonObject request;

        request.insert("id", code::signal_game_answer);
        request.insert("jsonrpc", "2.0");
        request.insert("method", "signal_game_answer");
        QJsonArray params;
        params.push_back(tango);
        request.insert("params", params);

        return QJsonDocument(request).toJson();
    }
};
