#include "TSMSoldier.h"
#include <UnigineVisualizer.h>
#include "Utils.h"

quat quatXT[60];
vec3 posXt[60];
vec3 posXX;
quat rotXX;
SendDataType mSendData;

void TSMSoldier::InitAnimFile() {
	mAvatar->setNumLayers(1);
	//int res = mAvatar->setAnimation(0, "Anim_file/WalkNormal.anim");
	//int res = mAvatar->setAnimation(0, "Anim_file/Run.anim");
	int res = mAvatar->setAnimation(0, "Anim_file/ProneMove.anim");
	mAvatar->setLayerEnabled(0, true);
	initFrameTime = Game::getTime();
	node->setWorldRotation(quat(0.0, 0.0,90));
}


void TSMSoldier::playAnimFile() {
	if (mFreeze) {
		initFrameTime = Game::getTime();
		stopFrameTime = currFrame;
	}
	else {
		mFrameTime = Game::getTime() - initFrameTime;
		currFrame = stopFrameTime + mFrameTime * mAnimationSpeed * 0.90;
	}

	mAvatar->setFrame(0, currFrame);
	posXX = (vec3)node->getWorldPosition();
	node->setWorldRotation(quat(0.0, 0.0, -90));
	if (Input::isKeyPressed(Input::KEY_Y)) {
		node->setWorldRotation(quat(0.0, 0.0, 180));
	}
	if (Input::isKeyPressed(Input::KEY_U)) {
		node->setWorldRotation(quat(0.0, 0.0, 0));
	}
	if (Input::isKeyPressed(Input::KEY_I)) {
		node->setWorldRotation(quat(0.0, 0.0, 90));
	}
	rotXX = node->getWorldRotation();
}

void TSMSoldier::sendDataTest() {
	vec3 pos;
	quat rot;
	vec3 scale;
	int boneId;
	//resetBones();
	for (int i = 0; i < gNumOfActiveBones + gNumOfFingerBones; i++) {
		boneId = BoneId[i];
		decomposeTransform(mAvatar->getBoneTransform(boneId), pos, rot, scale);
		posXt[boneId] = pos - initBonePos[boneId];
		//quatXT[boneId] = rot / initBoneRot[boneId];
		quatXT[boneId] = initBoneRot[boneId] * rot;
		float x2 = quatXT[boneId].getAngle(vec3_left);
		float y2 = quatXT[boneId].getAngle(vec3_forward);
		float z2 = quatXT[boneId].getAngle(vec3_up);
		if (gBoneRow[boneId] < gNumOfActiveBones) {
			lineDisplay(50, i + 5, gBoneNames[BoneId[i]] + String::format(" x=%.3f y=%.3f z=%.3f", x2, y2, z2));
			//lineDisplay(50, i + 5, String::format(" x=%.3f y=%.3f z=%.3f", posXt[boneId].x, posXt[boneId].y, posXt[boneId].z));
		}
	}
}


void TSMSoldier::processClientTest() {
	node->setWorldPosition((Vec3)posXX + Vec3(1.0, 0.0, 0.0));
	node->setWorldRotation(rotXX);
	mat4 transform = mat4_identity;
	int boneId;
	//resetBones();
	//for (int i = 0; i < gNumOfActiveBones + gNumOfFingerBones; i++) {
	for (int i = gNumOfActiveBones - 1 + gNumOfFingerBones; i >= 0; i--) {
		boneId = BoneId[i];	
		//composeTransform(transform, initBonePos[boneId] + posXt[boneId], quatXT[boneId] * initBoneRot[boneId], initScale[boneId]);
		composeTransform(transform, initBonePos[boneId] + posXt[boneId], initBoneRot[boneId] * quatXT[boneId], initScale[boneId]);
		//mAvatar->setBoneTransform(boneId, transform);
		mAvatar->setBoneTransform(boneId, transform);
	}
}


void TSMSoldier::sendData() {
	mSendData.Pos = (vec3)mCurWorldPos;
	mSendData.Rot = mCurWorldRot;
	mSendData.mProfile = mProfile;
	mSendData.mPrevProfile = mPrevProfile;
	mSendData.mActionMode = action_Mode;
	mSendData.tMovingSpeed = tMovingSpeed;
	mSendData.spineRotV = spineRotV;
	mSendData.headRotV = headRotV;
	mSendData.aimingDirection = aimingDirection;

	rifleAimStatus ? mSendData.Attribute |= 1 : mSendData.Attribute &= ~1;
	reloadStatus ? mSendData.Attribute |= 2 : mSendData.Attribute &= ~2;
	reload2Status ? mSendData.Attribute |= 4 : mSendData.Attribute &= ~4;
	isShooting ? mSendData.Attribute |= 8 : mSendData.Attribute &= ~8;
	triggerPull ? mSendData.Attribute |= 16 : mSendData.Attribute &= ~16;
	weaponOnTarget ? mSendData.Attribute |= 32 : mSendData.Attribute &= ~32;
	isDead ? mSendData.Attribute |= 64 : mSendData.Attribute &= ~64;
}


void TSMSoldier::processClient() {
	mCurWorldPos = (Vec3)mSendData.Pos + Vec3(2.0, 0.0, 0.0);
	mCurWorldRot = mSendData.Rot;
	node->setWorldPosition(mCurWorldPos);
	node->setWorldRotation(mCurWorldRot);

	mProfile = mSendData.mProfile;
	mPrevProfile = mSendData.mPrevProfile;;
	action_Mode = mSendData.mActionMode;
	tMovingSpeed = mSendData.tMovingSpeed;
	spineRotV = mSendData.spineRotV;
	headRotV = mSendData.headRotV;
	aimingDirection = mSendData.aimingDirection;

	rifleAimStatus = mSendData.Attribute & 1;
	reloadStatus = mSendData.Attribute & 2;
	reload2Status = mSendData.Attribute & 4;
	isShooting = mSendData.Attribute & 8;
	triggerPull = mSendData.Attribute & 16;
	weaponOnTarget = mSendData.Attribute & 32;
	isDead = mSendData.Attribute & 64;

	updateMovement();

	displayInfo();

}


