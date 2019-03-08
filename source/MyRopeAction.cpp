#include "stdafx.h"
#include "myheader.h"

MyRopeAction::MyRopeAction()		// 생성자 
{
	timer = Ogre::Timer::Timer();
	start = timer.getMilliseconds();
	ropeLen = 0;
	angle = 0;
	angleVelo = 0;
	ang = 0;
	accel = 0;
	rope.x = 0;
	rope.y = 0;
	pos.x = 0;
	pos.y = 0;
	forceX = 0;
	forceY = 0;
	gravity = 1;
}

void MyRopeAction::ropeSetting(POSITION ropePos)
{
	rope = ropePos;
	ropeLen = abs(sqrt(pow(rope.x - dc->x, 2) + pow(rope.y - dc->y, 2)));
	pos.x = dc->x - rope.x;
	pos.y = dc->y - rope.y;
	angle = atan2(rope.y - dc->y, rope.x - dc->x) - M_PI / 2;
	accel = (-1.0 * (forceX + forceY) / ropeLen) * sin(angle);
	ropeFlag = true;
	//ropePoint->setVisible(true);
	dc->checkIndex = 26;
	mf->checkIndex = 0;
	dc->gravFlag = false;
}

void MyRopeAction::setting(DrawMyCharacter* dc_, MyFrameListener * mf_, MyTestApp* mApp_)
{
	dc = dc_;
	mf = mf_;
	mApp = mApp_;
}

void MyRopeAction::checkRope() {
	if (ropeFlag == false)
		ropeSetNull();
}

void MyRopeAction::ropeSetNull() {
	rope.x = NULL;
	rope.y = NULL;
	//ropePoint->setVisible(false);
	//mApp->scnMgr->destroySceneNode(mf->ropeNode);				// 삭제
	//mf->ropeNode = NULL;
}

bool MyRopeAction::frameStarted(const Ogre::FrameEvent& evt)
{
	checkRope();
	if (rope.x == NULL && rope.y == NULL) {		// 로프가 없을때 
		if (dc->y > dc->limitY && dc->gravFlag == true) {		// 바닥보다 위에 
			forceY -= gravity * 1.5;
		}
		else if (dc->y - dc->limitY <50) {			// 바닥에 도착
			forceX = 0;
			forceY = 0;
			dc->y = dc->limitY;
			dc->gravFlag = false;
		}
		dc->x += forceX;
		dc->y += forceY;
	}
	else {
		dc->gravFlag = true;				// 로프 운동 중에는 에이밍노드 가리기
		ang = angle;
		angleVelo = (-1.3 * gravity / ropeLen)*sin(angle);
		accel += angleVelo * 3;
		accel *= 0.999;

		ang += accel;
		if ((ang * 180 / M_PI) >= 90) {
			ropeSetNull();
		}
		else {
			angle = ang;
		}
		forceX = ropeLen * accel*cos(angle);
		forceY = ropeLen * accel*sin(angle);

		pos.x += forceX;
		pos.y += forceY;

		dc->x = pos.x + rope.x;
		dc->y = pos.y + rope.y;

		//ropePoint->setPosition(rope.x, rope.y, 100);
	}
	return true;
}
