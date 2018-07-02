#include "stdafx.h"
#include "myheader.h"

//! [constructor]
MyTestApp::MyTestApp() : OgreBites::ApplicationContext("OgreTutorialApp"), OgreBites::InputListener()
{

}
//! [constructor]

//! [key_handler]
bool MyTestApp::keyPressed(const OgreBites::KeyboardEvent& evt)
{

	if (evt.keysym.sym == SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}

	if (evt.keysym.sym == SDLK_LEFT)
	{
		dc->x -= 10;
		mf->amingNode->setPosition(dc->x, dc->y+300, 100);
	}

	if (evt.keysym.sym == SDLK_RIGHT)
	{
		dc->x += 10;
		mf->amingNode->setPosition(dc->x,dc->y+300,100);
	}

	if (evt.keysym.sym == SDLK_UP)		up = true;
	if (evt.keysym.sym == SDLK_DOWN)	down = true;

	if (evt.keysym.sym == SDLK_r)
	{
		if (ra->ropeFlag) {		//로프 있으면
			ra->ropeFlag = false;
		}
		else {
			POSITION pos;
			pos.x = dc->x - 550;
			pos.y = dc->y + 1000;
			ra->ropeSetting(pos);
		}
	}

	if (evt.keysym.sym == SDLK_l)
	{
		lightFlag = !lightFlag;
		spotlight->setVisible(lightFlag);
	}

	if (evt.keysym.sym == SDLK_k)
	{

	}

	if (evt.keysym.sym == SDLK_g)
	{
		mf->createBullet();
	}

	if (evt.keysym.sym == SDLK_f && !ra->ropeFlag )
	{
		mf->createRope();
	}

	return true;
}

bool MyTestApp::keyReleased(const OgreBites::KeyboardEvent& evt) {
	if (evt.keysym.sym == SDLK_UP && up) up = false;

	if (evt.keysym.sym == SDLK_DOWN && down) down = false;
	
	return true;
}

bool MyTestApp::mousePressed(const OgreBites::MouseButtonEvent& evt) {
	if (evt.button == SDL_BUTTON_LEFT) {
		Ogre::RaySceneQuery* mRaySceneQuery = scnMgr->createRayQuery(Ogre::Ray());
		float width = getRenderWindow()->getViewport(0)->getActualWidth();
		float height = getRenderWindow()->getViewport(0)->getActualHeight();
		float nomX = evt.x / width;
		float nomY = evt.y / height;
		Ogre::Ray cameraRay = cam->getCameraToViewportRay(nomX, nomY);
		mRaySceneQuery->setRay(cameraRay);
		Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();

		Ogre::RaySceneQueryResult::iterator iter = result.begin();
		//if (iter != result.end())
		//{
		if (iter->worldFragment)
		{
			exit(0);
		}
		else if (iter->movable)
		{
			iter->movable->setVisible(false);
		}
		//}

		scnMgr->destroyQuery(mRaySceneQuery);
	}
	return true;
}

//! [key_handler]

//! [setup]
void MyTestApp::setup(void)
{
	int i;
	char name[10];

	// do not forget to call the base first
	OgreBites::ApplicationContext::setup();

	// register for input events
	addInputListener(this);

	// get a pointer to the already created root
	root = getRoot();
	scnMgr = root->createSceneManager();

	mf = new MyFrameListener();
	MyAI* AI = new MyAI();
	AIFrameListener* AIf = new AIFrameListener();
	ra = new MyRopeAction();
	dc = new DrawMyCharacter();

	// register our scene with the RTSS
	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scnMgr);

	// finally something to render
	head1 = scnMgr->getRootSceneNode()->createChildSceneNode("head1");
	
	Ogre::Entity* ogreHead1;
	ogreHead1 = scnMgr->createEntity("ogrehead.mesh");
	ogreHead1->setCastShadows(true);

	head1->attachObject(ogreHead1);
	head1->setScale(Ogre::Vector3(2, 2, 2));
	head1->setPosition(-400, 4050, 100);

	for (i = 0; i <27; i++) {
		sprintf(name, "block%d", i + 1);
		BlockScn[i] = scnMgr->getRootSceneNode()->createChildSceneNode(name);
		Block[i] = scnMgr->createEntity("cube.mesh");
		Block[i]->setCastShadows(false);
		Block[i]->setMaterialName("Examples/Rockwall");
		BlockScn[i]->attachObject(Block[i]);
		BlockScn[i]->setScale(2.5, 0.5, 2.5);
	}

	BlockScn[0]->setPosition(-1000, 250, 0);
	BlockScn[1]->setPosition(-500, 500, 0);
	BlockScn[2]->setPosition(0, 750, 0);
	BlockScn[3]->setPosition(500, 1000, 0);
	BlockScn[4]->setPosition(1000, 1250, 0);
	BlockScn[5]->setPosition(400, 1650, 0);
	BlockScn[6]->setPosition(-600, 1850, 0);
	BlockScn[7]->setPosition(1100, 2000, 0);
	BlockScn[8]->setPosition(0, 2200, 0);
	BlockScn[9]->setPosition(-1150, 2250, 0);
	BlockScn[10]->setPosition(750, 2350, 0);
	BlockScn[11]->setPosition(-400, 2500, 0);
	BlockScn[12]->setPosition(-750, 2800, 0);
	BlockScn[13]->setPosition(1100, 2900, 0);
	BlockScn[14]->setPosition(450, 3050, 0);
	BlockScn[15]->setPosition(-50, 3300, 0);
	BlockScn[16]->setPosition(-1100, 3400, 0);
	BlockScn[17]->setPosition(-550, 3550, 0);
	BlockScn[18]->setPosition(800, 3700, 0);
	BlockScn[19]->setPosition(-900, 3800, 0);
	BlockScn[20]->setPosition(0, 4000, 0);
	BlockScn[21]->setPosition(-400, 4400, 0);
	BlockScn[22]->setPosition(1050, 4800, 0);
	BlockScn[23]->setPosition(250, 5300, 0);
	BlockScn[24]->setPosition(-150, 5800, 0);
	BlockScn[25]->setPosition(900, 6600, 0);		//goal
	BlockScn[25]->setScale(3.5, 0.5, 2.5);
	BlockScn[26]->setPosition(400, 7000, 0);


	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 4000, 500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* groundE = scnMgr->createEntity("ground");
	scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundE);

	groundE->setCastShadows(false);
	groundE->setMaterialName("Examples/Rockwall");

	// without light we would just get a black screen    
	spotlight = scnMgr->createLight("SpotLight");
	spotlight->setDiffuseColour(1.0, 1.0, 1.0);
	spotlight->setSpecularColour(1.0, 1.0, 1.0);
	spotlight->setType(Ogre::Light::LT_SPOTLIGHT);
	spotlight->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);

	Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(spotlight);
	lightNode->setDirection(Ogre::Vector3(0, 0, -1));
	lightNode->setPosition(Ogre::Vector3(0, 1200, 5000));

	spotlight->setSpotlightRange(Ogre::Degree(1000), Ogre::Degree(1000));

	// second light
	spotlight2 = scnMgr->createLight("SpotLight2");
	spotlight2->setDiffuseColour(1.0, 1.0, 1.0);
	spotlight2->setSpecularColour(1.0, 1.0, 1.0);
	spotlight2->setType(Ogre::Light::LT_SPOTLIGHT);
	spotlight2->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);

	Ogre::SceneNode* lightNode2 = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode2->attachObject(spotlight2);
	lightNode2->setDirection(Ogre::Vector3(0, 0, 0));
	lightNode2->setPosition(Ogre::Vector3(-3000, 14000, 0));

	spotlight2->setSpotlightRange(Ogre::Degree(1000), Ogre::Degree(1000));

	// also need to tell where we are
	camNode = scnMgr->getRootSceneNode()->createChildSceneNode();

	// create the camera
	cam = scnMgr->createCamera("myCam");
	cam->setNearClipDistance(5); // specific to this sample
	camNode->attachObject(cam);

	dc->setting(head1, BlockScn, camNode);
	ra->setting(dc, mf,this);
	mf->setting(this, dc,ra);
	AI->AIsetting(this);
	AIf->AIFLsetting(this, AI);

	scnMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);


	root->addFrameListener(dc);
	root->addFrameListener(ra);
	root->addFrameListener(mf);
	root->addFrameListener(AIf);
	
	// and tell it to render into the main window
	getRenderWindow()->addViewport(cam);

}
//! [setup]