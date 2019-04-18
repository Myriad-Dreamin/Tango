
#pragma once



#include "StartBox.h"
#include "magic_macros.h"

namespace app_space {

	bool StartBox::init()
	{
		ccbutton::initRenderer();
		ccbutton::setFocused(true);
		ccbutton::loadTextureNormal("start_scene/button1.png");
		// ccbutton:: loadTexturePressed
		// ccbutton:: loadTextureDisabled
		ccbutton::setPressedActionEnabled(true);
		ccbutton::setScale9Enabled(false);
		ccbutton::setTitleLabel(cocos2d::Label::createWithTTF(siyuan_light_45, "开始游戏"));

		ccbutton::addTouchEventListener([this](Ref* refer, Widget::TouchEventType event_type){
			CCLOG("touched: %p %p\n", this, refer);
			auto start_box = (StartBox*)refer;
			switch (event_type) {
				case cocos2d::ui::Widget::TouchEventType::BEGAN: {
					start_box->runAction(cocos2d::MoveBy::create(
						0.2, cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width * 0.02, 0)
					));
					break;
				}
				case cocos2d::ui::Widget::TouchEventType::MOVED: {
					CCLOG("button moved");
					break;
				}
				case cocos2d::ui::Widget::TouchEventType::ENDED: {
					CCLOG("button ended");
					start_box->runAction(cocos2d::MoveBy::create(
						0.2, cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width * -0.02, 0)
					));
					break;
				}
				case cocos2d::ui::Widget::TouchEventType::CANCELED: {
					CCLOG("button canceled");
					start_box->runAction(cocos2d::MoveBy::create(
						0.2, cocos2d::Vec2(cocos2d::Director::getInstance()->getVisibleSize().width * -0.02, 0)
					));
					break;
				}
				default: {
					CCLOG("button other ..");
					break;
				}
			}
			return;
		});
		// CCLOG("init start_button: %s", init_success ? "true" : "false");
		return true;
	}
}