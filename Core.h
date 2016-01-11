#ifndef TUTORIALAPPLICATION_H
#define TUTORIALAPPLICATION_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "ContentManager.h"
#include "KeyListener.h"
#include "PhysicsManager.h"

class Core
	: public Ogre::WindowEventListener,
	public Ogre::FrameListener
{
public:
	Core();
	virtual ~Core();

	bool go();

private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

	//virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	Ogre::Root* mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;

	ContentManager* contentManager;
	KeyListener* keyListener;
	PhysicsManager* physicsManager;


};

#endif