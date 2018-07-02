#include "stdafx.h"
#include "myheader.h"

MyFrameListener::MyFrameListener()      // ������ 
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
	// ���� ���̹� Visible 
	if (dc->gravFlag == true || dc->y < dc->limitY) {
		amingNode->setVisible(false);
		amingNode->setPosition(dc->x, dc->y + 300, 100);
	}
	else {
		amingNode->setVisible(true);
		amingNode->translate(_mApp->dir);
	}


	if (amingNode->getPosition().y - dc->y >= 0)		// ���ֳ̹�尡 �� ���� ���� ��
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
		if (ropeNode->getPosition().y > _mApp->BlockScn[checkIndex]->getPosition().y - 50) {			// �� ������ y�� ���̰� �ǾƷ� ������ ������
			checkIndex++;											// ���� ������
			if (checkIndex == 27)									// ��� ���� �� �˻� ������
				rlimitY = 8000;										// ù��° ����� �� ������ y�� �ִ� ����
		}
		else {
			// �� ������ y�� ���̰� �ǾƷ� ������ ������
			if (ropeNode->getPosition().x >(_mApp->BlockScn[checkIndex]->getPosition().x - 150) &&
				ropeNode->getPosition().x < (_mApp->BlockScn[checkIndex]->getPosition().x + 150))		//	�� ������ x��ǥ�� �ǾƷ� �� �ȿ� ������
			{
				rlimitY = _mApp->BlockScn[checkIndex]->getPosition().y - 50;		// ���� ���� ���̰� �� ������ y�� �ִ�
				if (ropeNode->getPosition().y > rlimitY - 30) {
					rcoll = true;
					ropeNode->setPosition(ropeNode->getPosition().x, _mApp->BlockScn[checkIndex]->getPosition().y - 50, 100);
				}
			}
			else {													// �� ĳ������ x��ǥ�� ���� �� �ȿ� ������
				rlimitY = 8000;										// �� �������� �ö󰡶� 				
																	//gravFlag = true;										// �߷��� �ֱ�
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
	if (rcoll == false && ropeNode->getPosition().y < rlimitY)		// �浹 ���߰� �Ѱ躸�� ������ 
		ropeNode->translate(shotdir * 0.1);							// ���� ����
	if (timer.getMilliseconds() - ropeStart > 1500 || rcoll == true)			//�ð��� ������ �Ǵ� �浹������
	{
		if (rcoll == false || ropeNode->getPosition().y >= 8000) {	// �浹 �������� 
			_mApp->scnMgr->destroySceneNode(ropeNode);				// ����
			ropeNode = NULL;
		}
		else {	/*													// �浹 ������
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

	// ȸ���߽��� C�� ����  O�� ��ġ�ϵ��� ȸ���� �� T�� �Բ� �����̵�
	tx -= cx, ty -= cy;

	// ���� O�� ���Ͽ� ȸ���� �� T�� q���� ��ŭ ȸ��
	*nx = tx * cosq - ty * sinq;
	*ny = ty * cosq + tx * sinq;

	// ���� O�� ������ ȸ�� �߽��� C�� ��ġ�ϵ��� ȸ���� �� N�� �Բ� �̵�
	*nx += cx, *ny += cy;
}