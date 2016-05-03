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

using namespace Ogre;
class ParticleManager {
public:
	ParticleManager(Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera, Ogre::Root* mRoot, Ogre::RenderWindow* mWindow, Ogre::TerrainGroup* mTerrainGroup) {
		_mSceneMgr = mSceneMgr;
		_mCamera = mCamera;
		_mRoot = mRoot;
		_mWindow = mWindow;
		_mTerrainGroup = mTerrainGroup;
		counter = 0;
		terrainRayCol = Ogre::Vector3(0, 0, 0);
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

	void createSpellMouse(Ogre::Vector3 position, Ogre::Real livingDistance) {
		Ogre::Vector3 endPosition = Ogre::Vector3(terrainRayCol.x, position.y, terrainRayCol.z);
		createSpell(position, endPosition, livingDistance);
	}
	Ogre::Vector3 groundBellow(Ogre::Vector3 position) {

		Ogre::Ray rayBellow(position,
			Ogre::Vector3(0, -1, 0));
		Ogre::TerrainGroup::RayResult mouseResult = _mTerrainGroup->rayIntersects(rayBellow);

		return mouseResult.position;
	}

	void createSpell(Ogre::Vector3 position, Ogre::Vector3 target, Ogre::Real livingDistance) {
		
		ParticleModel * particleSpell =  new ParticleModel;
		
		Ogre::String particleName = "smoke" + std::to_string(counter)+"_";
		//ParticleSystem* sunParticle = _mSceneMgr->createParticleSystem(particleName, "Examples/smoke");
		ParticleSystem* sunParticle = _mSceneMgr->createParticleSystem(particleName, "Examples/fireCast");


		SceneNode* particleNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode(particleName);
		particleNode->setPosition(position);
		particleNode->attachObject(sunParticle);

		particleSpell->setId(counter);
		particleSpell->setParticleName(particleName);
		particleSpell->setParticleNode(particleNode);
		particleSpell->setPosition(position);
		//Ogre::Vector3 temp = target.normalisedCopy()*100;
		particleSpell->setEndPosition(target);
		particleSpell->setLivingDistance(livingDistance);
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


		Ogre::Vector3 current = currentParticle->getParticleNode()->getPosition();
		Ogre::Vector3 target = currentParticle->getEndPosition();
		//test
		Ogre::Vector3 terrainBellow = groundBellow(current);
		Ogre::Real heightDif = current.y - terrainBellow.y;
		//
		

		Ogre::Vector3 direction = Ogre::Vector3(target.x- current.x,0, target.z- current.z)*100;
		direction = direction.normalisedCopy();
		Ogre::Vector3 position = current + (Ogre::Vector3(direction.x * delta, 0, direction.z * delta));

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


	void particleControls(Ogre::FrameEvent fe) {
		
		std::vector<ParticleModel*> particleSpellsTemp;
		for (std::vector<ParticleModel*>::iterator it = particleSpells.begin(); it != particleSpells.end(); ++it) {
			
			Ogre::Real distance = (*it)->getPosition().distance((*it)->getParticleNode()->getPosition());
			
			if(distance>= (*it)->getLivingDistance()-5 || ((*it)->getParticleNode()->getPosition().x == (*it)->getEndPosition().x && (*it)->getParticleNode()->getPosition().z == (*it)->getEndPosition().z)){
				(*it)->getParticleNode()->removeAndDestroyAllChildren();
				_mSceneMgr->destroySceneNode((*it)->getParticleNode());
				//it = particleSpells.erase(it);
				//delete * it;
			}else{
				moveAllongVector((*it), fe);
				particleSpellsTemp.push_back((*it));
			}

			//(*it)->getBot()->getPosition().x

		}

		particleSpells = particleSpellsTemp;
	}

private:

	Ogre::SceneManager* _mSceneMgr;
	Ogre::Camera* _mCamera;
	Ogre::Root* _mRoot;
	Ogre::RenderWindow* _mWindow;
	int counter;
	std::vector<ParticleModel*> particleSpells;
	Ogre::TerrainGroup* _mTerrainGroup;
	//Ray result
	Ogre::Vector3 terrainRayCol;

};
#endif