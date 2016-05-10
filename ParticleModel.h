#ifndef ParticleModel_H
#define ParticleModel_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

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
class ParticleModel {
public:
	ParticleModel() {
		timer = new Ogre::Timer();
	}
	~ParticleModel() {


	}

	int getId() {
		return id;
	}
	Ogre::SceneNode* getParticleNode() {
		return particleNode;
	}
	Ogre::Vector3  getPosition() {
		return position;
	}
	Ogre::Vector3  getDirectionVector() {
		return directionVector;
	}
	Ogre::String  getParticleName() {
		return particleName;
	}
	Ogre::Real getTimeToLive() {
		return timeToLive;
	}
	Ogre::Timer * getTimer() {
		return timer;

	}
	Ogre::Real getDamage() {
		return damage;
	}
	int getType() {
		return type;
	}


	void setId(int _id) {
		id = _id;
	}
	void setType(int _type) {
		type = _type;
	}
	
	void setParticleNode(Ogre::SceneNode* _particleNode) {
		particleNode = _particleNode;
	}
	void setPosition(Ogre::Vector3  _position) {
		position = _position;
	}
	void setDirectionVector(Ogre::Vector3  _directionVector) {
		directionVector = _directionVector;
	}
	void setParticleName(Ogre::String _particleName) {
		particleName = _particleName;
	}
	void setTimeToLive(Ogre::Real _timeToLive) {
		timeToLive = _timeToLive;
	}
	void setDamage(Ogre::Real _damage) {
		damage = _damage;
	}
	
private:
	int id;
	Ogre::SceneNode* particleNode;
	Ogre::Vector3  position;
	Ogre::Vector3  directionVector;
	Ogre::String particleName;
	Ogre::Real timeToLive;
	Ogre::Timer* timer;
	Ogre::Real damage;
	int type;//0 - player 1-enemy
};
#endif