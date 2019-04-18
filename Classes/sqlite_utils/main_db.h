
#pragma once

#include "cocos2d.h"
#include "cocos/base/CCRef.h"
#include "sqlite_utils.h"
#include "logic_db.h"


namespace app_space
{
	class UserDB: public LogicBase
	{
		/*
		 * 闯关者属性要求必须含有：闯关者姓名、已闯关关卡数、闯关者经验值、闯关者等级，若有需要可以自行添加其他属性
		 * 出题者属性要求必须含有：出题者姓名、出题者出题数目、等级，若有需要可以自行添加其余属性。
		 */

		/*
		 * Table user:
		 * uint64   user_id    primary_key   increment
		 * string  name                      not null
		 * // finished count
		 * uint32   fincnt     initialized   0
		 * // finished level
		 * uint32   finlevel   initialized   0
		 * uint64   exp        initialized   0
		 * // contributed count
		 * uint32   concnt     initialized   0
		 * // contributed level
		 * uint32   conlevel   initialized   0
		 *
		 */
	public:
		UserDB();

		UserDB(SqliteBase *sqlite_handler);

		void init();

		// void create_user(const std::string user_name, );
	};

	class MainDB: public cocos2d::Ref, public SqliteBase
	{
	protected:
		UserDB user_db;
	public:
		MainDB ();

		MainDB (const std::string &database_path, const std::string &encoding = defalut_encoding);

		MainDB (const MainDB &right_base);

		MainDB (MainDB &&right_base);

		virtual ~MainDB ();

		void init();

		// MainDB operator= (const MainDB & right_base);

		// MainDB operator= (const SqliteBase & right_base);
	};
}
