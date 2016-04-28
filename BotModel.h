#ifndef BOT_MODEL_H
#define BOT_MODEL_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

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

	}
	~BotModel() {


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

private:	
	AnimatedCharacter* bot;
	CharacterInputHelper botHelper;
	AnimatedCharacterDescription botDesc;
	Critter::Node* botNode;
};
#endif