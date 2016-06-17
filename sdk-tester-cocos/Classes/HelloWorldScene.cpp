#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
using namespace std;
#include "HelloWorldScene.h"
#include "IvySDK.h"

USING_NS_CC;

/////////////////////////////////////////////////////////////

//计费点要与平台给的ID相对应
#define BILLING_ID_ACTIVE_GAME 1

static void onPaymentSuccess(int billId) {
	switch (billId) {
	case BILLING_ID_ACTIVE_GAME:
		CCLOG("game actived!");
		break;
	}
}

void HelloWorld::onPaymentResult(int resultCode, int billingId) {
    CCLOG("billing %i failure code %i", billingId, resultCode);
	switch (resultCode) {
	case IvySDK::PAYMENT_RESULT_SUCCESS:
		onPaymentSuccess(billingId);
		break;

	default:
		CCLOG("billing %i result code %i", billingId, resultCode);
		break;
	}
}

void onFreecoinResult(int coins) {
	CCLOG("I get %i coins", coins);
}

void onReceiveSNSResult(int resultType, bool success, int extra) {
    switch(resultType) {
        case IvySDK::SNS_RESULT_LOGIN:
            if (success) {
                // now do your login logic, get profile, friends etc.
                const char* mestring = IvySDK::me();
                CCLOG("me string is %s", mestring);
            }
            break;
            
        case IvySDK::SNS_RESULT_LIKE:
            if (success) {
                CCLOG("thank you for like us.");
            }
            break;
            
        case IvySDK::SNS_RESULT_INVITE:
            if (success) {
                CCLOG("thank you invite your friends. you will receive 10 golds.");
            }
            break;
            
        case IvySDK::SNS_RESULT_CHALLENGE:
            if (extra > 3) {
                CCLOG("thank you challenge 3 friends, you will receive 10 golds.");
            }
            break;
    }
}

/////////////////////////////////////////////////////////

Scene* HelloWorld::scene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}
	
	CCMenuItemFont::setFontSize(11);
	IvySDK::registerPaymentCallback(onPaymentResult);
	IvySDK::registerFreecoinCallback(onFreecoinResult);
    IvySDK::registerSNSCallback(onReceiveSNSResult);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto closeItem = MenuItemImage::create("CloseNormal.png",
			"CloseSelected.png",
			CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(
			origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	///////////////////////////////////////////////////////////////////////////////

	float py = origin.y + 250;
	float px = origin.x + 40;

	ccMenuCallback handler = CC_CALLBACK_1(HelloWorld::AdButtonClicked, this);
    const int label_size = 21;
    const char* labels[label_size] = {
        "Pause AD", // 1
        "PassLevel",// 2
        "Custom", // 3
        "Exit AD",// 4
        "Banner",// 5
        "Close Banner",// 6
        "Share", // 7
        "MoreGame", // 8
        "Freecoin", // 9
        "Pay", // 10
        "Get Data", // 11
        "Track Event", // 12
        "Rate", //13
        "login", // 14
        "logout", // 15
        "isLogin",// 16
        "Like", // 17
        "Challenge", // 18
        "Me", // 19
        "Friends", // 20
        "Invite" // 21
    };
    
    int xOffset = 0;
    int yOffset = 0;
    
    for(int i=0; i<label_size; ++i) {
        CCMenuItemFont *laber1 = CCMenuItemFont::create(labels[i], handler);
        laber1->setPosition(ccp(px + xOffset, py - yOffset));
        laber1->setTag(i+1);
        CCMenu *menu1 = CCMenu::create(laber1, NULL);
        menu1->setPosition(CCPointZero);
        addChild(menu1, 3);
        
        yOffset += 40;
        if (yOffset > 200) {
            xOffset += 100;
            yOffset = 0;
        }
    }
    
	return true;
}

void HelloWorld::AdButtonClicked(Ref* ref) {
	int tag = static_cast<CCMenu*>(ref)->getTag();
	switch (tag) {
	case 1:// Show Pause Ad
		IvySDK::showInterstitial(IvySDK::AD_POS_GAME_PAUSE);
		break;
	case 2:// Show Passlevel Ad
		IvySDK::showInterstitial(IvySDK::AD_POS_GAME_PASSLEVEL);
		break;
	case 3:// Show Custom Ad
		IvySDK::showInterstitial(IvySDK::AD_POS_GAME_CUSTOM);
		break;
	case 4:// Show Exit Ad
		IvySDK::onQuit();
		break;
	case 5:// Show Banner
		IvySDK::showBanner(IvySDK::AD_POS_MIDDLE_TOP);
		break;
	case 6:// Close Banner
		IvySDK::closeBanner();
		break;
	case 7:// Share
		IvySDK::share();
		break;
	case 8:// More Game
		IvySDK::showMoreGame();
		break;
	case 9:// Show Free Coin
		IvySDK::showFreeCoin(1);
		break;
	case 10:// Do billing
		IvySDK::doBilling(BILLING_ID_ACTIVE_GAME);
		break;
            
        case 11:
            CCLOG("extra data: %s", IvySDK::getExtraData());
            break;
            
        case 12:
            IvySDK::trackEvent("test category", "test action", "test label", 3);
            break;
            
        case 13:
            IvySDK::rateUs();
            break;
            
        case 14:
            IvySDK::login();
            break;
            
        case 15:
            IvySDK::logout();
            break;
            
        case 16:
            CCLOG("isLogin: %s", IvySDK::isLogin() ? "true": "false");
            break;
            
        case 17:
            IvySDK::like();
            break;
            
        case 18:
            IvySDK::challenge("wa challenge", "speed come with me");
            break;
            
        case 19:
            CCLOG("me is %s", IvySDK::me());
            break;
            
        case 20:
            CCLOG("friends are: %s", IvySDK::friends());
            break;
            
        case 21:
            IvySDK::invite();
            break;
            
	default:
		break;
	}
}

void HelloWorld::menuCloseCallback(Ref* sender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox(
			"You pressed the close button. Windows Store Apps do not implement a close button.",
			"Alert");
	return;
#endif
	IvySDK::onQuit();
	//Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
