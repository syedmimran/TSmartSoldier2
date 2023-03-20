#include "TSMSoldier.h"


void TSMSoldier::setFingers(int bone_num) {
	quat q = quat_identity;
	switch (bone_num) {
	case SWAT_LeftIndex1:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_LeftMiddle1:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_LeftPinky1:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_LeftRing1:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_LeftIndex2:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_LeftMiddle2:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_LeftPinky2:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_LeftRing2:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	case SWAT_LeftIndex3:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_LeftMiddle3:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_LeftPinky3:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_LeftRing3:
		q = quat(boneRot[bone_num].x - cLeftFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;


	case SWAT_RightIndex1:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_RightMiddle1:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_RightPinky1:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_RightRing1:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;

	case SWAT_RightIndex2:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_RightMiddle2:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_RightPinky2:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_RightRing2:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_RightIndex3:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_RightMiddle3:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_RightPinky3:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_RightRing3:
		q = quat(boneRot[bone_num].x - cRightFingerAngle, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
		/*
	case SWAT_LeftThumb1:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		//q = quat(mRot1.x, mRot1.y, mRot1.z);
		break;
		
	case SWAT_LeftThumb2:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		//q = quat(mRot2.x, mRot2.y, mRot2.z);
		break;
	case SWAT_LeftThumb3:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		//q = quat(mRot3.x, mRot3.y, mRot3.z);
		break;
	case SWAT_RightThumb1:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_RightThumb2:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
	case SWAT_RightThumb3:
		q = quat(boneRot[bone_num].x, boneRot[bone_num].y, boneRot[bone_num].z);
		break;
		*/
	}
	tRot = tRot * q;

}


