#ifndef Bounds_H
#define Bounds_H

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
class Bounds {
public:
	Bounds() {

	}
	~Bounds() {


	}

	void setId(int _id) {
		id = _id;
	}
	int getId() {
		return id;
	}

	void setWorldId(int _Worldid) {
		WorldId = _Worldid;
	}
	int getWorldId() {
		return WorldId;
	}

	void setType(int _type) {
		type = _type;
	}
	int getType() {
		return type;
	}

	void setPosition(Ogre::Vector3 _position) {
		position = _position;
	}
	Ogre::Vector3 getPosition() {
		return position;
	}
	void setDimensions(Ogre::Vector3 _dimensions) {
		dimensions = _dimensions;
	}
	Ogre::Vector3 getDimensions() {
		return dimensions;
	}

private:
	int id;
	int WorldId;
	Ogre::Vector3 position;
	Ogre::Vector3 dimensions;
	int type;


};
#endif