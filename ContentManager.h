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

class ContentManager {

public:
	ContentManager(Ogre::Root* mRoot, Ogre::RenderWindow* mWindow, Ogre::SceneManager* mSceneMgr);
	~ContentManager();

	void loadContent() {
		createLights();
		createCamera();
		createSceneElements();
	}

	Ogre::Camera* getmCamera() {
		return mCamera;
	}

private:
	Ogre::Root* _mRoot;
	Ogre::RenderWindow* _mWindow;
	Ogre::SceneManager* _mSceneMgr;
	Ogre::Camera* mCamera;

	/**************Camera*********************/
	void createCamera() {
		mCamera = _mSceneMgr->createCamera("MainCam");
		mCamera->setPosition(0, 0, 80);
		mCamera->lookAt(0, 0, -300);
		mCamera->setNearClipDistance(5);

		Ogre::Viewport* vp = _mWindow->addViewport(mCamera);
		vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

		mCamera->setAspectRatio(
			Ogre::Real(vp->getActualWidth()) /
			Ogre::Real(vp->getActualHeight()));
	}
	/*************Lights***********************/
	void createLights() {
		_mSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

		Ogre::Light* light = _mSceneMgr->createLight("MainLight");
		light->setPosition(20, 80, 50);

	}
	/*************Scene elements*****************************/
	void createSceneElements() {
		// Create Scene
		Ogre::Entity* ogreEntity = _mSceneMgr->createEntity("ogrehead.mesh");

		Ogre::SceneNode* ogreNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		ogreNode->attachObject(ogreEntity);
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