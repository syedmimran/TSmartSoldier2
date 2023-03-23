#include "TSMSoldier.h"


void TSMSoldier::setFingers(int bone_num) {
	quat q = quat_identity;
	switch (bone_num) {
	case SMS_LeftIndex1:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_LeftMiddle1:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_LeftPinky1:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_LeftRing1:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_LeftIndex2:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_LeftMiddle2:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_LeftPinky2:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_LeftRing2:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	case SMS_LeftIndex3:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_LeftMiddle3:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_LeftPinky3:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_LeftRing3:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;


	case SMS_RightIndex1:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_RightMiddle1:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_RightPinky1:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_RightRing1:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	case SMS_RightIndex2:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_RightMiddle2:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_RightPinky2:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_RightRing2:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_RightIndex3:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_RightMiddle3:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_RightPinky3:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_RightRing3:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
		/*
	case SMS_LeftThumb1:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		//q = quat(mRot1.x, mRot1.y, mRot1.z);
		break;
		
	case SMS_LeftThumb2:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		//q = quat(mRot2.x, mRot2.y, mRot2.z);
		break;
	case SMS_LeftThumb3:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		//q = quat(mRot3.x, mRot3.y, mRot3.z);
		break;
	case SMS_RightThumb1:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_RightThumb2:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SMS_RightThumb3:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
		*/
	}
	tRot = tRot * q;

}


