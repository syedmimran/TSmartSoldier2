#include "TSMSoldier.h"


void TSMSoldier::updateWeaponOnTarget() {
	if (!weaponOnTarget || !rifleAimStatus || reloadSeq != 0) {
		mRifle->setRotation(rifleRot);
		return;
	}

	Vec3 pos2 = mTargetPos;
	Vec3 pos1 = mRifle->getWorldPosition();
	float x = pos1.x - pos2.x;
	float y = pos1.y - pos2.y;
	float z = pos1.z - pos2.z;
	float angle = Math::atan2(pos1.z - pos2.z, sqrt(x * x + y * y)) * Consts::RAD2DEG;
	spineRotV.x = angle;
	spineRotV.z = -3.40;
	aimingDirection = vec3(pos2 - pos1);
	aimingDirection.normalize();
	mRifle->setWorldDirection(aimingDirection, vec3_up, AXIS_Y);
}



void TSMSoldier::updateShooting() {
	if (isDead || inTransition)
		return;
	setRifle(mProfile, rifleAimStatus);
	if (rifleAimStatus || !inReloadOff || !inReload2Off) {
		setReload(mProfile, reloadStatus);
		setReload2(mProfile, reload2Status);
	}
	if (triggerPull && rifleAimStatus && !AimingInTransition && !ReloadInTransition && !Reload2InTransition) {
		triggerCycle = Math::sin(sinValueTrigger);
		sinValueTrigger += 50.0 * Game::getIFps();
		if (triggerCycle > 0.99 && mBulletCount >= 0) {
			mBulletCount--;
			if (mBulletCount > 0) 
				isShooting = true;
			if (mBulletCount < 0) {
				reloadTimer = 0.0;
				reloadWait = true;
			}
		}
		//setTargetPos(Game::getPlayer()->getWorldPosition());
	}
	else {
		sinValueTrigger = 0.0;
	}

	if (isShooting) {
		PropertyPtr	prop = mRifle->getProperty(0);
		PropertyParameterPtr param = prop->getParameterPtr("trigger");
		param->setValueInt(1);
	}

	reloadTimer += Game::getIFps();
	if (reloadTimer > reloadWaitTime) {
		reloadWait = false;
		reloadTimer = 0.0;
	}

	if (mBulletCount < 0 && !reloadWait) {
		reloadWait = true;
		//if (reloadSeq != 2) // test reload
		   reloadSeq++;
		switch (reloadSeq) {
		case 0:
			reloadStatus = false;
			reload2Status = false;
			reloadWaitTime = 0.5;
			break;

		case 1:
			reloadStatus = true;
			reload2Status = false;
			reloadWaitTime = 1.0;
			break;

		case 2:
			reloadStatus = true;
			reload2Status = true;
			reloadWaitTime = 0.5;
			break;
		case 3:
			reloadStatus = true;
			reload2Status = false;
			reloadWaitTime = 0.5;
			break;
		case 4:
			reloadStatus = false;
			reload2Status = false;
			mMagazineCount--;
			if (mMagazineCount > 0)
			   mBulletCount = magazineCapacity;
			else {
				changeAction(AC_RETREAT);
				reloadWaitTime = 5.0;
			}
			reloadSeq = 0;
			break;
		}

	}
	shootingOk = rifleAimStatus && reloadSeq == 0 && mBulletCount > 0;
}



void TSMSoldier::setAimming(int profile, int bone_num) {
	quat q = quat_identity;
	
	switch (bone_num) {

	case SMS_RightArm:
		q = jointRot[ROT_R_ARM][profile][AIMMING];
		//q = quat(mRot1.x, mRot1.y, mRot1.z);

		break;
	case SMS_RightElbow:
		q = jointRot[ROT_R_ELBOW][profile][AIMMING];
		//q = quat(mRot2.x, mRot2.y, mRot2.z);
		break;
	case SMS_RightHand:
		q = jointRot[ROT_R_HAND][profile][AIMMING];
		//q = quat(mRot3.x, mRot3.y, mRot3.z);
		break;
	case SMS_LeftArm:
		q = jointRot[ROT_L_ARM][profile][AIMMING];
		//q = quat(mRot1.x, mRot1.y, mRot1.z);
		break;
	case SMS_LeftElbow:
		q = jointRot[ROT_L_ELBOW][profile][AIMMING];
		//q = quat(mRot2.x, mRot2.y, mRot2.z);
		break;
	case SMS_LeftHand:
		q = jointRot[ROT_L_HAND][profile][AIMMING];
		//q = quat(mRot3.x, mRot3.y, mRot3.z);
		break;
	}

	tRot = tRot * q;
	setReloading(profile,bone_num);
}


void TSMSoldier::setReloading(int profile, int bone_num) {
	quat q = quat_identity;
	switch (bone_num) {

	case SMS_RightArm:
		q = jointRot[ROT_R_ARM][profile][RELOAD];
		//q = quat(mRot1.x, mRot1.y, mRot1.z);
		break;

	case SMS_RightElbow:
		q = jointRot[ROT_R_ELBOW][profile][RELOAD];
		//q = quat(mRot2.x, mRot2.y, mRot2.z);
		break;

	case SMS_RightHand:
		q = jointRot[ROT_R_HAND][profile][RELOAD];
		//q = quat(mRot3.x, mRot3.y, mRot3.z);
		break;

	case SMS_LeftArm:
		q = jointRot[ROT_L_ARM][profile][RELOAD];
		//q = quat(mRot1.x, mRot1.y, mRot1.z);
		break;

	case SMS_LeftElbow:
		q = jointRot[ROT_L_ELBOW][profile][RELOAD];
		//q = quat(mRot2.x, mRot2.y, mRot2.z);
		break;

	case SMS_LeftHand:
		q = jointRot[ROT_L_HAND][profile][RELOAD];
		//q = quat(mRot3.x, mRot3.y, mRot3.z);
		break;
	}
	
	tRot = tRot * q;
	setReloading2(profile, bone_num);

}


void TSMSoldier::setReloading2(int profile, int bone_num) {
	quat q = quat_identity;
	switch (bone_num) {

	case SMS_LeftArm:
		q = jointRot[ROT_L_ARM][profile][RELOAD2];
		//q = quat(mRot1.x, mRot1.y, mRot1.z);
		break;

	case SMS_LeftElbow:
		q = jointRot[ROT_L_ELBOW][profile][RELOAD2];
		//q = quat(mRot2.x, mRot2.y, mRot2.z);
		break;

	case SMS_LeftHand:
		q = jointRot[ROT_L_HAND][profile][RELOAD2];
		//q = quat(mRot3.x, mRot3.y, mRot3.z);
		break;
	}
	tRot = tRot * q;
}

void TSMSoldier::setShooting(int bone_num) {
	if (!shootingOk || !triggerPull)
		return;
	quat q = quat_identity;
	switch (bone_num) {
	case SMS_Spine:
		q = quat(-triggerCycle * 1.0, 0.0, 0.0);
		break;
	case SMS_LeftElbow:
		q = quat(0.0, -triggerCycle * 1.0, 0.0);
		break;

	case SMS_RightElbow:
		q = quat(0.0, triggerCycle * 1.0, 0.0);
		break;
	}
	tRot = tRot * q;
}


