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

#include "UserInterface.h"

using namespace Ogre;
using namespace OgreBites;
using namespace NxOgre;
using namespace Critter;

class Designer {
public:
	Designer(Ogre::SceneManager* mSceneMgr, UserInterface *  userInterface, Ogre::Camera* mCamera, Ogre::TerrainGroup* mTerrainGroup) {
		_mSceneMgr = mSceneMgr;
		_userInterface = userInterface;
		_mCamera = mCamera;
		_mTerrainGroup = mTerrainGroup;
	}
	~Designer() {


	}

	void cube() {
		Ogre::Entity* cubeEntity = _mSceneMgr->createEntity("cube.1m.mesh");

		cubeNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		cubeNode->attachObject(cubeEntity);
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

		//result.position
		//cubeNode->setPosition
		cubeNode->setPosition(result.position);
		//Show mouse coordinates
			Ogre::String merge = Ogre::StringConverter::toString(x) + " : \n" +
				Ogre::StringConverter::toString(y) + "\n" +
				Ogre::StringConverter::toString(result.position.x) +" : "+ 
			Ogre::StringConverter::toString(result.position.y) +" : "+
			Ogre::StringConverter::toString(result.position.z) + "\n";
		_userInterface->setLabelCaption2(merge);
		
	}

	
private:
	Ogre::SceneManager* _mSceneMgr;
	UserInterface *  _userInterface;
	Ogre::Camera* _mCamera;
	Ogre::TerrainGroup* _mTerrainGroup;

	//Cube
	Ogre::SceneNode* cubeNode;


};
#endif