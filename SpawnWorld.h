#ifndef SpawnWorld_H
#define SpawnWorld_H

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


using namespace Ogre;
class SpawnWorld {
public:
	SpawnWorld() {

	}
	~SpawnWorld() {


	}

	void setId(int _id) {
		id = _id;
	}
	int getId() {
		return id;
	}
	void setModelID(int _modelID) {
		modelID = _modelID;
	}
	int getModelID() {
		return modelID;
	}
	void setPosition(Ogre::Vector3 _position) {
		position = _position;
	}
	Ogre::Vector3 getPosition() {
		return position;
	}
	void setRotation(float _rotation) {
		rotation = _rotation;
	}
	float getRotation() {
		return rotation;
	}
	void setScale(float _scale) {
		scale = _scale;
	}
	float getScale() {
		return scale;
	}
	void setMass(float _mass) {
		mass = _mass;
	}
	float getMass() {
		return mass;
	}
	void setType(float _type) {
		type = _type;
	}
	float getType() {
		return type;
	}
private:
	int id;
	int modelID;
	Ogre::Vector3 position;
	float rotation;
	float scale;
	float mass;
	float type;


};
#endif