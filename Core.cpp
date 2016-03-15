#include "Core.h"

#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include "Spawns.h"


Core::Core()
	: mRoot(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mWindow(0),
	mSceneMgr(0)
{
}

Core::~Core()
{
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);

	windowClosed(mWindow);

	delete mRoot;
}

bool Core::go()
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif
	//load app settings
	appSettings = new AppSettings();

	//Load resources
	mRoot = new Ogre::Root(mPluginsCfg);

	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	Ogre::String name, locType;
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();

	while (secIt.hasMoreElements())
	{
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;

		for (it = settings->begin(); it != settings->end(); ++it)
		{
			locType = it->first;
			name = it->second;

			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
		}
	}

	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
		return false;

	/************Init window size******************/
	mWindow = mRoot->initialise(true, appSettings->getAppName());
	mWindow->setFullscreen(false, appSettings->getWidth(), appSettings->getHeight());
	//mWindow = mRoot->initialise(true, "Avalon");


	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

	//Load content
	contentManager = new ContentManager(mRoot, mWindow,mSceneMgr);
	contentManager->loadContent();
	//Physics manager
	physicsManager = new PhysicsManager(mSceneMgr, contentManager, contentManager->getTerrainGen()->getmTerrainGroup());
	
	//userInterface
	userInterface = new UserInterface(mWindow, mSceneMgr, appSettings);
	//MySql
	mySql = new MySql(appSettings);
	mySql->mySqlConnect();
	//Spawns
	Spawns* spawns = new Spawns( mRoot, mWindow, mSceneMgr, physicsManager);
	//spawns->createCottage(Ogre::Vector3(-87, 23, -334),0.1f);
	//spawns->createCottage(Ogre::Vector3(26, 25.2455-2, -331), 0.1f);//25.2455-1-0.2
	//spawns->createPallet(Ogre::Vector3(-42,  21, -307),1.0f);

	//spawns->createObject(mySql->getModelByName("tudorhouse"), Ogre::Vector3(-12, 26, -233), 0.01f, 10.0, 1);

	//spawns->createObject(mySql->getModelById(3), Ogre::Vector3(-205, 25.241, 19), 0.05f, 10.0, 1);

	float heightS = spawns->getSinbadHeight(Ogre::Vector3(-42, 25-4.5, -191), 1.0f);
	swap ="Sinabd height"+ Ogre::StringConverter::toString(heightS);
	//Populate world
	mySql->getWorld(spawns);
	mySql->spanwBounds(spawns);
	// OIS
	keyListener = new KeyListener(mWindow, contentManager, physicsManager, appSettings,spawns);
	//windowResized(mWindow);
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);

	mRoot->startRendering();

	return true;
}

bool Core::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	//Update character position
	Ogre::String my_string = "X: " + Ogre::StringConverter::toString(physicsManager->getCharacter()->getPosition().x)+"\n" +
							 "Y: " + Ogre::StringConverter::toString(physicsManager->getCharacter()->getPosition().y)+ "\n" +
							 "Z: " + Ogre::StringConverter::toString(physicsManager->getCharacter()->getPosition().z);
	userInterface->setLabelCaption(my_string,swap,"free");
	//
	if (mWindow->isClosed()) return false;

	physicsManager->updatePhysics(fe);
	keyListener->listen(fe);
	return appSettings->isRender();

	//return true;
}
/*
void Core::windowResized(Ogre::RenderWindow* rw)
{
	int left, top;
	unsigned int width, height, depth;

	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState& ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}
*/
void Core::windowClosed(Ogre::RenderWindow* rw)
{
	if (rw == mWindow)
	{
		keyListener->destroyInputs();		
	}
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main(int argc, char *argv[])
#endif
	{
		Core app;

		try
		{
			app.go();
		}
		catch (Ogre::Exception& e)
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(
				NULL,
				e.getFullDescription().c_str(),
				"An exception has occured!",
				MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}
#ifdef __cplusplus
}
#endif