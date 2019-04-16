
#pragma once

#include "cocos2d.h"
#include "cocos/base/CCRef.h"
#include "sqlite_utils.h"


namespace app_space
{
	class MainDB: public cocos2d::Ref, public SqliteBase
	{
	public:
		MainDB ();

		MainDB (const std::string &database_path, const std::string &encoding = defalut_encoding);

		MainDB (const MainDB &right_base);

		MainDB (MainDB &&right_base);

		virtual ~MainDB ();

		MainDB operator= (const MainDB & right_base);

		MainDB operator= (const SqliteBase & right_base);

		action_result open (
			const std::string &database_path,
			const std::string &encoding = defalut_encoding
		);

		action_result close ();
	};
};
