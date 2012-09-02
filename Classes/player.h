#pragma once

#include "cocos2d.h"
#include "Box2D/Box2D.h"

class player
{
public:
	player(cocos2d::CCLayer* gl);
	~player(void);

	cocos2d::CCSprite* initPlayer(void);

	b2Fixture* GetPlayerFixture();

	void jump(void);

	void walk(void);

private:
	cocos2d::CCLayer* gameLayer;

	cocos2d::CCSprite* playerSprite;

	b2Body* playerBody;

	b2Fixture* m_playerFixture;

	cocos2d::CCAnimation* walkAnimation;

	cocos2d::CCAnimation* jumpAnimation;

};

