
#pragma once


#include "main_db.h"


namespace app_space
{
	UserDB::UserDB (): LogicBase() {}

	UserDB::UserDB (SqliteBase *sqlite_handler) : LogicBase(sqlite_handler) {}

	void UserDB::init()
	{
		/*
		 * Sqlite3 doesn't support uint32, so we use uint64 instead.
		 * Table user:
		 * uint32   user_id    primary_key   increment
		 * string   name                     not null
		 * // finished count
		 * uint32   fincnt     initialized   0
		 * // finished level
		 * uint32   finlevel   initialized   0
		 * uint64   exp        initialized   0
		 * // contributed count
		 * uint32   concnt     initialized   0
		 * // contributed level
		 * uint32   conlevel   initialized   0
		 * uint32   conexp   initialized   0
		 */
		static const std::string create_table_cmd =
			"CREATE TABLE IF NOT EXISTS user_table ("
			"user_id INTEGER PRIMARY KEY AUTOINCREMENT, "
			"name NCHAR(30) NOT NULL, "
			"fincnt UNSIGNED BIG INT DEFAULT 0, "
			"finlevel UNSIGNED BIG INT DEFAULT 0, "
			"finexp UNSIGNED BIG INT DEFAULT 0, "
			"concnt UNSIGNED BIG INT DEFAULT 0, "
			"conlevel UNSIGNED BIG INT DEFAULT 0, "
			"conexp UNSIGNED BIG INT DEFAULT 0"
			"); ";
		SQLITE_STATUS create_status = handler->exec(create_table_cmd);

		if (create_status != SQLITE_OK) {
			auto excp = SqliteException(create_status);
			CCLOGERROR("can not create table: %s", excp.what());
			throw excp;
		}
	}

	MainDB::MainDB (): SqliteBase()
	{
		user_db.rely_on(this);
	}

	MainDB::MainDB (const std::string & database_path, const std::string & encoding):
		SqliteBase(database_path, encoding)
	{
		user_db.rely_on(this);
	}

	MainDB::MainDB (const MainDB & right_base): SqliteBase(right_base)
	{
		user_db.rely_on(this);
	}
	
	MainDB::MainDB (MainDB && right_base): SqliteBase(right_base)
	{
		user_db.rely_on(this);
	}
	
	MainDB::~MainDB ()
	{
		user_db.rely_off();
	}
	void MainDB::init()
	{
		user_db.init();
	}
}