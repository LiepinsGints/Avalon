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
#include "MySql.h"
#include "ParticleManager.h"
#include "Sound.h"
#include "DesignerObjects.h"
class UserInterface {
public:
	UserInterface(Ogre::RenderWindow* mWindow, Ogre::SceneManager* mSceneMgr, AppSettings* appSettings, Designer *designer, MySql * mySql, Spawns* spawns, ParticleManager * particleManager, Sound * sound) {
		_mWindow = mWindow;
		_mSceneMgr = mSceneMgr;
		_appSettings = appSettings;
		_designer = designer;
		_mySql = mySql;
		_spawns = spawns;
		_particleManager = particleManager;
		_sound = sound;
		mPlatform = new MyGUI::OgrePlatform();
		mPlatform->initialise(_mWindow, _mSceneMgr);
		mGUI = new MyGUI::Gui();
		mGUI->initialise();
		//Rot
		rotX = 0;
		rotY = 0;
		rotZ = 0;
		//Button
		loadUi();
		getMeshList();
		//
		lineNumber = 0;
		consoleEnable = false;
		//label
		label();
		

	}

	~UserInterface() {


	}
	//Console
	void addLineToConsole(Ogre::String line) {
		if(consoleEnable==true){
			consoleOutput->addText(Ogre::StringConverter::toString(lineNumber) +line+"\n");
			lineNumber++;
		}
	}
	void clearConsole() {
		consoleOutput->removeAllItems();
		consoleOutput->deleteAllItems();
		consoleOutput->clearUserStrings();
	}
	void consoleState() {
		if (consoleEnable == false) {
			consoleEnable = true;
		}
		else {
			consoleEnable = false;
		}
	}
	void disableTestInterface() {
		//Disable test interface
		mLabel->setVisible(false);
		mLabel2->setVisible(false);
		consoleInput->setVisible(false);
		consoleOutput->setVisible(false);
		consoleSubmit->setVisible(false);
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
	//Mesh list create
	void getMeshList() {
		_mySql->getModelList(meshList);
	}
	//Create world item
	void createWorldObject() {
		//
		DesignerObjects * designerObject = new DesignerObjects();
		_spawns->settCurrentDesignerObject(designerObject);
		//
		Ogre::Vector3 pos = _designer->getCubePos();
		Ogre::Vector3 scale = _designer->getShapeScale();
		Ogre::Vector3 shapeRot = _designer->getShapeRotation();
		_designer->resetRotation();
		Ogre::Vector3 dimensions = _designer->getCubeDimensions();
		float scaleGlobal = 1;
		float mass = 10;
		int typePhys=0;
		int id = -1;
		if (meshTypes->getIndexSelected() == MyGUI::ITEM_NONE) {
			typePhys = 0;
		}
		else {
			Ogre::String selectResult= meshTypes->getItemNameAt(meshTypes->getIndexSelected());
			
			if (selectResult=="Walls")
			{
				typePhys = 0;
			}
			else if(selectResult=="Objects")
			{
				typePhys = 1;
			}
			else if (selectResult == "Mesh only")
			{
				typePhys = 2;
			}
			else if (selectResult == "Bound only")
			{
				typePhys = 3;
			}
			designerObject->setType(typePhys);
		}
		if(typePhys==0 || typePhys == 1 || typePhys == 2){
		//Spawn before bound so we can use id
			_mySql->createSpawn(
				getSelectedShapeName(),
				pos.x,
				pos.y,
				pos.z,
				scale.x,
				scale.y,
				scale.z,
				1,//scale
				shapeRot,
				100,//weight
				0//type
				);
			id = _mySql->getLastInsertID();
			designerObject->setWorldId(id);

		}
		if(typePhys==0 || typePhys == 1 || typePhys == 3){
		//Save object data in DB
			int lastInsert = -1;
			if (typePhys == 0 || typePhys == 1) {
				lastInsert = _mySql->getLastInsertID();
			}
			_mySql->createBound(
				pos.x,
				pos.y,
				pos.z,
				dimensions.x,
				dimensions.y,
				dimensions.z,
				shapeRot,
				typePhys,
				lastInsert
				);
			designerObject->setBoundId(_mySql->getLastInsertID());
		}
		//_designer->addShapeToScene();
		//
		//Show created object in scene
		if (typePhys == 0 || typePhys == 1){
			_spawns->createObjectBoxDescription(
				id,
				getSelectedShapeName(),
				pos,
				scale,
				dimensions,
				scaleGlobal,
				shapeRot,
				mass,
				typePhys);
		}
		else if (typePhys == 2) {
			_spawns->createMeshOnly(
				id,
				getSelectedShapeName(),
				pos,
				scale,
				shapeRot
				);
		}
		else if(typePhys == 3){
			_spawns->createBoundingBox(
				pos,
				dimensions,
				shapeRot
				);
		}
	
		designerObjects.push_back(designerObject);
	}
	//update user frame
	void updateUserFrame() {
		Ogre::Real userWindowWidth = _appSettings->getWidth() / 4;
		Ogre::Real userWindowHeight = _appSettings->getHeight() / 6;

		Ogre::Real manaHpWidth = userWindowWidth / 2;
		Ogre::Real manaHpHeight = userWindowHeight / 8;

		Ogre::Real currentHp = (manaHpWidth / _appSettings->getPlayerHealth()) * _spawns->getHealth();
		Ogre::Real currentMana = (manaHpWidth / _appSettings->getPlayerMana()) * _spawns->getMana();

		userHealth->setSize(currentHp, userHealth->getHeight());
		//mana

		userMana->setSize(currentMana, userMana->getHeight());

		//
		if(_spawns->getScore()>=200){
			greenBallImage->setVisible(true);
		}
	}
	//Score update
	void updateScore() {
		playerScore->setCaption("Score: " + Ogre::StringConverter::toString(_spawns->getScore()));
	}
private:
	Ogre::RenderWindow* _mWindow;
	Ogre::SceneManager* _mSceneMgr;
	Designer* _designer;
	Spawns* _spawns;
	MySql * _mySql;

	MyGUI::Gui* mGUI;
	MyGUI::OgrePlatform* mPlatform;

	Ogre::Real rotX;
	Ogre::Real rotY;
	Ogre::Real rotZ;

	AppSettings* _appSettings;
	ParticleManager * _particleManager;
	//Sound
	Sound * _sound;
	//UI elements	
	MyGUI::EditBox* mLabel;
	MyGUI::EditBox* mLabel2;
	//User frame
	//PlayerImage.png
	MyGUI::ScrollView* userWindow;
	MyGUI::ImageBox * playerImage;
	MyGUI::TextBox * textBoxHealth;
	MyGUI::ScrollView* userHealth;
	MyGUI::ScrollView* userHealthSub;
	MyGUI::TextBox * textBoxMana;
	MyGUI::ScrollView* userMana;
	MyGUI::ScrollView* userManaSub;
	//Score
	MyGUI::TextBox * playerScore;
	//Cast Bar
	MyGUI::Window* castBar;
	MyGUI::ImageBox * fireBallImage;
	MyGUI::ImageBox * greenBallImage;
	//Console
	MyGUI::Edit * consoleInput;
	MyGUI::ComboBox* consoleOutput;
	MyGUI::Button* consoleSubmit;
	int lineNumber;
	bool consoleEnable;

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
	MyGUI::ListBox * meshList;
	MyGUI::TextBox * textBoxRotX;
	MyGUI::Edit * objectRotX;
	MyGUI::TextBox * textBoxRotY;
	MyGUI::Edit * objectRotY;
	MyGUI::TextBox * textBoxRotZ;
	MyGUI::Edit * objectRotZ;
	MyGUI::Button* setRotation;

	MyGUI::TextBox * textBoxShapeOffsetY;
	MyGUI::Edit * objectShapeOffsetY;
	MyGUI::Button* setShapeOffsetY;

	MyGUI::Button* undo;

	MyGUI::ListBox * meshTypes;

	//
	std::vector<DesignerObjects*> designerObjects;
	/**Close structure***/
	void loadUi() {	
		
		//
		createUserFrame();
		createMainMenu();
		createDesignerMenu();
		createPlayerScore();
		createCastBar();
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
		mLabel2 = MyGUI::Gui::getInstance().createWidget<MyGUI::EditBox>("EditBoxStretch", MyGUI::IntCoord(10, 40, 250, 100), MyGUI::Align::Default, "Overlapped");
		mLabel2->setCaption("some edit");
		mLabel2->setTextAlign(MyGUI::Align::Center);
		mLabel2->setEditMultiLine(true);

	}
	//MY Gui 
	std::string getSelectedShapeName() {
		if (meshList->getIndexSelected() == MyGUI::ITEM_NONE) {
			return("cube.1m.mesh");
		}
		else {
			mLabel2->setCaption(meshList->getItemNameAt(meshList->getIndexSelected()));
			return(meshList->getItemNameAt(meshList->getIndexSelected()));
		}
	}
	void showShape(MyGUI::Widget* _widget) {
			createShape(getSelectedShapeName());		
	}
	//
	void undoAddition(MyGUI::Widget* _widget) {
		if (designerObjects.size() > 0) {
			_spawns->undoDesignerObject(designerObjects.back());
			switch (designerObjects.back()->getType())
			{
			case 0:
				_mySql->deleteById(designerObjects.back()->getWorldId(), "world");
				_mySql->deleteById(designerObjects.back()->getBoundId(), "bounds");
				break;
			case 1:
				_mySql->deleteById(designerObjects.back()->getWorldId(), "world");
				_mySql->deleteById(designerObjects.back()->getBoundId(), "bounds");

				break;
			case 2:
				_mySql->deleteById(designerObjects.back()->getWorldId(), "world");
				break;
			case 3:
				_mySql->deleteById(designerObjects.back()->getBoundId(), "bounds");
				break;
			default:
				break;
			}
			//remowe last element
			designerObjects.pop_back();
		}
		
	}
	//<--test end
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
	void createShape(Ogre::String shapeName) {
		//_designer->cube();
		_designer->createShape(shapeName);
		if (createBoundingBox->getCaption() == "Show shape") {
			objectWidth->setCaption("");
			objectWidth->insertText(Ogre::StringConverter::toString(_designer->getCubeDimensions().x));

			objectHeight->setCaption("");
			objectHeight->insertText(Ogre::StringConverter::toString(_designer->getCubeDimensions().y));

			objectDeep->setCaption("");
			objectDeep->insertText(Ogre::StringConverter::toString(_designer->getCubeDimensions().z));
			createBoundingBox->setCaption("Hide shape");
		}
		else {
			createBoundingBox->setCaption("Show shape");
		}
		
	}
	void scaleDesignerCube(MyGUI::Widget* _widget) {
		//_designer->scaleCube(10, 10, 10);
		if(_designer->getShapeType()==1){
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
	}
	void rotateShape(MyGUI::Widget* _widget) {
		rotX = Ogre::StringConverter::parseReal(objectRotX->getCaption());
		rotY = Ogre::StringConverter::parseReal(objectRotY->getCaption());
		rotZ = Ogre::StringConverter::parseReal(objectRotZ->getCaption());

		_designer->rotateShape(Ogre::Vector3(rotX, rotY,rotX));
	}
	/****Label block****/
	void label() {
		mLabel = MyGUI::Gui::getInstance().createWidget<MyGUI::EditBox>("EditBoxStretch", MyGUI::IntCoord(10, 250, 250, 180), MyGUI::Align::Default, "Overlapped");
		mLabel->setCaption("some edit");
		mLabel->setTextAlign(MyGUI::Align::Center);
		mLabel->setEditMultiLine(true);
	}
	/*Test*/
	void setShapeOffset(MyGUI::Widget* _widget) {
		if (_designer->getShapeType() == 1) {
			_designer->setShapeOffsetY(Ogre::StringConverter::parseReal(objectShapeOffsetY->getCaption()));
		}
	}
	/**********Player score*************/
	void createPlayerScore() {
		Ogre::Real userWindowWidth = _appSettings->getWidth()- _appSettings->getWidth() / 10;
		Ogre::Real userWindowHeight = _appSettings->getHeight() / 6;

		Ogre::Real scoreX = userWindowWidth - userWindowWidth/10;
		Ogre::Real scoreY = userWindowHeight/5;

		Ogre::Real scoreW = userWindowWidth / 5;
		Ogre::Real scoreH = userWindowHeight / 6;


		playerScore = mGUI->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(scoreX, scoreY, scoreW, scoreH), MyGUI::Align::Default, "Overlapped");
		playerScore->setCaption("Score: 0");
		playerScore->setFontHeight(20);

	}
	/*Cast bar*/
	void castFireballFront(MyGUI::Widget* _widget) {
		if (_spawns->getCastTimer()->getMilliseconds()>2000) {
			Ogre::Real mana = _spawns->getMana();
			if (mana >= 10) {
				//_particleManager->createSpellMouse(_spawns->getCharacter()->getPosition(), 2000, 20, "fireCastPlayer", 0);
				_particleManager->createSpell(_spawns->getCharacter()->getPosition(), _spawns->getmCamera()->getDerivedDirection(), 2000, 20, "fireCastPlayer", 0);
				_sound->playCastAudio("L_BAZOO.wav", false);
				_spawns->setMana(_spawns->getMana() - 10);
				updateUserFrame();
				_spawns->resetCastTimer();
			}
		}
	}
	void castGreenBallFront(MyGUI::Widget* _widget) {
		if (_spawns->getCastTimer()->getMilliseconds()>2000) {
			Ogre::Real mana = _spawns->getMana();
			if (mana >= 30) {
				//
				_particleManager->createSpell(_spawns->getCharacter()->getPosition(), _spawns->getmCamera()->getDerivedDirection(), 2000, 30, "greenBolt", 0);
				_spawns->setMana(_spawns->getMana() - 30);
				updateUserFrame();
				_spawns->resetCastTimer();
				_sound->playCastAudio("L_FLAME.wav", false);

			}
		}
	}
	void createCastBar() {
		/*
		Ogre::Real castBarWidth = _appSettings->getWidth() - _appSettings->getWidth() / 5;
		Ogre::Real castBarHeight = _appSettings->getHeight() / 8;

		Ogre::Real castBarX = _appSettings->getWidth() / 10;
		Ogre::Real castBarY = _appSettings->getHeight() - castBarHeight;

		castBar = mGUI->createWidget<MyGUI::Window>("Window", castBarX, castBarY, castBarWidth, castBarHeight, MyGUI::Align::Default, "Main");
		castBar->setCaption("Cast Bar");
		*/
		//fireball
		//fireBallImage
		Ogre::Real castBarWidth = _appSettings->getWidth() - _appSettings->getWidth() / 5;
		Ogre::Real castBarHeight = _appSettings->getHeight() / 10;

		Ogre::Real spellHeight = castBarHeight;
		Ogre::Real spellWidth = castBarHeight;

		Ogre::Real castBarX = _appSettings->getWidth() / 10;
		Ogre::Real castBarY = _appSettings->getHeight() - castBarHeight;

		fireBallImage = mGUI->createWidget<MyGUI::ImageBox>("ImageBox", castBarX, castBarY, spellWidth, spellHeight, MyGUI::Align::Default, "Main");
		fireBallImage->setImageTexture("Fireball1_GD_BlueKPL.png");
		fireBallImage->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::castFireballFront);

		greenBallImage = mGUI->createWidget<MyGUI::ImageBox>("ImageBox", castBarX + spellWidth, castBarY, spellWidth, spellHeight, MyGUI::Align::Default, "Main");
		greenBallImage->setImageTexture("Fireball1_GD_GreenKPL.png");
		greenBallImage->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::castGreenBallFront);
		greenBallImage->setVisible(false);
		
	}

	/**********User frame*************/
	void createUserFrame() {
		Ogre::Real userWindowWidth = _appSettings->getWidth() / 4;
		Ogre::Real userWindowHeight = _appSettings->getHeight()/6;
	

		//userWindow = mGUI->createWidget<MyGUI::ScrollView>("ScrollView", 0, 0, userWindowWidth, userWindowHeight, MyGUI::Align::Default, "Main");
		//userWindow->setCanvasSize(userWindowWidth-10, userWindowHeight-10);


		//user image
		Ogre::Real imagePosX = userWindowWidth / 10;
		Ogre::Real imagePosY = userWindowHeight / 4;
		playerImage = mGUI->createWidget<MyGUI::ImageBox>("ImageBox", imagePosX, imagePosY, 40, 40, MyGUI::Align::Default, "Main");
		playerImage->setImageTexture("PlayerImage.png");

		//health
		Ogre::Real manaHpWidth = userWindowWidth / 2;
		Ogre::Real manaHpHeight = userWindowHeight / 8;
		Ogre::Real hpX = imagePosX + playerImage->getWidth()+ userWindowWidth/20;
		Ogre::Real hpY = imagePosY ;

		//sub
		userHealthSub = mGUI->createWidget<MyGUI::ScrollView>("ScrollView", hpX, hpY, manaHpWidth, manaHpHeight, MyGUI::Align::Default, "Main");
		userHealthSub->setColour(MyGUI::Colour(1, 1, 1, 1));
		//Front
		userHealth = mGUI->createWidget<MyGUI::ScrollView>("ScrollView", hpX, hpY, manaHpWidth, manaHpHeight, MyGUI::Align::Default, "Main");
		userHealth->setColour(MyGUI::Colour(1, 0, 0, 1));

		//textBoxHealth = userHealth->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(0, 0, manaHpWidth, manaHpHeight), MyGUI::Align::Default, "Overlapped");
		//textBoxHealth->setCaption("100");
		//mana
		Ogre::Real manaX = hpX;
		Ogre::Real manaY = imagePosY + playerImage->getHeight() - manaHpHeight;

		userManaSub = mGUI->createWidget<MyGUI::ScrollView>("ScrollView", manaX, manaY, manaHpWidth, manaHpHeight, MyGUI::Align::Default, "Main");
		userManaSub->setColour(MyGUI::Colour(1, 1, 1, 1));
		//front
		userMana = mGUI->createWidget<MyGUI::ScrollView>("ScrollView", manaX, manaY, manaHpWidth, manaHpHeight, MyGUI::Align::Default, "Main");
		userMana->setColour(MyGUI::Colour(0, 0, 1, 1));


		//textBoxRotZ = designerWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 20, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		//textBoxRotZ->setCaption("rotation Z");
	}

	
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

		//Mesh list
		meshList = designerWindow->createWidget<MyGUI::ListBox>("ListBox", initPositionX, initPositionY + buttonDistance, buttonWidth, buttonHeight * 5, MyGUI::Align::Default, "Main");
		//meshList->addItem("cube.1m.mesh");
		meshList->_setItemSelected(0);

		meshTypes = designerWindow->createWidget<MyGUI::ListBox>("ListBox", initPositionX, initPositionY + buttonDistance*5, buttonWidth, buttonHeight * 3, MyGUI::Align::Default, "Main");
		meshTypes->addItem("Walls");
		meshTypes->addItem("Objects");
		meshTypes->addItem("Mesh only");
		meshTypes->addItem("Bound only");
		//Create bounding box group
		createBoundingBox = designerWindow->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY + buttonDistance*8, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		createBoundingBox->setCaption("Show shape");
		createBoundingBox->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::showShape);

		//SHAPE DIMENSION CHANGE GROUP
		//Input width group
		textBoxWidth = designerWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 9, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		textBoxWidth->setCaption("X");
		
		objectWidth = designerWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance*10, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		objectWidth->insertText(Ogre::StringConverter::toString(_designer->getCubeDimensions().x));
		//
		//Input height group
		textBoxHeight = designerWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 11, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		textBoxHeight->setCaption("Y");
		

		objectHeight = designerWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 12, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		objectHeight->insertText(Ogre::StringConverter::toString(_designer->getCubeDimensions().y));
		//Input deep group		
		textBoxDeep = designerWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 13, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		textBoxDeep->setCaption("Z");

		objectDeep = designerWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 14, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		objectDeep->insertText(Ogre::StringConverter::toString(_designer->getCubeDimensions().z));
		//Change box dimensions
		setBoundingBoxDimension = designerWindow->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY + buttonDistance * 15, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		setBoundingBoxDimension->setCaption("Apply");
		setBoundingBoxDimension->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::scaleDesignerCube);
		
		//SHAPE rotation CHANGE GROUP
		//Input width group
		textBoxRotX = designerWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 16, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		textBoxRotX->setCaption("rotation X");

		objectRotX = designerWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 17, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		objectRotX->insertText(Ogre::StringConverter::toString(rotX));
		//
		//Input height group
		textBoxRotY = designerWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 18, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		textBoxRotY->setCaption("rotation Y");


		objectRotY = designerWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 19, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		objectRotY->insertText(Ogre::StringConverter::toString(rotY));
		//Input deep group		
		textBoxRotZ = designerWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 20, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		textBoxRotZ->setCaption("rotation Z");

		objectRotZ = designerWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 21, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		objectRotZ->insertText(Ogre::StringConverter::toString(rotZ));
		//Change box dimensions
		setRotation = designerWindow->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY + buttonDistance * 22, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		setRotation->setCaption("Apply");
		setRotation->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::rotateShape);

		//Shape corection y if shape is not centered
		textBoxShapeOffsetY = designerWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 23, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		textBoxShapeOffsetY->setCaption("Shape offset y");

		objectShapeOffsetY = designerWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(initPositionX, initPositionY + buttonDistance * 24, buttonWidth, buttonHeight), MyGUI::Align::Default, "Overlapped");
		objectShapeOffsetY->insertText(Ogre::StringConverter::toString(_designer->getOffset()));
		//
		setShapeOffsetY = designerWindow->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY + buttonDistance * 25, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		setShapeOffsetY->setCaption("Apply");
		setShapeOffsetY->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::setShapeOffset);

		//Undo
		undo = designerWindow->createWidget<MyGUI::Button>("Button", initPositionX, initPositionY + buttonDistance * 26, buttonWidth, buttonHeight, MyGUI::Align::Default, "Main");
		undo->setCaption("Undo");
		undo->eventMouseButtonClick += MyGUI::newDelegate(this, &UserInterface::undoAddition);
	}

};

#endif
