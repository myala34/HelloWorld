﻿#include "player.h"
#include "HelloWorldScene.h"



USING_NS_CC;
#define PTM_RATIO 32

player::player(CCLayer* gl)
{
	gameLayer = gl;
}


//player::player(void)
//{
//
//}

player::~player(void)
{
}

CCSprite* player::initPlayer()
{

	b2World* world = ((GameLayer*)gameLayer)->GetPhysicsWorld();

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	CCArray *animFrames = new CCArray(2);

	CCSpriteFrame *frame = cache->spriteFrameByName("monkey/walk/right_1.png");
	animFrames->addObject(frame);

	frame = cache->spriteFrameByName("monkey/walk/right_2.png");
	animFrames->addObject(frame);

	CCAnimation *animation = CCAnimation::animationWithSpriteFrames(animFrames, 0.1f);

	playerSprite = CCSprite::spriteWithSpriteFrameName("monkey/walk/right_1.png");
	playerSprite->setPosition(ccp(size.width/5, size.height/2));
	playerSprite->runAction(CCRepeatForever::actionWithAction(CCAnimate::actionWithAnimation(animation)));

	b2BodyDef playerBodyDef;
	playerBodyDef.type = b2_dynamicBody;
	playerBodyDef.position.Set(playerSprite->getPosition().x/PTM_RATIO, playerSprite->getPosition().y/PTM_RATIO);
	playerBodyDef.userData = playerSprite;

	playerBody = world->CreateBody(&playerBodyDef);

	b2PolygonShape playerShape;
	playerShape.SetAsBox(playerSprite->getContentSize().width/2/PTM_RATIO, playerSprite->getContentSize().height/2/PTM_RATIO);
	//monkeyShape.SetAsBox(0.5f, 0.5f);

	b2FixtureDef playerFixtureDef;
	playerFixtureDef.shape = &playerShape;
	playerFixtureDef.density = 1.0f;
	playerFixtureDef.friction = 0.3f;
	playerFixtureDef.restitution = 0.0f;
	m_playerFixture = playerBody->CreateFixture(&playerFixtureDef);

	return playerSprite;
}

void player::jump()
{
	printf("jump!");
	b2Vec2 impulse = b2Vec2(2.0f, 30.0f);
	playerBody->ApplyLinearImpulse(impulse, playerBody->GetWorldCenter());
}

b2Fixture* player::GetPlayerFixture()
{
	return m_playerFixture;
}
