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
	/*if (x > block[1]->getPosition().x-120 && x < block[1]->getPosition().x+100) {		// ���� x�� �ȿ� ���� ��
	limitY = block[1]->getPosition().y + 100;			// ���� y���� �� ĳ������ y�� �Ѱ�� ����
	}
	else {							// ���� x���� ���� ����
	limitY = 80;				// ���� �� ĳ������ y�� �Ѱ�� ����
	}*/

	if (checkIndex>-1) {
		if (y < block[checkIndex]->getPosition().y + 50) {			// �� ĳ������ y�� ���̰� ���� ������ ������
			checkIndex--;											// ���� ������
			if (checkIndex == -1)									// ��� ���� �� �˻� ������
				limitY = 50;										// ������ �� ĳ������ y�� �ִ� ����
		}
		else {
			// �� ĳ������ y�� ���̰� ���� ������ ������
			if (x >(block[checkIndex]->getPosition().x - 150) &&
				x < (block[checkIndex]->getPosition().x + 150))		//	�� ĳ������ x��ǥ�� ���� �� �ȿ� ������
			{
				limitY = block[checkIndex]->getPosition().y + 50;		// ���� ���� ���̰� �� ĳ������ y�� �ִ�
				if (y > block[checkIndex]->getPosition().y) {
					gravFlag = false;
				}
			}
			else {													// �� ĳ������ x��ǥ�� ���� �� �ȿ� ������
				limitY = 50;										// �ٴڱ��� �������� 				
				gravFlag = true;										// �߷��� �ֱ�
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