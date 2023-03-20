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
	//return; // rrr

	lineDisplay(1 + int(id), 2, String::format("%d",int(id)));

	//if (id != 1 && id != 2)
	if (id != 1 )
			return; 
	int x = 1;
	int y = 1;
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
	lineDisplay(x, y++ + id * 10, "ActionMode:" + ActionStr[(int)action_Mode] + String::format(" <%d : %d> behaviour_type=%d modeTimer =%.3f  isDead = %d", (int)action_Mode, prev_action_Mode, (int)behaviour_type, modeTimer[(int)action_Mode], isDead));
	lineDisplay(x, y++ + id * 10, String::format("alertOn=%d spineRotV.z=%f", alertOn, spineRotV.z));
	//lineDisplay(x, 5 + id * 10, String::format("spineRotV x=%f y=%f z=%f  ", spineRotV.x, spineRotV.y, spineRotV.z));

	lineDisplay(x, y++ + id * 10, String::format("mCurHeading=%f mTargetHeading=%f mHeadingDiff=%f mActualTurnRate=%f", mCurHeading, mTargetHeading, mHeadingDiff));
	lineDisplay(x, y++ + id * 10, String::format("mTargetPos x=%f y=%f z=%f", mTargetPos.x, mTargetPos.y, mTargetPos.z));
	lineDisplay(x, y++ + id * 10, "Profile: " + ProfileStr[mProfile] + "  prevProfile: " + ProfileStr[mPrevProfile]  + String::format(" tempProfile=%d inTransition =%d", tempProfile, inTransition));
	lineDisplay(x, y++ + id * 10, String::format("cLeftFingerAngle =%f cLeftFingerAngle =%f fingerTotalIndex=%d ", cLeftFingerAngle, cRightFingerAngle, fingerTotalIndex));
	lineDisplay(x, y++ + id * 10, String::format("mLeftFingerAngle =%f mLeftFingerAngle =%f  ", mLeftFingerAngle, mRightFingerAngle));

	//lineDisplay(x, y++ + id * 10, String::format("mBulletCount = %d rifleAimStatus=%d reloadSeq=%d reloadStatus=%d reload2Status=%d reloadWaitTime=%f reloadTimer=%f", mBulletCount, rifleAimStatus, reloadSeq, reloadStatus, reload2Status, reloadWaitTime, reloadTimer));
	lineDisplay(1, y + id * 10, "profileQueue" + String::format(" <%d>", newProfileQueue.size()));
	for (int i = 0; i < newProfileQueue.size(); i++) {
		lineDisplay(5 + i, y + id * 10, String::format("%d", newProfileQueue[i]));
	}
	lineDisplay(1, ++y + id * 10, String::format("inTransition=%d timer =%3.1f", inTransition, transitionTimer));

}



void TSMSoldier::initBonesAndAll() {
	
	mAvatar->setEnabled(1);
	mCurWorldPos = (Vec3)initPos;// +vec3(id, 0, 0);

	mAvatar->setRotation(quat_identity);

	initEyePoint();
	getBonesInitTransform();
	initActiveBones();
	setBoneRotInit();
	for (int i = 0; i < AC_NUM_OF_ACTIONS; i++) {
		modeTimer[i] = 0.0;
		mfirstModeState[i] = false;
	}
}


//------------------------------------------------------------------------------
void TSMSoldier::initActiveBones() {
	BoneId.resize(numOfBones);
	for (int i = 0; i < numOfBones; i++)
	   BoneId[i] = 0;

	BoneId[0] = SWAT_Hips;
	BoneId[1] = SWAT_Spine;
	BoneId[2] = SWAT_Spine1;
	BoneId[3] = SWAT_Spine2;
	BoneId[4] = SWAT_Neck;
	BoneId[5] = SWAT_Head;

	BoneId[6] = SWAT_LeftHip;
	BoneId[7] = SWAT_LeftKnee;
	BoneId[8] = SWAT_LeftFoot;
	BoneId[9] = SWAT_LeftToe;

	BoneId[10] = SWAT_RightHip;
	BoneId[11] = SWAT_RightKnee;
	BoneId[12] = SWAT_RightFoot;
	BoneId[13] = SWAT_RightToe;


	BoneId[14] = SWAT_LeftShoulder;
	BoneId[15] = SWAT_LeftArm;
	BoneId[16] = SWAT_LeftElbow;
	BoneId[17] = SWAT_LeftHand;

	BoneId[18] = SWAT_RightShoulder;
	BoneId[19] = SWAT_RightArm;
	BoneId[20] = SWAT_RightElbow;
	BoneId[21] = SWAT_RightHand;


	BoneId[22] = SWAT_LeftThumb1;
	BoneId[23] = SWAT_LeftThumb2;
	BoneId[24] = SWAT_LeftThumb3;
	BoneId[25] = SWAT_LeftIndex1;
	BoneId[26] = SWAT_LeftIndex2;
	BoneId[27] = SWAT_LeftIndex3;
	BoneId[28] = SWAT_LeftMiddle1;
	BoneId[29] = SWAT_LeftMiddle2;
	BoneId[30] = SWAT_LeftMiddle3;
	BoneId[31] = SWAT_LeftRing1;
	BoneId[32] = SWAT_LeftRing2;
	BoneId[33] = SWAT_LeftRing3;
	BoneId[34] = SWAT_LeftPinky1;
	BoneId[35] = SWAT_LeftPinky2;
	BoneId[36] = SWAT_LeftPinky3;



	BoneId[37] = SWAT_RightThumb1;
	BoneId[38] = SWAT_RightThumb2;
	BoneId[39] = SWAT_RightThumb3;
	BoneId[40] = SWAT_RightIndex1;
	BoneId[41] = SWAT_RightIndex2;
	BoneId[42] = SWAT_RightIndex3;
	BoneId[43] = SWAT_RightMiddle1;
	BoneId[44] = SWAT_RightMiddle2;
	BoneId[45] = SWAT_RightMiddle3;
	BoneId[46] = SWAT_RightRing1;
	BoneId[47] = SWAT_RightRing2;
	BoneId[48] = SWAT_RightRing3;
	BoneId[49] = SWAT_RightPinky1;
	BoneId[50] = SWAT_RightPinky2;
	BoneId[51] = SWAT_RightPinky3;
}

void TSMSoldier::getBonesInitTransform() {

	numOfBones = mAvatar->getNumBones();
	vec3 pos, scale;
	quat rot;
	for (int i = 0; i < numOfBones; i++) {
		decomposeTransform(mAvatar->getBoneTransform(i), pos, rot, scale);
		initQuat.append(rot);
		initScale.append(scale);
		initPosBone.append(pos);
		newInitPosBone.append(pos);
	}

	for (int i = SWAT_Hips; i < numOfBones; i++) {
		BoneRot.append(quat_identity);
	}
}


void TSMSoldier::setBoneRotInit() {
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
	// rrr
	lineDisplay(1, 4, String::format("ROT1 X=%3.1f Y=%3.1f Z=%3.1f", mRot1.x, mRot1.y, mRot1.z), vec4_green);
	lineDisplay(1, 5, String::format("ROT2 X=%3.1f Y=%3.1f Z=%3.1f", mRot2.x, mRot2.y, mRot2.z), vec4_green);
	lineDisplay(1, 6, String::format("ROT3 X=%3.1f Y=%3.1f Z=%3.1f", mRot3.x, mRot3.y, mRot3.z), vec4_green);

}


