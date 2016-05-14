#ifndef ParticleManager_H
#define ParticleManager_H

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

#include <OgreParticleSystem.h>
#include "ParticleModel.h"
#include "BotModel.h"
#include "Spawns.h" 
#include "UserInterface.h"
#include "PhysicsManager.h"
#include "Sound.h"
using namespace Ogre;
class ParticleManager {
public:
	ParticleManager(Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera, Ogre::Root* mRoot, Ogre::RenderWindow* mWindow, Ogre::TerrainGroup* mTerrainGroup, std::vector<BotModel*> mBots, PhysicsManager* physicsManager, Sound * sound) {
		_mSceneMgr = mSceneMgr;
		_mCamera = mCamera;
		_mRoot = mRoot;
		_mWindow = mWindow;
		_mTerrainGroup = mTerrainGroup;
		counter = 0;
		terrainRayCol = Ogre::Vector3(0, 0, 0);
		_mBots = mBots;
		_physicsManager = physicsManager;
		_sound = sound;
		//simple particle
		/*
		ParticleSystem* sunParticle = mSceneMgr->createParticleSystem("fireBall", "Examples/fireBall");
		SceneNode* particleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Particle");
		particleNode->setPosition(Ogre::Vector3(-37,29,-167));
		particleNode->attachObject(sunParticle);
		*/

	}
	~ParticleManager() {


	}
	void mousePos(const OIS::MouseEvent &arg)
	{
		Viewport* vp = _mSceneMgr->getCurrentViewport();

		//Real coordinates
		Ogre::Real x = arg.state.X.abs;
		Ogre::Real y = arg.state.Y.abs;
		//Normalized coordinates
		Ogre::Real normalX = arg.state.X.abs / Ogre::Real(vp->getActualWidth());
		Ogre::Real normalY = arg.state.Y.abs / Ogre::Real(vp->getActualHeight());

		Ogre::Ray mouseRay = _mCamera->getCameraToViewportRay(normalX, normalY);
		Ogre::TerrainGroup::RayResult mouseResult = _mTerrainGroup->rayIntersects(mouseRay);
		terrainRayCol = mouseResult.position;
	}

	void createSpellMouse(Ogre::Vector3 position, Ogre::Real timeToLive,Ogre::Real damage,Ogre::String spellName, int type) {
		Ogre::Vector3 endPosition = Ogre::Vector3(terrainRayCol.x, position.y, terrainRayCol.z);
		createSpell(position, endPosition- position, timeToLive,damage, spellName, type);
	}
	Ogre::Vector3 groundBellow(Ogre::Vector3 position) {

		Ogre::Ray rayBellow(position,
			Ogre::Vector3(0, -1, 0));
		Ogre::TerrainGroup::RayResult mouseResult = _mTerrainGroup->rayIntersects(rayBellow);

		return mouseResult.position;
	}

	void createSpell(Ogre::Vector3 position, Ogre::Vector3 target, Ogre::Real timeToLive, Ogre::Real damage, Ogre::String spellName, int type) {
		
		ParticleModel * particleSpell =  new ParticleModel;
		
		Ogre::String particleName = "spell" + std::to_string(counter)+"_";
		//ParticleSystem* sunParticle = _mSceneMgr->createParticleSystem(particleName, "Examples/smoke");
		ParticleSystem* sunParticle = _mSceneMgr->createParticleSystem(particleName, "Examples/"+spellName);


		SceneNode* particleNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode(particleName);
		particleNode->setPosition(position);
		particleNode->attachObject(sunParticle);

		particleSpell->setId(counter);
		particleSpell->setParticleName(particleName);
		particleSpell->setParticleNode(particleNode);
		particleSpell->setPosition(position);
		//Ogre::Vector3 temp = target.normalisedCopy()*100;
		particleSpell->setDirectionVector(target);
		particleSpell->setTimeToLive(timeToLive);
		particleSpell->getTimer()->reset();
		particleSpell->setDamage(damage);
		particleSpell->setType(type);
		particleSpells.push_back(particleSpell);
		counter++;
	}

	void moveAllongVector(ParticleModel* currentParticle, Ogre::FrameEvent fe) {
		//Vector3 newSpot = oldSpotVector3 + (directionVector3.normalized * distanceFloat);
		Ogre::Real delta = fe.timeSinceLastFrame *50;
		
		//Ogre::Vector3 direction = currentParticle->getEndPosition().normalisedCopy();
		//Ogre::Vector3 position = currentParticle->getEndPosition() + (Ogre::Vector3(direction.x + delta, 0, direction.z + delta));
		//
		//currentParticle->getParticleNode()->setPosition(position);


		Ogre::Vector3 current = currentParticle->getPosition();
		//Ogre::Vector3 current = currentParticle->getPosition();
		//Ogre::Vector3 target = currentParticle->getEndPosition();
		//test
		Ogre::Vector3 terrainBellow = groundBellow(current);
		Ogre::Real heightDif = current.y - terrainBellow.y;
		//
		

		Ogre::Vector3 direction = Ogre::Vector3(currentParticle->getDirectionVector().x,0, currentParticle->getDirectionVector().z);

		direction = direction.normalisedCopy();
		Ogre::Vector3 position = currentParticle->getParticleNode()->getPosition() + (Ogre::Vector3(direction.x * delta, 0, direction.z * delta));

		terrainBellow = groundBellow(position);
		Ogre::Real heightDifAfter = position.y - terrainBellow.y;
		if (heightDifAfter > heightDif) {
			position = Ogre::Vector3(position.x, position.y - (heightDifAfter - heightDif), position.z);
		}
		else if (heightDifAfter < heightDif) {
			position = Ogre::Vector3(position.x, position.y + (heightDif-heightDifAfter), position.z);
		}

		currentParticle->getParticleNode()->setPosition(Ogre::Vector3(position.x, position.y, position.z));

	
		

	}


	void particleControls(Ogre::FrameEvent fe,Spawns * spawns) {
		
		std::vector<ParticleModel*> particleSpellsTemp;
		for (std::vector<ParticleModel*>::iterator it = particleSpells.begin(); it != particleSpells.end(); ++it) {
			//Check if particle collides with bound
			bool collisionResult = checkCollision(Ogre::Vector3((*it)->getParticleNode()->getPosition()),
				Ogre::Vector3((*it)->getDirectionVector().x, (*it)->getDirectionVector().y + 10, (*it)->getDirectionVector().z));
			//checkCollision(Ogre::Vector3 startPosition, Ogre::Vector3 targetPosition)
			//Ogre::Real distance = (*it)->getPosition().distance((*it)->getParticleNode()->getPosition());

			bool botColides = false;
			if (collisionResult == false) {				
				//Check if botCollides
				if ((*it)->getType() == 0) {
					for (std::vector<BotModel*>::iterator itBots = _mBots.begin(); itBots != _mBots.end(); ++itBots) {
						if ((*itBots)->getAlive() == true) {
							Ogre::Real distance = (*it)->getParticleNode()->getPosition().distance((*itBots)->getBotNode()->getPosition());

							if (distance < 5) {
								(*itBots)->setHealth((*itBots)->getHealth() - (*it)->getDamage());
								botColides = true;
								break;
							}
						}
					}
				}
				else {
					Ogre::Real distance = (*it)->getParticleNode()->getPosition().distance(spawns->getCharacter()->getPosition());
					if (distance<5) {
						spawns->setHealth(spawns->getHealth() - (*it)->getDamage());
						botColides = true;
						//userInterface->updateUserFrame();
					}
				}
			}
			//Destroy particle
			if((*it)->getTimer()->getMilliseconds()>(*it)->getTimeToLive() || botColides==true || collisionResult == true){
				(*it)->getParticleNode()->removeAndDestroyAllChildren();
				_mSceneMgr->destroySceneNode((*it)->getParticleNode());
				//it = particleSpells.erase(it);
				//delete * it;
			}else{
				moveAllongVector((*it), fe);
				

				//If particle unused keep
				particleSpellsTemp.push_back((*it));
			}

			//(*it)->getBot()->getPosition().x

		}

		particleSpells = particleSpellsTemp;
	}

	bool checkCollision(Ogre::Vector3 startPosition, Ogre::Vector3 direction) {

		//Ogre::Vector3 direction = Ogre::Vector3(targetPosition.x - startPosition.x, (targetPosition.y - startPosition.y) + 10, targetPosition.z - startPosition.z);

		direction = direction.normalisedCopy();
		//
		NxOgre::Ray ray;
		ray.mDirection.from(direction);
		ray.mOrigin.from(Ogre::Vector3(startPosition.x, startPosition.y + 10, startPosition.z));

		RaycastHit hit = _physicsManager->getMScene()->raycastClosestShape(ray, NxOgre::Enums::ShapesType_All);
		if (hit.mRigidBody)
		{
			//
			Ogre::String hitMeshName = hit.mRigidBody->getName();

			if (hitMeshName == "") {
				return false;
			}
			else {
				//_sound->playEnvironmentAudio("I_METAL.wav", false);
				if(hit.mDistance <= 5) {
					_sound->playEnvironmentAudio("I_METAL.wav",false);
					return true;
				}
				else {
					return false;
				}
				
			}
		}
		else
		{
			return false;

		}
	}
private:

	Ogre::SceneManager* _mSceneMgr;
	Ogre::Camera* _mCamera;
	Ogre::Root* _mRoot;
	Ogre::RenderWindow* _mWindow;
	int counter;
	std::vector<ParticleModel*> particleSpells;
	Ogre::TerrainGroup* _mTerrainGroup;
	PhysicsManager* _physicsManager;
	//Ray result
	Ogre::Vector3 terrainRayCol;

	std::vector<BotModel*> _mBots;
	Sound * _sound;
};
#endif