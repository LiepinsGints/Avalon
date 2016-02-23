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

		//Button
		closeButton();
		//label
		label();
		

	}

	~UserInterface() {


	}
	//Change and update label text
	void setLabelCaption(Ogre::String labelText, Ogre::String labelText2, Ogre::String labelText3) {
		Ogre::String merge = labelText +"\n" + labelText2 + "\n"+ labelText3 + "\n";
		mLabel->setCaption(merge);

	}
	

	
private:
	Ogre::RenderWindow* _mWindow;
	Ogre::SceneManager* _mSceneMgr;

	MyGUI::Gui* mGUI;
	MyGUI::OgrePlatform* mPlatform;

	MyGUI::EditBox* mLabel;

	AppSettings* _appSettings;
	
	/**Close structure***/
	void closeButton() {
		MyGUI::ButtonPtr button = mGUI->createWidget<MyGUI::Button>("Button", 384, 40, 300, 26, MyGUI::Align::Default, "Main");
		button->setCaption("exit");
		button->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::closeApp);
	}
	void closeApp(MyGUI::Widget* _widget)
	{
		_appSettings->setRender(false);
	}
	/****Label block****/
	void label() {
		mLabel = MyGUI::Gui::getInstance().createWidget<MyGUI::EditBox>("EditBoxStretch", MyGUI::IntCoord(10, 10, 200, 100), MyGUI::Align::Default, "Overlapped");
		mLabel->setCaption("some edit");
		mLabel->setTextAlign(MyGUI::Align::Center);
		mLabel->setEditMultiLine(true);
	}



};

#endif