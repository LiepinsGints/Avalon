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
#include <CritterBody.h>
#include <NxOgreVec3.h>
#include <NxOgreBox.h>
#include <NxOgreBoxDescription.h>
#include <NxOgreMatrix.h>

class PhysicsManager {

public:
	PhysicsManager(Ogre::SceneManager* mSceneMgr);
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
		mBodyTemp = mRenderSystem->createBody(NxOgre::BoxDescription(1, 1, 1), NxOgre::Vec3(9, 30, 0), "cube.1m.mesh", bodyDescriptionTemp);
		mBodies.push_back(mBodyTemp);
	}
private:
	NxOgre::World*          mWorld;
	NxOgre::Scene*          mScene;
	float                   mLastTimeStep;
	NxOgre::Material*       mDefaultMaterial;
	Critter::RenderSystem*  mRenderSystem;
	Critter::Body*          mBody;
	Critter::Body*          mBody2;

	std::deque<Critter::Body*> mBodies;

	Ogre::SceneManager*		_mSceneMgr;

	
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

		// Setup a BodyDescription.
		Critter::BodyDescription bodyDescription;
		bodyDescription.mMass = 40.0f; // Set the mass to 40kg.
		mBody = mRenderSystem->createBody(NxOgre::BoxDescription(1, 1, 1), NxOgre::Vec3(10, 25, 0), "cube.1m.mesh", bodyDescription);

		Critter::BodyDescription bodyDescription2;
		bodyDescription2.mMass = 20.0f; // Set the mass to 20kg.
		mBody2 = mRenderSystem->createBody(NxOgre::BoxDescription(1, 1, 1), NxOgre::Vec3(9, 30, 0), "cube.1m.mesh", bodyDescription2);
		
	}
	
	/*********************destroy physics*********************/
	void stopPhysics()
	{
		NxOgre::World::destroyWorld();
	}
	
};
PhysicsManager::PhysicsManager(Ogre::SceneManager* mSceneMgr) {
	_mSceneMgr = mSceneMgr;
	setupPhysics();
}
PhysicsManager::~PhysicsManager() {
	stopPhysics();
}
#endif
