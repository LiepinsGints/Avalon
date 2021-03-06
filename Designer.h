#ifndef Designer_H
#define Designer_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include <OgreSceneQuery.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <iostream>
#include <string>

#include "OGRE/SdkSample.h"
#include "OGRE/SamplePlugin.h"
#include "NxOgre.h"
#include "critter.h"

#include "PhysicsManager.h"

using namespace Ogre;
using namespace OgreBites;
using namespace NxOgre;
using namespace Critter;

class Designer {
public:
	Designer(Ogre::SceneManager* mSceneMgr,  Ogre::Camera* mCamera, Ogre::TerrainGroup* mTerrainGroup, PhysicsManager* physicsManager) {
		_mSceneMgr = mSceneMgr;
		_mCamera = mCamera;
		_mTerrainGroup = mTerrainGroup;
		_physicsManager = physicsManager;
		shapeType = 0;
		counter = 0;
		shapeOffsetY = 0;
		lockPosition = 0;
		shapeRotation = Ogre::Vector3(0,0,0);
	}
	~Designer() {


	}
	//Cube representing bounding box
	
	Ogre::Vector3 getCubeDimensions() {
		if(shapeType==1){
			Ogre::AxisAlignedBox aab = objectEntity->getWorldBoundingBox(true);
			float width = aab.getSize().x;
			float height = aab.getSize().y;
			float deep = aab.getSize().z;
			return Ogre::Vector3(width,height,deep);
		}
		else {
			return Ogre::Vector3(0,0,0);
		}
	}
	void scaleCube(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
		objectNode->scale(x, y, z);

	}
	void setCubeSize(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
		Ogre::Vector3 cubeCurrentDimensions = getCubeDimensions();
		Ogre::Real scaleX = 1;
		Ogre::Real scaleY = 1;
		Ogre::Real scaleZ = 1;
		if (x != cubeCurrentDimensions.x && x>0) {
			scaleX = x / cubeCurrentDimensions.x;
		}
		if (y != cubeCurrentDimensions.y && y>0) {
			scaleY = y / cubeCurrentDimensions.y;
		}
		if (z != cubeCurrentDimensions.z && z>0) {
			scaleZ = z / cubeCurrentDimensions.z;
		}
		scaleCube(scaleX, scaleY, scaleZ);
	}
	//Mouse picking
	void mousePos(const OIS::MouseEvent &arg)
	{
		Viewport* vp = _mSceneMgr->getCurrentViewport();

		//Real coordinates
		Ogre::Real x = arg.state.X.abs ;
		Ogre::Real y = arg.state.Y.abs;
		//Normalized coordinates
		Ogre::Real normalX = arg.state.X.abs / Ogre::Real(vp->getActualWidth());
		Ogre::Real normalY = arg.state.Y.abs / Ogre::Real(vp->getActualHeight());

		Ogre::Ray mouseRay =_mCamera->getCameraToViewportRay(normalX, normalY);
		Ogre::TerrainGroup::RayResult result = _mTerrainGroup->rayIntersects(mouseRay);
		mouseShapeMove(result);
		//result.position
		//cubeNode->setPosition
		/**************************NxOgreRay****************************/
		// Use Ogre and OgreBites to work out where from the cursor the ray should start and end.
		NxOgre::Ray ray;
		ray.mDirection.from(mouseRay.getDirection());
		ray.mOrigin.from(mouseRay.getOrigin());
		
		RaycastHit hit =_physicsManager->getMScene()->raycastClosestShape(ray, NxOgre::Enums::ShapesType_All);

		if (hit.mRigidBody)
		{
			hitMeshName = hit.mRigidBody->getName();
			/*
			if (name.length())
				mHitResultText->setCaption("Hit '" + hit.mRigidBody->getName() + "' at " + hit.mWorldImpact.to_s());
			else
				mHitResultText->setCaption("Hit something at " + hit.mWorldImpact.to_s());
				*/
		}
		else
		{
			/*
			mHitResultText->setCaption("Didn't find anything.");
			*/
		}
		//
		/**********************OgreRay******************************/
		bool mMovableFound = false;
		Ogre::SceneNode* mCurObject;
		Ogre::RaySceneQuery* mRayScnQuery;
		mRayScnQuery = _mSceneMgr->createRayQuery(Ogre::Ray());
		mRayScnQuery->setRay(mouseRay);
		mRayScnQuery->setSortByDistance(true);
		//mRayScnQuery->setQueryMask(mRobotMode ? ROBOT_MASK : NINJA_MASK);

		Ogre::RaySceneQueryResult& rayResult = mRayScnQuery->execute();
		Ogre::RaySceneQueryResult::iterator it = rayResult.begin();

		for (; it != rayResult.end(); it++)
		{
			mMovableFound =
				it->movable &&
				it->movable->getName() != "" &&
				it->movable->getName() != "PlayerCam"&&
				it->movable->getName() != "Critter.VisualDebuggerNode"&&
				it->movable->getName() != " Critter.VisualDebuggerNode";
			if (mMovableFound)
			{
				mCurObject = it->movable->getParentSceneNode();
				//hitMeshName = mCurObject->getName();
				break;
			}
		}
		/*if (mMovableFound) {
			hitMeshName = mCurObject->getName();
		}*/
		
		//Show mouse coordinates
			Ogre::String merge = Ogre::StringConverter::toString(x) + " : \n" +
				Ogre::StringConverter::toString(y) + "\n" +
				Ogre::StringConverter::toString(result.position.x) +" : "+ 
			Ogre::StringConverter::toString(result.position.y) +" : "+
			Ogre::StringConverter::toString(result.position.z) + "\n";
		
	}
	//Designer shape controll
	void mouseShapeMove(Ogre::TerrainGroup::RayResult result) {
		if (shapeType == 1 && lockPosition == 0) {
			Ogre::Vector3 corectCubePosition(result.position.x, result.position.y + getCubeDimensions().y / 2+ shapeOffsetY, result.position.z);
			objectNode->setPosition(corectCubePosition);
		}

	}

	//Get current designer shape
	int getShapeType() {
		return shapeType;
	}
	void setShapeType(int type) {
		shapeType = type;
	}
	//creater cube
	void cube() {
		if (shapeType == 0) {
			objectEntity = _mSceneMgr->createEntity("cube.1m.mesh");
			objectNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
			objectNode->attachObject(objectEntity);
			shapeType = 1;
		}
		else {
			objectNode->detachAllObjects();
			shapeType = 0;
		}
	}
	Ogre::Vector3 getCubePos() {
		return objectNode->getPosition();
	}
	//Create shape
	void createShape(Ogre::String shapeName) {
		if (shapeType == 0) {
			objectEntity = _mSceneMgr->createEntity(shapeName);
			objectNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
			objectNode->attachObject(objectEntity);
			shapeType = 1;
			currentShapeName = shapeName;
			cubeInitialDimensions = getCubeDimensions();
		}
		else {
			objectNode->detachAllObjects();
			shapeType = 0;
			shapeOffsetY = 0;
		}
	}
	//Rotate shape
	void rotateShape(Ogre::Vector3 rotation) {
		if (shapeType == 1) {
			if (rotation.x != 0) {
				objectNode->rotate(Ogre::Vector3(1, 0, 0), Degree(rotation.x));
				shapeRotation.x += rotation.x;
			}
			if (rotation.y != 0) {
				objectNode->rotate(Ogre::Vector3(0, 1, 0), Degree(rotation.y));
				shapeRotation.y += rotation.y;
			}
			if (rotation.z != 0) {
				objectNode->rotate(Ogre::Vector3(0, 0, 1), Degree(rotation.z));
				shapeRotation.z += rotation.z;
			}
		}
	}
	//reset rotation
	void resetRotation() {
			objectNode->setOrientation(1, 0, 0, 0);
			shapeRotation.x = 0;
			shapeRotation.y = 0;
			shapeRotation.z = 0;
	}
	//get shape rotation
	Ogre::Vector3 getShapeRotation() {
		return shapeRotation;
	}
	//Add shape type to scene after creation
	void addShapeToScene() {
		Ogre::Vector3 pos = getCubePos();
		Ogre::Vector3 dimensions = getCubeDimensions();

		Ogre::Entity* tempEntity= _mSceneMgr->createEntity(currentShapeName+std::to_string(counter)+"designer", currentShapeName);
		 Ogre::SceneNode* tempNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		 tempNode->attachObject(tempEntity);
		 tempNode->setPosition(Ogre::Vector3(pos.x, pos.y, pos.z));
		 //
		 /*Ogre::Real scaleX = 1;
		 Ogre::Real scaleY = 1;
		 Ogre::Real scaleZ = 1;
		 Ogre::Vector3 cubeCurrentDimensions = getCubeDimensions();
		 scaleX = cubeInitialDimensions.x / cubeCurrentDimensions.x;
		 scaleY = cubeInitialDimensions.y / cubeCurrentDimensions.y;
		 scaleY = cubeInitialDimensions.z / cubeCurrentDimensions.z;
		 */
		 tempNode->scale(objectNode->getScale());
		 //
		 counter++;

		 //new Method

		 //End

	}
	Ogre::Vector3 getShapeScale() {
		return objectNode->getScale();
	}
	void setShapeOffsetY(Ogre::Real offset) {
		shapeOffsetY = offset;
	}
	Ogre::Real getOffset() {
		return shapeOffsetY;
	}
	void setLockPosition() {
		if (lockPosition == 0) {
			lockPosition = 1;
		}
		else
		{
			lockPosition = 0;
		}
	}

	Ogre::String getHitMeshName() {
		return hitMeshName;
	}
private:
	Ogre::SceneManager* _mSceneMgr;
	Ogre::Camera* _mCamera;
	Ogre::TerrainGroup* _mTerrainGroup;

	//Cube
	Ogre::Entity* objectEntity;
	Ogre::SceneNode* objectNode;
	Ogre::String  currentShapeName;
	Ogre::Vector3 cubeInitialDimensions;
	//Designer shapes 
	//0-none 1-cube 2-sphere
	int shapeType;
	int counter;//for entities spawn
	Ogre::String meshName;
	Ogre::Real shapeOffsetY;
	int lockPosition;
	Ogre::Vector3 shapeRotation;
	PhysicsManager* _physicsManager;
	Ogre::String hitMeshName;

};
#endif