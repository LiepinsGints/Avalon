#ifndef LOOT_MODEL_H
#define LOOT_MODEL_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <OgreTimer.h>
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

using namespace Ogre;
class LootModel {
public:
	LootModel() {
		type = 0;
		respawnable = false;
		timer = new Ogre::Timer();
		alive = true;
	}
	~LootModel() {


	}
	int getType() {
		return type;
	}
	void setType(int _type) {
		type = _type;
	}
	void setBoxBody(Critter::Body* _boxBody) {
		boxBody = _boxBody;
	}
	Critter::Body* getBoxBody() {
		return boxBody;
	}
	int getValue() {
		return value;
	}
	void setValue(int _value) {
		value = _value;
	}
	bool getRespawn() {
		return respawnable;
	}
	void setRespawnable(bool _respawnable) {
		respawnable = _respawnable;
	}

	Ogre::Vector3  getInitialPosition() {
		return initialPosition;
	}
	void setInitialPosition(Ogre::Vector3 _initialPosition) {
		initialPosition = _initialPosition;
	}
	Ogre::Vector3  getInitialRotation() {
		return   initialRotation;
	}
	void setInitialRotation(Ogre::Vector3 _initialRotation) {
		initialRotation = _initialRotation;
	}
	Ogre::Timer * getTimer() {
		return timer;
	}
	void startTimer() {
		timer->reset();
	}
	bool getAlive() {
		return alive;
	}
	void setAlive(bool _alive) {
		alive = _alive;
	}

	//detroy respawn
	//destroy
	void destroy(PhysicsManager* physicsManager) {
		
		
		
		boxBody->getNode()->destroyEntity(boxBody->getNode()->getEntityAt(0)->getName());
		
		
		physicsManager->getMRenderSystem()->destroyBody(boxBody);
		
		
		timer->reset();
		alive = false;
	

	}
	void respawn(PhysicsManager* physicsManager) {
	/*
		botNode = physicsManager->getMRenderSystem()->createNode();
		botNode->createAndAttachEntity(nodeName, meshName);
		setBot(physicsManager->getMRenderSystem()->createAnimatedCharacter(startPos, Ogre::Radian(0), getBotNode(), getBotDesc()));
		alive = true;
		health = 100;
		mana = 100;
*/
	}

private:
	int type;
	Critter::Body* boxBody;
	int value;
	bool respawnable;
	Ogre::Vector3 initialPosition;
	Ogre::Vector3 initialRotation;
	Ogre::Timer* timer;
	bool alive;
	

};
#endif