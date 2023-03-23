#include "TSMSoldier.h"
#include "Utils.h"
//#include "IsmatInterface.h" // rrr

void TSMSoldier::updateMovement() {	
	updateShooting();
	updateAvatarRot();
	updateTransition();
	setSinWaves();
	updateMovingSpeed();
	updateFingerAngles();
	updateBones();
}

void TSMSoldier::updateFingerAngles() {
	bool done1 = GetToValue(cLeftFingerAngle, mLeftFingerAngle, Game::getIFps()*10);
	bool done2 = GetToValue(cRightFingerAngle, mRightFingerAngle, Game::getIFps()*10);
	numOfFingerBonesIncluded = 30;
	if (done1 && done2)
		numOfFingerBonesIncluded = 0;
}

void TSMSoldier::updateMovingSpeed() {
	bool done1 = GetToValue(mMovingSpeed, tMovingSpeed, Game::getIFps());
}


//------------------------------------------
void TSMSoldier::updateAvatarRot() {
	setHeadRotation((Vec3)headRotV);
	setSpineRotation((Vec3)spineRotV);
}

void TSMSoldier::updateAvatarInput() {
	getInputRot();
	if (Input::isKeyPressed(Input::KEY_F10)) {
		shutdown();
	}

}

void TSMSoldier::setAlertSpine(bool on, float speed) {
	if (on && alertOn) {
		sinValueAlertZ += speed * Game::getIFps();
		spineRotV.z = Math::sin(sinValueAlertZ) * 45.0;
		if (spineRotV.z < 0.25 && spineRotV.z > -0.25) {
			alertOn = false;
	    }
	}
	else {
		spineRotV.z = 0.0;
		if (rand() % 500 < 2) {
			alertOn = true;
			sinValueAlertZ = (rand() % 2) ? 0.0 : Consts::PI;
		}
	}
}


void TSMSoldier::setAlertHead(bool on, float speed) {
	if (on && alertOn) {
		sinValueAlertX += speed * Game::getIFps();
		headRotV.y = Math::sin(sinValueAlertX) * 45.0;
		if (headRotV.y < 0.25 && headRotV.y > -0.25) {
			alertOn = false;
		}
	}
	else {
		headRotV.y = 0.0;
		if (rand() % 500 < 2) {
			alertOn = true;
			sinValueAlertX = (rand() % 2) ? 0.0 : Consts::PI;
		}
	}
}

void TSMSoldier::updatePosition() {
	if (fabs(mMovingSpeed) < 0.005) {
		mMovingSpeed = 0.0;
		sinValue = 0.0;
	}
	float v = mMovingSpeed * Game::getIFps() / (1.0 + fabs(rotateRate));
	movingInc = node->getWorldRotation() * Vec3(0.0, v*move_status, 0.0f);
	Vec3 vpos = node->getWorldPosition() + movingInc;
	// rrr
	//mTerrainHeight = IsmatInterface::get()->getWorldHeight(vpos.x, vpos.y, 1000.0);
	mTerrainHeight = getWorldHeight(vpos.x, vpos.y, 1000.0);
	vpos.z = mTerrainHeight;
	node->setWorldPosition(vpos);
	mCurWorldPos = node->getWorldPosition();
	turnXTo_G();
	if (Input::isKeyPressed(Input::KEY_Y)) {
		node->setWorldRotation(quat(0.0, 0.0, 180));
		return;
	}
	if (Input::isKeyPressed(Input::KEY_U)) {
		node->setWorldRotation(quat(0.0, 0.0,  0));
		return;
	}
	if (Input::isKeyPressed(Input::KEY_I)) {
		node->setWorldRotation(quat(0.0, 0.0, 90));
		return;
	}


	mCurWorldRot = node->getWorldRotation();
	mCurLocalPos = node->getPosition();
	mCurLocalRot = node->getRotation();
	//setTargetPos(Game::getPlayer()->getWorldPosition());
}


void TSMSoldier::setSinWaves() {
	switch (mProfile) {
	case PF_TPOSE:
		break;

	case PF_STANDING1:
		generateSinWavesStanding1();
		break;
	case PF_STANDING2:
		generateSinWavesStanding1();
		break;

	case PF_CROUCHING:
		generateSinWavesCrouching();
		break;

	case PF_PRONNING:
		generateSinWavesPronning();
		break;

	case PF_DYING1: case PF_DYING2: case PF_DYING3:
		generateSinWavesDying();
		break;
	}
}


void TSMSoldier::setAlerts() {
	if (rifleAimStatus)
		return;

	switch (mProfile) {
	case PF_TPOSE:
		break;

	case PF_STANDING1:
	case PF_STANDING2:
		setAlertSpine(true, 2.0);
		setAlertHead(false, 2.0);
		break;

	case PF_CROUCHING:
		setAlertSpine(false, 2.0);
		setAlertHead(true, 2.0);
		break;

	case PF_PRONNING:
		setAlertSpine(false, 2.0);
		setAlertHead(true, 2.0);
		break;

	case PF_DYING1: 
	case PF_DYING2: 
	case PF_DYING3:
		break;
	}
}



void TSMSoldier::updateAnimation(int bone_num) {
	switch (mProfile) {
	case PF_TPOSE:
		resetBones();
		break;
	case PF_STANDING1:
		setProfileStanding1(mProfile, bone_num);
		setAimming(mProfile, bone_num);
		setShooting(bone_num);
		break;
	case PF_STANDING2:
		setProfileStanding2(mProfile, bone_num);
		setAimming(mProfile, bone_num);
		setShooting(bone_num);
		break;
	case PF_CROUCHING:
		setProfileCrouching(mProfile, bone_num);
		setAimming(mProfile, bone_num);
		setShooting(bone_num);
		break;
	case PF_PRONNING:
		setProfilePronning(mProfile, bone_num);
		setAimming(mProfile, bone_num);
		setShooting(bone_num);
		break;
	case PF_DYING1:
	case PF_DYING2:
	case PF_DYING3:
		setProfileDying(mProfile, bone_num);
		break;
	}
	setSpineRotation(bone_num);
	setHeadRotation(bone_num);
	setFingers(bone_num);
}


void TSMSoldier::setSpineRotation(int bone_num) {
	if (bone_num != SMS_Spine2)
		return;
	tRot = tRot * spineRot;

}


void TSMSoldier::setHeadRotation(int bone_num) {
	if (bone_num != SMS_Head)
		return;
	tRot = tRot * headRot;
}


void TSMSoldier::setUp(float& x, float target, float rate) {
	if (x > target)
		x -= rate;
	if (x < target)
		x += rate;
}

void TSMSoldier::setRifle(int profile, bool aiming) {
	float ifps = Game::getIFps() * 1.5;
	if (aiming) {
		for (int i = ROT_R_ARM; i < NO_OF_ROTS; i++) {
			setUp(jointRot1[i][profile][AIMMING].x, jointRot2[i][profile][AIMMING].x, fabs(jointRot2[i][profile][AIMMING].x * ifps));
			setUp(jointRot1[i][profile][AIMMING].y, jointRot2[i][profile][AIMMING].y, fabs(jointRot2[i][profile][AIMMING].y * ifps));
			setUp(jointRot1[i][profile][AIMMING].z, jointRot2[i][profile][AIMMING].z, fabs(jointRot2[i][profile][AIMMING].z * ifps));
		}

		inAimingOff = false;
		caOnValue = (jointRot1[ROT_R_ELBOW][profile][AIMMING] - jointRot2[ROT_R_ELBOW][profile][AIMMING]).length();
		inAimingOn = fabs(caOnValue - paOnValue) < 0.001;
		paOnValue = caOnValue;

	}
	else {
		for (int i = ROT_R_ARM; i < NO_OF_ROTS; i++) {
			setUp(jointRot1[i][profile][AIMMING].x, 0.0, fabs(jointRot2[i][profile][AIMMING].x * ifps));
			setUp(jointRot1[i][profile][AIMMING].y, 0.0, fabs(jointRot2[i][profile][AIMMING].y * ifps));
			setUp(jointRot1[i][profile][AIMMING].z, 0.0, fabs(jointRot2[i][profile][AIMMING].z * ifps));
		}
		inAimingOn = false;
		caOffValue = (jointRot1[ROT_R_ELBOW][profile][AIMMING]).length();
		inAimingOff = fabs(caOffValue - paOffValue) < 0.001;
		paOffValue = caOffValue;
	}
	for (int i = ROT_R_ARM; i < NO_OF_ROTS; i++) 
		jointRot[i][profile][AIMMING] = quat(jointRot1[i][profile][AIMMING].x, jointRot1[i][profile][AIMMING].y, jointRot1[i][profile][AIMMING].z);
	AimingInTransition = !inAimingOn && !inAimingOff;
}


void TSMSoldier::setReload(int profile, bool reload) {
	float ifps = Game::getIFps() * 2;
	if (reload) {
		for (int i = ROT_R_ARM; i < NO_OF_ROTS; i++) {
			setUp(jointRot1[i][profile][RELOAD].x, jointRot2[i][profile][RELOAD].x, fabs(jointRot2[i][profile][RELOAD].x * ifps));
			setUp(jointRot1[i][profile][RELOAD].y, jointRot2[i][profile][RELOAD].y, fabs(jointRot2[i][profile][RELOAD].y * ifps));
			setUp(jointRot1[i][profile][RELOAD].z, jointRot2[i][profile][RELOAD].z, fabs(jointRot2[i][profile][RELOAD].z * ifps));
		}
		inReloadOff = false;
		crOnValue = (jointRot1[ROT_R_ELBOW][profile][RELOAD]).length();
		inReloadOn = fabs(crOnValue - prOnValue) < 0.001;
		prOnValue = crOnValue;
	}
	else {
		for (int i = ROT_R_ARM; i < NO_OF_ROTS; i++) {
			setUp(jointRot1[i][profile][RELOAD].x, 0.0, fabs(jointRot2[i][profile][RELOAD].x * ifps));
			setUp(jointRot1[i][profile][RELOAD].y, 0.0, fabs(jointRot2[i][profile][RELOAD].y * ifps));
			setUp(jointRot1[i][profile][RELOAD].z, 0.0, fabs(jointRot2[i][profile][RELOAD].z * ifps));
		}
		inReloadOn = false;
		crOffValue = (jointRot1[ROT_R_ELBOW][profile][RELOAD]).length();
		inReloadOff = fabs(crOffValue - prOffValue) < 0.001;
		prOffValue = crOffValue;
	}

	ReloadInTransition = !inReloadOn && !inReloadOff;
	for (int i = ROT_R_ARM; i < NO_OF_ROTS; i++)
		jointRot[i][profile][RELOAD] = quat(jointRot1[i][profile][RELOAD].x, jointRot1[i][profile][RELOAD].y, jointRot1[i][profile][RELOAD].z);
}


void TSMSoldier::setReload2(int profile, bool reload2) {
	float ifps = Game::getIFps() * 5;
	if (reload2) {
		for (int i = ROT_L_ARM; i <= ROT_L_HAND; i++) {
			setUp(jointRot1[i][profile][RELOAD2].x, jointRot2[i][profile][RELOAD2].x, fabs(jointRot2[i][profile][RELOAD2].x * ifps));
			setUp(jointRot1[i][profile][RELOAD2].y, jointRot2[i][profile][RELOAD2].y, fabs(jointRot2[i][profile][RELOAD2].y * ifps));
			setUp(jointRot1[i][profile][RELOAD2].z, jointRot2[i][profile][RELOAD2].z, fabs(jointRot2[i][profile][RELOAD2].z * ifps));
		}
		inReload2Off = false;
		cr2OnValue = (jointRot1[ROT_R_ELBOW][profile][RELOAD2]).length();
		inReload2On = fabs(cr2OnValue - pr2OnValue) < 0.001;
		pr2OnValue = cr2OnValue;
	}
	else {
		for (int i = ROT_L_ARM; i <= ROT_L_HAND; i++) {
			setUp(jointRot1[i][profile][RELOAD2].x, 0.0, fabs(jointRot2[i][profile][RELOAD2].x * ifps));
			setUp(jointRot1[i][profile][RELOAD2].y, 0.0, fabs(jointRot2[i][profile][RELOAD2].y * ifps));
			setUp(jointRot1[i][profile][RELOAD2].z, 0.0, fabs(jointRot2[i][profile][RELOAD2].z * ifps));
		}
		inReload2On = false;
		cr2OffValue = (jointRot1[ROT_R_ELBOW][profile][RELOAD2]).length();
		inReload2Off = fabs(cr2OffValue - pr2OffValue) < 0.001;
		pr2OffValue = cr2OffValue;
	}

	Reload2InTransition = !inReload2On && !inReload2Off;
	for (int i = ROT_L_ARM; i <= ROT_L_HAND; i++)
		jointRot[i][profile][RELOAD2] = quat(jointRot1[i][profile][RELOAD2].x, jointRot1[i][profile][RELOAD2].y, jointRot1[i][profile][RELOAD2].z);
}


void TSMSoldier::setHeadRotation(Vec3 rot) {
	headRot = quat(rot.x, rot.y, rot.z);
}

void TSMSoldier::setSpineRotation(Vec3 rot) {
	spineRot = quat(-rot.x, -rot.z, rot.y);
}


