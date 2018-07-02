#include "stdafx.h"
#include "myheader.h"

MyFrameListener::MyFrameListener()      // 생성자 
{
	//y = 1;
	timer = Ogre::Timer::Timer();
	start = timer.getMilliseconds();
	//ropeNode = NULL;
}

void MyFrameListener::setting(MyTestApp* mApp, DrawMyCharacter* dc_, MyRopeAction* ra_)
{
	_mApp = mApp;
	dc = dc_;
	ra = ra_;

	amingEntity = _mApp->scnMgr->createEntity("sphere.mesh");
	amingEntity->setCastShadows(true);

	amingNode = _mApp->scnMgr->getRootSceneNode()->createChildSceneNode("amingNode");
	amingNode->attachObject(amingEntity);
	amingNode->setScale(Ogre::Vector3(0.1, 0.1, 0.1));
	amingNode->setPosition(dc->x, dc->y + 300, 0);
	shotpos = amingNode->getPosition();
}


bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
	// 로프 에이밍 Visible 
	if (dc->gravFlag == true || dc->y < dc->limitY) {
		amingNode->setVisible(false);
		amingNode->setPosition(dc->x, dc->y + 300, 100);
	}
	else {
		amingNode->setVisible(true);
		amingNode->translate(_mApp->dir);
	}


	if (amingNode->getPosition().y - dc->y >= 0)		// 에이밍노드가 내 위에 있을 때
	{
		if (_mApp->up)
		{
			roate(&shotpos.x, &shotpos.y, amingNode->getPosition().x, amingNode->getPosition().y, dc->x, dc->y, 5);
			amingNode->setPosition(shotpos.x, shotpos.y, 0);
		}
		else if (_mApp->down)
		{
			roate(&shotpos.x, &shotpos.y, amingNode->getPosition().x, amingNode->getPosition().y, dc->x, dc->y, -5);
			amingNode->setPosition(shotpos.x, shotpos.y, 0);
		}
	}
	else
		amingNode->translate(0, 3, 0);

	fireBullet(evt.timeSinceLastFrame);

	if (ropeNode) {
		fireRope(evt.timeSinceLastFrame);
		ropeCollision();
	}

	return true;
}

void MyFrameListener::createBullet()
{

	S_BULLET *pNewBullet = new S_BULLET;
	memset(pNewBullet, 0, sizeof(S_BULLET));

	char szId[32] = "";
	sprintf_s(szId, 32, "ID:%d", (int)pNewBullet);

	pNewBullet->pEntity = _mApp->scnMgr->createEntity(szId, "sphere.mesh");
	pNewBullet->pSceneNode = _mApp->scnMgr->getRootSceneNode()->createChildSceneNode();
	pNewBullet->pSceneNode->attachObject(pNewBullet->pEntity);
	pNewBullet->pSceneNode->setScale(Ogre::Vector3(0.1, 0.1, 0.1));
	pNewBullet->pSceneNode->setPosition(dc->x, dc->y, 0);
	pNewBullet->fTime = 30.0f;
	pNewBullet->vDir = (amingNode->getPosition() - dc->head->getPosition()) * 0.2;
	//pNewBullet->fV = 10000.0f;
	pNewBullet->vBpos = pNewBullet->pSceneNode->getPosition();

	m_listBullet.push_back(pNewBullet);
}

void MyFrameListener::ropeCollision() {
	if (checkIndex<27) {
		if (ropeNode->getPosition().y > _mApp->BlockScn[checkIndex]->getPosition().y - 50) {			// 내 로프의 y의 높이가 맨아래 블럭보다 높으면
			checkIndex++;											// 다음 블럭으로
			if (checkIndex == 27)									// 모든 블럭을 다 검사 했으면
				rlimitY = 8000;										// 첫번째 블록이 내 로프의 y의 최대 높이
		}
		else {
			// 내 로프의 y의 높이가 맨아래 블럭보다 낮으면
			if (ropeNode->getPosition().x >(_mApp->BlockScn[checkIndex]->getPosition().x - 150) &&
				ropeNode->getPosition().x < (_mApp->BlockScn[checkIndex]->getPosition().x + 150))		//	내 로프의 x좌표가 맨아래 블럭 안에 있으면
			{
				rlimitY = _mApp->BlockScn[checkIndex]->getPosition().y - 50;		// 현재 블럭의 높이가 내 로프의 y의 최대
				if (ropeNode->getPosition().y > rlimitY - 30) {
					rcoll = true;
					ropeNode->setPosition(ropeNode->getPosition().x, _mApp->BlockScn[checkIndex]->getPosition().y - 50, 100);
				}
			}
			else {													// 내 캐릭터의 x좌표가 맨위 블럭 안에 없으면
				rlimitY = 8000;										// 맨 위블럭까지 올라가라 				
																	//gravFlag = true;										// 중력을 주기
			}
		}
	}
}

void MyFrameListener::fireBullet(float fTime)
{
	std::list<S_BULLET *>::iterator itor = m_listBullet.begin();
	while (itor != m_listBullet.end())
	{
		(*itor)->pSceneNode->translate((*itor)->vDir);
		(*itor)->fTime -= fTime;
		itor++;
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

void MyFrameListener::createRope()
{
	if (ropeNode)
	{
		_mApp->scnMgr->destroySceneNode(ropeNode);
	}
	ropeEntity = _mApp->scnMgr->createEntity("sphere.mesh");
	ropeNode = _mApp->scnMgr->getRootSceneNode()->createChildSceneNode("ropeNode");
	ropeNode->attachObject(ropeEntity);
	ropeNode->setScale(Ogre::Vector3(0.1, 0.1, 0.1));
	ropeNode->setPosition(dc->x, dc->y, 0);
	ropeStart = timer.getMilliseconds();
	shotdir = amingNode->getPosition() - dc->head->getPosition();

	rcoll = false;
	checkIndex = 0;
	rlimitY = 8000;
}

void MyFrameListener::fireRope(float fTime)
{
	if (rcoll == false && ropeNode->getPosition().y < rlimitY)		// 충돌 안했고 한계보다 작으면 
		ropeNode->translate(shotdir * 0.1);							// 로프 진행
	if (timer.getMilliseconds() - ropeStart > 1500 || rcoll == true)			//시간이 지나면 또는 충돌했으면
	{
		if (rcoll == false || ropeNode->getPosition().y >= 8000) {	// 충돌 안했으면 
			_mApp->scnMgr->destroySceneNode(ropeNode);				// 삭제
			ropeNode = NULL;
		}
		else {	/*													// 충돌 했으면
			POSITION pos;
			pos.x = ropeNode->getPosition().x;
			pos.y = ropeNode->getPosition().y;
			ra->ropeSetting(pos);*/
		}
	}


}

bool MyFrameListener::collisionArea(Ogre::Vector3 &vPos)
{
	bool fFlag = false;


	if (vPos[0] < -2000.0f)
		fFlag = true;

	if (vPos[0] > 2000.0f)
		fFlag = true;

	if (vPos[1] < 0.0f)
		fFlag = true;

	if (vPos[1] > 8000.0f)
		fFlag = true;

	return fFlag;
}

void MyFrameListener::roate(float* nx, float* ny, float tx, float ty, float cx, float cy, float q)
{
	q = q * M_PI / 180;
	float cosq = cos(q), sinq = sin(q);

	// 회전중심점 C가 원점  O와 일치하도록 회전할 점 T를 함께 평행이동
	tx -= cx, ty -= cy;

	// 원점 O에 대하여 회전할 점 T를 q라디안 만큼 회전
	*nx = tx * cosq - ty * sinq;
	*ny = ty * cosq + tx * sinq;

	// 원점 O가 원래의 회전 중심점 C와 일치하도록 회전된 점 N과 함께 이동
	*nx += cx, *ny += cy;
}