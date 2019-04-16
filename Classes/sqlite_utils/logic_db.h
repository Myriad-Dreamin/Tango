
#pragma

#include "sqlite_header.h"
#include "sqlite_utils.h"

namespace app_space {
	class LogicBase
	{
	protected:
		SqliteBase *handler;
	public:
		LogicBase();

		LogicBase(SqliteBase *sqlite_handler);

		void rely_on(SqliteBase *sqlite_handler);

		void rely_off();
	};
}