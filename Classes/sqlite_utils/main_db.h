
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
		 * ����������Ҫ����뺬�У��������������Ѵ��عؿ����������߾���ֵ�������ߵȼ���������Ҫ�������������������
		 * ����������Ҫ����뺬�У������������������߳�����Ŀ���ȼ���������Ҫ������������������ԡ�
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
		UserDB();

		UserDB(SqliteBase *sqlite_handler);

		void init();
	};

	class MainDB: public cocos2d::Ref, public SqliteBase
	{
	public:
		MainDB ();

		MainDB (const std::string &database_path, const std::string &encoding = defalut_encoding);

		MainDB (const MainDB &right_base);

		MainDB (MainDB &&right_base);

		virtual ~MainDB ();

		// MainDB operator= (const MainDB & right_base);

		// MainDB operator= (const SqliteBase & right_base);
	};
}
