#ifndef DesignerObjects_H
#define DesignerObjects_H

#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <NxOgre.h>
#include <critter.h>
#include <CritterCommon.h>
#include <CritterBody.h>
#include <NxOgreVec3.h>
#include <NxOgreBox.h>
#include <NxOgreBoxDescription.h>
#include <NxOgreMatrix.h>
#include <NxCharacter.h>

#include <iostream>
#include <string>


using namespace Ogre;
class DesignerObjects {
public:
	DesignerObjects() {
		worldId = -1;
		boundId = -1;
	}
	~DesignerObjects() {


	}

	void setWorldId(int _worldId) {
		worldId = _worldId;
	}
	int getWorldId() {
		return worldId;
	}
	
	void setBoundId(int _boundId) {
		boundId = _boundId;
	}
	int getBoundId() {
		return boundId;
	}

	void setType(int _type) {
		type = _type;
	}
	int getType() {
		return type;
	}

	void setmBody(Critter::Body* _mBody) {
		mBody = _mBody;
	}
	Critter::Body* getmBody() {
		return mBody;
	}

	void setmKinematicBody(Critter::KinematicBody* _mKinematicBody) {
		mKinematicBody = _mKinematicBody;
	}
	Critter::KinematicBody* getmKinematicBody () {
		return mKinematicBody;
	}

	void setModelNode(SceneNode* _modelNode) {
		modelNode = _modelNode;
	}
	SceneNode* getmodelNode() {
		return modelNode;
	}

	void setScene_geom(SceneGeometry* _scene_geom) {
		scene_geom = _scene_geom;
	}
	SceneGeometry* getScene_geom() {
		return scene_geom;
	}
private:
	int worldId;
	int boundId;
	int type;
	Critter::Body* mBody;
	Critter::KinematicBody* mKinematicBody;
	SceneNode* modelNode;
	SceneGeometry* scene_geom;

};
#endif