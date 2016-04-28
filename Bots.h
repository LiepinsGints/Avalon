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

using namespace Ogre;
class Bots {
public:
	Bots(Spawns * spawns, UserInterface * userInterface) {
		_spawns = spawns;
		_userInterface= userInterface;
	}
	~Bots() {


	}
	void spawnBot(Ogre::String meshName, Ogre::Vector3 position) {
		mBots.push_back(_spawns->createBot(meshName, position));
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
	Ogre::Real botTurnToPlayer() {
		

		//
		BotModel * firstBot = mBots.front();
		Ogre::Vector3 charPos = _spawns->getCharacter()->getPosition();
		Ogre::Vector3 botPos = firstBot->getBot()->getPosition();
		Ogre::Real distanceBetween = charPos.distance(botPos);
		if (distanceBetween > 10) {
			firstBot->getBotHelper().forward(100);
			firstBot->getBot()->setInput(firstBot->getBotHelper());
		}
		else {
			firstBot->getBotHelper().forward(0);
			firstBot->getBot()->setInput(firstBot->getBotHelper());
		}
		//firstBot->getBotDesc().mShape.mRadius;
		//
		firstBot->getBotNode()->getOrientation().getYaw();
		_spawns->getSinbadNode()->getOrientation().getYaw();
		//Calcualte vectors
		//Ogre::Vector3 vectorA = Ogre::Vector3(_spawns->getCharacter()->getPosition().x, _spawns->getCharacter()->getPosition().y, _spawns->getCharacter()->getPosition().z+1)-_spawns->getCharacter()->getPosition();
		//Ogre::Vector3 vectorB = firstBot->getBot()->getPosition() - _spawns->getCharacter()->getPosition();
		Ogre::Vector3 vectorA = Ogre::Vector3(
			_spawns->getCharacter()->getPosition().x - _spawns->getCharacter()->getPosition().x,
			_spawns->getCharacter()->getPosition().y - _spawns->getCharacter()->getPosition().y,
			_spawns->getCharacter()->getPosition().z+1 - _spawns->getCharacter()->getPosition().z
			);
		Ogre::Vector3 vectorB = Ogre::Vector3(
			firstBot->getBot()->getPosition().x - _spawns->getCharacter()->getPosition().x,
			firstBot->getBot()->getPosition().y - _spawns->getCharacter()->getPosition().y,
			firstBot->getBot()->getPosition().z - _spawns->getCharacter()->getPosition().z
			);
		
		Ogre::Real vectorAngle = angleBetweenVectors(vectorA, vectorB);
		//bots->corectAngle(Ogre::Degree(spawns->getSinbadNode()->getOrientation().getYaw()).valueDegrees())
		Ogre::Real playerCurAngle = corectAngle(Ogre::Degree(_spawns->getSinbadNode()->getOrientation().getYaw()).valueDegrees());
		Ogre::Real botAngle = corectAngle(Ogre::Degree(firstBot->getBotNode()->getOrientation().getYaw()).valueDegrees());
		//
		//_userInterface->addLineToConsole("bot angle : " + Ogre::StringConverter::toString(botAngle)+" Vector angle: "+Ogre::StringConverter::toString(vectorAngle));
		Ogre::Real angle = 180 - vectorAngle;
		if (angle < 0) {
			angle = 360 + angle;
			
		}

		_userInterface->addLineToConsole("bot angle : " + Ogre::StringConverter::toString(angle));

		Ogre::Real arkA;
		Ogre::Real arkB;
		if (botAngle <= angle) {
			arkA = angle - botAngle;
			arkB = 360 - arkA;

			if (arkA > -10 && arkA < 10) {
				firstBot->getBotHelper().left(0);
				firstBot->getBotHelper().right(0);
				firstBot->getBot()->setInput(firstBot->getBotHelper());
			}
			else {
				if (arkA < arkB) {
					firstBot->getBotHelper().right(0);
					firstBot->getBotHelper().input.is_turning = true;
					firstBot->getBotHelper().left(127);
					firstBot->getBot()->setInput(firstBot->getBotHelper());
				}
				else {
					firstBot->getBotHelper().left(0);
					firstBot->getBotHelper().input.is_turning = true;
					firstBot->getBotHelper().right(127);
					firstBot->getBot()->setInput(firstBot->getBotHelper());
				}
			}

		}
		else {
			arkA = botAngle - angle;
			arkB = 360 - arkA;

			if (arkA > -10 && arkA < 10) {
				firstBot->getBotHelper().left(0);
				firstBot->getBotHelper().right(0);
				firstBot->getBot()->setInput(firstBot->getBotHelper());
			}
			else {
				if (arkA < arkB) {
					firstBot->getBotHelper().left(0);
					firstBot->getBotHelper().input.is_turning = true;
					firstBot->getBotHelper().right(127);
					firstBot->getBot()->setInput(firstBot->getBotHelper());
				}
				else {
					firstBot->getBotHelper().right(0);
					firstBot->getBotHelper().input.is_turning = true;
					firstBot->getBotHelper().left(127);
					firstBot->getBot()->setInput(firstBot->getBotHelper());
				}
			}
		}

		
		



		
		

		//return vectorAngle;
		return angle;
			
	}
	

private:
	Spawns * _spawns;
	UserInterface * _userInterface;
	std::vector<BotModel*> mBots;

};
#endif