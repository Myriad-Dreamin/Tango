#ifndef JSON_RPC_H
#define JSON_RPC_H

#include <cstdint>
#include <vector>

#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "../types/UserFullInfo.h"
#include "../types/UserStatus.h"
#include "../types/TangoPair.h"

namespace client_rpc {
    enum RPCBaseError: int16_t {
        ParseError = -32700,
        InvalidRequest = -32600,
        MethodNotFound = -32601,
        InvalidParams = -32602,
        InternalError = -32603,
        ServerError = -32099
    };


    enum code: int16_t {
        author_sign_in = 0x0001,
        author_sign_up = 0x0002,
        consumer_sign_in = 0x0003,
        consumer_sign_up = 0x0004,
        logout = 0x0005,
        sync_status = 0x0006,
        submit_tango_items = 0x0007
    };

    QByteArray author_sign_in_request(QString account, QString password);
    QByteArray author_sign_in_returns(const UserFullInfo &info);

    QByteArray author_sign_up_request(QString account, QString password);
    QByteArray author_sign_up_returns(const UserFullInfo &info);

    QByteArray consumer_sign_in_request(QString account, QString password);
    QByteArray consumer_sign_in_returns(const UserFullInfo &info);

    QByteArray consumer_sign_up_request(QString account, QString password);
    QByteArray consumer_sign_up_returns(const UserFullInfo &info);

    QByteArray logout_requset();
    QByteArray logout_returns(const UserStatus &info);

    QByteArray sync_status_request();
    QByteArray sync_status_returns(const UserStatus &status, const UserFullInfo &author_info, const UserFullInfo &consumer_info);

    QByteArray submit_tango_items_request(const std::vector<TangoPair> &tango_list);
    QByteArray submit_tango_items_returns(const UserFullInfo &info);

    bool decode_json_params_object(QByteArray bytes_json, QJsonArray &params, int &id, QString &err);
    bool decode_json_rets_object(QByteArray bytes_json, QJsonValue &ret, int &id, QString &err);
    QByteArray err_invalid_request_result(QString err);
    QByteArray err_exec_error(int id, QString err);
    QByteArray err_method_not_found();
    QByteArray err_invalid_params();
    QByteArray success(int16_t id);

}


#endif // JSON_RPC_H
