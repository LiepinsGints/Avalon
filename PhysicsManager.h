#ifndef PhysicsManager_H
#define PhysicsManager_H

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

class PhysicsManager {

public:
	PhysicsManager(Ogre::SceneManager* mSceneMgr, ContentManager* contentManager);
	~PhysicsManager();

	/********************update physics********************/
	void updatePhysics(const Ogre::FrameEvent& fe) {
		mWorld->advance(fe.timeSinceLastFrame);
		mLastTimeStep = mScene->getTimeStep().getModified();
	}
	/*********************Create cube***************************/
	void createCube() {
		Critter::BodyDescription bodyDescriptionTemp;
		bodyDescriptionTemp.mMass = 20.0f; // Set the mass to 20kg.
		Critter::Body* mBodyTemp;
		mBodyTemp = mRenderSystem->createBody(NxOgre::BoxDescription(4, 4, 4), NxOgre::Vec3(9, 30, 0), "cube.1m.mesh", bodyDescriptionTemp);
		mBodyTemp->getNode()->setScale(4.0);
		mBodies.push_back(mBodyTemp);
		
	}
	/**********************CharacterControls***********************/
	Critter::AnimatedCharacter* getCharacter() {
		return mSinbad;
	}
	Critter::CharacterInputHelper getCharacterInputHelper() {
		return mSinbadHelper;
	}
	NxOgre::Actor* getActor() {
		return mTestActor;
	}

	/*******************Character controls***********************/
	void resetHelper() {
		mSinbadHelper.reset();
	}
	void applyHelper() {
		mSinbad->setInput(mSinbadHelper);
	}

	void left() {
		mSinbadHelper.input.is_turning = true;
		mSinbadHelper.left(127);		
	}
	void right() {
		mSinbadHelper.input.is_turning = true;
		mSinbadHelper.right(127);
	}
	void forward() {
		mSinbadHelper.forward(127);
	}
	void backward() {
		mSinbadHelper.backward(127);
	}
	void jump() {
		mSinbadHelper.up(127);
	}

private:
	NxOgre::World*          mWorld;
	NxOgre::Scene*          mScene;
	float                   mLastTimeStep;
	NxOgre::Material*       mDefaultMaterial;
	Critter::RenderSystem*  mRenderSystem;
	
	std::deque<Critter::Body*> mBodies;
	Ogre::SceneManager*		_mSceneMgr;

	ContentManager* _contentManager;
	//character
	Ogre::Real                      mCameraYaw, mCameraPitch;
	Ogre::Vector3                   mCameraOffset;
	Critter::AnimatedCharacter*     mSinbad;
	Critter::CharacterInputHelper   mSinbadHelper;
	NxOgre::Actor*                  mTestActor;
	
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
	/*****************Set up physics**********************/
	void setupPhysics()
	{
		// Create the world.
		mWorld = NxOgre::World::createWorld();

		mWorld->getRemoteDebugger()->connect();

		// Create the scene
		NxOgre::SceneDescription scene_description;
		scene_description.mGravity = NxOgre::Constants::MEAN_EARTH_GRAVITY;
		scene_description.mUseHardware = false;

		mScene = mWorld->createScene(scene_description);

		// Set default material properties
		mDefaultMaterial = mScene->getMaterial(0);
		mDefaultMaterial->setRestitution(0.1f);
		mDefaultMaterial->setDynamicFriction(0.9);
		mDefaultMaterial->setStaticFriction(0.5);

		// Plane creation
		mScene->createSceneGeometry(NxOgre::PlaneGeometryDescription());

		// Create the rendersystem.
		mRenderSystem = new Critter::RenderSystem(mScene, _mSceneMgr);
		mRenderSystem->createVisualDebugger();

		//create character
		// Fake Floor
		NxOgre::BoxDescription fake_floor_desc(1000, 1, 1000);
		fake_floor_desc.mGroup = Walls;
		mScene->createSceneGeometry(fake_floor_desc, NxOgre::Vec3(0, -0.5, 0));
		// Setup Animations. 
		
		mRenderSystem->addAnimation("sinbad.mesh", SinbadLower, Critter::Enums::StockAnimationID_Idle, "IdleBase");
		mRenderSystem->addAnimation("sinbad.mesh", SinbadUpper, Critter::Enums::StockAnimationID_Idle, "IdleTop");
		mRenderSystem->addAnimation("sinbad.mesh", SinbadLower, Critter::Enums::StockAnimationID_Forward, "RunBase");
		mRenderSystem->addAnimation("sinbad.mesh", SinbadUpper, Critter::Enums::StockAnimationID_Forward, "RunTop");
		mRenderSystem->addAnimation("sinbad.mesh", SinbadLower, Critter::Enums::StockAnimationID_Jump, "JumpStart", 5.0, false);
		mRenderSystem->addAnimation("sinbad.mesh", SinbadLower, Critter::Enums::StockAnimationID_Fall, "JumpLoop");
		mRenderSystem->addAnimation("sinbad.mesh", SinbadLower, Critter::Enums::StockAnimationID_Land, "JumpEnd", 5.0, false);
		mRenderSystem->addAnimation("sinbad.mesh", SinbadLower, 100, "Dance", 5.0, false);
		
		Critter::AnimatedCharacterDescription desc;
		desc.mShape = NxOgre::SimpleCapsule(5.6, 2);
		desc.mCollisionMask = (Walls << 1) | (Objects << 1);
		desc.mMaxGroundSpeed = 17.0f;
		desc.setJumpVelocityFromMaxHeight(mScene->getGravity().y, 3.50f);
		//Create critter node for sinbad mesh
		Critter::Node* sinbadNode = mRenderSystem->createNode();
		sinbadNode->createAndAttachEntity("sinbad.mesh");
		//Create animated character
		mSinbad = mRenderSystem->createAnimatedCharacter(Ogre::Vector3(0, 25, 0), Ogre::Radian(0), sinbadNode, desc);
		//Create 
		Ogre::SceneNode* camNode;
		camNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		camNode->attachObject(_contentManager->getmCamera());
		sinbadNode->addSceneNode(camNode);
		
		//Assign helper to sinbad
		mSinbad->setInput(mSinbadHelper);
		
		
	}
	
	/*********************destroy physics*********************/
	void stopPhysics()
	{
		NxOgre::World::destroyWorld();
	}
	
};
PhysicsManager::PhysicsManager(Ogre::SceneManager* mSceneMgr, ContentManager* contentManager) {
	_contentManager = contentManager;
	_mSceneMgr = mSceneMgr;
	setupPhysics();
}
PhysicsManager::~PhysicsManager() {
	stopPhysics();
}
#endif
