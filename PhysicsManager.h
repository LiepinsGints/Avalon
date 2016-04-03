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

using namespace Ogre;
using namespace NxOgre;
using namespace Critter;

class PhysicsManager {

public:
	PhysicsManager(Ogre::SceneManager* mSceneMgr, ContentManager* contentManager, Ogre::TerrainGroup* mTerrainGroup);
	~PhysicsManager();

	/********************update physics********************/
	void updatePhysics(const Ogre::FrameEvent& fe) {
		mWorld->advance(fe.timeSinceLastFrame);
		mLastTimeStep = mScene->getTimeStep().getModified();
	}
	/*********************Mbodies***************************/
	void addMBodies(Critter::Body* mBody) {
		mBodies.push_back(mBody);
	}
	/**********************render system***************************/
	Critter::RenderSystem*  getMRenderSystem(){
		return mRenderSystem;
	}
	/**********************get mscene*******************/
	NxOgre::Scene* getMScene() {
		return mScene;
	}
	/********************get Content manager*******************/
	ContentManager* getContentManager() {
		return _contentManager;
	}
	/*******************Create bounding box**********************/
	void createBoundingBox(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Real w, Ogre::Real h, Ogre::Real d) {
		NxOgre::BoxDescription boundingBox(w, h, d);
		boundingBox.mFlags += NxOgre::ShapeFlags::Visualisation;
		boundingBox.mGroup = Objects;//Objects Walls
		
		mScene->createSceneGeometry(boundingBox, NxOgre::Vec3(x, y, z));		
	}
	/*******************Create cylinder box**********************/
	void createBoundingCylinder(Ogre::Real x, Ogre::Real y, Ogre::Real z, Ogre::Real w, Ogre::Real h, Ogre::Real d) {
		NxOgre::BoxDescription boundingBox(w, h, d);
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
	
	//Terrain
	Ogre::TerrainGroup* _mTerrainGroup;
	//
	int terrainIterator;
	enum ControllerShapeGroups
	{
		NonCollidable = 0,      // Things that the character controller can go through such as Ghosts or tiny
								// objects like small rocks.
		Walls = 1,              // Walls, Floors and other static geometry that can't be moved.
		Objects = 2             // Boxes, Barrels and other dynamic parts of the scene that can be moved by pushing.
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
		VisualDebuggerDescription vd_desc;
		vd_desc.mCollision.shapes = true;
		//vd_desc.mCollision.AABB = true;
		
		mRenderSystem = new Critter::RenderSystem(mScene, _mSceneMgr);
		//mRenderSystem->createVisualDebugger();
		mRenderSystem->createVisualDebugger(vd_desc);

		//create character
		
		// Setup Animations. 
		/*
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
		mSinbad = mRenderSystem->createAnimatedCharacter(Ogre::Vector3(-42, 125, -234), Ogre::Radian(0), sinbadNode, desc);
		//Create 
		Ogre::SceneNode* camNode;
		camNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode();
		camNode->attachObject(_contentManager->getmCamera());
		sinbadNode->addSceneNode(camNode);
		//Assign helper to sinbad
		mSinbad->setInput(mSinbadHelper);
		*/
		//Create terrain
		/*NxOgre::HeightField* heightField = NxOgre::HeightFieldManager::getSingleton()->load(NxOgre::Path("ogre://hf.xhf"));
		NxOgre::HeightFieldGeometryDescription heightFieldDescr;
		heightFieldDescr.mHeightField = heightField;
		mScene->createSceneGeometry(heightFieldDescr);*/
		// Fake Floor
		/*NxOgre::BoxDescription fake_floor_desc(1000, 1, 1000);
		fake_floor_desc.mGroup = Walls;
		mScene->createSceneGeometry(fake_floor_desc, NxOgre::Vec3(0, -0.5, 0));
		*/
		/*test*/
		/*
		VisualDebuggerDescription vd_desc;
		vd_desc.mCollision.shapes = true;

		mRenderSystem->createVisualDebugger(vd_desc);

		NxOgre::ManualHeightField mh;
		mh.begin(100, 100);
		for (size_t x = 0; x < 100; x++)
		{
			for (size_t y = 0; y < 100; y++)
			{
				mh.sample(x + (y * 100));
			}
		}

		NxOgre::HeightField* hf = mh.end();

		NxOgre::HeightFieldGeometryDescription hfg_desc(hf);
		hfg_desc.mDimensions.set(1000, 1000, 1000);
		hfg_desc.mFlags += NxOgre::ShapeFlags::Visualisation;
		hfg_desc.mGroup = Walls;
		mScene->createSceneGeometry(hfg_desc);
		*/
		//_mTerrainGroup->getTerrain(2046,2046)->getMaterial();
		Ogre::Terrain* terrain = _mTerrainGroup->getTerrain(0, 0);
		loadTerrainGeometry();
		
		//for (int i = 0; i<this->getNxActor()->getNbShapes(); i++)
		//this->getNxActor()->getShapes()[i]->setFlag(NxShapeFlag::NX_SF_VISUALIZATION, false)
		/*test end*/ 
		/*************Test nxs start*************/
		/*
		NxOgre::ResourceSystem::getSingleton()->openProtocol(new Critter::OgreResourceProtocol());
		//test barrel
		NxOgre::Mesh* mBarrelMesh;
		const Matrix44& globalPose2 = Vec3(-0.5, 10, 20);
		const Vec3& initialVelocity = Vec3(0, 0, 0);
		mBarrelMesh = NxOgre::MeshManager::getSingleton()->load("ogre://column.nxs", "barrel");
		Critter::BodyDescription bodyDescription;
		bodyDescription.mMass = 40.0f;
		bodyDescription.mLinearVelocity = initialVelocity;
		bodyDescription.mGroup = Walls;

		Body* barrel = mRenderSystem->createBody(ConvexDescription(mBarrelMesh), globalPose2, "column.mesh", bodyDescription);
		//barrel->getNode()->setScale(10);
		
		addMBodies(barrel);
		*/
		//bodyDescription.mLinearVelocity = initialVelocity;

		//test playpen
		/*
		NxOgre::Mesh* mPlaypenMesh;
		mPlaypenMesh = NxOgre::MeshManager::getSingleton()->load("ogre://General/column_triangle.nxs", "column");
		
		const Matrix44& globalPose = Vec3 (0, 0.5, 0);
		SceneGeometry* scene_geom = mScene->createSceneGeometry(TriangleGeometryDescription(mPlaypenMesh), globalPose);

		mRenderSystem->createSceneNodeEntityPair("column.mesh", Vec3(globalPose), Quat(globalPose));
		*/
		//test island
		/*
		NxOgre::Mesh* island = NxOgre::MeshManager::getSingleton()->load("ogre://island.nxs", "island");

		mScene->createSceneGeometry(NxOgre::TriangleGeometryDescription(island), NxOgre::Vec3::ZERO);

		// The visualisation.
		SceneNode* islandNode = _mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0, 0, 0));
		islandNode->attachObject(_mSceneMgr->createEntity("island", "island.mesh"));
		islandNode->setScale(1, 1, 1);
		*/
		//manual
		/*
		ShapeDescriptions shapes;

		TriangleGeometryDescription platform_shape;
		platform_shape.mMesh = NxOgre::MeshManager::getSingleton()->load("ogre://platform.nxs");

		platform_shape.mGroupsMask.bits0 = 0xffffffff;
		platform_shape.mGroupsMask.bits1 = 0xffffffff;   // Want everything to collide with the platform ring.
		platform_shape.mGroupsMask.bits2 = 0xffffffff;
		platform_shape.mGroupsMask.bits3 = 0xffffffff;

		TriangleGeometryDescription forcefield_shape;
		forcefield_shape.mMesh = NxOgre::MeshManager::getSingleton()->load("ogre://forcefield.nxs");


		forcefield_shape.mGroupsMask.bits0 = 1;

		shapes.push_back(&platform_shape);
		shapes.push_back(&forcefield_shape);

		SceneGeometry* geom = mScene->createSceneGeometry(shapes, globalPose);
		Ogre::SceneNode* node = mRenderSystem->createSceneNodeEntityPair("nxogre.platform.mesh", Vec3(globalPose), Quat(globalPose));
		*/
		//
		
		/*************Test nxs end*************/
	}
	
	/***********load terrain from img***************/
	void loadTerrainGeometry()
	{
		
		TerrainGroup::TerrainIterator ti = _mTerrainGroup->getTerrainIterator();
		while (ti.hasMoreElements())
		{
			Terrain* t = ti.getNext()->instance;
			if (t)
			{
				//
				//const Ogre::String& name = 
				float* data = t->getHeightData();
				Ogre::uint16 size = t->getSize();
				Ogre::Real worldSize = t->getWorldSize();
				Ogre::Real minHeight = t->getMinHeight();
				Ogre::Real maxHeight = t->getMaxHeight();
				const Ogre::Vector3& position = t->getPosition();
				//
				/*u32 size = t->getSize();
				f32 worldSize = t->getWorldSize();
				f32 minHeight = t->getMinHeight();
				f32 maxHeight = t->getMaxHeight();
				f32* data = t->getHeightData();
				reVector3Df position = t->getPosition();
				*/
				// Create the manual heightfield
				NxOgre::ManualHeightField mhf;
				mhf.begin(size, size);
				Ogre::Real normMin = -32768.0f;
				Ogre::Real normMax = 32767.0f;
				// Sample the data to the manual heightfield
				for (int x = 0; x < size; ++x)
				{
					NxOgre::Enums::HeightFieldTesselation tess = NxOgre::Enums::HeightFieldTesselation_NW_SE;
					for (int z = size - 1; z >= 0; --z)
					{
						Ogre::Real height = data[(size * z) + x];
						short sample = (short)(((height - minHeight) / (maxHeight - minHeight)) * (normMax - normMin) + normMin);
						mhf.sample(sample, 0, 0, tess);
						if (tess == NxOgre::Enums::HeightFieldTesselation_NE_SW)
							tess = NxOgre::Enums::HeightFieldTesselation_NW_SE;
						else
							tess = NxOgre::Enums::HeightFieldTesselation_NE_SW;
					}
				}
				// Create the actual heightfield
				NxOgre::HeightField *hf = mhf.end("terrain"+ terrainIterator);
				Ogre::Real hf_size = worldSize + (worldSize / size);
				Ogre::Real hf_height = (maxHeight - minHeight) / 2.0f;
				Ogre::Real hf_pose_x = position.x - (worldSize / 2.0f);
				Ogre::Real hf_pose_y = position.y + ((maxHeight + minHeight) / 2.0f);
				Ogre::Real hf_pose_z = position.z - (worldSize / 2.0f);
#if NxOgreVersionMajor <= 1 && NxOgreVersionMinor <= 5
				NxOgre::HeightFieldGeometry* hfg = new NxOgre::HeightFieldGeometry(hf, NxOgre::Vec3(hf_size, hf_height, hf_size));
				hfg->setLocalPose(NxOgre::Matrix44(NxOgre::Vec3(hf_pose_x, hf_pose_y, hf_pose_z)));
				mScene->createSceneGeometry(hfg);
#else
				NxOgre::HeightFieldGeometryDescription desc(hf, NxOgre::Vec3(hf_size, hf_height, hf_size));
				desc.mGroup = Walls;
				desc.mFlags -= NxOgre::ShapeFlags::Visualisation;
				//setFlag(NxShapeFlag::NX_SF_VISUALIZATION, false);
				mScene->createSceneGeometry(desc, NxOgre::Matrix44(NxOgre::Vec3(hf_pose_x, hf_pose_y, hf_pose_z)));
#endif
			}
			}
	}
	/*********************destroy physics*********************/
	void stopPhysics()
	{
		NxOgre::World::destroyWorld();
	}
	
};
PhysicsManager::PhysicsManager(Ogre::SceneManager* mSceneMgr, ContentManager* contentManager, Ogre::TerrainGroup* mTerrainGroup) {
	_contentManager = contentManager;
	_mSceneMgr = mSceneMgr;
	_mTerrainGroup = mTerrainGroup;
	terrainIterator = 0;
	setupPhysics();
}
PhysicsManager::~PhysicsManager() {
	stopPhysics();
}
#endif
