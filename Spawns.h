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
#include "BotModel.h"
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
		botCounter = 0;
		manaTimer = new Ogre::Timer();
		castTimer = new Ogre::Timer();
		mSinbadNodeName = "mainCharacter";
		loadPredefinedWorld();
		score = 0;
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


	void createObjectBoxDescription(int id, Ogre::String meshName, Ogre::Vector3 position, Ogre::Vector3 scaleDimensions, Ogre::Vector3 boxDimensions, float scale, Ogre::Vector3 rotation, Ogre::Real mass, int type) {
		Critter::BodyDescription bodyDescriptionTemp;
		NxOgre::BoxDescription boundingBox(boxDimensions.x, boxDimensions.y, boxDimensions.z);
		//NxOgre::CapsuleDescription boundingCapsule(boxDimensions.x, boxDimensions.y);
		boundingBox.mFlags += NxOgre::ShapeFlags::Visualisation;
		
		switch (type) {
		case 0:
			createObjectBodyStatic(id,meshName, position, scaleDimensions, boxDimensions,scale ,rotation, mass);
			break;
		case 1:
			createObjectBody(id,meshName, position, scaleDimensions, boxDimensions, scale, rotation, mass);
			break;
		case 2:
			createObjectKinematicBody(id, meshName, position, scaleDimensions, boxDimensions, scale, rotation, mass);
			break;
		}
		
	}
	void createObjectBody(int id,Ogre::String meshName, Ogre::Vector3 position, Ogre::Vector3 scaleDimensions, Ogre::Vector3 boxDimensions, float scale, Ogre::Vector3 rotation, Ogre::Real mass) {
		Critter::BodyDescription bodyDescriptionTemp;
		NxOgre::BoxDescription boundingBox(boxDimensions.x, boxDimensions.y, boxDimensions.z);

		boundingBox.mFlags += NxOgre::ShapeFlags::Visualisation;
		bodyDescriptionTemp.mMass = mass; // Set the mass to 20kg.
		bodyDescriptionTemp.mName = Ogre::StringConverter::toString(id);
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
	void createObjectBodyStatic(int id, Ogre::String meshName, Ogre::Vector3 position, Ogre::Vector3 scaleDimensions, Ogre::Vector3 boxDimensions, float scale, Ogre::Vector3 rotation, Ogre::Real mass) {
		Critter::BodyDescription bodyDescriptionTemp;
		bodyDescriptionTemp.mName = Ogre::StringConverter::toString(id);
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
		createMeshOnly(id,meshName, position, scaleDimensions, rotation);
	}
	void createObjectKinematicBody(int id,Ogre::String meshName, Ogre::Vector3 position, Ogre::Vector3 scaleDimensions, Ogre::Vector3 boxDimensions, float scale, Ogre::Vector3 rotation, Ogre::Real mass) {
		Critter::BodyDescription bodyDescriptionTemp;
		bodyDescriptionTemp.mName = Ogre::StringConverter::toString(id);
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
	void createMeshOnly(int id, Ogre::String meshName, Ogre::Vector3 position, Ogre::Vector3 scaleDimensions, Ogre::Vector3 rotation) {
		SceneNode* modelNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode(position);
		modelNode->attachObject(_mSceneMgr->createEntity(Ogre::StringConverter::toString(id), meshName));
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
			NxOgre::Vec3(getCharacter()->getPosition().x, getCharacter()->getPosition().y + 40, getCharacter()->getPosition().z), "cube.1m.mesh", bodyDescriptionTemp);
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
	
	

	Critter::Node* getSinbadNode() {
		return sinbadNode;
	}
	/******************************Animated character****************************************/
	void setupCharacterAnimations() {
		//sinbad
		_physicsManager->getMRenderSystem()->addAnimation("sinbad.mesh", SinbadLower, Critter::Enums::StockAnimationID_Idle, "IdleBase");
		_physicsManager->getMRenderSystem()->addAnimation("sinbad.mesh", SinbadUpper, Critter::Enums::StockAnimationID_Idle, "IdleTop");
		_physicsManager->getMRenderSystem()->addAnimation("sinbad.mesh", SinbadLower, Critter::Enums::StockAnimationID_Forward, "RunBase");
		_physicsManager->getMRenderSystem()->addAnimation("sinbad.mesh", SinbadUpper, Critter::Enums::StockAnimationID_Forward, "RunTop");
		_physicsManager->getMRenderSystem()->addAnimation("sinbad.mesh", SinbadLower, Critter::Enums::StockAnimationID_Jump, "JumpStart", 5.0, false);
		_physicsManager->getMRenderSystem()->addAnimation("sinbad.mesh", SinbadLower, Critter::Enums::StockAnimationID_Fall, "JumpLoop");
		_physicsManager->getMRenderSystem()->addAnimation("sinbad.mesh", SinbadLower, Critter::Enums::StockAnimationID_Land, "JumpEnd", 5.0, false);
		_physicsManager->getMRenderSystem()->addAnimation("sinbad.mesh", SinbadLower, 100, "Dance", 5.0, false);

		//Robot
		//_physicsManager->getMRenderSystem()->addAnimation("robot.mesh", SinbadLower, Critter::Enums::StockAnimationID_Idle, "Idle");
		//_physicsManager->getMRenderSystem()->addAnimation("robot.mesh", SinbadLower, Critter::Enums::StockAnimationID_Forward, "Walk");

	}
	void createAnimatedCharacter(Ogre::String meshName,Ogre::Vector3 position, Ogre::Real groundSpeed,Ogre::Real health, Ogre::Real mana) {

		//Base stats
		setHealth(health);
		setMana(mana);
		alive = true;
		//
		Critter::AnimatedCharacterDescription desc;
		desc.mShape = NxOgre::SimpleCapsule(5.6, 2);
		desc.mCollisionMask = (Walls << 1) | (Objects << 1);
		desc.mMaxGroundSpeed = groundSpeed;
		desc.setJumpVelocityFromMaxHeight(_physicsManager->getMScene()->getGravity().y, 3.50f);
		//Create critter node for sinbad mesh
		sinbadNode = _physicsManager->getMRenderSystem()->createNode();
		sinbadNode->createAndAttachEntity(mSinbadNodeName,meshName);
		//Create animated character
		mSinbad = _physicsManager->getMRenderSystem()->createAnimatedCharacter(position, Ogre::Radian(0), sinbadNode, desc);
		//Create 
		//Ogre::SceneNode* camNode;
		camNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		camNode->attachObject(_physicsManager->getContentManager()->getmCamera());
		sinbadNode->addSceneNode(camNode);
		//Assign helper to sinbad
		mSinbad->setInput(mSinbadHelper);
	}
	//Character controlls
	/*******************Character controls***********************/
	Critter::AnimatedCharacter* getCharacter() {
		return mSinbad;
	}
	Critter::CharacterInputHelper getCharacterInputHelper() {
		return mSinbadHelper;
	}
	NxOgre::Actor* getActor() {
		return mTestActor;
	}

	//
	void resetHelper() {
		mSinbadHelper.reset();
	}
	void applyHelper() {
		mSinbad->setInput(mSinbadHelper);
	}

	void left(int speed) {
		mSinbadHelper.input.is_turning = true;
		//mSinbadHelper.leftFractional(122);
		mSinbadHelper.left(speed);
		
	}
	void right(int speed) {
		mSinbadHelper.input.is_turning = true;
		mSinbadHelper.right(speed);
	}
	void forward(int speed) {
		mSinbadHelper.forward(speed);
	}
	void backward(int speed) {
		mSinbadHelper.backward(speed);
	}
	void jump(int speed) {
		mSinbadHelper.up(speed);
	}
	void setHealth(Ogre::Real _health) {
		if (_health < 0) {
			health = 0;
		}
		else
		{
			health = _health;
		}
		if (health == 0) {
			alive = false;
		}
	}
	void setMana(Ogre::Real _mana) {
		if (_mana < 0) {
			mana = 0;
		}
		else if (_mana > 100) {
			mana = 100;
		}
		else
		{
			mana = _mana;
		}
	}
	void setAlive(bool _alive) {
		alive = _alive;
	}
	int getHealth() {
		return health;
	}
	int getMana() {
		return mana;
	}
	bool getAlive() {
		return alive;
	}
	Ogre::Camera* getmCamera() {
		return _physicsManager->getContentManager()->getmCamera();
	}

	void manaRegen() {
		if (mana < 100 && manaTimer->getMilliseconds()>1000) {
			manaTimer->reset();
			setMana(mana+2);
			
		}
		
	}
	void resetCastTimer() {
		castTimer->reset();
	}
	Ogre::Timer* getCastTimer() {
		return castTimer;
	}
	void setScore(Ogre::Real _score) {
		if (_score < 0) {
			score = 0;
		}
		else {
			score = _score;
		}
	}
	Ogre::Real getScore() {
		return score;
	}
	/****************************************************************************************/
	/**********************Animated Character bots**************************/
	/****************************************************************************************/	
	BotModel * createBot(Ogre::String meshName, Ogre::Vector3 position, Ogre::Real groundSpeed) {
		BotModel * botModel = new BotModel();

		/**/
		//Critter::AnimatedCharacter*     mBot;
		//Critter::CharacterInputHelper   mBotHelper;
		/**/
		botModel->setStartPos(position);
		//Critter::AnimatedCharacterDescription desc;
		botModel->getBotDesc().mShape = NxOgre::SimpleCapsule(5.6, 2);
		botModel->getBotDesc().mCollisionMask = (Walls << 1) | (Objects << 1);
		botModel->getBotDesc().mMaxGroundSpeed = groundSpeed;
		botModel->getBotDesc().setJumpVelocityFromMaxHeight(_physicsManager->getMScene()->getGravity().y, 3.50f);
		//Create critter node for sinbad mesh
		Critter::Node* node = _physicsManager->getMRenderSystem()->createNode();
		Ogre::String nodeName = meshName + std::to_string(botCounter);
		node->createAndAttachEntity(nodeName,meshName);
		botModel->setNodeName(nodeName);
		botModel->setMeshName(meshName);
		botModel->setBotNode(node);
		//Create animated character
		
		botModel->setBot(_physicsManager->getMRenderSystem()->createAnimatedCharacter(position, Ogre::Radian(0), botModel->getBotNode(), botModel->getBotDesc()));
		//Assign helper to sinbad
		botModel->getBot()->setInput(botModel->getBotHelper());
		botCounter++;
		return botModel;
	}


	/****************************************************************************************/
	/************************END Aniamted character spawn*************************/
	/****************************************************************************************/
	void characterRespawn() {
		
		if(alive ==false){
			//camNode->removeAndDestroyAllChildren();
			camNode->detachAllObjects();
			sinbadNode->destroyEntity(mSinbadNodeName);
			_physicsManager->getMRenderSystem()->destroyAnimatedCharacter(mSinbad);

			//reset camera
			Ogre::Camera* mCamera = _physicsManager->getContentManager()->getmCamera();
			mCamera->setPosition(0, 10, -30);
			mCamera->lookAt(0, 5, 0);
		
			createAnimatedCharacter("sinbad.mesh", Ogre::Vector3(-42, 125, -234), 17.0f, 100, 100);
			
			//****
		}
	}
	/******************************Load predefined****************************************/
	void loadPredefinedWorld() {
		setupCharacterAnimations();
		createAnimatedCharacter("sinbad.mesh", Ogre::Vector3(-42, 125, -234),17.0f,100,100);
		//Test sector
		

		//Test sector end
	}
private:
	PhysicsManager* _physicsManager;
	Ogre::Root* _mRoot;
	Ogre::RenderWindow* _mWindow;
	Ogre::SceneManager* _mSceneMgr;
	int counter;
	int botCounter;

	enum ControllerShapeGroups
	{
		NonCollidable = 0,      // Things that the character controller can go through such as Ghosts or tiny
								// objects like small rocks.
		Walls = 1,              // Walls, Floors and other static geometry that can't be moved.
		Objects = 2             // Boxes, Barrels and other dynamic parts of the scene that can be moved by pushing.
	};
	enum SinbadSections
	{
		SinbadLower,
		SinbadUpper
	};
	//Temp
	Critter::AnimatedCharacter*     mSinbad;
	Critter::CharacterInputHelper   mSinbadHelper;
	NxOgre::Actor*                  mTestActor;
	Critter::Node* sinbadNode;
	Ogre::Real health;
	Ogre::Real mana;
	bool alive;
	Ogre::String mSinbadNodeName;
	Ogre::SceneNode* camNode;
	//
	Ogre::Timer* manaTimer;
	Ogre::Timer* castTimer;
	Ogre::Real score;

};



#endif