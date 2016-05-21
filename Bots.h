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
#include "MySql.h"
#include "PhysicsManager.h"
#include "ParticleManager.h"
#include "AppSettings.h"
#include "LootItems.h"
using namespace Ogre;
class Bots {
public:
	Bots(Spawns * spawns,  MySql * mySql, PhysicsManager* physicsManager, AppSettings* appSettings, Sound * sound) {
		_spawns = spawns;
		_mySql = mySql;
		_physicsManager = physicsManager;
		_appSettings = appSettings;
		_sound = sound;
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
			botModel->getBotHelper().forward(_appSettings->getBotSpeed());
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
			0,
			0,
			1
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

	void botControls(ParticleManager * particlemanager, LootItems * lootItems) {
		//	
		for (std::vector<BotModel*>::iterator it = mBots.begin(); it != mBots.end(); ++it) {
			if((*it)->getHealth() != 0){
				Ogre::Vector3 charPos = _spawns->getCharacter()->getPosition();
				Ogre::Real startPosChDist = charPos.distance((*it)->getStartPos());

				bool collide = checkCollision((*it)->getBotNode()->getPosition(), charPos);

				if (startPosChDist <= _appSettings->getAgroRange() && collide == false) {
					
						//
						botMoveToPoint((*it), charPos);
						//

						//_particleManager->createSpellMouse(_spawns->getCharacter()->getPosition(), 5000, 20, "fireCastPlayer", 0);
						if ((*it)->getCastTimer()->getMilliseconds() >= 3000) {
							//smoke
							
							//
							if (_spawns->getScore() < 1000) {
								particlemanager->createSpell((*it)->getBotNode()->getPosition(), _spawns->getCharacter()->getPosition() - (*it)->getBotNode()->getPosition(), 3000, 20, "fireCast", 1);
								(*it)->resetCastTimer();
								_sound->playEnemyAudio("L_BAZOO.wav", false);
							}
							else {
								particlemanager->createSpell((*it)->getBotNode()->getPosition(), _spawns->getCharacter()->getPosition() - (*it)->getBotNode()->getPosition(), 3000, 40, "shadowBolt", 1);
								(*it)->resetCastTimer();
								_sound->playEnemyAudio("E_1.wav", false);
							}
							

						}
					
				
					
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
					
					//Spawn box as reward
					int randomType = Ogre::Math::RangeRandom(0, 3);
					int randomValue = 0;
					if (randomType < 2) {
						randomValue = Ogre::Math::RangeRandom(0, 50);
					}
					else {
						randomValue = Ogre::Math::RangeRandom(-50, 50);
					}
					
					lootItems->spawnLoot((*it)->getBotNode()->getPosition(), Ogre::Vector3 (0,0,0), randomType, randomValue, false);

					//destroy bot
					(*it)->setAlive(false);
					_spawns->setScore(_spawns->getScore() + 100);
					(*it)->destroy(_physicsManager);

				}
				else {
					if((*it)->getTimer()->getMilliseconds()>_appSettings->getRespawn()){
						(*it)->respawn(_physicsManager);
					}
				}
			}
			
		}
			
	}
	std::vector<BotModel*> getmBots() {
		return mBots;
	}

	//Check if textures preventing cast or movement
	bool checkCollision(Ogre::Vector3 enemyPosition,Ogre::Vector3 targetPosition) {
		
		Ogre::Vector3 direction = Ogre::Vector3(targetPosition.x-enemyPosition.x  , (targetPosition.y - enemyPosition.y)+10, targetPosition.z-enemyPosition.z);

		direction = direction.normalisedCopy();
		//
		NxOgre::Ray ray;
		ray.mDirection.from(direction);
		ray.mOrigin.from(Ogre::Vector3(enemyPosition.x, enemyPosition.y+5, enemyPosition.z));

		RaycastHit hit = _physicsManager->getMScene()->raycastClosestShape(ray, NxOgre::Enums::ShapesType_All);
		if (hit.mRigidBody)
		{		
			//
			Ogre::String hitMeshName = hit.mRigidBody->getName();

			if (hitMeshName =="") {
				return false;
			}
			else {
				return true;
			}			
		}
		else
		{
			return false;

		}
	}
private:
	Spawns * _spawns;
	std::vector<BotModel*> mBots;
	MySql * _mySql;
	PhysicsManager* _physicsManager;
	AppSettings* _appSettings;
	Sound * _sound;

};
#endif