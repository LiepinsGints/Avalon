#ifndef UserInterface_H
#define UserInterface_H

//MyGui
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

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

#include "AppSettings.h"

class UserInterface {
public:
	UserInterface(Ogre::RenderWindow* mWindow, Ogre::SceneManager* mSceneMgr, AppSettings* appSettings) {
		_mWindow = mWindow;
		_mSceneMgr = mSceneMgr;
		_appSettings = appSettings;

		mPlatform = new MyGUI::OgrePlatform();
		mPlatform->initialise(_mWindow, _mSceneMgr);
		mGUI = new MyGUI::Gui();
		mGUI->initialise();

		MyGUI::ButtonPtr button = mGUI->createWidget<MyGUI::Button>("Button", 384, 40, 300, 26, MyGUI::Align::Default, "Main");
		button->setCaption("exit");

		button->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::closeApp);
	}

	~UserInterface() {


	}

	
private:
	Ogre::RenderWindow* _mWindow;
	Ogre::SceneManager* _mSceneMgr;

	MyGUI::Gui* mGUI;
	MyGUI::OgrePlatform* mPlatform;

	AppSettings* _appSettings;
	
	void closeApp(MyGUI::Widget* _widget)
	{
		_appSettings->setRender(false);
	}
};

#endif
