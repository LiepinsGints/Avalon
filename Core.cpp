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
	
	
	//MySql
	mySql = new MySql(appSettings);
	mySql->mySqlConnect();
	//Spawns
	spawns = new Spawns( mRoot, mWindow, mSceneMgr, physicsManager);
	
	//Designer
	designer = new Designer(mSceneMgr,contentManager->getmCamera(), contentManager->getTerrainGen()->getmTerrainGroup(),physicsManager);
	//Sound
	sound = new Sound(mSceneMgr, contentManager->getmCamera(), mRoot, mWindow);
	//sound->playerMovement("FT_GRASS.wav");
	//sound->playCastAudio("FT_GRASS.wav", false);
	//bots
	bots = new Bots(spawns, mySql, physicsManager, appSettings,sound);
	//bots->spawnBot("sinbad.mesh", Ogre::Vector3(10, 5.5, -41), 17);
	//bots->spawnBot("sinbad.mesh", Ogre::Vector3(-82, 26, -217), 17);
	//bots->spawnBot("sinbad.mesh", Ogre::Vector3(-60, 26, -219), 17);
	//bots->spawnBot("sinbad.mesh", Ogre::Vector3(97, 26, -232), 17);
	//bots->spawnBot("sinbad.mesh", Ogre::Vector3(188, 26, -249), 17);
	
	//LootItems
	lootItems = new LootItems(spawns, mySql, physicsManager, appSettings, sound);
	//lootItems->spawnLoot(Ogre::Vector3(-681, 26, 860), Ogre::Vector3(0, 0, 0), 0, 25, true);
	//lootItems->spawnLoot(Ogre::Vector3(-709, 26, 863), Ogre::Vector3(0, 0, 0), 1, 25, true);
	//lootItems->spawnLoot(Ogre::Vector3(-744, 26, 866), Ogre::Vector3(0, 0, 0), 0, 25, true);
	//lootItems->spawnLoot(Ogre::Vector3(-771, 26, 869), Ogre::Vector3(0, 0, 0), 1, 25, true);
	//lootItems->spawnLoot(Ogre::Vector3(-159, 26, 731), Ogre::Vector3(0, 0, 0), 0, 75, true);
	//ParticleManager
	particleManager = new ParticleManager(mSceneMgr, contentManager->getmCamera(), mRoot, mWindow, contentManager->getTerrainGen()->getmTerrainGroup(), bots->getmBots(), physicsManager,sound);
	//userInterface
	userInterface = new UserInterface(mWindow, mSceneMgr, appSettings, designer, mySql,spawns, particleManager,sound);
	userInterface->disableTestInterface();
	//float heightS = spawns->getSinbadHeight(Ogre::Vector3(-42, 25-4.5, -191), 1.0f);
	//swap ="Sinabd height"+ Ogre::StringConverter::toString(heightS);
	//Populate world
	mySql->getWorld(spawns);
	
	//Sky
	sky = new Sky(mSceneMgr, contentManager->getmCamera(), mRoot, mWindow);

	//sound->playAudio("AMB_1N.wav",false);
	//Water
	//water = new Water(mSceneMgr, contentManager->getmCamera(), mRoot, mWindow);
	
	// OIS
	keyListener = new KeyListener(mWindow, contentManager, physicsManager, appSettings, spawns, designer, userInterface,particleManager, sound);

	//windowResized(mWindow);
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);

	mRoot->startRendering();

	return true;
}

bool Core::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	//test section
	if(userInterface->designerActive()==false){
	 bots->botControls(particleManager, lootItems);
	 spawns->manaRegen();
	 spawns->characterRespawn();
	 spawns->positionUpdate();
	 userInterface->updateUserFrame();
	 lootItems->updateLootItems();
	}
	//getOrientation().getYaw();
	//Update character position
	Ogre::String my_string = "X: " + Ogre::StringConverter::toString(spawns->getCharacter()->getPosition().x)+"\n" +
							 "Y: " + Ogre::StringConverter::toString(spawns->getCharacter()->getPosition().y)+ "\n" +
							 "Z: " + Ogre::StringConverter::toString(spawns->getCharacter()->getPosition().z)+"\n"+
							 "yaw: " + Ogre::StringConverter::toString(bots->corectAngle(Ogre::Degree(spawns->getSinbadNode()->getOrientation().getYaw()).valueDegrees()))+"\n"
								 ;
	
	userInterface->setLabelCaption(my_string,swap, "Hit mesh id: " + designer->getHitMeshName());
	//
	if (mWindow->isClosed()) return false;

	physicsManager->updatePhysics(fe);
	keyListener->listen(fe);
	return appSettings->isRender();

	//return true;
}
bool Core::frameStarted(const Ogre::FrameEvent& fe)
{
	particleManager->particleControls(fe, spawns);
	userInterface->updateUserFrame();
	userInterface->updateScore();
	return true;
}

bool Core::frameEnded(const Ogre::FrameEvent& fe)
{
	

	return true;
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