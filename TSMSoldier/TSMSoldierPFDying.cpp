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
	if (!mFreeze)
		sinValue += Math::pow(fabs(mMovingSpeed), 0.55) * speedVariation * ifps;
	s_1 = Math::sin(sinValue);
	s_2 = Math::sin(sinValue * 2);
	sp_1 = Math::sin(sinValue + Consts::PI / 2.0);
	float mv = fabs(mMovingSpeed / 500.0);
	x10 = Math::pow(mv, 0.10f);
	x20 = Math::pow(mv, 0.20f);
	x30 = Math::pow(mv, 0.30f);
	x40 = Math::pow(mv, 0.40f);
	x50 = Math::pow(mv, 0.50f);
	x75 = Math::pow(mv, 0.75f);
	x125 = Math::pow(mv, 1.25f);
	stepAngle = s_1 * x20 * 30;
	forwardMove = mMovingSpeed * 2;
	clampValue(forwardMove, -1.0, 1.0);
}

void TSMSoldier::setProfileDying(int profile, int bone_num) {
	quat q = quat_identity;
	float k = 0.0;
	float tMovingSpeed = mMovingSpeed;
	float ifps = Game::getIFps() * 1.0;
	setUp(boneRot[bone_num].x, boneRot2[profile][bone_num].x, ifps * fabs(boneRot2[profile][bone_num].x- boneRot2[mPrevProfile][bone_num].x));
	setUp(boneRot[bone_num].y, boneRot2[profile][bone_num].y, ifps * fabs(boneRot2[profile][bone_num].y- boneRot2[mPrevProfile][bone_num].y));
	setUp(boneRot[bone_num].z, boneRot2[profile][bone_num].z, ifps * fabs(boneRot2[profile][bone_num].z- boneRot2[mPrevProfile][bone_num].z));
	switch (bone_num) {
	case SMS_Hips:
		tPos = vec3(0.0, s_2 * 2.0 * x75 * 1.0, s_1 * x75 * 1.5 + boneRot[bone_num].z);
		q = quat(-tMovingSpeed * 2.5 + boneRot[bone_num].x, boneRot[bone_num].y, -stepAngle * 1.0);
		break;

	case SMS_Spine:
		nodAngle = boneRot[bone_num].x + 5 + tMovingSpeed * 10;
		q = quat(nodAngle + stepAngle * 1 + s1_d *30, stepAngle * fabs(tMovingSpeed) * 0.5, stepAngle * 0.75);
		break;

	case SMS_Spine2:
		nodAngle = boneRot[bone_num].x;
		q = quat(nodAngle + s_2 * x50 * 2, -stepAngle * fabs(tMovingSpeed) + s1_d * 90, stepAngle * fabs(tMovingSpeed) * 0.25 + s1_d * 30);
		break;

	case SMS_Neck:
		q = quat(-s_2 * x50 * 20 + boneRot[bone_num].x, stepAngle * fabs(tMovingSpeed) * 0.25, 0.0);
		break;

	case SMS_LeftHip:
		q = quat(stepAngle * x20 * 0.0 - fabs(tMovingSpeed) * 2.0 + boneRot[bone_num].x, 10.0 + boneRot[bone_num].y, stepAngle * 1.0 + boneRot[bone_num].z + stepAngle * x20 * 0.0);
		break;


	case SMS_LeftKnee:
		if ((boneRot[bone_num] - boneRot2[profile][bone_num]).length() < 0.5)
			deadTransition = false;
		k = x125 * (s_1 + 1.0);
		q = quat(k * x10 * 1.0 + boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	case SMS_RightHip:
		q = quat(-stepAngle * x20 * 0.0 - fabs(tMovingSpeed) * 2.0 + boneRot[bone_num].x, -10.0 + boneRot[bone_num].y, stepAngle * 1.0 + boneRot[bone_num].z - stepAngle * x20 * 5.0);
		break;

	case SMS_LeftFoot:
		if (sp_1 > 0.0)
			k = sp_1 * x30 * 75.0;
		q = quat(-stepAngle * 1.5 + k, 0.0, -10.0);
		break;


	case SMS_RightKnee:
		k = x30 * (s_1 - 1.0);
		q = quat(-k * x10 * 30 + boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;


	case SMS_RightFoot:
		if (sp_1 < 0.0)
			k = sp_1 * x30 * 75.0;
		q = quat(stepAngle * 1.5 - k, 0.0, 10.0);
		break;

	case SMS_LeftToe:
		q = quat(stepAngle * 0.5 + 10.0, 0.0, 0.0);
		break;

	case SMS_RightToe:
		q = quat(-stepAngle * 0.5 + 10.0, 0.0, 0.0);
		break;

	case SMS_LeftArm:
		q = quat(-stepAngle * 2.5 + boneRot[bone_num].x + s1_d * 30, boneRot[bone_num].y, boneRot[bone_num].z + fabs(tMovingSpeed) * 5 + s1_d * 30);
		break;

	case SMS_LeftElbow:
		q = quat(boneRot[bone_num].x, 3.5 * stepAngle + boneRot[bone_num].y, boneRot[bone_num].z);// *quat(mRot2.x, mRot2.y, mRot2.z);
		break;

	case SMS_LeftHand:
		q = quat(boneRot[bone_num].x, 3.5 * stepAngle + boneRot[bone_num].y, boneRot[bone_num].z);// *quat(mRot3.x, mRot3.y, mRot3.z);
		break;


	case SMS_RightArm:
		q = quat(stepAngle * 2.5 + boneRot[bone_num].x, boneRot[bone_num].y,  + boneRot[bone_num].z - fabs(tMovingSpeed) * 5 - s1_d * 30);
		break;

	case SMS_RightElbow:
		q = quat(boneRot[bone_num].x, 4.5 * stepAngle + boneRot[bone_num].y, boneRot[bone_num].z) ;
		break;

	case SMS_RightHand:
		q = quat(boneRot[bone_num].x, 3.5 * stepAngle + boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	}
	setFingers(bone_num);
	tRot = tRot * q;
}



