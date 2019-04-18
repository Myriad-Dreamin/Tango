#pragma once

#include <iostream>
#include "cocos2d.h"
#include "ui/UIButton.h"

namespace app_space
{
	using ccbutton = cocos2d::ui::Button;
	class ExitBox: public ccbutton
	{

	public:
		virtual bool init();

		CREATE_FUNC(ExitBox)

	};
}

