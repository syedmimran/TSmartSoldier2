#include "TSMSoldier.h"
#include "Utils.h"

extern String BoneNames[];

void TSMSoldier::generateSinWavesStanding1() {
	if (!mFreeze) 
		sinValue += Math::pow(fabs(mMovingSpeed), 0.55) * speedVariation * Game::getIFps();
	s_05 = Math::sin(sinValue*0.5);
	s_1 = Math::sin(sinValue);
	s_2 = Math::sin(sinValue * 2);
	sp_1 = Math::sin(sinValue + Consts::PI / 2.0);
	sp_2 = Math::sin(sinValue * 2 + Consts::PI / 4.0);
	float mv = fabs(mMovingSpeed / 500.0);
	x10 = Math::pow(mv, 0.10f);
	x20 = Math::pow(mv, 0.20f);
	x30 = Math::pow(mv, 0.30f);
	x40 = Math::pow(mv, 0.40f);
	x50 = Math::pow(mv, 0.50f);
	x75 = Math::pow(mv, 0.75f);
	x125 = Math::pow(mv, 1.25f);
	stepAngle = s_1 * x30 * 30;
	stepAngle_off = sp_1 * x30 * 30;
	forwardMove = mMovingSpeed * 2;
	clampValue(forwardMove, -1.0, 1.0);
	mLeftFingerAngle = 50 - fabs(mMovingSpeed) * 15;
	mRightFingerAngle = 60;
}

void TSMSoldier::setProfileStanding1(int profile, int bone_num) {
	quat q = quat_identity;
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
	//float bum_roll = 1.0;
	//if (gender = GN_FEMALE)
	//	bum_roll = 2.0;
	float ifps = Game::getIFps();
	float tMovingSpeed = mMovingSpeed;
	float fabsMovingSpeed = fabs(mMovingSpeed);

	setUp(boneRot[bone_num].x, boneRot2[profile][bone_num].x, ifps * fabs(boneRot2[profile][bone_num].x - boneRot2[mPrevProfile][bone_num].x));
	setUp(boneRot[bone_num].y, boneRot2[profile][bone_num].y, ifps * fabs(boneRot2[profile][bone_num].y - boneRot2[mPrevProfile][bone_num].y));
	setUp(boneRot[bone_num].z, boneRot2[profile][bone_num].z, ifps * fabs(boneRot2[profile][bone_num].z - boneRot2[mPrevProfile][bone_num].z));
	switch (bone_num) {
		case SMS_Hips:
			tPos = vec3(s_2 * 0.50 * x20 * 0.1, s_2 * 0.025 * x30, boneRot[bone_num].z + s_2 * x20 * 0.10);
			q = quat(boneRot[bone_num].x - tMovingSpeed * 1.0 - 4, -stepAngle * 10.00 * x50, -stepAngle * 0.10);
			break;

		case SMS_Spine:
			nodAngle = tMovingSpeed * 5.0;
			q = quat(boneRot[bone_num].x - nodAngle, stepAngle * x40 * aimValue * 2.0, stepAngle * 0.15);
			break;

		case SMS_Spine1:
			q = quat(boneRot[bone_num].x + s_2 * x50 * 2, stepAngle * x40 * aimValue * 10.0, 0.0);
			break;

		case SMS_Spine2:
			q = quat(boneRot[bone_num].x - s_2 * x50 * 2, stepAngle * x40 * aimValue * 20.0, 0.0);
			break;

		case SMS_Neck:
			q = quat(boneRot[bone_num].x - s_2 * x50 * 20, 0.0, 0.0);
			break;

		case SMS_LeftHip:
			q = quat(boneRot[bone_num].x + stepAngle * 5.0 - 250 * x40, boneRot[bone_num].y - stepAngle *10.0 * x50, boneRot[bone_num].z);
			break;

		case SMS_RightHip:
			q = quat(boneRot[bone_num].x - stepAngle * 5.0 - 250 * x40, boneRot[bone_num].y -stepAngle*10.0*x50, boneRot[bone_num].z);
			break;

		case SMS_LeftKnee:
			f1  = 1.0;
			if (stepAngle_off < 0.0)
				f1 = 2.5;
			q = quat(boneRot[bone_num].x - stepAngle_off * 3.0 * f1 + 1750 * x75 + 200 * x50, boneRot[bone_num].y, boneRot[bone_num].z - fabsMovingSpeed);
			break;

		case SMS_RightKnee:
			f1 = 1.0;
			if (stepAngle_off > 0.0)
				f1 = 2.5;
			q = quat(boneRot[bone_num].x + stepAngle_off * 3.0 * f1 + 1750 * x75 + 200 * x50, boneRot[bone_num].y, boneRot[bone_num].z + fabsMovingSpeed);
			break;

		case SMS_LeftFoot:
			q = quat(-s_1 * 15.0 + stepAngle, 0.0, 0.0);
			break;
	
		case SMS_RightFoot:
			q = quat(s_1 * 15.0 - stepAngle, 0.0, 0.0);
			break;

		case SMS_LeftToe:
			k = 0.0;
			if (stepAngle > 0)
				k = 1.0;
			q = quat(-s_1 * x30 * 50.0 * k * fabsMovingSpeed, 0.0, 0.0);
			break;

		case SMS_RightToe:
			k = 0.0;
			if (stepAngle < 0)
				k = 1.0;
			q = quat(s_1 * x30 * 50.0 * k * fabsMovingSpeed, 0.0, 0.0);
			break;

		case SMS_LeftArm:
			q = quat(boneRot[bone_num].x + x75 * 500 * aimValue, boneRot[bone_num].y, boneRot[bone_num].z + stepAngle * 5.0 * aimValue);
			break;

		case SMS_LeftElbow:
			f1 = 0.0;
			if (stepAngle > 0.0)
				f1 = 5.0;
			q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z + stepAngle * 1.0 * f1 * aimValue + 10 * fabsMovingSpeed * aimValue);
			break;

		case SMS_LeftHand:
			q = quat(boneRot[bone_num].x + stepAngle * 2.0, boneRot[bone_num].y + stepAngle * 5.0, boneRot[bone_num].z);
			break;

		case SMS_RightArm:
			f1 = 0.0;
			if (stepAngle > 0.0)
				f1 = 2.0;
			q = quat(boneRot[bone_num].x + x75 * 500 * aimValue, boneRot[bone_num].y, boneRot[bone_num].z + stepAngle * 0.5 * f1 * aimValue + 10 * fabsMovingSpeed * aimValue);
			break;

		case SMS_RightElbow:
			cValue = (boneRot[bone_num] - boneRot2[profile][bone_num]).length();
			inTransition = fabs(cValue - pValue) > 0.00001;
			pValue = cValue;
			q = quat(boneRot[bone_num].x,boneRot[bone_num].y, boneRot[bone_num].z + x30 * 2.5 * stepAngle * aimValue - 10 * fabsMovingSpeed * aimValue);
			break;

		case SMS_RightHand:
			q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
			break;

	}
	//if (gBoneRow[bone_num] < gNumOfActiveBones && gTesting)
	//	lineDisplay(58, gBoneRow[bone_num] + 5, gBoneNames[bone_num] + String::format(" x=%.3f y=%.3f z=%.3f", -q.getAngle(vec3_left), q.getAngle(vec3_up), -q.getAngle(vec3_forward)));
	tRot = tRot * q;
}


