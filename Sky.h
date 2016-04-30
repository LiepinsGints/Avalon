#ifndef Sky_H
#define Sky_H

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

#include <Caelum.h>
#include <CaelumSystem.h>

using namespace Ogre;
class Sky {
public:
	Sky(Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera, Ogre::Root* mRoot, Ogre::RenderWindow* mWindow) {
		_mSceneMgr = mSceneMgr;
		_mCamera = mCamera;
		_mRoot = mRoot;
		_mWindow = mWindow;
		
	    mCaelumSystem = new Caelum::CaelumSystem(_mRoot, _mSceneMgr, Caelum::CaelumSystem::CAELUM_COMPONENTS_DEFAULT);
		mCaelumSystem->setSceneFogDensityMultiplier(0.00008f); // or some other small value.
	//	mCaelumSystem->setManageSceneFog(true);
		_mRoot->addFrameListener(mCaelumSystem);
		_mWindow->addListener(mCaelumSystem);
	}
	~Sky() {


	}

	
private:
	
	Caelum::CaelumSystem* mCaelumSystem;

	Ogre::SceneManager* _mSceneMgr;
	Ogre::Camera* _mCamera;
	Ogre::Root* _mRoot;
	Ogre::RenderWindow* _mWindow;




};
#endif