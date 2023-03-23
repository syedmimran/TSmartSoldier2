#include "TSMSoldier.h"
#include "Utils.h"

void TSMSoldier::generateSinWavesPronning() {
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


void TSMSoldier::setProfilePronning(int profile, int bone_num) {
	quat q = quat_identity;
	float k = 0.0;
	float k1 = 0.0;
	float zz1 = 0.0;

	float kx = 1.0;
	if (rifleAimStatus)
		kx = 0.0;
	float tMovingSpeed = mMovingSpeed;
	float ifps = Game::getIFps();
	setUp(boneRot[bone_num].x, boneRot2[profile][bone_num].x, ifps * fabs(boneRot2[profile][bone_num].x - boneRot2[mPrevProfile][bone_num].x));
	setUp(boneRot[bone_num].y, boneRot2[profile][bone_num].y, ifps * fabs(boneRot2[profile][bone_num].y - boneRot2[mPrevProfile][bone_num].y));
	setUp(boneRot[bone_num].z, boneRot2[profile][bone_num].z, ifps * fabs(boneRot2[profile][bone_num].z - boneRot2[mPrevProfile][bone_num].z));
	switch (bone_num) {
	case SMS_Hips:
		k = 0.0;
		if (sp_1 > 0.0)
			k = 1.0;
		// left, front, up
		tPos = vec3(s_1 * 0.1, sp_1 *0.15*k, boneRot[bone_num].z + sp_1 * 0.10 * k);
		// pitch, roll, yaw
		q = quat(boneRot[bone_num].x - stepAngle * 0.5, boneRot[bone_num].y - sp_1 * 5.00, -stepAngle * 0.6);
		break;

	case SMS_Spine:
		nodAngle = s_1 * 10;
		q = quat(boneRot[bone_num].x + nodAngle, stepAngle * 0.1, -stepAngle * 1.50);
		break;

	case SMS_Spine2:
		nodAngle = s_1 * 20;
		q = quat(boneRot[bone_num].x - nodAngle, -stepAngle * 0.25, -stepAngle * 2.00);
		break;

	case SMS_Neck:
		q = quat(boneRot[bone_num].x - s_2 * x50 * 20, stepAngle * fabs(tMovingSpeed) * 0.25, 0.0);
		break;

	case SMS_LeftHip:
		k = 0.0;
		if (sp_1 > 0.0)
			k = 1.0;
		q = quat(boneRot[bone_num].x - sp_1 * 20 * k - 5, boneRot[bone_num].y + 50, boneRot[bone_num].z);
		//q = quat(mRot1.x, mRot1.y, mRot1.z);
		break;

	case SMS_LeftKnee:
		k = 0.0;
		if (sp_1 > 0.0)
			k = 1.0;
		q = quat(boneRot[bone_num].x + sp_1 * 40 * k + 10, boneRot[bone_num].y , boneRot[bone_num].z);
		//q = quat(mRot2.x, mRot2.y, mRot2.z);
		break;

	case SMS_LeftFoot:
		if (sp_1 > 0.0)
			k = sp_1 * x30 * 30.0;
		q = quat(stepAngle * 0.5 + k, 0.0, 0.0);
		//q = quat(mRot3.x, mRot3.y, mRot3.z);
		break;

	case SMS_RightHip:
		k = 0;
		if (sp_1 > 0)
			k = sp_1;
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y - stepAngle * 3.0, boneRot[bone_num].z + stepAngle * 3.0);
		//q = quat(mRot1.x, mRot1.y, mRot1.z);
		break;

	case SMS_RightKnee:
		q = quat(boneRot[bone_num].x - stepAngle * 3.0, boneRot[bone_num].y - stepAngle * 6.0, boneRot[bone_num].z - stepAngle * 1.0);
		//q = quat(mRot2.x, mRot2.y, mRot2.z);
		break;


	case SMS_RightFoot:
		k = 0.0;
		if (s_1 < 0.0)
			k = 1.0;
		q = quat(boneRot[bone_num].x -stepAngle * 0.0, boneRot[bone_num].y, boneRot[bone_num].z - stepAngle * 1.0*k);
		//q = quat(mRot3.x, mRot3.y, mRot3.z);
		break;


	case SMS_LeftToe:
		q = quat(stepAngle * 0.5 + 10.0, 0.0, 0.0);
		break;

	case SMS_RightToe:
		q = quat(-stepAngle * 0.5 + 10.0, 0.0, 0.0);
		break;

	case SMS_LeftArm:
		q = quat(boneRot[bone_num].x + fabs(tMovingSpeed) * 5, boneRot[bone_num].y, boneRot[bone_num].z + stepAngle * 2.5);
		//q = quat(boneRot[bone_num].x + mRot1.x, boneRot[bone_num].y + mRot1.y, boneRot[bone_num].z + mRot1.z);
		break;

	case SMS_LeftElbow:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z + x30 * 5 * stepAngle * kx + 20 * fabs(tMovingSpeed) * kx);
		//q = quat(boneRot[bone_num].x + mRot2.x, boneRot[bone_num].y + mRot2.y, boneRot[bone_num].z + mRot2.z);
		break;

	case SMS_LeftHand:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		//q = quat(mRot3.x, mRot3.y, mRot3.z);
		break;

	case SMS_RightArm:
		q = quat(boneRot[bone_num].x + fabs(tMovingSpeed) * 5, boneRot[bone_num].y - stepAngle * 5.0, boneRot[bone_num].z);
		//q = quat(boneRot[bone_num].x + mRot1.x, boneRot[bone_num].y + mRot1.y, boneRot[bone_num].z + mRot1.z);
		break;

	case SMS_RightElbow:
		cValue = (boneRot[bone_num] - boneRot2[profile][bone_num]).length();
		inTransition = fabs(cValue - pValue) > 0.00001;
		pValue = cValue;
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z + x30 * 2.5 * stepAngle * kx - 10 * fabs(tMovingSpeed) * kx);
		//q = quat(boneRot[bone_num].x + mRot2.x, boneRot[bone_num].y + mRot2.y, boneRot[bone_num].z + mRot2.z);
		break;

	case SMS_RightHand:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		//q = quat(boneRot[bone_num].x + mRot3.x, boneRot[bone_num].y + mRot3.y, boneRot[bone_num].z + mRot3.z);
		break;
	}
	//if (gBoneRow[bone_num] < gNumOfActiveBones && gTesting)
	//	lineDisplay(58, gBoneRow[bone_num] + 5, gBoneNames[bone_num] + String::format(" x=%.3f y=%.3f z=%.3f", -q.getAngle(vec3_left), q.getAngle(vec3_up), -q.getAngle(vec3_forward)));
	tRot = tRot * q;
}