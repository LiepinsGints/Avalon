#ifndef Designer_H
#define Designer_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <iostream>
#include <string>

#include "OGRE/SdkSample.h"
#include "OGRE/SamplePlugin.h"
#include "NxOgre.h"
#include "critter.h"


using namespace Ogre;
using namespace OgreBites;
using namespace NxOgre;
using namespace Critter;

class Designer {
public:
	Designer(Ogre::SceneManager* mSceneMgr,  Ogre::Camera* mCamera, Ogre::TerrainGroup* mTerrainGroup) {
		_mSceneMgr = mSceneMgr;
		_mCamera = mCamera;
		_mTerrainGroup = mTerrainGroup;
		shapeType = 0;
	}
	~Designer() {


	}
	//Cube representing bounding box
	
	Ogre::Vector3 getCubeDimensions() {
		if(shapeType==1){
			Ogre::AxisAlignedBox aab = objectEntity->getWorldBoundingBox(true);
			float width = aab.getSize().x;
			float height = aab.getSize().y;
			float deep = aab.getSize().z;
			return Ogre::Vector3(width,height,deep);
		}
		else {
			return Ogre::Vector3(0,0,0);
		}
	}
	void scaleCube(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
		objectNode->scale(x, y, z);

	}
	void setCubeSize(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
		Ogre::Vector3 cubeCurrentDimensions = getCubeDimensions();
		Ogre::Real scaleX = 1;
		Ogre::Real scaleY = 1;
		Ogre::Real scaleZ = 1;
		if (x != cubeCurrentDimensions.x && x>0) {
			scaleX = x / cubeCurrentDimensions.x;
		}
		if (y != cubeCurrentDimensions.y && y>0) {
			scaleY = y / cubeCurrentDimensions.y;
		}
		if (z != cubeCurrentDimensions.z && z>0) {
			scaleZ = z / cubeCurrentDimensions.z;
		}
		scaleCube(scaleX, scaleY, scaleZ);
	}
	//Mouse picking
	void mousePos(const OIS::MouseEvent &arg)
	{
		Viewport* vp = _mSceneMgr->getCurrentViewport();

		//Real coordinates
		Ogre::Real x = arg.state.X.abs ;
		Ogre::Real y = arg.state.Y.abs;
		//Normalized coordinates
		Ogre::Real normalX = arg.state.X.abs / Ogre::Real(vp->getActualWidth());
		Ogre::Real normalY = arg.state.Y.abs / Ogre::Real(vp->getActualHeight());

		Ogre::Ray mouseRay =_mCamera->getCameraToViewportRay(normalX, normalY);
		Ogre::TerrainGroup::RayResult result = _mTerrainGroup->rayIntersects(mouseRay);
		mouseShapeMove(result);
		//result.position
		//cubeNode->setPosition
		
		//Show mouse coordinates
			Ogre::String merge = Ogre::StringConverter::toString(x) + " : \n" +
				Ogre::StringConverter::toString(y) + "\n" +
				Ogre::StringConverter::toString(result.position.x) +" : "+ 
			Ogre::StringConverter::toString(result.position.y) +" : "+
			Ogre::StringConverter::toString(result.position.z) + "\n";
		
	}
	//Designer shape controll
	void mouseShapeMove(Ogre::TerrainGroup::RayResult result) {
		if (shapeType == 1) {
			Ogre::Vector3 corectCubePosition(result.position.x, result.position.y + getCubeDimensions().y / 2, result.position.z);
			objectNode->setPosition(corectCubePosition);
		}

	}

	//Get current designer shape
	int getShapeType() {
		return shapeType;
	}

	//creater cube
	void cube() {
		if (shapeType == 0) {
			objectEntity = _mSceneMgr->createEntity("cube.1m.mesh");
			objectNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
			objectNode->attachObject(objectEntity);
			shapeType = 1;
		}
		else {
			objectNode->detachAllObjects();
			shapeType = 0;
		}
	}
	Ogre::Vector3 getCubePos() {
		return objectNode->getPosition();
	}
private:
	Ogre::SceneManager* _mSceneMgr;
	Ogre::Camera* _mCamera;
	Ogre::TerrainGroup* _mTerrainGroup;

	//Cube
	Ogre::Entity* objectEntity;
	Ogre::SceneNode* objectNode;

	//Designer shapes 
	//0-none 1-cube 2-sphere
	int shapeType;
	Ogre::String meshName;

	

};
#endif