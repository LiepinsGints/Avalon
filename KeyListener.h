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
#include "Spawns.h"
#include "Designer.h"
#include "UserInterface.h"
//MyGui
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
#include "MySql.h"

#include "ParticleManager.h"
#include "Sound.h"

class KeyListener:
	public OIS::MouseListener,
	public OIS::KeyListener
{
public:
	KeyListener(Ogre::RenderWindow* mWindow,
		ContentManager* contentManager,
		PhysicsManager* physicsManager,
		AppSettings* appSettings,
		Spawns* spawns,
		Designer* designer,
		UserInterface* userinterface,
		ParticleManager * particleManager,
		Sound * sound);
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
		_designer->mousePos(arg);
		_particleManager->mousePos(arg);
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
		//**********Nx ogre picking*************
		
		//<-- picking end
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
				_spawns->createCube(30,20,10);
				timer->reset();
			}
			break;
		case OIS::KC_N:
			if (timer->getMilliseconds()>1000) {
				
				timer->reset();
			}
			break;
	
			//Character controls
		case OIS::KC_UP:
			_spawns->forward(127);
			forward = true;
			break;

		case OIS::KC_DOWN:
			_spawns->backward(127);
			backward = true;
			break;

		case OIS::KC_LEFT:
			_spawns->left(127);
			left = true;
			break;

		case OIS::KC_RIGHT:
			_spawns->right(127);
			right = true;
			break;
		case OIS::KC_NUMPAD0:
			_spawns->jump(127);
			break;
			/*Editor controls*/
		case OIS::KC_NUMPAD8:
			
			break;

		case OIS::KC_NUMPAD4:
			if (timer->getMilliseconds()>1000 && _designer->getShapeType() == 1) {
				_userinterface->consoleState();
				timer->reset();
			}
			break;

		case OIS::KC_NUMPAD6:
			
			break;

		case OIS::KC_NUMPAD2:
			if (timer->getMilliseconds()>1000 && _designer->getShapeType() == 1) {
				_designer->setLockPosition();
				timer->reset();
			}
			break;
		case OIS::KC_NUMPAD5:
			if (timer->getMilliseconds()>1000 && _designer->getShapeType()==1) {
				_userinterface->createWorldObject();
				timer->reset();
			}
			break;
		//Alternative controls
		case OIS::KC_W:
			_spawns->forward(127);
			forward = true;
			break;
		case OIS::KC_A:
			_spawns->left(127);
			left = true;
			break;
		case OIS::KC_D:
			_spawns->right(127);
			right = true;
			break;
		case OIS::KC_S:
			_spawns->backward(127);
			backward = true;
			break;
		case OIS::KC_SPACE:
			_spawns->jump(127);
			break;
		case OIS::KC_E:
			if (_spawns->getCastTimer()->getMilliseconds()>2000) {
				Ogre::Real mana = _spawns->getMana();
				if(mana>=10){
					_particleManager->createSpellMouse(_spawns->getCharacter()->getPosition(), 2000,20,"fireCastPlayer",0);
					_spawns->setMana(_spawns->getMana() - 10);
					_userinterface->updateUserFrame();
					_spawns->resetCastTimer();
					_sound->playCastAudio("L_BAZOO.wav", false);
				}
			}
			break;
		case OIS::KC_R:
			if (_spawns->getCastTimer()->getMilliseconds()>2000 && _spawns->getScore()>=200) {
				Ogre::Real mana = _spawns->getMana();
				if (mana >= 30) {
					_particleManager->createSpellMouse(_spawns->getCharacter()->getPosition(), 2000, 30, "greenBolt", 0);
					_spawns->setMana(_spawns->getMana() - 30);
					_userinterface->updateUserFrame();
					_spawns->resetCastTimer();
					_sound->playCastAudio("L_FLAME.wav", false);
				}
			}
			break;
		default:
			break;
		}
		_spawns->applyHelper();
		MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);
		return true;
	}

	bool keyReleased(const OIS::KeyEvent &arg)
	{
		soundStopMovement();
		switch (arg.key)
		{
		case OIS::KC_ESCAPE:
			//_appSettings->setRender(false);
			_userinterface->showHideMainMenu();
			break;

		case OIS::KC_B:
			if (timer->getMilliseconds()>500) {
				_spawns->createCube(30,20,10);
				timer->reset();
			}
			break;
			//Character controls
		case OIS::KC_UP:
			_spawns->forward(0);
			forward = false;
			break;

		case OIS::KC_DOWN:
			_spawns->backward(0);
			backward = false;
			break;

		case OIS::KC_LEFT:
			_spawns->left(0);
			left = false;
			break;

		case OIS::KC_RIGHT:
			_spawns->right(0);
			right = false;
			break;
		case OIS::KC_NUMPAD0:
			_spawns->jump(0);
			break;
			//Alternative controls
		case OIS::KC_W:
			_spawns->forward(0);
			forward = false;
			break;
		case OIS::KC_A:
			_spawns->left(0);
			right = false;
			break;
		case OIS::KC_D:
			_spawns->right(0);
			left = false;
			break;
		case OIS::KC_S:
			_spawns->backward(0);
			backward = false;
			break;
		case OIS::KC_SPACE:
			_spawns->jump(0);
			break;

		default:
			break;
		}
		_spawns->applyHelper();
		

		MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
		return true;
	}

	void soundStopMovement() {
		if (forward == false && left == false && right == false && backward == false) {
			_sound->disableMovement();
		}
		else {
			_sound->playerMovement("FT_GRASS.wav");
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
	AppSettings* _appSettings;
	Spawns* _spawns;
	Designer* _designer;
	UserInterface * _userinterface;
	MySql * mySql;
	ParticleManager * _particleManager;
	Sound * _sound;
	//
	bool forward;
	bool left;
	bool right;
	bool backward;

	
};

KeyListener::KeyListener(Ogre::RenderWindow* mWindow, ContentManager* contentManager, PhysicsManager* physicsManager, AppSettings* appSettings, Spawns* spawns, Designer* designer, UserInterface * userinterface, ParticleManager * particleManager, Sound * sound)
{
	//init scene variables
	_mWindow = mWindow;
	_contentManager = contentManager;
	_physicsManager = physicsManager;
	_appSettings = appSettings;
	_spawns = spawns;
	_designer = designer;
	_userinterface = userinterface;
	_particleManager = particleManager;
	_sound = sound;
	mySql = new MySql(_appSettings);
	mySql->mySqlConnect();
	//Init timer
	timer = new Ogre::Timer();
	timer->reset();
	//
	forward = false;
	left = false;
	right = false;
	backward = false;
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