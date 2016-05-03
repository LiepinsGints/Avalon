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
	Ogre::Vector3  getEndPosition() {
		return endPosition;
	}
	Ogre::String  getParticleName() {
		return particleName;
	}
	Ogre::Real getLivingDistance() {
		return livingDistance;
	}

	void setId(int _id) {
		id = _id;
	}
	
	void setParticleNode(Ogre::SceneNode* _particleNode) {
		particleNode = _particleNode;
	}
	void setPosition(Ogre::Vector3  _position) {
		position = _position;
	}
	void setEndPosition(Ogre::Vector3  _endPosition) {
		endPosition = _endPosition;
	}
	void setParticleName(Ogre::String _particleName) {
		particleName = _particleName;
	}
	void setLivingDistance(Ogre::Real _livingDistance) {
		livingDistance = _livingDistance;
	}
private:
	int id;
	Ogre::SceneNode* particleNode;
	Ogre::Vector3  position;
	Ogre::Vector3  endPosition;
	Ogre::String particleName;
	Ogre::Real livingDistance;
};
#endif