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
#include "AppSettings.h"

//MyGui
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

class KeyListener:
	public OIS::MouseListener,
	public OIS::KeyListener
{
public:
	KeyListener(Ogre::RenderWindow* mWindow, ContentManager* contentManager, PhysicsManager* physicsManager, AppSettings* appSettings);
	~KeyListener();
	//Listen for key or mouse input
	bool listen(const Ogre::FrameEvent& fe) {
		mKeyboard->capture();
		mMouse->capture();		

		mMouse->setEventCallback(this);
		mKeyboard->setEventCallback(this);

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
	//MyGui key listener
	bool mouseMoved(const OIS::MouseEvent &arg)
	{
		MyGUI::InputManager::getInstance().injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
		return true;
	}

	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
		return true;
	}

	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		MyGUI::InputManager::getInstance().injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
		return true;
	}

	bool keyPressed(const OIS::KeyEvent &arg)
	{
		switch (arg.key)
		{
		case OIS::KC_ESCAPE:
			//render = false;
			break;

		case OIS::KC_B:
			if (timer->getMilliseconds()>1000) {
				_physicsManager->createCube();
				timer->reset();
			}
			break;
			//Character controls
		case OIS::KC_UP:
			_physicsManager->forward(127);
			break;

		case OIS::KC_DOWN:
			_physicsManager->backward(127);
			break;

		case OIS::KC_LEFT:
			_physicsManager->left(127);
			break;

		case OIS::KC_RIGHT:
			_physicsManager->right(127);
			break;
		case OIS::KC_NUMPAD0:
			_physicsManager->jump(127);
			break;
		default:
			break;
		}
		_physicsManager->applyHelper();
		MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);
		return true;
	}

	bool keyReleased(const OIS::KeyEvent &arg)
	{
		switch (arg.key)
		{
		case OIS::KC_ESCAPE:
			_appSettings->setRender(false);
			break;

		case OIS::KC_B:
			if (timer->getMilliseconds()>500) {
				_physicsManager->createCube();
				timer->reset();
			}
			break;
			//Character controls
		case OIS::KC_UP:
			_physicsManager->forward(0);
			break;

		case OIS::KC_DOWN:
			_physicsManager->backward(0);
			break;

		case OIS::KC_LEFT:
			_physicsManager->left(0);
			break;

		case OIS::KC_RIGHT:
			_physicsManager->right(0);
			break;
		case OIS::KC_NUMPAD0:
			_physicsManager->jump(0);
			break;
		default:
			break;
		}
		_physicsManager->applyHelper();

		MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
		return true;
	}



private:
	OIS::InputManager* mInputMgr;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse* mMouse;

	Ogre::RenderWindow* _mWindow;
	Ogre::Timer* timer;

	ContentManager* _contentManager;
	PhysicsManager* _physicsManager;
	AppSettings* _appSettings;
	
};

KeyListener::KeyListener(Ogre::RenderWindow* mWindow, ContentManager* contentManager, PhysicsManager* physicsManager, AppSettings* appSettings)
{
	//init scene variables
	_mWindow = mWindow;
	_contentManager = contentManager;
	_physicsManager = physicsManager;
	_appSettings = appSettings;
	//Init timer
	timer = new Ogre::Timer();
	timer->reset();
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
		mInputMgr->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(
		mInputMgr->createInputObject(OIS::OISMouse, true));

	//Dimensions
	const OIS::MouseState &mouseState = mMouse->getMouseState(); // mMouse is type of OIS::Mouse*
	mouseState.width = 1024; // your rendering area width
	mouseState.height = 768; // your rendering area height
}

KeyListener::~KeyListener()
{
}



#endif