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
	void createObject(Models * model,Ogre::Vector3 position, float scaleX, float scaleY, float scaleZ,float scale, Ogre::Real mass, int type ) {
			
		Ogre::Entity* ogreCottage = _mSceneMgr->createEntity(model->getName() + std::to_string(counter), model->getMeshName());

		Ogre::SceneNode* cottageNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		cottageNode->attachObject(ogreCottage);
		cottageNode->scale(scaleX, scaleY, scaleZ);

		
		//
		Ogre::AxisAlignedBox aab = ogreCottage->getBoundingBox();
		float width = aab.getSize().x * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float height = aab.getSize().y * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		float deep = aab.getSize().z * (scale - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
		//cottageNode->setPosition(Ogre::Vector3(position.x, height / 2 + position.y, position.z));
		cottageNode->setPosition(Ogre::Vector3(position.x, position.y, position.z));
		counter++;
	}
	void createObjectCapsuleDescription() {
		
	}


	void createObjectBoxDescription(Ogre::String meshName, Ogre::Vector3 position, Ogre::Vector3 scaleDimensions, Ogre::Vector3 boxDimensions, float scale, Ogre::Vector3 rotation, Ogre::Real mass, int type) {
		Critter::BodyDescription bodyDescriptionTemp;
		NxOgre::BoxDescription boundingBox(boxDimensions.x, boxDimensions.y, boxDimensions.z);
		//NxOgre::CapsuleDescription boundingCapsule(boxDimensions.x, boxDimensions.y);
		boundingBox.mFlags += NxOgre::ShapeFlags::Visualisation;
		
		switch (type) {
		case 0:
			createObjectBodyStatic(meshName, position, scaleDimensions, boxDimensions,scale ,rotation, mass);
			break;
		case 1:
			createObjectBody(meshName, position, scaleDimensions, boxDimensions, scale, rotation, mass);
			break;
		}
		
	}
	void createObjectBody(Ogre::String meshName, Ogre::Vector3 position, Ogre::Vector3 scaleDimensions, Ogre::Vector3 boxDimensions, float scale, Ogre::Vector3 rotation, Ogre::Real mass) {
		Critter::BodyDescription bodyDescriptionTemp;
		NxOgre::BoxDescription boundingBox(boxDimensions.x, boxDimensions.y, boxDimensions.z);

		boundingBox.mFlags += NxOgre::ShapeFlags::Visualisation;
		bodyDescriptionTemp.mMass = 20.0f; // Set the mass to 20kg.
		Critter::Body* mBodyTemp;
		mBodyTemp = _physicsManager->getMRenderSystem()->createBody(
			boundingBox,
			NxOgre::Vec3(position.x, position.y, position.z),
			meshName,
			bodyDescriptionTemp
			);
		mBodyTemp->getNode()->setScale(scaleDimensions);
		//_physicsManager->getMRenderSystem()->
		//rotate
		if (rotation.x != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotation.x), Ogre::Vector3(1, 0, 0));
			mBodyTemp->setGlobalOrientationQuat(rot->w, rot->x, rot->y, rot->z);
		}
		if (rotation.y != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotation.y), Ogre::Vector3(0, 1, 0));
			mBodyTemp->setGlobalOrientationQuat(rot->w, rot->x, rot->y, rot->z);
		}
		if (rotation.z != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotation.z), Ogre::Vector3(0, 0, 1));
			mBodyTemp->setGlobalOrientationQuat(rot->w, rot->x, rot->y, rot->z);
		}
		_physicsManager->addMBodies(mBodyTemp);
	}
	void createObjectBodyStatic(Ogre::String meshName, Ogre::Vector3 position, Ogre::Vector3 scaleDimensions, Ogre::Vector3 boxDimensions, float scale, Ogre::Vector3 rotation, Ogre::Real mass) {
		Critter::BodyDescription bodyDescriptionTemp;
		NxOgre::BoxDescription boundingBox(boxDimensions.x, boxDimensions.y, boxDimensions.z);
		boundingBox.mFlags += NxOgre::ShapeFlags::Visualisation;
		boundingBox.mGroup = Walls ;
		
		/*
		Critter::Node* staticNode = _physicsManager->getMRenderSystem()->createNode();
		staticNode->createAndAttachEntity(meshName);
		*/

		Matrix44 globalPose = NxOgre::Vec3(position.x, position.y, position.z);
		
		if (rotation.x != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotation.x), Ogre::Vector3(1, 0, 0));
			boundingBox.mLocalPose.set(NxOgre::Quat(rot->w, rot->x, rot->y, rot->z));
		}
		if (rotation.y != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotation.y), Ogre::Vector3(0, 1, 0));
			boundingBox.mLocalPose.set(NxOgre::Quat(rot->w, rot->x, rot->y, rot->z));
		}
		if (rotation.z != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotation.z), Ogre::Vector3(0, 0, 1));
			boundingBox.mLocalPose.set(NxOgre::Quat(rot->w, rot->x, rot->y, rot->z));
		}

		SceneGeometry* scene_geom = _physicsManager->getMScene()->createSceneGeometry(boundingBox, globalPose, bodyDescriptionTemp);
		
		//_physicsManager->getMRenderSystem()->createSceneNodeEntityPair(meshName, Vec3(globalPose), NxOgre::Quat(rot->w, rot->x, rot->y, rot->z));
		createMeshOnly(meshName, position, scaleDimensions, rotation);
	}
	void createObjectKinematicBody(Ogre::String meshName, Ogre::Vector3 position, Ogre::Vector3 scaleDimensions, Ogre::Vector3 boxDimensions, float scale, Ogre::Vector3 rotation, Ogre::Real mass) {
		Critter::BodyDescription bodyDescriptionTemp;
		NxOgre::BoxDescription boundingBox(boxDimensions.x, boxDimensions.y, boxDimensions.z);

		boundingBox.mFlags += NxOgre::ShapeFlags::Visualisation;
		Critter::KinematicBody * kinematiBodyTemp;
		kinematiBodyTemp = _physicsManager->getMRenderSystem()->createKinematicBody(
			boundingBox,
			NxOgre::Vec3(position.x, position.y, position.z),
			meshName,
			bodyDescriptionTemp
			);
		kinematiBodyTemp->getNode()->setScale(scaleDimensions);
		if (rotation.x != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotation.x), Ogre::Vector3(1, 0, 0));
			kinematiBodyTemp->setGlobalOrientationQuat(NxOgre::Quat(rot->w, rot->x, rot->y, rot->z));
		}
		if (rotation.y != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotation.y), Ogre::Vector3(0, 1, 0));
			kinematiBodyTemp->setGlobalOrientationQuat(NxOgre::Quat(rot->w, rot->x, rot->y, rot->z));
		}
		if (rotation.z != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotation.z), Ogre::Vector3(0, 0, 1));
			kinematiBodyTemp->setGlobalOrientationQuat(NxOgre::Quat(rot->w, rot->x, rot->y, rot->z));
		}
	}

	//Create mesh
	void createMeshOnly(Ogre::String meshName, Ogre::Vector3 position, Ogre::Vector3 scaleDimensions, Ogre::Vector3 rotation) {
		SceneNode* modelNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode(position);
		modelNode->attachObject(_mSceneMgr->createEntity(meshName + "-" + Ogre::StringConverter::toString(counter), meshName));
		modelNode->setScale(scaleDimensions.x, scaleDimensions.y, scaleDimensions.z);
		//Rotate node
		if (rotation.x != 0) {
			modelNode->rotate(Ogre::Vector3(1, 0, 0), Degree(rotation.x));

		}
		if (rotation.y != 0) {
			modelNode->rotate(Ogre::Vector3(0, 1, 0), Degree(rotation.y));

		}
		if (rotation.z != 0) {
			modelNode->rotate(Ogre::Vector3(0, 0, 1), Degree(rotation.z));

		}

		//modelNode->rotate()
		counter++;
	}
	void spawnWorld() {
	
	}
	/*********temp****************************/
	void createBoundingBox(Ogre::Vector3 position, Ogre::Vector3 boxDimensions,  Ogre::Vector3 rotation) {
		//_physicsManager->createBoundingBox(x, y, z, w, h, d);

		NxOgre::BoxDescription boundingBox(boxDimensions.x, boxDimensions.y, boxDimensions.z);
		boundingBox.mFlags += NxOgre::ShapeFlags::Visualisation;
		boundingBox.mGroup = Objects;//Objects Walls

		Matrix44 globalPose = NxOgre::Vec3(position.x, position.y, position.z);

		if (rotation.x != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotation.x), Ogre::Vector3(1, 0, 0));
			boundingBox.mLocalPose.set(NxOgre::Quat(rot->w, rot->x, rot->y, rot->z));
		}
		if (rotation.y != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotation.y), Ogre::Vector3(0, 1, 0));
			boundingBox.mLocalPose.set(NxOgre::Quat(rot->w, rot->x, rot->y, rot->z));
		}
		if (rotation.z != 0) {
			Ogre::Quaternion * rot = new Ogre::Quaternion(Ogre::Degree(rotation.z), Ogre::Vector3(0, 0, 1));
			boundingBox.mLocalPose.set(NxOgre::Quat(rot->w, rot->x, rot->y, rot->z));
		}

		_physicsManager->getMScene()->createSceneGeometry(boundingBox, NxOgre::Vec3(position.x, position.y, position.z));
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

	enum ControllerShapeGroups
	{
		NonCollidable = 0,      // Things that the character controller can go through such as Ghosts or tiny
								// objects like small rocks.
		Walls = 1,              // Walls, Floors and other static geometry that can't be moved.
		Objects = 2             // Boxes, Barrels and other dynamic parts of the scene that can be moved by pushing.
	};
};



#endif