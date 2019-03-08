#include "stdafx.h"
#include "myheader.h"

MyAI::MyAI()
{

}


void MyAI::AIsetting(MyTestApp* mApp)
{
	_mApp = mApp;

	enemyEntity = _mApp->scnMgr->createEntity("ogrehead.mesh");
	enemyEntity->setCastShadows(true);

	towerEntity = _mApp->scnMgr->createEntity("ogrehead.mesh");
	towerEntity->setCastShadows(true);

	enemyNode = _mApp->scnMgr->getRootSceneNode()->createChildSceneNode("enemyNode");
	enemyNode->attachObject(enemyEntity);
	enemyNode->setScale(Ogre::Vector3(2, 2, 2));
	enemyNode->setPosition(_mApp->BlockScn[1]->getPosition().x, (_mApp->BlockScn[1]->getPosition().y) + 75, 0);

	towerNode = _mApp->scnMgr->getRootSceneNode()->createChildSceneNode("towerNode");
	towerNode->attachObject(towerEntity);
	towerNode->setScale(Ogre::Vector3(2, 2, 2));
	towerNode->setPosition(_mApp->BlockScn[3]->getPosition().x, (_mApp->BlockScn[3]->getPosition().y) + 75, 0);


}