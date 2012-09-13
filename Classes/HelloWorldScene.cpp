#include "HelloWorldScene.h"

#define PTM_RATIO 32



USING_NS_CC;


enum 
{
	kTagPlayer,
	kTagNode,
	kTagGrossini,
	kTagFloor,
};

void GameLayer::initPhysics()
{

	CCSize s = CCDirector::sharedDirector()->getWinSize();

	b2Vec2 gravity;
	gravity.Set(0.0f, -15.0f);
	world = new b2World(gravity);

	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // bottom-left corner

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2EdgeShape groundBox;

	// bottom

	//groundBox.Set(b2Vec2(0,0), b2Vec2(s.width/PTM_RATIO,0));
	//m_bottomFixture = groundBody->CreateFixture(&groundBox,0);

	// top
	groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
	groundBody->CreateFixture(&groundBox,0);

	// left
	groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
	groundBody->CreateFixture(&groundBox,0);

	// right
	//groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
	//groundBody->CreateFixture(&groundBox,0);

	m_debugDraw = new GLESDebugDraw(PTM_RATIO);
	world->SetDebugDraw(m_debugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	m_debugDraw->SetFlags(flags);

}


CCScene* GameLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameLayer *layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void GameLayer::initBackground()
{
	CCTileMapAtlas* tilemap = CCTileMapAtlas::create(s_TilesPng, s_LevelMapTga, 16, 16);
	tilemap->releaseMap();

	tilemap->setAnchorPoint(ccp(0,0));
	tilemap->getTexture()->setAliasTexParameters();

	CCSprite* background = CCSprite::create(s_back);
	background->setScale(1.5f);
	background->setAnchorPoint(ccp(0,0));

	CCParallaxNode* voidNode = CCParallaxNode::create();

	voidNode->addChild(background, -1, ccp(0.4f, 0.5f), CCPointZero);
	voidNode->addChild(tilemap, 1, ccp(1.0f, 1.0f), ccp(0, -200));

	this->addChild(voidNode, 0, kTagNode);
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	this->m_canJump = true;

	this->initPhysics();

	this->m_contactListener = new MyContactListener();
	this->world->SetContactListener(m_contactListener);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(GameLayer::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 10);

    /////////////////////////////
    // 3. add your codes below...

    //// add a label shows "Hello World"
    //// create and initialize a label
    //CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    //// ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    //// position the label on the center of the screen
    //pLabel->setPosition( ccp(size.width / 2, size.height - 50) );

    //// add the label as a child to this layer
    //this->addChild(pLabel, 1);

    //// add "GameLayer" splash screen"
    //CCSprite* pSprite = CCSprite::create("GameLayer.png");

    //// position the sprite on the center of the screen
    //pSprite->setPosition( ccp(size.width/2, size.height/2) );

    //// add the sprite as a child to this layer
    //this->addChild(pSprite, 0);
    
	this->initBackground();

	GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("floor_test.plist");

	CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	//cache->addSpriteFramesWithFile("background.plist");
	cache->addSpriteFramesWithFile("jungle.plist");
    cache->addSpriteFramesWithFile("Player.plist");
	cache->addSpriteFramesWithFile("floor.plist");


	CCSprite* floor1 = CCSprite::spriteWithSpriteFrameName("tile1.png");
	this->addChild(floor1, 1, kTagFloor);
	b2BodyDef floor1bodyDef;
	b2Body* floor1Body = world->CreateBody(&floor1bodyDef);

	GB2ShapeCache::sharedGB2ShapeCache()->addFixturesToBody(floor1Body, "tile1");

	//CCSprite* backgroud = CCSprite::spriteWithSpriteFrameName("jungle.png");
	//this->addChild(backgroud, 0);
	//backgroud->setAnchorPoint(CCPointZero);
	//backgroud->setPosition(CCPointZero);


	//CCSprite* floorBackground = CCSprite::spriteWithSpriteFrameName("floor/grassbehind.png");
	//this->addChild(floorBackground, 1, kTagFloor);
	//floorBackground->setAnchorPoint(CCPointZero);
	//b2Body* floorBody = addPhysicsObject(floorBackground, b2_staticBody);
	//floorBackground->setPosition(CCPointZero);




	m_player = new player(this);

	CCSprite* newPlayerSprite = m_player->initPlayer();

	m_playerFixture = m_player->GetPlayerFixture();


	this->addChild(newPlayerSprite, 1, kTagPlayer);

	this->setTouchEnabled(true);

	this->scheduleUpdate();
    return true;
}

void GameLayer::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

	//delete m_playerFixture;
	//delete m_bottomFixture;
	delete m_contactListener;
	delete m_player;	
	delete world;
	delete m_debugDraw;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameLayer::update(float dt)
{
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/

	int velocityIterations = 10;
	int positionIterations = 10;

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	world->Step(dt, velocityIterations, positionIterations);

	for(b2Body *b = world->GetBodyList(); b; b=b->GetNext()) {
		if (b->GetUserData() != NULL) {
			CCSprite *sprite = (CCSprite *)b->GetUserData();
			CCPoint spritePrevPosition = sprite->getPosition();
			sprite->setPosition(ccp(b->GetPosition().x * PTM_RATIO,
				b->GetPosition().y * PTM_RATIO));
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));

			if (sprite->getTag() == kTagPlayer)
			{
				CCPoint diff = ccpSub(sprite->getPosition(), spritePrevPosition);

				CCNode* backgroundNode = getChildByTag(kTagNode);
				CCPoint currentPos = backgroundNode->getPosition();
				CCPoint newPos = ccp(currentPos.x + diff.x, currentPos.y);
				backgroundNode->setPosition(newPos);

				//CCPoint mainLayerPos = this->getPosition();
				//this->setPosition(ccpSub(mainLayerPos, diff));
			}
		}
	}

	std::vector<MyContact>::iterator pos;
	for (pos = m_contactListener->_contacts.begin(); pos != m_contactListener->_contacts.end(); ++pos)
	{
		MyContact contact = *pos;

		CCSprite* spriteA = (CCSprite *)contact.fixtureA->GetBody()->GetUserData();
		CCSprite* spriteB = (CCSprite *)contact.fixtureB->GetBody()->GetUserData();

		if(spriteA != NULL && spriteB != NULL)
		{

		
		if((spriteA->getTag() == kTagFloor && spriteB->getTag() == kTagPlayer) ||
			(spriteA->getTag() == kTagPlayer && spriteB->getTag() == kTagFloor))
		//if ((contact.fixtureA == m_bottomFixture && contact.fixtureB == m_playerFixture) ||
		//	(contact.fixtureA == m_playerFixture && contact.fixtureB == m_bottomFixture))
		{
			//CCLOG("down !");
			if (m_canJump == false)
			{
				m_canJump = true;
				m_player->walk();
			}

		}
		}
		
	}
}


void GameLayer::ccTouchesBegan(CCSet *touches, CCEvent *pEvent)
{
	CCSetIterator it;
	CCTouch* touch;

	for (it = touches->begin(); it != touches->end(); it++)
	{
		touch = (CCTouch*)(*it);

		if (touch == NULL)
		{
			break;
		}

		CCPoint location = touch->locationInView();

		location = CCDirector::sharedDirector()->convertToGL(location);

		if(m_canJump == true)
		{
			m_canJump = false;
			m_player->jump();
		}
	}
}

void GameLayer::ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *pEvent)
{
	//CCSetIterator it;
	//CCTouch* touch;

	//for (it = touches->begin(); it != touches->end(); it++)
	//{
	//	touch = (CCTouch*)(*it);

	//	if (touch == NULL)
	//	{
	//		break;
	//	}

	//	CCPoint location = touch->locationInView();

	//	location = CCDirector::sharedDirector()->convertToGL(location);

	//}

}

b2World* GameLayer::GetPhysicsWorld()
{
	return world;
}

b2Body* GameLayer::addPhysicsObject(cocos2d::CCSprite* ObjectSprite, b2BodyType type)
{
	CCPoint initPos = ccp(0.0f, 0.0f);
	ObjectSprite->setPosition(initPos);
	b2Body* newObjectBody = NULL;
	b2BodyDef newObjectBodyDef;
	newObjectBodyDef.type = type;
	newObjectBodyDef.position.Set(initPos.x, initPos.y);
	newObjectBodyDef.userData = ObjectSprite;
	newObjectBody = world->CreateBody(&newObjectBodyDef);



	b2PolygonShape newObjectShape;
	newObjectShape.SetAsBox(ObjectSprite->getContentSize().width/PTM_RATIO/2, ObjectSprite->getContentSize().height/PTM_RATIO/2);

	b2FixtureDef newObjectFixtureDef;
	newObjectFixtureDef.shape = &newObjectShape;
	//newObjectFixtureDef.density = 10.0f;
	//newObjectFixtureDef.friction = 1.0f;
	//newObjectFixtureDef.restitution = 0.0f;
	newObjectBody->CreateFixture(&newObjectFixtureDef);

	return newObjectBody;
}

void GameLayer::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    world->DrawDebugData();
    
    kmGLPopMatrix();
}

