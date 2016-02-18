#ifndef SPAWNS_H
#define SPAWNS_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>

#include "PhysicsManager.h"

class Spawns 
{
public:
	Spawns(Ogre::Root* mRoot, Ogre::RenderWindow* mWindow, Ogre::SceneManager* mSceneMgr, PhysicsManager* physicsManager){
		_mRoot = mRoot;
		_mWindow = mWindow;
		_mSceneMgr = mSceneMgr;
		_physicsManager = physicsManager;
	};
	~Spawns() {
	};

	//Create Cottage
	void createCottage() {
		Ogre::Entity* ogreCottage = _mSceneMgr->createEntity("Head", "tudorhouse.mesh");

		Ogre::SceneNode* cottageNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		cottageNode->attachObject(ogreCottage);
		cottageNode->scale(0.1f, 0.1f, 0.1f);

		//
		Ogre::AxisAlignedBox aab = ogreCottage->getBoundingBox();
		float width = aab.getSize().x * (0.1f - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float height = aab.getSize().y * (0.1f - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float deep = aab.getSize().z * (0.1f - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		cottageNode->setPosition(Ogre::Vector3(-87, height/2+23 , -334));

		//
		_physicsManager->createBoundingBox(cottageNode->getPosition().x, cottageNode->getPosition().y, cottageNode->getPosition().z,
			width, height,deep);
	}
	void createPallet() {
		float scale = 1.0;
		
		Ogre::Entity* ogreCottage = _mSceneMgr->createEntity("Pallet", "WoodPallet.mesh");

		Ogre::SceneNode* cottageNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		cottageNode->attachObject(ogreCottage);
		cottageNode->scale(scale, scale, scale);

		//
		Ogre::AxisAlignedBox aab = ogreCottage->getBoundingBox();
		float width = aab.getSize().x * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float height = aab.getSize().y * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float deep = aab.getSize().z * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		cottageNode->setPosition(Ogre::Vector3(-42, height / 2 +21, -307));

		//
		_physicsManager->createBoundingBox(cottageNode->getPosition().x, cottageNode->getPosition().y, cottageNode->getPosition().z,
			width, height, deep);
	}
private:
	PhysicsManager* _physicsManager;
	Ogre::Root* _mRoot;
	Ogre::RenderWindow* _mWindow;
	Ogre::SceneManager* _mSceneMgr;

};



#endif