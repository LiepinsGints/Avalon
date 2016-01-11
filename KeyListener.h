#ifndef KEYLISTENER_H
#define KEYLISTENER_H

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreException.h>
#include <OgreConfigFile.h>
#include "OgreRenderWindow.h"
#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include <OgreCamera.h>
#include "OgreViewport.h"
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <Ogre.h>
#include <OgreTimer.h>

#include "ContentManager.h"
#include "PhysicsManager.h"

class KeyListener
{
public:
	KeyListener(Ogre::RenderWindow* mWindow, ContentManager* contentManager, PhysicsManager* physicsManager);
	~KeyListener();
	//Listen for key or mouse input
	bool listen(const Ogre::FrameEvent& fe) {
		mKeyboard->capture();
		mMouse->capture();

		//Close program
		if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
		//Create some cube
		if (mKeyboard->isKeyDown(OIS::KC_B) && timer->getMilliseconds()>500) {
			_physicsManager->createCube();
			timer->reset();
		}
		//Character controls
		_physicsManager->resetHelper();
		if (mKeyboard->isKeyDown(OIS::KC_UP)) {			
			_physicsManager->backward();
		}
		if (mKeyboard->isKeyDown(OIS::KC_DOWN)) {
			_physicsManager->forward();
		}
		if (mKeyboard->isKeyDown(OIS::KC_LEFT) ) {
			_physicsManager->left();
		}
		if (mKeyboard->isKeyDown(OIS::KC_RIGHT)) {
			_physicsManager->right();
		}
		if (mKeyboard->isKeyDown(OIS::KC_NUMPAD0)) {
			_physicsManager->jump();
		}
	
		_physicsManager->applyHelper();
		return true;
	}

	void destroyInputs() {
		if (mInputMgr)
		{
			mInputMgr->destroyInputObject(mMouse);
			mInputMgr->destroyInputObject(mKeyboard);

			OIS::InputManager::destroyInputSystem(mInputMgr);
			mInputMgr = 0;
		}
	}
private:
	OIS::InputManager* mInputMgr;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse* mMouse;

	Ogre::RenderWindow* _mWindow;
	Ogre::Timer* timer;

	ContentManager* _contentManager;
	PhysicsManager* _physicsManager;
	
	//Character states
	bool forward;
	bool backward;
	bool left;
	bool right;
};

KeyListener::KeyListener(Ogre::RenderWindow* mWindow, ContentManager* contentManager, PhysicsManager* physicsManager)
{
	//init scene variables
	_mWindow = mWindow;
	_contentManager = contentManager;
	_physicsManager = physicsManager;
	//Init timer
	timer = new Ogre::Timer();
	timer->reset();
	//init key states
	forward = false;
	backward = false;
	left = false;
	right = false;
	//Init OIS
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHandle = 0;
	std::ostringstream windowHandleStr;

	mWindow->getCustomAttribute("WINDOW", &windowHandle);
	windowHandleStr << windowHandle;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));

	mInputMgr = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(
		mInputMgr->createInputObject(OIS::OISKeyboard, false));
	mMouse = static_cast<OIS::Mouse*>(
		mInputMgr->createInputObject(OIS::OISMouse, false));
}

KeyListener::~KeyListener()
{
}



#endif