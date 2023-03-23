#include "TSMSoldier.h"
#include "Utils.h"


void TSMSoldier::generateSinWavesCrouching() {
	if (!mFreeze)
		sinValue += Math::pow(fabs(mMovingSpeed), 0.55) * speedVariation * 1.5 * Game::getIFps();
	s_05 = Math::sin(sinValue * 0.5);
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
	mLeftFingerAngle = 50 - fabs(mMovingSpeed) * 15;
	mRightFingerAngle = 60;
}


void TSMSoldier::setProfileCrouching(int profile, int bone_num) {
	quat q = quat_identity;
	float k = 0.0;
	float k1 = 0.0;
	float zz1 = 0.0;

	float kx = 1.0;
	if (rifleAimStatus)
		kx = 0.0;
	float tMovingSpeed = mMovingSpeed;
	float fabsMovingSpeed = fabs(mMovingSpeed);
	float ifps = Game::getIFps();
	setUp(boneRot[bone_num].x, boneRot2[profile][bone_num].x, ifps * fabs(boneRot2[profile][bone_num].x - boneRot2[mPrevProfile][bone_num].x));
	setUp(boneRot[bone_num].y, boneRot2[profile][bone_num].y, ifps * fabs(boneRot2[profile][bone_num].y - boneRot2[mPrevProfile][bone_num].y));
	setUp(boneRot[bone_num].z, boneRot2[profile][bone_num].z, ifps * fabs(boneRot2[profile][bone_num].z - boneRot2[mPrevProfile][bone_num].z));
	switch (bone_num) {
	case SMS_Hips:
		setUp(boneRot[bone_num].z, boneRot2[profile][bone_num].z, ifps * fabs(boneRot2[profile][bone_num].z - boneRot2[mPrevProfile][bone_num].z));
		k = fabsMovingSpeed * x30;
		if (k > 0.35)
			k = 0.35;
		tPos = vec3(s_2 * 0.025 * x20, s_2 * 0.05 * x20, boneRot[bone_num].z + s_2 * x20 * 0.05 + k);
		q = quat(boneRot[bone_num].x - tMovingSpeed * 1.5, 0.0, -stepAngle * 1.0);
		break;

	case SMS_Spine:
		nodAngle = 5 + tMovingSpeed * 5;
		q = quat(boneRot[bone_num].x - nodAngle, stepAngle * fabs(tMovingSpeed) * 0.5, stepAngle * 0.50);
		break;

	case SMS_Spine2:
		q = quat(boneRot[bone_num].x - s_2 * x50 * 2, -stepAngle * fabs(tMovingSpeed) * 0.25, stepAngle * fabs(tMovingSpeed) * 0.25);
		break;

	case SMS_Neck:
		q = quat(boneRot[bone_num].x - s_2 * x50 * 20, stepAngle * fabs(tMovingSpeed) * 0.25, 0.0);
		break;

	case SMS_LeftHip:
		q = quat(boneRot[bone_num].x + stepAngle * x50 * 25.0 + 100 * fabsMovingSpeed * x30, boneRot[bone_num].y, boneRot[bone_num].z + stepAngle * 1.0);
		break;

	case SMS_RightHip:
		q = quat(boneRot[bone_num].x - stepAngle * x50 * 25.0 - 100 * fabsMovingSpeed * x30, boneRot[bone_num].y, boneRot[bone_num].z + stepAngle * 1.0);
		break;


	case SMS_LeftKnee:
		zz1 = sp_1 * forwardMove;
		if (zz1 < 0.0)
			k1 = zz1 * 30 * x75;
		q = quat(boneRot[bone_num].x - k1 - 100 * fabsMovingSpeed * x30, boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	case SMS_RightKnee:
		zz1 = sp_1 * forwardMove;
		if (zz1 > 0.0)
			k1 = zz1 * 30 * x75;
		q = quat(boneRot[bone_num].x + k1  - 100 * fabsMovingSpeed * x30, boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	case SMS_LeftFoot:
		if (sp_1 > 0.0)
			k = sp_1 * x30 * 30.0;
		q = quat(stepAngle * 0.5 + k, 0.0, 0.0);
		break;

	case SMS_RightFoot:
		if (sp_1 < 0.0)
			k = sp_1 * x30 * 30.0;
		q = quat(-stepAngle * 0.5 - k, 0.0, 0.0);
		break;

	case SMS_LeftToe:
		q = quat(stepAngle * 0.5 + 10.0, 0.0, 0.0);
		break;

	case SMS_RightToe:
		q = quat(-stepAngle * 0.5 + 10.0, 0.0, 0.0);
		break;

	case SMS_LeftArm:
		q = quat(boneRot[bone_num].x + fabs(tMovingSpeed) * 5 * kx, boneRot[bone_num].y, boneRot[bone_num].z + stepAngle * 2.5 * kx);
		//q = quat(boneRot[bone_num].x + mRot1.x, boneRot[bone_num].y + mRot1.y, boneRot[bone_num].z + mRot1.z);
		break;

	case SMS_LeftElbow:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z + x30 * 5 * stepAngle * kx + 20 * fabs(tMovingSpeed) * kx);
		//q = quat(boneRot[bone_num].x + mRot2.x, boneRot[bone_num].y + mRot2.y, boneRot[bone_num].z + mRot2.z);
		break;

	case SMS_LeftHand:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		//q = quat(boneRot[bone_num].x + mRot3.x, boneRot[bone_num].y + mRot3.y, boneRot[bone_num].z + mRot3.z);
		break;

	case SMS_RightArm:
		q = quat(boneRot[bone_num].x + fabs(tMovingSpeed) * 5 * kx, boneRot[bone_num].y, boneRot[bone_num].z + stepAngle * 1.5 * kx);
		break;

	case SMS_RightElbow:
		cValue = (boneRot[bone_num] - boneRot2[profile][bone_num]).length();
		inTransition = fabs(cValue - pValue) > 0.00001;
		pValue = cValue;
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z + x30 * 2.5 * stepAngle * kx - 10 * fabs(tMovingSpeed) * kx);
		break;

	case SMS_RightHand:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	}
	//if (gBoneRow[bone_num] < gNumOfActiveBones && gTesting)
	//	lineDisplay(58, gBoneRow[bone_num] + 5, gBoneNames[bone_num] + String::format(" x=%.3f y=%.3f z=%.3f", -q.getAngle(vec3_left), q.getAngle(vec3_up), -q.getAngle(vec3_forward)));
	tRot = tRot * q;
}

