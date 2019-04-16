
#pragma once


#include "sqlite_header.h"
#include "sqlite_error.h"
#include <ctime>

namespace app_space
{
	typedef bool action_result;

	const std::string defalut_encoding = "utf-8";
	// cache

	class SqliteBase
	{
	private:
		
		sqlite3 *sqlite_base_handler;

	protected:
		
		std::string self_encoding;
		char *errmsg; 

	public:
		
		SqliteBase ();

		SqliteBase (const std::string &database_path, const std::string &encoding=defalut_encoding);

		SqliteBase (const SqliteBase &right_base);

		SqliteBase (SqliteBase &&right_base);

		// 不允许被override
		~SqliteBase ();

		SqliteBase operator= (const SqliteBase & right_base);

		action_result open (
			const std::string &database_path,
			const std::string &encoding = defalut_encoding
		);

		action_result close ();

		SQLITE_EXCEPTION exec (
			const std::string &sql_query,
			sqlite3_callback cb_func = nullptr,
			void *data = nullptr
		);
	};
}