
#pragma once



#include "OptionBox.h"

namespace app_space
{
	void OptionBox::on_touch_event(cocos2d::Ref* refer, cocos2d::ui::Widget::TouchEventType event_type)
	{
		switch (event_type) {
			case cocos2d::ui::Widget::TouchEventType::BEGAN: {
				CCLOG("button began");
				break;
			}
			case cocos2d::ui::Widget::TouchEventType::MOVED: {
				CCLOG("button moved");
				break;
			}
			case cocos2d::ui::Widget::TouchEventType::ENDED: {
				CCLOG("button ended");

				break;
			}
			case cocos2d::ui::Widget::TouchEventType::CANCELED: {
				CCLOG("button canceled");
				break;
			}
			default: {
				CCLOG("button other ..");
				break;
			}
		}
		return;
	}
	void OptionBox::on_click_event(cocos2d::Ref* refer)
	{
		CCLOG("click began %p\n", refer);
		return;
	}
	bool OptionBox::init()
	{
		ccbutton::initRenderer();
		ccbutton::loadTextureNormal("start_scene/button2.png");
		ccbutton::setPressedActionEnabled(true);
		ccbutton::setScale9Enabled(false);
		ccbutton::setTitleText("Testiasdfojioasdjfoijioasdjfoijasiodfj");
		CCLOG("this option: %p caller: %p\n", this, on_click_event);
		ccbutton::addClickEventListener(Widget::ccWidgetClickCallback(on_click_event));
		// CCLOG("init option_button: %s", init_success ? "true" : "false");
		return true;
	}

}