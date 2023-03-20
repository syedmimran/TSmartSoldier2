#include "TSMSoldier.h"
#include "Utils.h"

void TSMSoldier::generateSinWavesStanding2() {
	sinValue += Math::pow(fabs(mMovingSpeed), 0.55) * 5.0 * Game::getIFps();
	s_05 = Math::sin(sinValue * 0.5);
	s_1 = Math::sin(sinValue);
	s_2 = Math::sin(sinValue * 2);
	sp_1 = Math::sin(sinValue + Consts::PI / 2.0);
	sp_2 = Math::sin(sinValue * 2 + Consts::PI / 4.0);
	float mv = fabs(mMovingSpeed / 500.0);
	x10 = Math::pow(mv, 0.10f);
	x15 = Math::pow(mv, 0.15f);
	x25 = Math::pow(mv, 0.25f);
	x45 = Math::pow(mv, 0.45f);
	x75 = Math::pow(mv, 0.75f);
	x125 = Math::pow(fabs(mMovingSpeed), 1.25);
	stepAngle = s_1 * x25 * 15;
	stepAngle_off = sp_1 * x75 * 1000;
	forwardMove = mMovingSpeed * 2;
	clampValue(forwardMove, -1.0, 1.0);
	mLeftFingerAngle = 50 - fabs(mMovingSpeed) * 15;
	mRightFingerAngle = 60;
}

void TSMSoldier::setProfileStanding2(int profile, int bone_num) {
	quat q = quat_identity;
	vec3 p = vec3_zero;
	float k = 0.0;
	float k1 = 0.0;
	float zz1 = 0.0;
	float f1 = 0.0;

	float aimValue = 1.0;
	float aimValue2 = 0.15;
	if (rifleAimStatus) {
		aimValue = 0.0;
		aimValue2 = 1.0;
	}
	float bum_roll = 1.0;
	if (gender = GN_FEMALE)
		bum_roll = 2.0;
	float ifps = Game::getIFps();
	float tMovingSpeed = mMovingSpeed;
	float fabsMovingSpeed = fabs(mMovingSpeed);

	setUp(boneRot[bone_num].x, boneRot2[profile][bone_num].x, ifps * fabs(boneRot2[profile][bone_num].x - boneRot2[mPrevProfile][bone_num].x));
	setUp(boneRot[bone_num].y, boneRot2[profile][bone_num].y, ifps * fabs(boneRot2[profile][bone_num].y - boneRot2[mPrevProfile][bone_num].y));
	setUp(boneRot[bone_num].z, boneRot2[profile][bone_num].z, ifps * fabs(boneRot2[profile][bone_num].z - boneRot2[mPrevProfile][bone_num].z));
	switch (bone_num) {
	case SWAT_Hips:
		p = node->getWorldRotation() * vec3(s_2 * 0.50 * x15 * 0.1, s_2 * 0.025 * x25, boneRot[bone_num].z + s_2 * x15 * 0.05);
		q = quat(boneRot[bone_num].x - tMovingSpeed * 1.0, -stepAngle * bum_roll * 1.00, -stepAngle * bum_roll * 0.25);
		break;

	case SWAT_Spine:
		nodAngle = tMovingSpeed * 5.0 - 2;
		q = quat(boneRot[bone_num].x - nodAngle, stepAngle * x45 * bum_roll * aimValue * 5.0, stepAngle * bum_roll * 0.15);
		break;

	case SWAT_Spine1:
		q = quat(boneRot[bone_num].x + s_2 * x45 * 2, -stepAngle * x45 * bum_roll * aimValue * 2.0, 0.0);
		break;

	case SWAT_Spine2:
		q = quat(boneRot[bone_num].x - s_2 * x45 * 2, stepAngle * x45 * bum_roll * aimValue * 2.0, 0.0);
		break;

	case SWAT_Neck:
		q = quat(boneRot[bone_num].x - s_2 * x45 * 20, 0.0, 0.0);
		break;

	case SWAT_LeftHip:
		if (fabsMovingSpeed > 0.001)
			boneRot[bone_num] = boneRot2[PF_TPOSE][bone_num];
		f1 = 1.0;
		if (stepAngle < 0.0)
			f1 = 5.0;
		q = quat(boneRot[bone_num].x + stepAngle * x25 * 10.0 * f1 - sp_2 * 10, boneRot[bone_num].y - stepAngle * 1, boneRot[bone_num].z + stepAngle * 0.5);
		break;

	case SWAT_RightHip:
		if (fabsMovingSpeed > 0.001)
			boneRot[bone_num] = boneRot2[PF_TPOSE][bone_num];
		f1 = 1.0;
		if (stepAngle > 0.0)
			f1 = 5.0;
		q = quat(boneRot[bone_num].x - stepAngle * x25 * 10.0 * f1 - sp_2 * 10, boneRot[bone_num].y - stepAngle * 1, boneRot[bone_num].z + stepAngle * 0.5);
		break;

	case SWAT_LeftKnee:
		if (fabsMovingSpeed > 0.001)
			boneRot[bone_num] = boneRot2[PF_TPOSE][bone_num];
		f1 = 0.0;
		if (stepAngle_off < 0.0)
			f1 = 2.5;
		q = quat(boneRot[bone_num].x - stepAngle_off * x10 * 2.5 * f1 + 10 * fabsMovingSpeed, boneRot[bone_num].y, boneRot[bone_num].z - fabsMovingSpeed);
		break;

	case SWAT_RightKnee:
		if (fabsMovingSpeed > 0.001)
			boneRot[bone_num] = boneRot2[PF_TPOSE][bone_num];
		f1 = 0.0;
		if (stepAngle_off > 0.0)
			f1 = 2.5;
		q = quat(boneRot[bone_num].x + stepAngle_off * x10 * 2.5 * f1 + 10 * fabsMovingSpeed, boneRot[bone_num].y, boneRot[bone_num].z + fabsMovingSpeed);
		break;

	case SWAT_LeftFoot:
		k = 0.0;
		if (sp_1 > 0.0)
			k = sp_1 * x25 * 50.0;

		q = quat(-s_1 * 10.0 + k, 0.0, 0.0);
		break;

	case SWAT_RightFoot:
		k = 0.0;
		if (sp_1 < 0.0)
			k = sp_1 * x25 * 50.0;

		q = quat(s_1 * 10.0 - k, 0.0, 0.0);
		break;

	case SWAT_LeftToe:
		k = 0.0;
		if (stepAngle > 0)
			k = 1.0;
		q = quat(-s_1 * x25 * 50.0 * k * fabsMovingSpeed, 0.0, 0.0);
		break;

	case SWAT_RightToe:
		k = 0.0;
		if (stepAngle < 0)
			k = 1.0;
		q = quat(s_1 * x25 * 50.0 * k * fabsMovingSpeed, 0.0, 0.0);
		break;

	case SWAT_LeftArm:
		q = quat(boneRot[bone_num].x + fabsMovingSpeed * 2 * aimValue, boneRot[bone_num].y - 10, boneRot[bone_num].z + stepAngle * 2.0 * aimValue);
		break;

	case SWAT_LeftElbow:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z - x25 * 1.0 * stepAngle_off * aimValue + 10 * fabsMovingSpeed * aimValue);
		break;

	case SWAT_LeftHand:
		q = quat(boneRot[bone_num].x + stepAngle * 2.0, boneRot[bone_num].y + stepAngle * 1.0, boneRot[bone_num].z);
		break;

	case SWAT_RightArm:
		q = quat(boneRot[bone_num].x + fabsMovingSpeed * 5 * aimValue, boneRot[bone_num].y - fabsMovingSpeed * 8, boneRot[bone_num].z + stepAngle * 1.5 * aimValue + fabsMovingSpeed * 5);
		break;

	case SWAT_RightElbow:
		cValue = (boneRot[bone_num] - boneRot2[profile][bone_num]).length();
		inTransition = fabs(cValue - pValue) > 0.00001;
		pValue = cValue;
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z + x25 * 2.5 * stepAngle * aimValue - 10 * fabsMovingSpeed * aimValue);
		break;

	case SWAT_RightHand:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	}
	tPos = p;
	tRot = tRot * q;
}


