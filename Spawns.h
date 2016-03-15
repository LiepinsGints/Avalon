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

#include <NxOgre.h>
#include <critter.h>
#include <CritterCommon.h>
#include <CritterBody.h>
#include <NxOgreVec3.h>
#include <NxOgreBox.h>
#include <NxOgreBoxDescription.h>
#include <NxOgreMatrix.h>
#include <NxCharacter.h>

#include "PhysicsManager.h"

//
#include "Models.h"

class Spawns 
{
public:
	Spawns(Ogre::Root* mRoot, Ogre::RenderWindow* mWindow, Ogre::SceneManager* mSceneMgr, PhysicsManager* physicsManager){
		_mRoot = mRoot;
		_mWindow = mWindow;
		_mSceneMgr = mSceneMgr;
		_physicsManager = physicsManager;
		counter = 0;
	};
	~Spawns() {
	};
	//Create object with simple bounding box
	//Type NonCollidable = 0,    Walls = 1,  Objects = 2   
	void createObject(Models * model,Ogre::Vector3 position, float scale, Ogre::Real mass, int type ) {
		
		/*
		//Box descr 0.02 remowe margins
		NxOgre::BoxDescription boxDesc(scale*model->getDimensions().x- scale*model->getDimensions().x*0.02
			, scale*model->getDimensions().y - scale*model->getDimensions().y*0.02
			, scale*model->getDimensions().z - scale*model->getDimensions().z*0.02);
		//boxDesc.mGroup = type;
		
		//Body descr
		Critter::BodyDescription bodyDescriptionTemp;
		bodyDescriptionTemp.mMass = mass; // S
		

		Critter::Body* mBodyTemp;
		mBodyTemp = _physicsManager->getMRenderSystem()->createBody(boxDesc,
			NxOgre::Vec3(position.x, position.y, position.z), model->getMeshName(), bodyDescriptionTemp);
		mBodyTemp->getNode()->setScale(scale);
		_physicsManager->addMBodies(mBodyTemp);
		*/
		
		Ogre::Entity* ogreCottage = _mSceneMgr->createEntity(model->getName() + std::to_string(counter), model->getMeshName());

		Ogre::SceneNode* cottageNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		cottageNode->attachObject(ogreCottage);
		cottageNode->scale(scale, scale, scale);

		
		//
		Ogre::AxisAlignedBox aab = ogreCottage->getBoundingBox();
		float width = aab.getSize().x * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float height = aab.getSize().y * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float deep = aab.getSize().z * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		cottageNode->setPosition(Ogre::Vector3(position.x, height / 2 + position.y, position.z));
		/*
		//
		_physicsManager->createBoundingBox(cottageNode->getPosition().x, cottageNode->getPosition().y, cottageNode->getPosition().z,
			width, height, deep);

		*/
		counter++;
	}

	void spawnWorld() {

	}
	/*********temp****************************/
	void createBoundingBox(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Real w, Ogre::Real h, Ogre::Real d) {
		_physicsManager->createBoundingBox(x, y, z, w, h, d);
	}
	//create cube
	void createCube() {
		Critter::BodyDescription bodyDescriptionTemp;
		bodyDescriptionTemp.mMass = 20.0f; // Set the mass to 20kg.
		Critter::Body* mBodyTemp;
		mBodyTemp = _physicsManager->getMRenderSystem()->createBody(NxOgre::BoxDescription(4, 4, 4), 
			NxOgre::Vec3(_physicsManager->getCharacter()->getPosition().x, _physicsManager->getCharacter()->getPosition().y + 40, _physicsManager->getCharacter()->getPosition().z), "cube.1m.mesh", bodyDescriptionTemp);
		mBodyTemp->getNode()->setScale(4.0);
		_physicsManager->addMBodies(mBodyTemp);

	}
	//Create Sinbad
	void createSinbad() {
		Critter::BodyDescription bodyDescriptionTemp;
		bodyDescriptionTemp.mMass = 20.0f; // Set the mass to 20kg.
		Critter::Body* mBodyTemp;
		mBodyTemp = _physicsManager->getMRenderSystem()->createBody(NxOgre::BoxDescription(4, 4, 4),
			NxOgre::Vec3(_physicsManager->getCharacter()->getPosition().x, _physicsManager->getCharacter()->getPosition().y + 40, _physicsManager->getCharacter()->getPosition().z), "sinbad.mesh", bodyDescriptionTemp);
		mBodyTemp->getNode()->setScale(1.0);
		_physicsManager->addMBodies(mBodyTemp);

	}
	
	//Create Cottage
	void createCottage(Ogre::Vector3 position, float scale) {
		Ogre::Entity* ogreCottage = _mSceneMgr->createEntity("Cottage"+ counter, "tudorhouse.mesh");

		Ogre::SceneNode* cottageNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		cottageNode->attachObject(ogreCottage);
		cottageNode->scale(scale, scale, scale);

		//
		Ogre::AxisAlignedBox aab = ogreCottage->getBoundingBox();
		float width = aab.getSize().x * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float height = aab.getSize().y * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float deep = aab.getSize().z * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		cottageNode->setPosition(Ogre::Vector3(position.x, height/2+ position.y, position.z));
		
		//
		_physicsManager->createBoundingBox(cottageNode->getPosition().x, cottageNode->getPosition().y-20, cottageNode->getPosition().z,
			width, height,deep);

		counter++;
	}
	//Create pallet
	void createPallet(Ogre::Vector3 position, float scale) {
		
		Ogre::Entity* ogreCottage = _mSceneMgr->createEntity("Pallet"+ counter, "WoodPallet.mesh");

		Ogre::SceneNode* cottageNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		cottageNode->attachObject(ogreCottage);
		cottageNode->scale(scale, scale, scale);

		//
		Ogre::AxisAlignedBox aab = ogreCottage->getBoundingBox();
		float width = aab.getSize().x * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float height = aab.getSize().y * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float deep = aab.getSize().z * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		cottageNode->setPosition(Ogre::Vector3(position.x, height / 2 + position.y, position.z));
		//
		_physicsManager->createBoundingBox(cottageNode->getPosition().x, cottageNode->getPosition().y, cottageNode->getPosition().z,
			width, height, deep);

		counter++;
	}
	//create sinbad sinbad.mesh
	void createSinbad(Ogre::Vector3 position, float scale) {

		Ogre::Entity* ogreCottage = _mSceneMgr->createEntity("Sinbad" + counter, "sinbad.mesh");

		Ogre::SceneNode* cottageNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		cottageNode->attachObject(ogreCottage);
		cottageNode->scale(scale, scale, scale);

		//
		Ogre::AxisAlignedBox aab = ogreCottage->getBoundingBox();
		float width = aab.getSize().x * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float height = aab.getSize().y * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float deep = aab.getSize().z * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		cottageNode->setPosition(Ogre::Vector3(position.x, height / 2 + position.y, position.z));
		//
		_physicsManager->createBoundingBox(cottageNode->getPosition().x, cottageNode->getPosition().y, cottageNode->getPosition().z,
			width, height, deep);

		counter++;
	}
	//get sinbadHeight
	float getSinbadHeight(Ogre::Vector3 position, float scale) {
		Ogre::Entity* ogreCottage = _mSceneMgr->createEntity("Sinbad" + counter, "sinbad.mesh");

		Ogre::SceneNode* cottageNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		cottageNode->attachObject(ogreCottage);
		cottageNode->scale(scale, scale, scale);

		//
		Ogre::AxisAlignedBox aab = ogreCottage->getBoundingBox();
		float width = aab.getSize().x * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float height = aab.getSize().y * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float deep = aab.getSize().z * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		cottageNode->setPosition(Ogre::Vector3(position.x, height / 2 + position.y, position.z));
		//
		_physicsManager->createBoundingBox(cottageNode->getPosition().x, cottageNode->getPosition().y, cottageNode->getPosition().z,
			width, height, deep);

		counter++;
		return height;
	}

private:
	PhysicsManager* _physicsManager;
	Ogre::Root* _mRoot;
	Ogre::RenderWindow* _mWindow;
	Ogre::SceneManager* _mSceneMgr;
	int counter;
};



#endif