#pragma once

#include <iostream>
#include "cocos2d.h"
#include "ui/UIButton.h"

namespace app_space
{
	using ccbutton = cocos2d::ui::Button;
	class OptionBox: public ccbutton
	{

	public:
		static void on_touch_event(cocos2d::Ref * refer, cocos2d::ui::Widget::TouchEventType event_type);
		static void on_click_event(cocos2d::Ref * refer);
		virtual bool init();

		CREATE_FUNC(OptionBox)

	};
}

