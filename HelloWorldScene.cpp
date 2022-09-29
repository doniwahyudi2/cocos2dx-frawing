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

#include "nyoba.h"
#include "mewarnai.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace cocos2d::ui;
using namespace tinyxml2;

Scene* nyoba::createScene()
{
    return nyoba::create();
}



// on "init" you need to initialize your instance
bool nyoba::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    auto bg = Sprite::create("stage/box1.png");
    bg->setScale(1111);
    bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(bg);

    _texture = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888);
    _texture->retain();
    _texture->setPosition(visibleSize / 2);
    this->addChild(_texture);

    _brush = Sprite::create("gameplay2/brush.png");
    _brush->setScale(0.7);
    _brush->setColor(cocos2d::Color3B::WHITE);
    _brush->retain();

    _texture->beginWithClear(1, 1, 1, 1);
    _texture->end();

    auto listener = EventListenerTouchOneByOne::create();
    //listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(nyoba::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(nyoba::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(nyoba::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


    auto kapal = Sprite::create("gambar/101.png");
    kapal->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    kapal->setScale(1.1);
    this->addChild(kapal);

 
    auto b_back = Button::create("stage/b_back.png");
    b_back->setAnchorPoint(Point(0, 1));
    b_back->setPosition(Vec2(20 + origin.x, visibleSize.height + origin.y - 10));
    this->addChild(b_back);
    b_back->setZoomScale(-0.1);
    b_back->addClickEventListener([=](Ref* Sender) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/touch.mp3");
        auto gr_scene = mewarnai::createScene();
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.7, gr_scene));
        });

    auto camera = Button::create("gameplay2/b_capture.png");
    camera->setAnchorPoint(Point(0, 1));
    camera->setPosition(Vec2(20 + origin.x, visibleSize.height + origin.y - 120));
    this->addChild(camera);
    camera->setZoomScale(-0.1);
    camera->addClickEventListener([=](Ref* Sender) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/photo.mp3");
        utils::captureScreen(CC_CALLBACK_2(nyoba::afterCapture, this), "Screenshot.png");
        });

    auto sampah = Button::create("gameplay2/b_delete.png");
    sampah->setAnchorPoint(Point(0, 1));
    sampah->setPosition(Vec2(20 + origin.x, visibleSize.height + origin.y - 600));
    this->addChild(sampah);
    sampah->setZoomScale(-0.1);
    sampah->addClickEventListener([=](Ref* Sender) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/touch.mp3");
        
        });
       
    auto panel = Sprite::create("gameplay2/panelwarnacrop.png");
    panel->setPosition(Vec2(visibleSize.width / 2 + origin.x + 720, visibleSize.height / 2 + origin.y));
    panel->setScaleY(1.5);
    this->addChild(panel);


    auto panel2 = Sprite::create("gameplay2/panelwarnacrop.png");
    panel2->setAnchorPoint(Point(1, 0));
    panel2->setPosition(Vec2(visibleSize.width / 2 + origin.x + 720, visibleSize.height / 2 + origin.y - 180));
    panel2->setScaleY(1.5);
    panel2->setOpacity(0);
    this->addChild(panel2);

    scr_objek = ScrollView::create();
    scr_objek->setDirection(ScrollView::Direction::VERTICAL);
    scr_objek->setContentSize(Size(visibleSize.width + origin.x, +1000));
    scr_objek->setInnerContainerSize(Size((171 * (6 + 10)) - scr_objek->getContentSize().height + 280, 0));
    scr_objek->setAnchorPoint(Point(0.5, 0.5));
    scr_objek->setPosition(Vec2(panel->getPosition()));
    scr_objek->setBounceEnabled(true);
    scr_objek->setClippingEnabled(true);
    scr_objek->setSwallowTouches(false);
    this->addChild(scr_objek, 99);
    scr_objek->setScrollBarOpacity(0);

    for (int i = 0; i < 7; i++)
    {
        krayon2[i] = Sprite::create("gameplay2/crayon_a.png");
        krayon2[i]->setAnchorPoint(Point(0.5, 0.5));
        krayon2[i]->setPosition(Vec2(1010 + ((krayon2[i]->getContentSize().width - 5) * i),
            panel2->getContentSize().height - 0));
        scr_objek->addChild(krayon2[i]);
        krayon2[i]->setScale(1.5);
        if (i > 0) {
            krayon2[i]->setPosition(Vec2(krayon2[i - 1]->getPositionX(),
                panel->getContentSize().height - 130));
        };
        if (i > 1)
        {
            krayon2[i]->setPosition(Vec2(krayon2[i - 1]->getPositionX(),
                panel->getContentSize().height - 130 * 2));
        };
        if (i > 2) {
            krayon2[i]->setPosition(Vec2(krayon2[i - 1]->getPositionX(),
                panel->getContentSize().height - 130 * 3));
        };
        if (i > 3)
        {
            krayon2[i]->setPosition(Vec2(krayon2[i - 1]->getPositionX(),
                panel->getContentSize().height - 130 * 4));
        };
        if (i > 4) {
            krayon2[i]->setPosition(Vec2(krayon2[i - 1]->getPositionX(),
                panel->getContentSize().height - 130 * 5));
        };
        if (i > 5) {
            krayon2[i]->setPosition(Vec2(krayon2[i - 1]->getPositionX(),
                panel->getContentSize().height - 130 * 6));
        };
        if (i > 6) {
            krayon2[i]->setPosition(Vec2(krayon2[i - 1]->getPositionX(),
                panel->getContentSize().height - 130 * 7));
        };


        krayon[i] = ui::Button::create("gameplay2/crayon_b.png");
        krayon[i]->setAnchorPoint(Point(0.5, 0.5));
        krayon[i]->setPosition(Vec2(krayon2[i]->getContentSize() / 2));
        krayon2[i]->addChild(krayon[i]);
        krayon[i]->setZoomScale(-0.1);
        krayon[0]->addClickEventListener([=](Ref* Sender) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/fillcolor.mp3");
            _brush->setColor(cocos2d::Color3B::RED);
            });
    }
    krayon[1]->addClickEventListener([=](Ref* Sender) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/fillcolor.mp3");
        _brush->setColor(cocos2d::Color3B::GREEN);
        });


    krayon[2]->addClickEventListener([=](Ref* Sender) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/fillcolor.mp3");
        _brush->setColor(cocos2d::Color3B::BLUE);
        });

    krayon[3]->addClickEventListener([=](Ref* Sender) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/fillcolor.mp3");
        _brush->setColor(cocos2d::Color3B::YELLOW);
        });
    krayon[4]->addClickEventListener([=](Ref* Sender) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/fillcolor.mp3");
        _brush->setColor(cocos2d::Color3B::ORANGE);
        });
    krayon[5]->addClickEventListener([=](Ref* Sender) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/fillcolor.mp3");
        _brush->setColor(cocos2d::Color3B::BLACK);
        });
    krayon[6]->addClickEventListener([=](Ref* Sender) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/fillcolor.mp3");
        _brush->setColor(cocos2d::Color3B::MAGENTA);
        });

    krayon2[0]->setColor(cocos2d::Color3B::RED);
    krayon2[1]->setColor(cocos2d::Color3B::GREEN);
    krayon2[2]->setColor(cocos2d::Color3B::BLUE);
    krayon2[3]->setColor(cocos2d::Color3B::YELLOW);
    krayon2[4]->setColor(cocos2d::Color3B::ORANGE);
    krayon2[5]->setColor(cocos2d::Color3B::BLACK);
    krayon2[6]->setColor(cocos2d::Color3B::MAGENTA);

    krayon[0]->setColor(cocos2d::Color3B::RED);
    krayon[1]->setColor(cocos2d::Color3B::GREEN);
    krayon[2]->setColor(cocos2d::Color3B::BLUE);
    krayon[3]->setColor(cocos2d::Color3B::YELLOW);
    krayon[4]->setColor(cocos2d::Color3B::ORANGE);
    krayon[5]->setColor(cocos2d::Color3B::BLACK);
    krayon[6]->setColor(cocos2d::Color3B::MAGENTA);

    auto b_eraser = Button::create("gameplay2/b_eraser.png");
    b_eraser->setPosition(Vec2(visibleSize.width / 2 + origin.x + 700, visibleSize.height / 2 + origin.y + 400));
    this->addChild(b_eraser);
    b_eraser->setScale(1.1);
    b_eraser->setZoomScale(-0.1);
    b_eraser->addClickEventListener([=](Ref* Sender) {
        _brush->setColor(cocos2d::Color3B::WHITE);
        });

    return true;
}

bool nyoba::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    return  true;
}
void nyoba::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {
 
    auto start = Director::getInstance()->convertToGL(touch->getLocationInView());
    auto end = Director::getInstance()->convertToGL(touch->getPreviousLocationInView());

    _texture->begin();

    float distance = start.distance(end);

    for (int i = 0; i < distance; i++)
    {
        float difx = end.x - start.x;
        float dify = end.y - start.y;
        float delta = (float)i / distance;
        _brush->setPosition(
            Vec2(start.x + (difx * delta), start.y + (dify * delta)));
        //  CCLOG("drawPoint: x:%f y:%f",_brush->getPosition().x,_brush->getPosition().y);

        _brush->visit();
    }
    _texture->end();
}
void nyoba::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
}


void nyoba::afterCapture(bool succeed, const std::string& outputFile) {
    
        if ( succeed )
        {
            auto sprite = Sprite::create(outputFile);
            this->addChild(sprite);

            Size screenSize = Director::getInstance( )->getWinSize();
            sprite->setPosition(screenSize.width / 2, screenSize.height / 2);
            sprite->setScale(0.99);
        }
        else
        {
            log("Screen capture failed");
        }

}
