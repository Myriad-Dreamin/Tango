/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "StartScene.h"
USING_NS_CC;

Scene* StartScene::createScene()
{
    return StartScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	Size windows_size = Director::getInstance()->getVisibleSize();


	auto login_box = app_space::LoginBox::create();
	login_box->setPosition(cocos2d::Point(windows_size.width * 0.1, windows_size.height * 0.9));

	auto start_button = app_space::StartBox::create();
	start_button->setPosition(Point(windows_size.width * 0.55, windows_size.height * 0.63));
	
	// auto option_button = app_space::OptionBox::create();
	// option_button->init();
	// option_button->setPosition(Point(windows_size.width * 0.55, windows_size.height * 0.50));


	auto center_circle = Sprite::create("start_scene/center_circle.png");
	center_circle->setPosition(Point(windows_size.width * 0.6, windows_size.height * 0.50));
	
	// MessageBox("消息内容", "Message Header");
	// LabelTTF *label = LabelTTF::create();
	// label->setString("这是我的文本");
	// label->setFontSize(36);
	// label->setPosition(Point(windows_size.width / 2, windows_size.height / 3));
	TextFieldTTF *tf = TextFieldTTF::textFieldWithPlaceHolder("在这里输入", "宋体", 20);
	tf->setPosition(Point(windows_size.width / 4, 3 * windows_size.height / 4));
	
	// addChild(label);
	addChild(tf);
	addChild(center_circle, LAYER1 + HEIGHT0);
	addChild(start_button, LAYER1 + HEIGHT1);
	addChild(login_box, LAYER1 + HEIGHT0);
	// addChild(option_button, LAYER1 + HEIGHT1);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [tf](Touch *t, Event *event) {
		log("catcher");

		if (tf->getBoundingBox().containsPoint(t->getLocation())) {
			log("catcherer");
			tf->attachWithIME();
		}
		else {
			tf->detachWithIME();
		}

		return false;
	};

	Director::getInstance()->getEventDispatcher()->
		addEventListenerWithSceneGraphPriority(listener, tf);

    return true;
}


void StartScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
