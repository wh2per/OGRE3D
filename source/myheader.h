#pragma once
#ifndef MYHEADER_H__
#define MYHEADER_H__

#include "resource.h"

#include <Ogre.h>
#include <Bites/OgreApplicationContext.h>
#include <OgreAxisAlignedBox.h>
#include <SDL.h>
#include <iostream>
#include <cmath>
#include <Windows.h>
#include <OgreTimer.h>

typedef struct pos {
	float x;
	float y;
}POSITION;

class MyFrameListener;
class MyTestApp;

class DrawMyCharacter : public Ogre::FrameListener {
public:
	Ogre::SceneNode* head;
	Ogre::SceneNode* camNode;
	Ogre::SceneNode* block[27];
	float x;
	float y;

	bool gravFlag;

	float limitY;
	int checkIndex;

	DrawMyCharacter();
	bool frameStarted(const Ogre::FrameEvent& evt);
	void setting(Ogre::SceneNode* head_, Ogre::SceneNode* BlockScn[],Ogre::SceneNode* camNode_);
	void collision();
};

class MyRopeAction : public Ogre::FrameListener
{
public:
	DrawMyCharacter * dc;
	MyFrameListener * mf;
	MyTestApp * mApp;
	Ogre::Timer timer;
	Ogre::SceneNode* ropePoint;
	unsigned long start;
	bool stopFlag = true;
	float gravity;
	float forceX;
	float forceY;
	POSITION rope;
	POSITION pos;	//로프와 상대위치
	float ropeLen;
	float angle;
	float ang;
	float angleVelo;
	float accel;

	bool ropeFlag = false;

	MyRopeAction();
	void setting(DrawMyCharacter* dc_, MyFrameListener * mf_, MyTestApp* mApp);
	void ropeSetting(POSITION ropePos);
	void ropeSetNull();
	void checkRope();
	bool frameStarted(const Ogre::FrameEvent& evt);
};

class MyTestApp : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
private:
	MyFrameListener * mf;
public:
	MyTestApp();
	void setup();
	bool keyPressed(const OgreBites::KeyboardEvent& evt);
	bool keyReleased(const OgreBites::KeyboardEvent& evt);
	bool mouseMoved(const OgreBites::MouseMotionEvent& evt) { return true; }
	bool mousePressed(const OgreBites::MouseButtonEvent& evt);
	bool mouseReleased(const OgreBites::MouseButtonEvent& evt) { return true; }
	bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt) { return true; }
	bool touchMoved(const OgreBites::TouchFingerEvent& evt) { return true; }
	bool touchPressed(const OgreBites::TouchFingerEvent& evt) { return true; }
	bool touchReleased(const OgreBites::TouchFingerEvent& evt) { return true; }
	Ogre::SceneNode* head1;
	Ogre::SceneNode* ropePoint;
	Ogre::Root* root;
	Ogre::SceneManager* scnMgr;
	Ogre::Camera* cam;
	Ogre::SceneNode* camNode;

	Ogre::Light* spotlight;
	Ogre::Light* spotlight2;
	bool lightFlag = true;
	bool up = false;
	bool down = false;
	int directionX, directionY = 0;

	Ogre::Entity* Block[27];
	Ogre::SceneNode* BlockScn[27];

	Ogre::Vector3 dir;
	Ogre::Vector3 shotdir;
	bool sJump = false;

	DrawMyCharacter* dc;
	MyRopeAction* ra;
};


class MyFrameListener : public Ogre::FrameListener
{
private:
	struct S_BULLET //총알 구조체
	{
		Ogre::Entity *pEntity;
		Ogre::SceneNode *pSceneNode;
		float fTime;
		Ogre::Vector3 vDir;
		float fV;
		Ogre::Vector3 vBpos;
	};
public:
	MyTestApp* _mApp;
	DrawMyCharacter* dc;
	Ogre::Vector3 shotpos;
	Ogre::Vector3 shotdir;
	Ogre::Timer timer;
	Ogre::Entity* amingEntity;
	Ogre::SceneNode* amingNode;
	Ogre::Entity* ropeEntity;

	Ogre::SceneNode* ropeNode;

	int checkIndex;
	float rlimitY;
	bool rcoll;
	bool ropeNodeFlag;

	MyRopeAction* ra;
	std::list<S_BULLET *> m_listBullet;
	//float y;
	unsigned long start;
	unsigned long ropeStart;
	void roate(float* nx, float* ny, float tx, float ty, float cx, float cy, float q);
	void fireBullet(float fTime);
	void fireRope(float fTime);
	void ropeCollision();

public:
	MyFrameListener();
	void setting(MyTestApp* mApp, DrawMyCharacter* dc, MyRopeAction* ra);
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool collisionArea(Ogre::Vector3 &vPos);
	void createRope();
	void createBullet();
	//int wayPoint[4][4] = { { 300,400,0,5 },{ 600,700,0,10 },{ 700,200,0,12 },{ 1000,500,0,20 } };
	//Ogre::Vector3 cdir;
};

class MyAI
{
private:
	Ogre::Entity* enemyEntity;
	Ogre::Entity* towerEntity;
	MyTestApp* _mApp;
public:
	MyAI();
	void AIsetting(MyTestApp* mApp);
	Ogre::SceneNode* enemyNode;
	Ogre::SceneNode* towerNode;
};

class AIFrameListener : public Ogre::FrameListener
{
private:
	struct S_BULLET //총알 구조체
	{
		Ogre::Entity *pEntity;
		Ogre::SceneNode *pSceneNode;
		float fTime;
		Ogre::Vector3 vDir;
		float fV;
		Ogre::Vector3 vBpos;
	};
private:
	MyAI * _mAI;
	MyTestApp* _mApp;
	Ogre::SceneNode* _enemyNode;
	Ogre::Vector3 vRight;
	Ogre::Timer timer;
	std::list<S_BULLET *> m_listBullet;
	const int WIDTH = 100;
	float xRange;
	float start;
	bool right;

	void enemyMoving();
	void createBullet();
	void fireBullet(float fTime);
	bool collisionArea(Ogre::Vector3 &vPos);
public:
	AIFrameListener();
	void AIFLsetting(MyTestApp* mApp, MyAI* mAI);
	bool frameStarted(const Ogre::FrameEvent& evt);
};

#endif