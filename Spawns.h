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
			
		Ogre::Entity* ogreCottage = _mSceneMgr->createEntity(model->getName() + std::to_string(counter), model->getMeshName());

		Ogre::SceneNode* cottageNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		cottageNode->attachObject(ogreCottage);
		cottageNode->scale(scale, scale, scale);

		
		//
		Ogre::AxisAlignedBox aab = ogreCottage->getBoundingBox();
		float width = aab.getSize().x * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float height = aab.getSize().y * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float deep = aab.getSize().z * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		//cottageNode->setPosition(Ogre::Vector3(position.x, height / 2 + position.y, position.z));
		cottageNode->setPosition(Ogre::Vector3(position.x, position.y, position.z));
		counter++;
	}

	void spawnWorld() {
		ShapeDescriptions shapes;
		
		BoxDescription side1(Vec3(15.136,7*4, 0.4*4), 0, Vec3(0, 0, -3.9 * 4));
		BoxDescription side2(Vec3(15.136 * 4, 7 * 4, 0.4 * 4), 0, Vec3(0, 0, 3.9 * 4));
		BoxDescription side3(Vec3(15.136 * 4, 0.4 * 4, 8 * 4), 0, Vec3(0, -3.4 * 4, 0));
		BoxDescription side4(Vec3(15.136 * 4, 0.4 * 4, 8 * 4), 0, Vec3(0, 3.4 * 4, 0));

		shapes.push_back(&side1);
		shapes.push_back(&side1);
		shapes.push_back(&side2);
		shapes.push_back(&side3);

		BodyDescription description;
		description.mMass = 1000;

		Body* container = _physicsManager->getMRenderSystem()->createBody(shapes, Vec3(-123, 25, -207), "container.mesh", description);
		container->getNode()->setScale(4.0);
	}
	/*********temp****************************/
	void createBoundingBox(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Real w, Ogre::Real h, Ogre::Real d) {
		_physicsManager->createBoundingBox(x, y, z, w, h, d);
	}
	//create cube
	void createCube(Ogre::Real rotX, Ogre::Real rotY, Ogre::Real rotZ) {
		Critter::BodyDescription bodyDescriptionTemp;
		bodyDescriptionTemp.mMass = 20.0f; // Set the mass to 20kg.
		Critter::Body* mBodyTemp;
		
		mBodyTemp = _physicsManager->getMRenderSystem()->createBody(NxOgre::BoxDescription(4, 4, 4), 
			NxOgre::Vec3(_physicsManager->getCharacter()->getPosition().x, _physicsManager->getCharacter()->getPosition().y + 40, _physicsManager->getCharacter()->getPosition().z), "cube.1m.mesh", bodyDescriptionTemp);
		mBodyTemp->getNode()->setScale(4.0);
		//rotate
		if (rotX!=0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotX), Ogre::Vector3(1, 0, 0));
			mBodyTemp->setGlobalOrientationQuat(rot->w, rot->x, rot->y, rot->z);
		}
		if (rotY != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotY), Ogre::Vector3(0, 1, 0));
			mBodyTemp->setGlobalOrientationQuat(rot->w, rot->x, rot->y, rot->z);
		}
		if (rotZ != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotZ), Ogre::Vector3(0, 0, 1));
			mBodyTemp->setGlobalOrientationQuat(rot->w, rot->x, rot->y, rot->z);
		}
		//Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(45), Ogre::Vector3(0, 1, 0));
		//mBodyTemp->setGlobalOrientationQuat(rot.w, rot.x, rot.y, rot.z);
		//mBodyTemp->setGlobalOrientationQuat(rot->w, rot->x, rot->y, rot->z);

		//
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