#include "stdafx.h"
#include "myheader.h"

AIFrameListener::AIFrameListener()
{
	vRight = Ogre::Vector3(1, 0, 0);
	right = true;
	start = timer.getMilliseconds();
}

void AIFrameListener::AIFLsetting(MyTestApp* mApp, MyAI* mAI)
{
	_mApp = mApp;
	_mAI = mAI;
	_enemyNode = _mAI->enemyNode;
	xRange = _enemyNode->getPosition().x;

}

bool AIFrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
	enemyMoving();
	if (timer.getMilliseconds() - start > 3000)
	{
		createBullet();
		start = timer.getMilliseconds();
	}
	fireBullet(evt.timeSinceLastFrame);
	return true;
}

void AIFrameListener::enemyMoving()
{
	if (right)
		_enemyNode->translate(vRight);
	else if (_enemyNode->getPosition().x <= xRange - WIDTH)
	{
		_enemyNode->translate(vRight);
		right = true;
	}
	if (_enemyNode->getPosition().x >= xRange + WIDTH || !right)
	{
		_enemyNode->translate(-vRight);
		right = false;
	}
}

void AIFrameListener::createBullet()
{
	S_BULLET *pNewBullet = new S_BULLET;
	memset(pNewBullet, 0, sizeof(S_BULLET));
	char szId[32] = "";
	sprintf_s(szId, 32, "ID:%d", (int)pNewBullet);
	pNewBullet->pEntity = _mApp->scnMgr->createEntity(szId, "sphere.mesh");
	pNewBullet->pSceneNode = _mApp->scnMgr->getRootSceneNode()->createChildSceneNode();
	pNewBullet->pSceneNode->attachObject(pNewBullet->pEntity);
	pNewBullet->pSceneNode->setScale(Ogre::Vector3(0.3, 0.3, 0.3));
	pNewBullet->pSceneNode->setPosition(_mAI->towerNode->getPosition());
	pNewBullet->fTime = 30.0f;
	pNewBullet->vDir = Ogre::Vector3(-10, 0, 0);
	//pNewBullet->fV = 10000.0f;
	pNewBullet->vBpos = pNewBullet->pSceneNode->getPosition();

	m_listBullet.push_back(pNewBullet);
}

void AIFrameListener::fireBullet(float fTime)
{
	std::list<S_BULLET *>::iterator itor = m_listBullet.begin();
	//int i = 1;
	while (itor != m_listBullet.end())
	{
		(*itor)->pSceneNode->translate((*itor)->vDir);
		(*itor)->fTime -= fTime;
		itor++;
		//i *= -1;
	}
	Ogre::Vector3 vTmp(0.0f, 0.0f, 0.0f);
	itor = m_listBullet.begin();

	while (itor != m_listBullet.end())
	{
		vTmp = (*itor)->pSceneNode->getPosition();
		if (collisionArea(vTmp)) //|| (*itor)->fTime < 0.0f)
		{
			_mApp->scnMgr->destroyEntity((*itor)->pEntity);
			_mApp->scnMgr->destroySceneNode((*itor)->pSceneNode);
			itor = m_listBullet.erase(itor);
		}

		else
			itor++;
	}

}


bool AIFrameListener::collisionArea(Ogre::Vector3 &vPos)
{
	bool fFlag = false;

	if (vPos[0] < -2000.0f)
		fFlag = true;

	return fFlag;
}