#include "stdafx.h"
#include "myheader.h"

DrawMyCharacter::DrawMyCharacter() {
	limitY = 50;
	checkIndex = 26;
	gravFlag = true;
}

void DrawMyCharacter::setting(Ogre::SceneNode* head_, Ogre::SceneNode* BlockScn[], Ogre::SceneNode* camNode_) {
	int i = 0;
	head = head_;
	x = head->getPosition().x;
	y = head->getPosition().y;
	for (i = 0; i<27; i++)
		block[i] = BlockScn[i];
	camNode = camNode_;
}

void DrawMyCharacter::collision() {
	/*if (x > block[1]->getPosition().x-120 && x < block[1]->getPosition().x+100) {		// 블럭의 x축 안에 있을 때
	limitY = block[1]->getPosition().y + 100;			// 블럭의 y축을 내 캐릭터의 y축 한계로 지정
	}
	else {							// 블럭의 x축을 벗어 나면
	limitY = 80;				// 땅을 내 캐릭터의 y축 한계로 지정
	}*/

	if (checkIndex>-1) {
		if (y < block[checkIndex]->getPosition().y + 50) {			// 내 캐릭터의 y의 높이가 맨위 블럭보다 낮으면
			checkIndex--;											// 다음 블럭으로
			if (checkIndex == -1)									// 모든 블럭을 다 검사 했으면
				limitY = 50;										// 지면이 내 캐릭터의 y의 최대 높이
		}
		else {
			// 내 캐릭터의 y의 높이가 맨위 블럭보다 높으면
			if (x >(block[checkIndex]->getPosition().x - 150) &&
				x < (block[checkIndex]->getPosition().x + 150))		//	내 캐릭터의 x좌표가 맨위 블럭 안에 있으면
			{
				limitY = block[checkIndex]->getPosition().y + 50;		// 현재 블럭의 높이가 내 캐릭터의 y의 최대
				if (y > block[checkIndex]->getPosition().y) {
					gravFlag = false;
				}
			}
			else {													// 내 캐릭터의 x좌표가 맨위 블럭 안에 없으면
				limitY = 50;										// 바닥까지 떨어져라 				
				gravFlag = true;										// 중력을 주기
			}
		}
	}
}

bool DrawMyCharacter::frameStarted(const Ogre::FrameEvent& evt) {
	collision();
	head->setPosition(x, y, 0);
	if (y > 1500) {
		camNode->setPosition(x, y, 4000);
		camNode->lookAt(Ogre::Vector3(x, y, 0), Ogre::Node::TS_WORLD);
	}
	else {
		camNode->setPosition(x, 1500, 4000);
		camNode->lookAt(Ogre::Vector3(x, 1500, 0), Ogre::Node::TS_WORLD);
	}
	return true;
}