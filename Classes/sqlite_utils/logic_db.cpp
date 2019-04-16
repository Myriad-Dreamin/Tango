

#include "logic_db.h"

namespace app_space
{
	LogicBase::LogicBase ()
	{
		handler = nullptr;
	}
	LogicBase::LogicBase (SqliteBase *sqlite_handler)
	{
		handler = sqlite_handler;
	}
	void LogicBase::rely_on (SqliteBase *sqlite_handler)
	{
		handler = sqlite_handler;
	}
	void LogicBase::rely_off ()
	{
		handler = nullptr;
	}
}