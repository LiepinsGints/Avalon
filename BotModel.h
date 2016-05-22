#ifndef BOT_MODEL_H
#define BOT_MODEL_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <OgreTimer.h>
#include <iostream>
#include <string>

#include <NxOgre.h>
#include <critter.h>
#include <CritterCommon.h>
#include <CritterBody.h>
#include <NxOgreVec3.h>
#include <NxOgreBox.h>
#include <NxOgreBoxDescription.h>
#include <NxOgreMatrix.h>
#include <NxCharacter.h>
#include <vector>
#include "PhysicsManager.h"
#include "Spawns.h" 

using namespace Ogre;
class BotModel {
public:
	BotModel() {
		//defaults
		mana = 100;
		health = 100;
		alive = true;
		timer = new Ogre::Timer();
		castTimer = new Ogre::Timer();
		sinceLastAgroTimer = new Ogre::Timer();
	}
	~BotModel() {


	}

	int getId() {
		return id;
	}
	AnimatedCharacter* getBot() {
		return bot;
	}
	CharacterInputHelper& getBotHelper() {
		return botHelper;
	}
	AnimatedCharacterDescription& getBotDesc() {
		return botDesc;
	}
	Critter::Node* getBotNode() {
		return botNode;
	}
	Ogre::Vector3  getStartPos() {
		return startPos;
	}
	Ogre::String  getMeshName() {
		return meshName;
	}
	Ogre::String  getNodeName() {
		return nodeName;
	}
	int getHealth() {
		return health;
	}
	int getMana() {
		return mana;
	}
	Ogre::Timer * getTimer() {
		return timer;
	}
	Ogre::Timer * getCastTimer() {
		return castTimer;
	}
	Ogre::Timer * getSinceLastAgroTimer() {
		return sinceLastAgroTimer;
	}
	
	bool getAlive() {
		return alive;
	}
	
	void setId(int _id) {
		id = _id;
	}
	void setBot(AnimatedCharacter* _bot) {
		bot = _bot;
	}
	void setBotHelper(CharacterInputHelper _botHelper) {
		botHelper = _botHelper;
	}
	void setBotDesc(AnimatedCharacterDescription _botDesc) {
		botDesc = _botDesc;
	}
	void setBotNode(Critter::Node* _botNode) {
		botNode = _botNode;
	}
	void setStartPos(Ogre::Vector3  _startPos) {
		startPos= _startPos;
	}
	void setMeshName(Ogre::String _meshName) {
		meshName = _meshName;
	}
	void setNodeName(Ogre::String _nodeName) {
		nodeName = _nodeName;
	}
	void setHealth(Ogre::Real _health) {
		if (_health < 0) {
			health = 0;
			timer->reset();
		}
		else
		{
			health = _health;
		}
		
	}
	void setMana(Ogre::Real _mana) {
		mana = _mana;
	}
	void startTimer() {
		timer->reset();
	}
	void resetCastTimer() {
		castTimer->reset();
	}
	void resetSinceLastAgroTimer() {
		sinceLastAgroTimer->reset();
	}
	void setAlive(bool _alive) {
		alive = _alive;
	}

	//destroy
	void destroy(PhysicsManager* physicsManager) {
		botNode->destroyEntity(nodeName);
		//physicsManager->getMRenderSystem()->destroyNode(botNode);
		physicsManager->getMRenderSystem()->destroyAnimatedCharacter(bot);
		timer->reset();
		//physicsManager->getMRenderSystem()->destroyNode(botNode);
		//botNode->destroyEntity(nodeName);
		//botNode->getEntityAt(0)->addVisibilityFlags(0);

	}
	void respawn(PhysicsManager* physicsManager) {
		botNode = physicsManager->getMRenderSystem()->createNode();
		botNode->createAndAttachEntity(nodeName, meshName);
		setBot(physicsManager->getMRenderSystem()->createAnimatedCharacter(startPos, Ogre::Radian(0), getBotNode(), getBotDesc()));
		alive = true;
		health = 100;
		mana = 100;
	}
private:	
	int id;
	AnimatedCharacter* bot;
	CharacterInputHelper botHelper;
	AnimatedCharacterDescription botDesc;
	Critter::Node* botNode;
	Ogre::Vector3  startPos;
	Ogre::String meshName;
	Ogre::Real health;
	Ogre::Real mana;
	Ogre::Timer* timer;
	Ogre::Timer* castTimer;
	Ogre::Timer* sinceLastAgroTimer;
	Ogre::String nodeName;
	bool alive;
	
};
#endif