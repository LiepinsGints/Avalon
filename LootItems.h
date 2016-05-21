#ifndef LOOT_H
#define LOOT_H

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
#include <math.h>  
#include "PhysicsManager.h"
#include "ParticleManager.h"
#include "AppSettings.h"
#include "LootModel.h"
#include "MySql.h"
using namespace Ogre;
class LootItems {
public:
	LootItems(Spawns * spawns, MySql * mySql, PhysicsManager* physicsManager, AppSettings* appSettings, Sound * sound) {
		_spawns = spawns;
		_physicsManager = physicsManager;
		_appSettings = appSettings;
		_sound = sound;
		_mySql = mySql;
		getLoot();

	}
	~LootItems() {


	}

	void spawnLoot(Ogre::Vector3 position, Ogre::Vector3 rotation, int type, int value, bool respawnable) {
		if (respawnable == true) {
			mLootRespawnable.push_back(_spawns->createCrate(position, rotation, type, value, respawnable));
		}
		else {
			mLootNotRespawnable.push_back(_spawns->createCrate(position, rotation, type, value, respawnable));
		}		
	}

	void updateLootItems() {
		int randomNum;
		Ogre::Vector3 characterPosition = Ogre::Vector3(_spawns->getCharacter()->getPosition().x, _spawns->getCharacter()->getPosition().y, _spawns->getCharacter()->getPosition().z);
		//----Check static first
		for (std::vector<LootModel*>::iterator it = mLootRespawnable.begin(); it != mLootRespawnable.end(); ++it) {
			

			if ((*it)->getAlive() == true) {
				Ogre::Vector3 boxPosition = Ogre::Vector3((*it)->getBoxBody()->getGlobalPosition().x, (*it)->getBoxBody()->getGlobalPosition().y, (*it)->getBoxBody()->getGlobalPosition().z);
				Ogre::Real distance = boxPosition.distance(characterPosition);

				if (distance <= 6) {
					switch ((*it)->getType())
					{
					case 0:
						if (_spawns->getHealth() < 100) {
							_spawns->setHealth(_spawns->getHealth() + (*it)->getValue());
							(*it)->destroy(_physicsManager);
						}

						break;
					case 1:
						if (_spawns->getMana() < 100) {
							_spawns->setMana(_spawns->getMana() + (*it)->getValue());
							(*it)->destroy(_physicsManager);
						}

						break;
					case 2:
						randomNum = Ogre::Math::RangeRandom(0, 3);
						switch (randomNum)
						{
						case 0:				
							_spawns->setHealth(_spawns->getHealth() + (*it)->getValue());
							(*it)->destroy(_physicsManager);
							
							break;
						case 1:
							_spawns->setMana(_spawns->getMana() + (*it)->getValue());
							(*it)->destroy(_physicsManager);
							break;
						case 2:
							_spawns->setScore(_spawns->getScore()+(*it)->getValue());
							(*it)->destroy(_physicsManager);
							break;
						default:
							break;
						}
						break;
					default:

						break;
					}

				}
			}
			else {
				if ((*it)->getTimer()->getMilliseconds() >= _appSettings->getRespawn()) {
					(*it) = _spawns->createCrate((*it)->getInitialPosition(), (*it)->getInitialRotation(), (*it)->getType(), (*it)->getValue(), true);
				}
			}
			

		}
		//----Check dinamic
		std::vector<LootModel*> mLootNotRespawnableTemp;
		for (std::vector<LootModel*>::iterator it = mLootNotRespawnable.begin(); it != mLootNotRespawnable.end(); ++it) {

			Ogre::Vector3 boxPosition = Ogre::Vector3((*it)->getBoxBody()->getGlobalPosition().x, (*it)->getBoxBody()->getGlobalPosition().y, (*it)->getBoxBody()->getGlobalPosition().z);
			Ogre::Real distance = boxPosition.distance(characterPosition);

			if (distance <= 6) {
				switch ((*it)->getType())
				{
				case 0:
					if (_spawns->getHealth() < 100) {
						_spawns->setHealth(_spawns->getHealth() + (*it)->getValue());
						(*it)->destroy(_physicsManager);
					}
					else {
						mLootNotRespawnableTemp.push_back((*it));
					}

					break;
				case 1:
					if (_spawns->getMana() < 100) {
						_spawns->setMana(_spawns->getMana() + (*it)->getValue());
						(*it)->destroy(_physicsManager);
					}
					else {
						mLootNotRespawnableTemp.push_back((*it));
					}

					break;
				case 2:
					randomNum = Ogre::Math::RangeRandom(0, 3);
					switch (randomNum)
					{
					case 0:
						_spawns->setHealth(_spawns->getHealth() + (*it)->getValue());
						(*it)->destroy(_physicsManager);

						break;
					case 1:
						_spawns->setMana(_spawns->getMana() + (*it)->getValue());
						(*it)->destroy(_physicsManager);
						break;
					case 2:
						_spawns->setScore(_spawns->getScore() + (*it)->getValue());
						(*it)->destroy(_physicsManager);
						break;
					default:
						break;
					}
					break;
				default:

					break;
				}

			}
			else {
				mLootNotRespawnableTemp.push_back((*it));
			}
		}
		mLootNotRespawnable = mLootNotRespawnableTemp;
	}

	void getLoot() {
		mLootRespawnable = _mySql->getLoot(_spawns);
	}
private:
	Spawns * _spawns;
	std::vector<LootModel*> mLootRespawnable;
	std::vector<LootModel*> mLootNotRespawnable;
	PhysicsManager* _physicsManager;
	AppSettings* _appSettings;
	Sound * _sound;//BG_PLANE.wav
	MySql * _mySql;

};
#endif