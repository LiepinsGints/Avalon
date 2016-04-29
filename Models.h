#ifndef Models_H
#define Models_H

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
class Models {
public:
	Models() {
		
	}
	~Models() {


	}

	void setId(int _id) {
		id = _id;
	}
	int getId() {
		return id;
	}
	void setName(std::string _name) {
		name = _name;
	}
	std::string getName() {
		return name;
	}
	void setMeshName(std::string _meshName) {
		meshName = _meshName;
	}
	std::string getMeshName() {
		return meshName;
	}
	void setDimensions(Ogre::Vector3 _dimensions) {
		dimensions = _dimensions;
	}
	Ogre::Vector3 getDimensions() {
		return dimensions;
	}
	void setType(int _type) {
		type = _type;
	}
	int getType() {
		return type;
	}
private:
	int id;
	std::string name;
	std::string meshName;
	Ogre::Vector3 dimensions;
	int type;

};
#endif