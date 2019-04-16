
#pragma once


#include "sqlite_header.h"
#include "sqlite_error.h"
#include <utility>
#include <ctime>


namespace app_space
{
	typedef bool action_result;
	template<typename T> struct query_result
	{
		T ret;
		SQLITE_STATUS status;
	};

	const std::string defalut_encoding = "utf-8";
	// cache

	class SqliteBase
	{
	private:
		
		sqlite3 *sqlite_base_handler;

	protected:
		
		std::string self_encoding; 

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

		SQLITE_STATUS exec (
			const std::string &sql_query,
			sqlite3_callback cb_func = nullptr,
			void *data = nullptr
		);

		query_result<int> is_table_exist(
			const std::string &table_query
		);
	};
}