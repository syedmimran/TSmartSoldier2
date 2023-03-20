#include "TSMSoldier.h"
#include "Utils.h"


void TSMSoldier::generateSinWavesDying() {
	float ifps = Game::getIFps();
	GetToValue(mLeftFingerAngle, 10, ifps * 10);
	GetToValue(mRightFingerAngle, 10, ifps * 10);
	if (deadTransition) {
		sinValueDead += 2.0 * ifps;
		s1_d = Math::sin(sinValueDead);
	}
	else {
		sinValueDead = 0.0;
		return;
	}
	sinValue += Math::pow(fabs(mMovingSpeed), 0.55) * 5.0 * ifps;
	s_1 = Math::sin(sinValue);
	s_2 = Math::sin(sinValue * 2);
	sp_1 = Math::sin(sinValue + Consts::PI / 2.0);
	float mv = fabs(mMovingSpeed / 500.0);
	x10 = Math::pow(mv, 0.10f);
	x15 = Math::pow(mv, 0.15f);
	x25 = Math::pow(mv, 0.25f);
	x45 = Math::pow(mv, 0.45f);
	x75 = Math::pow(mv, 0.75f);
	x125 = Math::pow(fabs(mMovingSpeed), 1.25);
	stepAngle = s_1 * x25 * 30;
	forwardMove = mMovingSpeed * 2;
	clampValue(forwardMove, -1.0, 1.0);
}

void TSMSoldier::setProfileDying(int profile, int bone_num) {
	quat q = quat_identity;
	vec3 p = vec3_zero;
	float k = 0.0;
	float tMovingSpeed = mMovingSpeed;
	if ((boneRot[SWAT_Hips] - boneRot2[profile][SWAT_Hips]).length() < 1.0)
		deadTransition = false;
	float ifps = Game::getIFps() * 0.25;
	setUp(boneRot[bone_num].x, boneRot2[profile][bone_num].x, ifps * fabs(boneRot2[profile][bone_num].x- boneRot2[mPrevProfile][bone_num].x));
	setUp(boneRot[bone_num].y, boneRot2[profile][bone_num].y, ifps * fabs(boneRot2[profile][bone_num].y- boneRot2[mPrevProfile][bone_num].y));
	setUp(boneRot[bone_num].z, boneRot2[profile][bone_num].z, ifps * fabs(boneRot2[profile][bone_num].z- boneRot2[mPrevProfile][bone_num].z));
	switch (bone_num) {
	case SWAT_Hips:
		p = node->getWorldRotation() * vec3(0.0, s_2 * 2.0 * x75 * 1.0, s_1 * x75 * 1.5 + boneRot[bone_num].z);
		q = quat(-tMovingSpeed * 2.5 + boneRot[bone_num].x, boneRot[bone_num].y, -stepAngle * 1.0);
		break;

	case SWAT_Spine:
		nodAngle = boneRot[bone_num].x + 5 + tMovingSpeed * 10;
		q = quat(nodAngle + stepAngle * 1 + s1_d *30, stepAngle * fabs(tMovingSpeed) * 0.5, stepAngle * 0.75);
		break;

	case SWAT_Spine2:
		nodAngle = boneRot[bone_num].x;
		q = quat(nodAngle + s_2 * x45 * 2, -stepAngle * fabs(tMovingSpeed) + s1_d * 90, stepAngle * fabs(tMovingSpeed) * 0.25 + s1_d * 30);
		break;

	case SWAT_Neck:
		q = quat(-s_2 * x45 * 20 + boneRot[bone_num].x, stepAngle * fabs(tMovingSpeed) * 0.25, 0.0);
		break;

	case SWAT_LeftHip:
		q = quat(stepAngle * x15 * 0.0 - fabs(tMovingSpeed) * 2.0 + boneRot[bone_num].x, 10.0 + boneRot[bone_num].y, stepAngle * 1.0 + boneRot[bone_num].z + stepAngle * x15 * 0.0);
		break;


	case SWAT_LeftKnee:
		k = x125 * (s_1 + 1.0);
		q = quat(k * x10 * 1.0 + boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	case SWAT_RightHip:
		q = quat(-stepAngle * x15 * 0.0 - fabs(tMovingSpeed) * 2.0 + boneRot[bone_num].x, -10.0 + boneRot[bone_num].y, stepAngle * 1.0 + boneRot[bone_num].z - stepAngle * x15 * 5.0);
		break;

	case SWAT_LeftFoot:
		if (sp_1 > 0.0)
			k = sp_1 * x25 * 75.0;
		q = quat(-stepAngle * 1.5 + k, 0.0, -10.0);
		break;


	case SWAT_RightKnee:
		k = x25 * (s_1 - 1.0);
		q = quat(-k * x10 * 30 + boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;


	case SWAT_RightFoot:
		if (sp_1 < 0.0)
			k = sp_1 * x25 * 75.0;
		q = quat(stepAngle * 1.5 - k, 0.0, 10.0);
		break;

	case SWAT_LeftToe:
		q = quat(stepAngle * 0.5 + 10.0, 0.0, 0.0);
		break;

	case SWAT_RightToe:
		q = quat(-stepAngle * 0.5 + 10.0, 0.0, 0.0);
		break;

	case SWAT_LeftArm:
		q = quat(-stepAngle * 2.5 + boneRot[bone_num].x + s1_d * 30, boneRot[bone_num].y, boneRot[bone_num].z + fabs(tMovingSpeed) * 5 + s1_d * 30);
		break;

	case SWAT_LeftElbow:
		q = quat(boneRot[bone_num].x, 3.5 * stepAngle + boneRot[bone_num].y, boneRot[bone_num].z);// *quat(mRot2.x, mRot2.y, mRot2.z);
		break;

	case SWAT_LeftHand:
		q = quat(boneRot[bone_num].x, 3.5 * stepAngle + boneRot[bone_num].y, boneRot[bone_num].z);// *quat(mRot3.x, mRot3.y, mRot3.z);
		break;


	case SWAT_RightArm:
		q = quat(stepAngle * 2.5 + boneRot[bone_num].x, boneRot[bone_num].y,  + boneRot[bone_num].z - fabs(tMovingSpeed) * 5 - s1_d * 30);
		break;

	case SWAT_RightElbow:
		q = quat(boneRot[bone_num].x, 4.5 * stepAngle + boneRot[bone_num].y, boneRot[bone_num].z) ;
		break;

	case SWAT_RightHand:
		q = quat(boneRot[bone_num].x, 3.5 * stepAngle + boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	}
	setFingers(bone_num);
	tPos = p;
	tRot = tRot * q;
}



