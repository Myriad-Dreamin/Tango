
#pragma once



#include "ExitBox.h"

namespace app_space
{

	bool ExitBox::init()
	{
		bool init_success = true;
		ccbutton::loadTextureNormal("start_scene/button2.png");
		CCLOG("init start_button: %s", init_success ? "true" : "false");

		return init_success;
	}

}