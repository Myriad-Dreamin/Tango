
#pragma once


#include "main_db.h"


namespace app_space
{
	MainDB::MainDB (): SqliteBase() {}

	MainDB::MainDB (const std::string & database_path, const std::string & encoding):
		SqliteBase(database_path, encoding)
	{}

	MainDB::MainDB (const MainDB & right_base): SqliteBase(right_base) {}
	
	MainDB::MainDB (MainDB && right_base): SqliteBase(right_base) {}
	
	MainDB::~MainDB () {}

	action_result MainDB::open(const std::string & database_path, const std::string & encoding)
	{
		return SqliteBase::open(database_path, encoding);
	}
	action_result MainDB::close()
	{
		return SqliteBase::close();
	}
}