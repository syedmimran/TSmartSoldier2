#include "TSMSoldier.h"
#include "Utils.h"


void TSMSoldier::generateSinWavesCrouching() {
	sinValue += Math::pow(fabs(mMovingSpeed), 0.55) * 5.0 * Game::getIFps();
	s_05 = Math::sin(sinValue * 0.5);
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
	mLeftFingerAngle = 50 - fabs(mMovingSpeed) * 15;
	mRightFingerAngle = 60;
}


void TSMSoldier::setProfileCrouching(int profile, int bone_num) {
	quat q = quat_identity;
	vec3 p = vec3_zero;
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
	case SWAT_Hips:
		setUp(boneRot[bone_num].z, boneRot2[profile][bone_num].z, ifps * fabs(boneRot2[profile][bone_num].z - boneRot2[mPrevProfile][bone_num].z));
		k = fabsMovingSpeed * x25;
		if (k > 0.35)
			k = 0.35;
		p = node->getWorldRotation() * vec3(s_2 * 0.025 * x15, s_2 * 0.05 * x15, boneRot[bone_num].z + s_2 * x15 * 0.05 + k);
		q = quat(boneRot[bone_num].x - tMovingSpeed * 1.5, 0.0, -stepAngle * 1.0);
		break;

	case SWAT_Spine:
		nodAngle = 5 + tMovingSpeed * 5;
		q = quat(boneRot[bone_num].x - nodAngle, stepAngle * fabs(tMovingSpeed) * 0.5, stepAngle * 0.50);
		break;

	case SWAT_Spine2:
		q = quat(boneRot[bone_num].x - s_2 * x45 * 2, -stepAngle * fabs(tMovingSpeed) * 0.25, stepAngle * fabs(tMovingSpeed) * 0.25);
		break;

	case SWAT_Neck:
		q = quat(boneRot[bone_num].x - s_2 * x45 * 20, stepAngle * fabs(tMovingSpeed) * 0.25, 0.0);
		break;

	case SWAT_LeftHip:
		if (fabs(tMovingSpeed) > 0.001)
			boneRot[bone_num] = boneRot2[PF_CROUCHING][bone_num];
		q = quat(boneRot[bone_num].x + stepAngle * x45 * 25.0 + 100 * fabsMovingSpeed * x25, boneRot[bone_num].y, boneRot[bone_num].z + stepAngle * 1.0);
		break;

	case SWAT_RightHip:
		if (fabs(tMovingSpeed) > 0.001)
			boneRot[bone_num] = boneRot2[PF_CROUCHING][bone_num];
		q = quat(boneRot[bone_num].x - stepAngle * x45 * 25.0 - 50 * fabsMovingSpeed * x25, boneRot[bone_num].y, boneRot[bone_num].z + stepAngle * 1.0);
		break;


	case SWAT_LeftKnee:
		if (fabs(tMovingSpeed) > 0.001)
			boneRot[bone_num] = boneRot2[PF_CROUCHING][bone_num];
		k = x125 * (s_1 + 1.0);
		zz1 = sp_1 * forwardMove;
		if (zz1 < 0.0)
			k1 = zz1 * 30 * x75;
		q = quat(boneRot[bone_num].x - k1 + k * x10 * 0 - 10 * fabsMovingSpeed * x25, boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	case SWAT_RightKnee:
		if (fabs(tMovingSpeed) > 0.001)
			boneRot[bone_num] = boneRot2[PF_CROUCHING][bone_num];
		k = x125 * (s_1 - 1.0);
		zz1 = sp_1 * forwardMove;
		if (zz1 > 0.0)
			k1 = zz1 * 30 * x75;
		q = quat(boneRot[bone_num].x + k1 - k * x10 * 0 - 100 * fabsMovingSpeed * x25, boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	case SWAT_LeftFoot:
		if (sp_1 > 0.0)
			k = sp_1 * x25 * 30.0;
		q = quat(stepAngle * 0.5 + k, 0.0, 0.0);
		break;

	case SWAT_RightFoot:
		if (sp_1 < 0.0)
			k = sp_1 * x25 * 30.0;
		q = quat(-stepAngle * 0.5 - k, 0.0, 0.0);
		break;

	case SWAT_LeftToe:
		q = quat(stepAngle * 0.5 + 10.0, 0.0, 0.0);
		break;

	case SWAT_RightToe:
		q = quat(-stepAngle * 0.5 + 10.0, 0.0, 0.0);
		break;

	case SWAT_LeftArm:
		q = quat(boneRot[bone_num].x + fabs(tMovingSpeed) * 5 * kx, boneRot[bone_num].y, boneRot[bone_num].z + stepAngle * 2.5 * kx);
		//q = quat(boneRot[bone_num].x + mRot1.x, boneRot[bone_num].y + mRot1.y, boneRot[bone_num].z + mRot1.z);
		break;

	case SWAT_LeftElbow:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z + x25 * 5 * stepAngle * kx + 20 * fabs(tMovingSpeed) * kx);
		//q = quat(boneRot[bone_num].x + mRot2.x, boneRot[bone_num].y + mRot2.y, boneRot[bone_num].z + mRot2.z);
		break;

	case SWAT_LeftHand:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		//q = quat(boneRot[bone_num].x + mRot3.x, boneRot[bone_num].y + mRot3.y, boneRot[bone_num].z + mRot3.z);
		break;

	case SWAT_RightArm:
		q = quat(boneRot[bone_num].x + fabs(tMovingSpeed) * 5 * kx, boneRot[bone_num].y, boneRot[bone_num].z + stepAngle * 1.5 * kx);
		break;

	case SWAT_RightElbow:
		cValue = (boneRot[bone_num] - boneRot2[profile][bone_num]).length();
		inTransition = fabs(cValue - pValue) > 0.00001;
		pValue = cValue;
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z + x25 * 2.5 * stepAngle * kx - 10 * fabs(tMovingSpeed) * kx);
		break;

	case SWAT_RightHand:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	}
	tPos = p;
	tRot = tRot * q;
}

