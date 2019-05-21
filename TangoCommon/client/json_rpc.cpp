
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

        QJsonArray params;
        params.push_back(info);

        request.insert("result", params);

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

        return QJsonDocument(request).toJson();
    }

    QByteArray sync_status_returns(const UserStatus &status, const UserFullInfo &author_info, const UserFullInfo &consumer_info)
    {
        QJsonObject request;
        request.insert("id", code::logout);
        request.insert("jsonrpc", "2.0");

        QJsonArray params;
        params.push_back(status);
        params.push_back(UserFullInfo::to_json_array(author_info));
        params.push_back(UserFullInfo::to_json_array(consumer_info));

        request.insert("result", params);

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

        QJsonArray params;
        params.push_back(UserFullInfo::to_json_array(info));

        request.insert("params", params);

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
        if (json_decoder.isObject()) {
            QJsonObject ret = json_decoder.object();

            if (ret.length() == 4) {
                if (!ret.contains("id") || !ret.contains("method") || !ret.contains("jsonrpc") || !ret.contains("params")) {
                    err = "missing body";
                    return false;
                }
                if (!ret["params"].isArray()) {
                    err = "invalid params";
                    return false;
                }
                if (ret["jsonrpc"].toString() != "2.0") {
                    err = "unsupported jsonrpc";
                    return false;
                }

                params = ret["params"].toArray();
                id = ret["id"].toInt();
                return true;
            } else {
                err = "mismatch body count";
                return false;
            }
        }
        err = "not obj";
        return false;
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
        if (json_decoder.isObject()) {
            QJsonObject ret = json_decoder.object();

            if (ret.length() == 3 || ret.length() == 4) {
                if (!(ret.contains("id") || !(ret.contains("result")) || (ret.contains("error") && ret.contains("code"))) || !ret.contains("jsonrpc")) {
                    err = "missing body";
                    return false;
                }
                if (ret["jsonrpc"].toString() != "2.0") {
                    err = "unsupported jsonrpc";
                    return false;
                }

                if (ret.contains("error")) {
                    qDebug() << "error occurs" << ret["code"].toInt() << ret["error"].toString();
                    err = ret["error"].toString();
                    return false;
                }

                rets = ret["result"];
                id = ret["id"].toInt();
            } else {
                err = "mismatch return json";
                return false;
            }

            return true;
        }
        err = "not obj";
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

};


