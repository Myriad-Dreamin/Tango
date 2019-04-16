#include "sqlite_error.h"

namespace app_space
{
	 SqliteException::SqliteException(SQLITE_EXCEPTION sql_err)
	{
		switch (sql_err) {
			case SQLITE_ERROR: {
				err_desc = "SQL error or missing database";
				break;
			}
			case SQLITE_PERM: {
				err_desc = "Access permission denied";
				break;
			}
			case SQLITE_ABORT: {
				err_desc = "Callback routine requested an abort";
				break;
			}
			case SQLITE_BUSY: {
				err_desc = "The database file is locked";
				break;
			}
			case SQLITE_LOCKED: {
				err_desc = "A table in the database is locked";
				break;
			}
			default: {
				err_desc = "unknown sqlite error code: " + std::to_string(sql_err);
				break;
			}
		}
	}

	 SqliteException::SqliteException(const char * sql_err)
	{
		err_desc = std::string(sql_err);
	}

	 SqliteException::SqliteException(const std::string & sql_err)
	{
		err_desc = sql_err;
	}

	 SqliteException::SqliteException(const SqliteException & sql_err)
	{
		err_desc = sql_err.err_desc;
	}

	 const char * SqliteException::what() const
	{
		return err_desc.c_str();
	}
}

