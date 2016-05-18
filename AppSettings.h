#ifndef AppSettings_H
#define AppSettings_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <iostream>
#include <string>

#include "mysql_connection.h"
using namespace Ogre;
class AppSettings {
public:
	AppSettings() {
		//Aplication settings
		render = true;
		appName = "Avalon";
		width = 1024;
		height = 768;
		mouseSensitivity = 1;

		//Db settings
		adress = "tcp://127.0.0.1:3311";
		userName = "root";
		password = "root";
		dbName = "Avalon";

		//Player settings
		playerHealth = 100;
		playerMana = 100;

		//Npc settings
		enemyHealth = 100;
		enemyMana = 100;
		agroRange = 150;
		respawn = 360000;//5 min
		botSpeed = 80;
	}
	~AppSettings() {


	}
	
	Ogre::String getAppName() {
		return appName;
	}
	void setAppName(Ogre::String _appName) {
		appName = _appName;
	}
	int getWidth() {
		return width;
	}
	void setWidth(int _width) {
		width= _width;
	}
	int getHeight() {
		return height;
	}
	void setHeight(int _height) {
		height = height;
	}
	int getMouseSensitivity() {
		return mouseSensitivity;
	}
	void setMouseSensitivity(int _mouseSensitivity) {
		mouseSensitivity = _mouseSensitivity;
	}
	bool isRender() {
		return render;
	}
	void setRender(bool _render) {
		render = _render;
	}
	void setAgroRange(Ogre::Real _agroRange) {
		agroRange = _agroRange;
	}
	void setRespawn(Ogre::Real _respawn) {
		respawn = _respawn;
	}
	void setBotSpeed(Ogre::Real _botSpeed) {
		botSpeed = _botSpeed;
	}
	//Mysql
	sql::SQLString getMySqlAdress() {
		return adress;
	}
	sql::SQLString getMySqlUserName() {
		return userName;
	}
	sql::SQLString getMySqlPassword() {
		return password;
	}
	sql::SQLString getDbName() {
		return dbName;
	}
	//Player bot settings
	Ogre::Real getPlayerHealth() {
		return playerHealth;
	}
	Ogre::Real getPlayerMana() {
		return playerMana;
	}
	Ogre::Real getEnemyHealth() {
		return enemyHealth;
	}
	Ogre::Real getEnemyMana() {
		return enemyMana;
	}
	Ogre::Real getAgroRange() {
		return agroRange;
	}
	Ogre::Real getRespawn() {
		return respawn;
	}
	Ogre::Real getBotSpeed() {
		return botSpeed;
	}
private:
	Ogre::String appName;
	bool render;
	//screen dimensions
	int width;
	int height;
	//mouse
	int mouseSensitivity;
	//Mysql
	sql::SQLString adress;
	sql::SQLString userName;
	sql::SQLString password;
	sql::SQLString dbName;
	
	Ogre::Real playerHealth;
	Ogre::Real playerMana;

	Ogre::Real enemyHealth;
	Ogre::Real enemyMana;

	Ogre::Real agroRange;
	Ogre::Real respawn;
	Ogre::Real botSpeed;
	//OIS::KeyCode up;
	//OIS::KeyCode down;




};
#endif