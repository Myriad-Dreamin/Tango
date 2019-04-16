
# pragma once


#include "sqlite_header.h"


namespace app_space
{
	typedef int SQLITE_STATUS;

	class SqliteException
	{
	private:
		std::string err_desc;
	public:
		SqliteException::SqliteException (SQLITE_STATUS sql_err);

		SqliteException::SqliteException (const char *sql_err);

		SqliteException::SqliteException (const std::string &sql_err);

		SqliteException::SqliteException (const SqliteException &sql_err);

		const char *SqliteException::what () const;
	};
}