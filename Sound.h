#ifndef Sound_H
#define Sound_H

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

#include "SoundManager.h"


using namespace Ogre;
class Sound {
public:
	Sound(Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera, Ogre::Root* mRoot, Ogre::RenderWindow* mWindow) {
		_mSceneMgr = mSceneMgr;
		_mCamera = mCamera;
		_mRoot = mRoot;
		_mWindow = mWindow;
	//
		soundMgr = SoundManager::createManager();
		soundMgr->init();
		soundMgr->setAudioPath((char*) ".\\Sound\\");

		isMoving = false;
		//setupSound();

	}
	~Sound() {


	}

	void setupSound()
	{
		soundMgr->releaseAudio(audioId);
		soundMgr->loadAudio("AMB_1N.wav", &audioId, true);
		soundMgr->playAudio(audioId, false);
	}

	void playAudio(Ogre::String trackName, bool repeat) {

		
		soundMgr->releaseAudio(audioId);
		// Just for testing
		soundMgr->loadAudio(trackName, &audioId, repeat);
		soundMgr->playAudio(audioId, false);
		//soundMgr->releaseAudio(audioId);
	}

	//Player cast 
	void playCastAudio(Ogre::String trackName, bool repeat) {
		soundMgr->releaseAudio(playerSpell);
		soundMgr->loadAudio(trackName, &playerSpell, repeat);
		soundMgr->playAudio(playerSpell, false);
	}
	//Player sound
	void playerAudio(Ogre::String trackName, bool repeat) {

		soundMgr->releaseAudio(playerAud);
		// Just for testing
		soundMgr->loadAudio(trackName, &playerAud, repeat);
		soundMgr->playAudio(playerAud, false);
		//soundMgr->releaseAudio(audioId);
	}
	//Player movement
	void playerMovement(Ogre::String trackName) {
		if (isMoving ==false) {
			// Just for testing
			soundMgr->loadAudio(trackName, &playerMove, true);
			soundMgr->playAudio(playerMove, false);
			isMoving = true;
		}
		
		//soundMgr->releaseAudio(audioId);
	}
	void disableMovement() {
		if (isMoving == true) {
			soundMgr->releaseAudio(playerMove);
			isMoving = false;
		}
	}
	//Environment sound 
	void playEnvironmentAudio(Ogre::String trackName, bool repeat) {

		soundMgr->releaseAudio(environment);
		// Just for testing
		soundMgr->loadAudio(trackName, &environment, repeat);
		soundMgr->playAudio(environment, false);
		//soundMgr->releaseAudio(audioId);
	}
	//Enemy sound 
	void playEnemyAudio(Ogre::String trackName, bool repeat) {

		soundMgr->releaseAudio(enemySpell);
		// Just for testing
		soundMgr->loadAudio(trackName, &enemySpell, repeat);
		soundMgr->playAudio(enemySpell, false);
		//soundMgr->releaseAudio(audioId);
	}

	//sound controls

private:

	Ogre::SceneManager* _mSceneMgr;
	Ogre::Camera* _mCamera;
	Ogre::Root* _mRoot;
	Ogre::RenderWindow* _mWindow;
	SoundManager * soundMgr;
	//
	//std::vector<int*> sounds;
	//
	unsigned int audioId;
	//
	unsigned int playerSpell;
	unsigned int playerAud;
	unsigned int playerMove;
	bool isMoving;

	unsigned int environment;
	unsigned int enemySpell;

	


};
#endif