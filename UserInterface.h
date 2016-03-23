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
#include "Designer.h"

class UserInterface {
public:
	UserInterface(Ogre::RenderWindow* mWindow, Ogre::SceneManager* mSceneMgr, AppSettings* appSettings, Designer *designer) {
		_mWindow = mWindow;
		_mSceneMgr = mSceneMgr;
		_appSettings = appSettings;
		_designer = designer;

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
	//Set box size
	void setBoundingSize() {

		
	}
private:
	Ogre::RenderWindow* _mWindow;
	Ogre::SceneManager* _mSceneMgr;
	Designer* _designer;

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
	MyGUI::Button* closeDesigner;
	MyGUI::Button* createBoundingBox;
	MyGUI::TextBox * textBoxWidth;
	MyGUI::Edit * objectWidth;
	MyGUI::TextBox * textBoxHeight;
	MyGUI::Edit * objectHeight;
	MyGUI::TextBox * textBoxDeep;
	MyGUI::Edit * objectDeep;
	MyGUI::Button* setBoundingBoxDimension;
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
	void scaleDesignerCube(MyGUI::Widget* _widget) {
		//_designer->scaleCube(10, 10, 10);
		_designer->setCubeSize(
			Ogre::StringConverter::parseReal(objectWidth->getCaption()),
			Ogre::StringConverter::parseReal(objectHeight->getCaption()),
			Ogre::StringConverter::parseReal(objectDeep->getCaption())
			);
		;
		
		objectWidth->setCaption("");
		objectWidth->insertText(Ogre::StringConverter::toString(_designer->getCubeDimensions().x));
		
		objectHeight->setCaption("");
		objectHeight->insertText(Ogre::StringConverter::toString(_designer->getCubeDimensions().y));
		
		objectDeep->setCaption("");
		objectDeep->insertText(Ogre::StringConverter::toString(_designer->getCubeDimensions().z));
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
		int buttonCount = 2;
		Ogre::Real buttonWidth = designerWindowWidth * 0.9;
		Ogre::Real buttonHeight = designerWindowHeight / 40;
		Ogre::Real buttonDistance = buttonHeight + buttonHeight / 4;

		Ogre::Real buttonGroupHeight = buttonCount* buttonHeight + buttonDistance*(buttonCount - 1);

		Ogre::Real initPositionX = designerWindowWidth / 2 - buttonWidth / 2;
		Ogre::Real initPositionY =  buttonHeight;

		//close designer button group
		closeDesigner = designerWindow->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		closeDesigner->setCaption("Close designer");
		closeDesigner->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::closeDesignerMenu);

		//Create bounding box group
		createBoundingBox = designerWindow->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY + buttonDistance, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		createBoundingBox->setCaption("Create bounding box");

		//Input width group
		textBoxWidth = designerWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 2, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		textBoxWidth->setCaption("X");
		
		objectWidth = designerWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance*3, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		objectWidth->insertText(Ogre::StringConverter::toString(_designer->getCubeDimensions().x));
		//
		//Input height group
		textBoxHeight = designerWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 4, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		textBoxHeight->setCaption("Y");
		

		objectHeight = designerWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 5, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		objectHeight->insertText(Ogre::StringConverter::toString(_designer->getCubeDimensions().y));
		//Input deep group		
		textBoxDeep = designerWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 6, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		textBoxDeep->setCaption("Z");

		objectDeep = designerWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 7, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		objectDeep->insertText(Ogre::StringConverter::toString(_designer->getCubeDimensions().z));
		//Change box dimensions
		setBoundingBoxDimension = designerWindow->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY + buttonDistance * 8, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		setBoundingBoxDimension->setCaption("Apply");
		setBoundingBoxDimension->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::scaleDesignerCube);

		//objectDeep->getText;
	}

};

#endif
