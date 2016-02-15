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


using namespace Ogre;
class AppSettings {
public:
	AppSettings() {
		render = true;
		appName = "Avalon";
		width = 1024;
		height = 768;
		mouseSensitivity = 1;
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
private:
	Ogre::String appName;
	bool render;
	//screen dimensions
	int width;
	int height;
	//mouse
	int mouseSensitivity;
	//Ois controlls
	//OIS::KeyCode up;
	//OIS::KeyCode down;




};
#endif