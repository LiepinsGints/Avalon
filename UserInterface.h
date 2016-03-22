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
		loadUi();
		//label
		label();
		

	}

	~UserInterface() {


	}
	//Change and update label text
	void setLabelCaption(Ogre::String labelText, Ogre::String labelText2, Ogre::String labelText3) {
		const MyGUI::IntPoint mousePos = MyGUI::InputManager::getInstance().getMousePosition();
		Ogre::String merge = labelText +"\n" + 
			labelText2 + "\n"
			+ labelText3 + "\n"
			;
		mLabel->setCaption(merge);
		//mLabel->setVisible(false);

	}
	//mouse
	void setLabelCaption2(Ogre::String labelText) {

		mLabel2->setCaption(labelText);

	}
	//Main menu public functions
	void showHideMainMenu() {
		if (settingsButton->getVisible()) {
			designerButton->setVisible(false);
			settingsButton->setVisible(false);
			graphicsButton->setVisible(false);
			exitButton->setVisible(false);
			closeMenuButton->setVisible(false);
		}
		else {
			designerButton->setVisible(true);
			settingsButton->setVisible(true);
			graphicsButton->setVisible(true);
			exitButton->setVisible(true);
			closeMenuButton->setVisible(true);
		}

	}
	//Designer menu public functions
	void showHideDesignerMenu() {		
		if (designerWindow->getVisible()) {
			designerWindow->setVisible(false);
		}
		else {
			designerWindow->setVisible(true);
		}
	}
private:
	Ogre::RenderWindow* _mWindow;
	Ogre::SceneManager* _mSceneMgr;

	MyGUI::Gui* mGUI;
	MyGUI::OgrePlatform* mPlatform;

	AppSettings* _appSettings;
	//UI elements	
	MyGUI::EditBox* mLabel;
	MyGUI::EditBox* mLabel2;
	
	//Console
	MyGUI::Edit * consoleInput;
	MyGUI::ComboBox* consoleOutput;
	MyGUI::Button* consoleSubmit;
	
	//Main menu elements
	MyGUI::Button* designerButton;
	MyGUI::Button* settingsButton;
	MyGUI::Button* graphicsButton;
	MyGUI::Button* exitButton;
	MyGUI::Button* closeMenuButton;
	//Designer menu elements
	MyGUI::Window* designerWindow;
	/**Close structure***/
	void loadUi() {		
		createMainMenu();
		createDesignerMenu();
		//test
		//Console input
		consoleInput = mGUI->createWidget<MyGUI::EditBox>("EditBoxStretch",
			 _appSettings->getWidth()*0.01,
			_appSettings->getHeight()-30,
			260, 26, MyGUI::Align::Default, "Main");
		//Console output
		consoleOutput = mGUI->createWidget<MyGUI::ComboBox>("EditBoxStretch",
			_appSettings->getWidth()*0.01,
			_appSettings->getHeight() -180,
			300, 150, MyGUI::Align::Default, "Main");
		//Console button
		consoleSubmit = mGUI->createWidget<MyGUI::Button>("Button", 
			_appSettings->getWidth()*0.01+260,
			_appSettings->getHeight() - 30,
			40, 26, MyGUI::Align::Default, "Main");
		consoleSubmit->setCaption("GO...");
		//Label 2
		mLabel2 = MyGUI::Gui::getInstance().createWidget<MyGUI::EditBox>("EditBoxStretch", MyGUI::IntCoord(10, 150, 250, 100), MyGUI::Align::Default, "Overlapped");
		mLabel2->setCaption("some edit");
		mLabel2->setTextAlign(MyGUI::Align::Center);
		mLabel2->setEditMultiLine(true);

	}
	void closeApp(MyGUI::Widget* _widget)
	{
		_appSettings->setRender(false);
	}
	void closeMainMenu(MyGUI::Widget* _widget) {
		showHideMainMenu();
	}
	void closeDesignerMenu(MyGUI::Widget* _widget) {
		showHideDesignerMenu();
	}
	/****Label block****/
	void label() {
		mLabel = MyGUI::Gui::getInstance().createWidget<MyGUI::EditBox>("EditBoxStretch", MyGUI::IntCoord(10, 10, 250, 100), MyGUI::Align::Default, "Overlapped");
		mLabel->setCaption("some edit");
		mLabel->setTextAlign(MyGUI::Align::Center);
		mLabel->setEditMultiLine(true);
	}
	/*Test*/

	/**********MAIN MENU***********/
	void createMainMenu() {
		//settingsButton;
		//graphicsButton;
		//exitButton;

		int buttonCount = 5;
		Ogre::Real buttonWidth = _appSettings->getWidth() / 5;
		Ogre::Real buttonHeight = _appSettings->getHeight() / 40;
		Ogre::Real buttonDistance = buttonHeight+ buttonHeight / 2;

		Ogre::Real buttonGroupHeight = buttonCount* buttonHeight + buttonDistance*(buttonCount-1);

		Ogre::Real initPositionX = _appSettings->getWidth() / 2- buttonWidth/2;
		Ogre::Real initPositionY = _appSettings->getHeight() / 2- buttonGroupHeight/2;
		
		//Show designer
		
		designerButton = mGUI->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		designerButton->setCaption("Show/Hide designer");
		designerButton->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::closeDesignerMenu);
		designerButton->setVisible(false);
		//create settings button
		settingsButton = mGUI->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY + buttonDistance, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		settingsButton->setCaption("Settings");
		settingsButton->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::closeApp);
		settingsButton->setVisible(false);
		//create graphics button
		graphicsButton = mGUI->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY+buttonDistance*2, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		graphicsButton->setCaption("Graphics");
		graphicsButton->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::closeApp);
		graphicsButton->setVisible(false);
		//create exit button
		exitButton = mGUI->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY+buttonDistance*3, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		exitButton->setCaption("Exit");
		exitButton->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::closeApp);
		exitButton->setVisible(false);
		//Close menu button
		closeMenuButton = mGUI->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY + buttonDistance * 4, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		closeMenuButton->setCaption("Close main menu");
		closeMenuButton->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::closeMainMenu);
		closeMenuButton->setVisible(false);
	}
	/**********Designer MENU***********/
	void createDesignerMenu() {
		Ogre::Real designerWindowWidth = _appSettings->getWidth() / 6;
		Ogre::Real designerWindowHeight = _appSettings->getHeight();
		Ogre::Real windowPosX = _appSettings->getWidth() - designerWindowWidth;

		designerWindow = mGUI->createWidget<MyGUI::Window>("Window", windowPosX, 0, designerWindowWidth, designerWindowHeight, MyGUI::Align::Default, "Main");
		designerWindow->setCaption("Designer menu");
		designerWindow->setVisible(false);
		//Design menu buttons
		int buttonCount = 5;
		Ogre::Real buttonWidth = designerWindowWidth * 0.9;
		Ogre::Real buttonHeight = designerWindowHeight / 40;
		Ogre::Real buttonDistance = buttonHeight + buttonHeight / 2;

		Ogre::Real buttonGroupHeight = buttonCount* buttonHeight + buttonDistance*(buttonCount - 1);

		Ogre::Real initPositionX = designerWindowWidth / 2 - buttonWidth / 2;
		Ogre::Real initPositionY =  buttonHeight;

		MyGUI::Button* closeDesigner;
		closeDesigner = designerWindow->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		closeDesigner->setCaption("Close designer");
		closeDesigner->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::closeDesignerMenu);

		MyGUI::Button* createBoundingBox;
		createBoundingBox = designerWindow->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY + buttonDistance, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		createBoundingBox->setCaption("Create bounding box");
	}

};

#endif
