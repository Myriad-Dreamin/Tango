
#pragma once

#include "sqlite_utils.h"

namespace app_space
{
	SqliteBase::SqliteBase()
	{
		sqlite_base_handler = nullptr;
	}

	SqliteBase::SqliteBase(
		const std::string &database_path,
		const std::string &encoding
	) {
	}

	SqliteBase::SqliteBase(const SqliteBase & right_base)
	{
		if (sqlite_base_handler != nullptr) {
			close();
		}
		sqlite_base_handler = right_base.sqlite_base_handler;
		self_encoding = right_base.self_encoding;
	}

	SqliteBase::SqliteBase(SqliteBase && right_base)
	{
		if (sqlite_base_handler != nullptr) {
			close();
		}
		sqlite_base_handler = right_base.sqlite_base_handler;
		right_base.sqlite_base_handler = nullptr;
		self_encoding = std::move(right_base.self_encoding);
	}

	SqliteBase::~SqliteBase()
	{
		close();
	}

	SqliteBase SqliteBase::operator=(const SqliteBase & right_base)
	{
		if (sqlite_base_handler != nullptr) {
			close();
		}
		sqlite_base_handler = right_base.sqlite_base_handler;
		self_encoding = right_base.self_encoding;
		return *this;
	}

	action_result SqliteBase::open(const std::string & database_path, const std::string &encoding)
	{
		action_result open_success = true;
		if (encoding == "utf-8") {
			// _v2
			int open_status = sqlite3_open(database_path.c_str(), &sqlite_base_handler);
			if (open_status == SQLITE_OK) {
				
			} else {
				sqlite3_close(sqlite_base_handler);
				sqlite_base_handler = nullptr;
				open_success = false;
				auto excp = SqliteException(open_status);
				CCLOGERROR("can not open database: %s", excp.what());
				throw excp;
			}
		} else if (encoding == "utf-16") {
			sqlite3_open16(database_path.c_str(), &sqlite_base_handler);
		} else {
			CCLOGERROR("invalid encoding of database: %s", encoding.c_str());
			throw std::invalid_argument("invalid encoding of database: " + encoding);
		}
		self_encoding = encoding;
		return open_success;
	}

	action_result SqliteBase::close()
	{
		if (sqlite_base_handler == nullptr) {
			return false;
		}
		reclose:
		int close_status = sqlite3_close(sqlite_base_handler);
		if (close_status == SQLITE_BUSY) {
			Sleep(100);
			goto reclose;
		}
		sqlite_base_handler = nullptr;

		return true;
	}
	
	// noexception will occur
	SQLITE_EXCEPTION SqliteBase::exec (
		const std::string &sql_query,
		sqlite3_callback cb_func,
		void *dat
	) {
		return sqlite3_exec(sqlite_base_handler, sql_query.c_str(), cb_func, dat, &errmsg);
	}


}
