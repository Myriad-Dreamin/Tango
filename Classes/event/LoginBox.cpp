
#pragma once



#include "LoginBox.h"
#include "magic_macros.h"

namespace app_space {

	bool LoginBox::init()
	{
		bool init_success = initWithFile("start_scene/login.png");
		CCLOG("init login_box %d\n", init_success);
		return true;
	}
}