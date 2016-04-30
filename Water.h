#ifndef Water_H
#define Water_H

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

#include <Hydrax.h>
#include <Modules/ProjectedGrid/ProjectedGrid.h>


using namespace Ogre;
class Water {
public:
	Water(Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera, Ogre::Root* mRoot, Ogre::RenderWindow* mWindow) {
		_mSceneMgr = mSceneMgr;
		_mCamera = mCamera;
		_mRoot = mRoot;
		_mWindow = mWindow;
		//
		//G:/OgreSDK/media/Hydrax
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("G:\\OgreSDK\\media\\Hydrax", "FileSystem", "Hydrax", true);
		//
		/*mHydrax = new Hydrax::Hydrax(_mSceneMgr, _mCamera, _mWindow->getViewport(0));
		Hydrax::Module::ProjectedGrid* mModule = new Hydrax::Module::ProjectedGrid( 
			mHydrax,  
			new Hydrax::Noise::Perlin(),
			Ogre::Plane(Ogre::Vector3(0, 1, 0), Ogre::Vector3(0, 0, 0)),  
			Hydrax::MaterialManager::NM_VERTEX,  
			Hydrax::Module::ProjectedGrid::Options(64));  
		mHydrax->setModule(mModule);
		mHydrax->loadCfg("HydraxDemo.hdx");
		mHydrax->create();
	*/
		mHydrax = new Hydrax::Hydrax(mSceneMgr, _mCamera, mWindow->getViewport(0));

		Hydrax::Module::ProjectedGrid * module = new Hydrax::Module::ProjectedGrid(mHydrax, new Hydrax::Noise::Perlin(/*Hydrax::Noise::Perlin::Options(4,0.0425,0.809,1.54,1.67,8,Ogre::Vector3(0.5,50,150000))Hydrax::Noise::Perlin::Options(18,0.032f,0.24,1.8f,1.32f)*/), Ogre::Plane(Ogre::Vector3(0, 1, 0), Ogre::Vector3(0, 0, 0)), Hydrax::MaterialManager::NM_VERTEX, Hydrax::Module::ProjectedGrid::Options(/*106,45,50,true*/));

		mHydrax->setModule(static_cast<Hydrax::Module::Module *>(module));

		mHydrax->loadCfg("Avalon.hdx");

		mHydrax->create();


	}
	~Water() {


	}


private:

	Hydrax::Hydrax * mHydrax;

	Ogre::SceneManager* _mSceneMgr;
	Ogre::Camera* _mCamera;
	Ogre::Root* _mRoot;
	Ogre::RenderWindow* _mWindow;




};
#endif