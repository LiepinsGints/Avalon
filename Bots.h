#ifndef BOTS_H
#define BOTS_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <iostream>
#include <string>

#include <NxOgre.h>
#include <critter.h>
#include <CritterCommon.h>
#include <CritterBody.h>
#include <NxOgreVec3.h>
#include <NxOgreBox.h>
#include <NxOgreBoxDescription.h>
#include <NxOgreMatrix.h>
#include <NxCharacter.h>
#include <vector>

#include "PhysicsManager.h"
#include "Spawns.h" 
#include "BotModel.h"
#include <math.h>  
#include "UserInterface.h"
#include "MySql.h"
#include "PhysicsManager.h"
using namespace Ogre;
class Bots {
public:
	Bots(Spawns * spawns, UserInterface * userInterface, MySql * mySql, PhysicsManager* physicsManager) {
		_spawns = spawns;
		_userInterface= userInterface;
		_mySql = mySql;
		_physicsManager = physicsManager;
		getBots();

	}
	~Bots() {


	}

	void spawnBot(Ogre::String meshName, Ogre::Vector3 position, Ogre::Real speed) {
		mBots.push_back(_spawns->createBot(meshName, position, speed));
	}

	Ogre::Real angleBetweenVectors(Ogre::Vector3 a, Ogre::Vector3 b) {
		Ogre::Real dotProduct = a.x * b.x + a.y * b.y + a.z * b.z;
		Ogre::Real aMagnitude = a.length();
		Ogre::Real bMagnitude = b.length();

		Ogre::Real angle = (acos(dotProduct / (aMagnitude * bMagnitude)))* 180.0 / Ogre::Math::PI;
		
		if (a.x < b.x) {
			angle *= -1;
		}
		angle = corectAngle(angle);
		return angle;
	}

	Ogre::Real corectAngle(Ogre::Real angle) {

		Ogre::Real angleDeg = angle;
		if (angleDeg < 0) {
			angleDeg *= -1;
		}
		else if(angleDeg > 0) {
			angleDeg = 360 - angleDeg;
		}

		return angleDeg;
	}

	void getBots() {
		mBots = _mySql->getBots(_spawns);
	}


	void botMoveToPoint(BotModel * botModel, Ogre::Vector3 charPos) {
		
		Ogre::Vector3 botPos = botModel->getBot()->getPosition();
		Ogre::Real distanceBetween = charPos.distance(botPos);
		if (distanceBetween > 5) {
			botModel->getBotHelper().forward(100);
			botModel->getBot()->setInput(botModel->getBotHelper());
		}
		else {
			botModel->getBotHelper().forward(0);
			botModel->getBot()->setInput(botModel->getBotHelper());
		}
		//firstBot->getBotDesc().mShape.mRadius;
		//
		botModel->getBotNode()->getOrientation().getYaw();
		//_spawns->getSinbadNode()->getOrientation().getYaw();
		//Calcualte vectors
		Ogre::Vector3 vectorA = Ogre::Vector3(
			charPos.x - charPos.x,
			charPos.y - charPos.y,
			charPos.z + 1 - charPos.z
			);
		Ogre::Vector3 vectorB = Ogre::Vector3(
			botModel->getBot()->getPosition().x - charPos.x,
			botModel->getBot()->getPosition().y - charPos.y,
			botModel->getBot()->getPosition().z - charPos.z
			);

		Ogre::Real vectorAngle = angleBetweenVectors(vectorA, vectorB);

		//Ogre::Real playerCurAngle = corectAngle(Ogre::Degree(_spawns->getSinbadNode()->getOrientation().getYaw()).valueDegrees());
		Ogre::Real botAngle = corectAngle(Ogre::Degree(botModel->getBotNode()->getOrientation().getYaw()).valueDegrees());
		//
		Ogre::Real angle = 180 - vectorAngle;
		if (angle < 0) {
			angle = 360 + angle;

		}

		Ogre::Real arkA;
		Ogre::Real arkB;
		if (botAngle <= angle) {
			arkA = angle - botAngle;
			arkB = 360 - arkA;

			if (arkA > -10 && arkA < 10) {
				botModel->getBotHelper().left(0);
				botModel->getBotHelper().right(0);
				botModel->getBot()->setInput(botModel->getBotHelper());
			}
			else {
				if (arkA < arkB) {
					botModel->getBotHelper().right(0);
					botModel->getBotHelper().input.is_turning = true;
					botModel->getBotHelper().left(127);
					botModel->getBot()->setInput(botModel->getBotHelper());
				}
				else {
					botModel->getBotHelper().left(0);
					botModel->getBotHelper().input.is_turning = true;
					botModel->getBotHelper().right(127);
					botModel->getBot()->setInput(botModel->getBotHelper());
				}
			}

		}
		else {
			arkA = botAngle - angle;
			arkB = 360 - arkA;

			if (arkA > -10 && arkA < 10) {
				botModel->getBotHelper().left(0);
				botModel->getBotHelper().right(0);
				botModel->getBot()->setInput(botModel->getBotHelper());
			}
			else {
				if (arkA < arkB) {
					botModel->getBotHelper().left(0);
					botModel->getBotHelper().input.is_turning = true;
					botModel->getBotHelper().right(127);
					botModel->getBot()->setInput(botModel->getBotHelper());
				}
				else {
					botModel->getBotHelper().right(0);
					botModel->getBotHelper().input.is_turning = true;
					botModel->getBotHelper().left(127);
					botModel->getBot()->setInput(botModel->getBotHelper());
				}
			}
		}
	}

	void botControls() {
		//	
		for (std::vector<BotModel*>::iterator it = mBots.begin(); it != mBots.end(); ++it) {
			if((*it)->getHealth() != 0){
				Ogre::Vector3 charPos = _spawns->getCharacter()->getPosition();
				Ogre::Real startPosChDist = charPos.distance((*it)->getStartPos());

				if (startPosChDist <= 120) {
					botMoveToPoint((*it), charPos);
				}
				else {
					if((*it)->getStartPos().x!= (*it)->getBot()->getPosition().x && (*it)->getStartPos().z != (*it)->getBot()->getPosition().z){
					
						botMoveToPoint((*it), Ogre::Vector3((*it)->getStartPos().x, (*it)->getStartPos().y, (*it)->getStartPos().z));
					}
				}
			}
			else {
				if((*it)->getAlive()==true){
					(*it)->getBotHelper().right(0);
					(*it)->getBotHelper().left(0);
					(*it)->getBotHelper().forward(0);
					(*it)->getBotHelper().backward(0);
					(*it)->getBot()->setInput((*it)->getBotHelper());
					//
					(*it)->setAlive(false);
					(*it)->destroy(_physicsManager);

				}
				else {
					if((*it)->getTimer()->getMilliseconds()>10000){
						(*it)->respawn(_physicsManager);
					}
				}
			}
			
		}
			
	}
	std::vector<BotModel*> getmBots() {
		return mBots;
	}

private:
	Spawns * _spawns;
	UserInterface * _userInterface;
	std::vector<BotModel*> mBots;
	MySql * _mySql;
	PhysicsManager* _physicsManager;


};
#endif