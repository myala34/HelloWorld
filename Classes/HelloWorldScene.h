#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GLES-Render.h"
#include "player.h"
#include "MyContactListener.h"

class GameLayer : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    LAYER_CREATE_FUNC(GameLayer);

	void initPhysics();

	void update(float dt);

	void draw();

	void virtual ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *pEvent);
	void virtual ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *pEvent);



	b2World* GetPhysicsWorld();

private:
	//CCSprite* backgroud;
	b2World* world;
	b2Fixture* m_bottomFixture;
	b2Fixture* m_playerFixture;
	GLESDebugDraw * m_debugDraw;
	player* m_player;
	MyContactListener* m_contactListener;
	bool m_canJump;
};

#endif // __HELLOWORLD_SCENE_H__
