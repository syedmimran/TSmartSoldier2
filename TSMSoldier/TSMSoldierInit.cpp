#include "TSMSoldier.h"
#include "Utils.h"

String ActionStr[AC_NUM_OF_ACTIONS] = {
	"TESTING",
	"PATROL",
	"SPOT",
	"AIM",
	"SHOOT",
	"ATTACK",
	"TAKECOVER",
	"AC_RETREAT",
	"AC_THROW_GRENADE",
	"DEAD"
};


String ProfileStr[PF_NUM_OF_PROFILES] = {
	"TPOSE",
	"STANDING1",
	"STANDING2",
	"SCROCHING",
	"PRONNING",
	"DYING1",
	"DYING2",
	"DYING3"
};

void TSMSoldier::displayInfo() {

	lineDisplay(1 + int(id), 2, String::format("%d",int(id)));

	if (id != 1 )
		return; 
	int x = 1;
	int y = 1;
	lineDisplay(x, y++ + id * 10, String::format("mFreeze =%d sinValue%f", mFreeze, sinValue));
	lineDisplay(x, y++ + id * 10, String::format("mTargetArrived=%d mTargetDistance = %f pointPatrolIndex=%d", mTargetArrived, mTargetDistance, pointPatrolIndex));
	lineDisplay(x, y++ + id * 10, String::format("mTargetPos x=%f y=%f z=%f", mTargetPos.x, mTargetPos.y, mTargetPos.z));

	//lineDisplay(x, y++ + id * 10, String::format("theEnemyFound=%d mBulletCount = %d reloadSeq=%d", theEnemyFound, mBulletCount, reloadSeq));
	//lineDisplay(x, y++ + id * 10, String::format("behaviourType=%d  inTransition=%d inAiming=%d", (int)behaviourType, inTransition));
	//lineDisplay(x, y++ + id * 10, String::format("mEnemyDistance=%.3f wpDistance=%.3f  ", mEnemyDistance, wpDistance));
	//lineDisplay(x, y++ + id * 10, String::format("headRotV x=%.3f y=%.3f  z=%.3f", headRotV.x, headRotV.y, headRotV.z));
	//lineDisplay(x, y++ + id * 10, String::format("mTargetHeading =%.3f mCurHeading =%.3f mHeadingDiff =%.3f", mTargetHeading, mCurHeading, mHeadingDiff));
	//lineDisplay(x, y++ + id * 10, String::format("inAimingOn=%d  inAimingOff=%d AimingInTransition=%d", inAimingOn, inAimingOff, AimingInTransition));
	//lineDisplay(x, y++ + id * 10, String::format("inReloadOn=%d  inReloadOff=%d ReloadInTransition=%d", inReloadOn, inReloadOff, ReloadInTransition));
	lineDisplay(x, y++ + id * 10, String::format("mMovingSpeed =%.3f tMovingSpeed =%.3f ", mMovingSpeed, tMovingSpeed));
	lineDisplay(x, y++ + id * 10, "ActionMode:" + ActionStr[(int)action_Mode]+":"+ActionStr[prev_action_Mode] + String::format(" <%d : %d> behaviour_type=%d modeTimer =%.3f  isDead = %d", (int)action_Mode, prev_action_Mode, (int)behaviour_type, modeTimer[(int)action_Mode], (int)isDead));
	lineDisplay(x, y++ + id * 10, String::format("alertOn=%d spineRotV.z=%f", alertOn, spineRotV.z));
	//lineDisplay(x, 5 + id * 10, String::format("spineRotV x=%f y=%f z=%f  ", spineRotV.x, spineRotV.y, spineRotV.z));

	lineDisplay(x, y++ + id * 10, String::format("mCurHeading=%f mTargetHeading=%f mHeadingDiff=%f mActualTurnRate=%f", mCurHeading, mTargetHeading, mHeadingDiff));
	lineDisplay(x, y++ + id * 10, String::format("mTargetPos x=%f y=%f z=%f", mTargetPos.x, mTargetPos.y, mTargetPos.z));
	lineDisplay(x, y++ + id * 10, "Profile: " + ProfileStr[mProfile] + "  prevProfile: " + ProfileStr[mPrevProfile]  + String::format(" tempProfile=%d inTransition =%d", tempProfile, inTransition));
	lineDisplay(x, y++ + id * 10, String::format("cLeftFingerAngle =%f cLeftFingerAngle =%f fingerTotalIndex=%d ", cLeftFingerAngle, cRightFingerAngle, numOfFingerBonesIncluded));
	lineDisplay(x, y++ + id * 10, String::format("mLeftFingerAngle =%f mLeftFingerAngle =%f  ", mLeftFingerAngle, mRightFingerAngle));

	//lineDisplay(x, y++ + id * 10, String::format("mBulletCount = %d rifleAimStatus=%d reloadSeq=%d reloadStatus=%d reload2Status=%d reloadWaitTime=%f reloadTimer=%f", mBulletCount, rifleAimStatus, reloadSeq, reloadStatus, reload2Status, reloadWaitTime, reloadTimer));
	lineDisplay(1, y + id * 10, "profileQueue" + String::format(" <%d>", newProfileQueue.size()));
	for (int i = 0; i < newProfileQueue.size(); i++) {
		lineDisplay(5 + i, y + id * 10, String::format("%d", newProfileQueue[i]));
	}
	lineDisplay(1, ++y + id * 10, String::format("inTransition=%d timer =%3.1f", inTransition, transitionTimer));

}



void TSMSoldier::InitBonesAndAll() {
	
	mAvatar->setEnabled(1);
	mCurWorldPos = (Vec3)initPos;// +vec3(id, 0, 0);

	mAvatar->setRotation(quat_identity);

	InitEyePoint();
	InitBonesTransform();
	InitActiveBones();
	InitBonesRot();
	for (int i = 0; i < AC_NUM_OF_ACTIONS; i++) {
		modeTimer[i] = 0.0;
		mFirstAction[i] = false;
	}
}


//------------------------------------------------------------------------------
void TSMSoldier::InitActiveBones() {
	BoneId.resize(numOfBones);
	for (int i = 0; i < numOfBones; i++) {
		BoneId[i] = gBoneId[i];
	}
}


void TSMSoldier::InitBonesTransform() {

	numOfBones = mAvatar->getNumBones();
	vec3 pos, scale;
	quat rot;
	for (int i = 0; i < numOfBones; i++) {
		decomposeTransform(mAvatar->getBoneTransform(i), pos, rot, scale);
		initBonePos.append(pos);
		initBoneRot.append(rot);
		initScale.append(scale);
	}
}


void TSMSoldier::InitBonesRot() {
	for (int i = 0; i < numOfBones; i++) {
		boneRot[i].x = boneRot[i].y = boneRot[i].z = 0.0;
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = ROT_R_ARM; k < NO_OF_ROTS; k++) {
				jointRot1[k][i][j] = vec3_zero;
				jointRot[k][i][j] = quat_identity;
			}
		}
	}
}

void TSMSoldier::InitEyePoint() {
	if (!mAvatar)
		return;

	NodePtr childNode;
	int index = mAvatar->findChild("headTransform");
	if (index == -1)
		return;
	childNode = mAvatar->getChild(index);
	index = childNode->findChild("eyePoint");
	if (index == -1)
		return;
	eyePoint = childNode->getChild(index);
}



void TSMSoldier::getInputRot() {
	if (Input::isKeyPressed(Input::KEY_INSERT)) {
		mRot1.x += 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_DELETE)) {
		mRot1.x -= 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_HOME)) {
		mRot1.y += 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_END)) {
		mRot1.y -= 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_PGUP)) {
		mRot1.z += 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_PGDOWN)) {
		mRot1.z -= 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_R)) {
		mRot1 = Vec3(0.0, 0.0, 0.0);
	}


	if (Input::isKeyPressed(Input::KEY_DIGIT_1)) {
		mRot2.x += 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_DIGIT_2)) {
		mRot2.x -= 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_DIGIT_3)) {
		mRot2.y += 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_DIGIT_4)) {
		mRot2.y -= 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_DIGIT_5)) {
		mRot2.z += 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_DIGIT_6)) {
		mRot2.z -= 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_R)) {
		mRot2 = Vec3(0.0, 0.0, 0.0);
	}

	if (Input::isKeyPressed(Input::KEY_F)) {
		mRot3.x += 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_G)) {
		mRot3.x -= 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_H)) {
		mRot3.y += 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_J)) {
		mRot3.y -= 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_K)) {
		mRot3.z += 0.25;
	}
	if (Input::isKeyPressed(Input::KEY_L)) {
		mRot3.z -= 0.25;
	}

	if (Input::isKeyPressed(Input::KEY_R)) {
		mRot1 = Vec3(0.0, 0.0, 0.0);
		mRot2 = Vec3(0.0, 0.0, 0.0);
		mRot3 = Vec3(0.0, 0.0, 0.0);
	}
	lineDisplay(1, 4, String::format("ROT1 X=%3.1f Y=%3.1f Z=%3.1f", mRot1.x, mRot1.y, mRot1.z));
	lineDisplay(1, 5, String::format("ROT2 X=%3.1f Y=%3.1f Z=%3.1f", mRot2.x, mRot2.y, mRot2.z));
	lineDisplay(1, 6, String::format("ROT3 X=%3.1f Y=%3.1f Z=%3.1f", mRot3.x, mRot3.y, mRot3.z));	
}


