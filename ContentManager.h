#ifndef ScenaManager_H
#define ScenaManager_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include <OgreMeshManager.h>

#include "TerrainGen.h"


class ContentManager {

public:
	ContentManager(Ogre::Root* mRoot, Ogre::RenderWindow* mWindow, Ogre::SceneManager* mSceneMgr);
	~ContentManager();

	void loadContent() {
		createLights();
		createCamera();
		createSceneElements();
		//createSky();
	}

	Ogre::Camera* getmCamera() {
		return mCamera;
	}

	TerrainGen * getTerrainGen() {
		return terrainGen;
	}

	

private:
	Ogre::Root* _mRoot;
	Ogre::RenderWindow* _mWindow;
	Ogre::SceneManager* _mSceneMgr;
	Ogre::Camera* mCamera;
	TerrainGen * terrainGen;
	//Spawns * spawns;
	//Lights
	Ogre::Light* light;
	/**************Camera*********************/
	void createCamera() {
		mCamera = _mSceneMgr->createCamera("MainCam");
		mCamera->setPosition(0, 10, -30);
		mCamera->lookAt(0, 5, 0);
		mCamera->setNearClipDistance(5);
		mCamera->setFarClipDistance(1000);

		Ogre::Viewport* vp = _mWindow->addViewport(mCamera);
		vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
		vp->setOverlaysEnabled(true);
		mCamera->setAspectRatio(
			Ogre::Real(vp->getActualWidth()) /
			Ogre::Real(vp->getActualHeight()));
	}
	/*************Lights***********************/
	void createLights() {
		_mSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

		light = _mSceneMgr->createLight("MainLight");
		light->setPosition(0, 180, 0);

		/*Ogre::Light* light2 = _mSceneMgr->createLight();
		light2->setType(Ogre::Light::LT_POINT);
		light2->setPosition(-10, 40, 20);
		light2->setSpecularColour(Ogre::ColourValue::White);
		*/
	}
	void createSky() {
		_mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	}
	
	/*************Scene elements*****************************/
	void createSceneElements() {
	
		Ogre::ColourValue background = Ogre::ColourValue(16.f / 255.f, 16.f / 255.f, 16.f / 255.f);
		_mSceneMgr->setFog(Ogre::FOG_EXP, background, 0.001, 800, 1000);


		// set shadow properties
		_mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
		_mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
		_mSceneMgr->setShadowTextureSize(1024);
		_mSceneMgr->setShadowTextureCount(1);


		//Create terrain	
		terrainGen = new TerrainGen(_mRoot, _mWindow, mCamera, _mSceneMgr, light);
		terrainGen->loadTerrain();
		

	}
};
ContentManager::ContentManager(Ogre::Root* mRoot, Ogre::RenderWindow* mWindow, Ogre::SceneManager* mSceneMgr) {
	//init local variables
	_mRoot = mRoot;
	_mWindow = mWindow;
	_mSceneMgr = mSceneMgr;
	
}
ContentManager::~ContentManager() {

}

#endif