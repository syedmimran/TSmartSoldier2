#include "TSMSoldier.h"
#include "IK_Sqlite.h"

// using namespace Ikramatic;
// rrr
vec3 jointRot2[NO_OF_ROTS][PF_NUM_OF_PROFILES][4];
vec3 boneRot2[PF_NUM_OF_PROFILES][NUM_OF_BONES];
int gBoneId[NUM_OF_BONES];
int gBoneRow[NUM_OF_BONES];
int gNumOfActiveBones = 22;
int gNumOfFingerBones = 30;


char * TAvatarDatabase3("../data/BonesData/TSmartSoldier2.db");
String TableNameProfile[16] = { 
	"BR_TPose",
	"BR_Standing1",
	"BR_Standing2",
	"BR_Crouching",
	"BR_Pronning",
	"BR_Dying1",
	"BR_Dying2",
	"BR_Dying3",
	"BR_Reserve1"
};

String TableNameAimming[3] = {
	"BR_Aimming_Standing",
	"BR_Aimming_Crouching",
	"BR_Aimming_Pronning"
};


String TableNameReload[3] = {
	"BR_Reload_Standing",
	"BR_Reload_Crouching",
	"BR_Reload_Pronning"
};

String TableNameReload2[3] = {
	"BR_Reload2_Standing",
	"BR_Reload2_Crouching",
	"BR_Reload2_Pronning"
};

String gBoneNames[56] = {
"Hips",
"Spine",
"Spine1",
"Spine2",
"Neck",
"Neck1",
"Head",
"REye",
"LEye",
"Jaw",
"LeftShoulder",
"LeftArm",
"LeftElbow",
"LeftHand",

"LeftThumb1",
"LeftThumb",
"LeftThumb3",
"LeftIndex1",
"LeftIndex2",
"LeftIndex3",
"LeftMiddle1",
"LeftMiddle2",
"LeftMiddle3",
"LeftRing1",
"LeftRing2",
"LeftRing3",
"LeftPinky1",
"LeftPinky2",
"LeftPinky3",

"RightShoulder",
"RightArm",
"RightElbow",
"RightHand",

"RightThumb1",
"RightThumb2",
"RightThumb3",
"RightIndex1",
"RightIndex2",
"RightIndex3",
"RightMiddle1",
"RightMiddle2",
"RightMiddle3",
"RightRing1",
"RightRing2",
"RightRing3",
"RightPinky1",
"RightPinky2",
"RightPinky3",

"LeftHip",
"LeftKnee",
"LeftFoot",
"LeftToe",

"RightHip",
"RightKnee",
"RightFoot",
"RightToe"
};



bool ReadSmartSoldierRotData() {
	gBoneId[0] = SMS_Hips;
	gBoneId[1] = SMS_Spine;
	gBoneId[2] = SMS_Spine1;
	gBoneId[3] = SMS_Spine2;
	gBoneId[4] = SMS_Neck;
	gBoneId[5] = SMS_Head;

	gBoneId[6] = SMS_LeftHip;
	gBoneId[7] = SMS_LeftKnee;
	gBoneId[8] = SMS_LeftFoot;
	gBoneId[9] = SMS_LeftToe;

	gBoneId[10] = SMS_RightHip;
	gBoneId[11] = SMS_RightKnee;
	gBoneId[12] = SMS_RightFoot;
	gBoneId[13] = SMS_RightToe;


	gBoneId[14] = SMS_LeftShoulder;
	gBoneId[15] = SMS_LeftArm;
	gBoneId[16] = SMS_LeftElbow;
	gBoneId[17] = SMS_LeftHand;

	gBoneId[18] = SMS_RightShoulder;
	gBoneId[19] = SMS_RightArm;
	gBoneId[20] = SMS_RightElbow;
	gBoneId[21] = SMS_RightHand;

	gBoneId[22] = SMS_LeftThumb1;
	gBoneId[23] = SMS_LeftThumb2;
	gBoneId[24] = SMS_LeftThumb3;
	gBoneId[25] = SMS_LeftIndex1;
	gBoneId[26] = SMS_LeftIndex2;
	gBoneId[27] = SMS_LeftIndex3;
	gBoneId[28] = SMS_LeftMiddle1;
	gBoneId[29] = SMS_LeftMiddle2;
	gBoneId[30] = SMS_LeftMiddle3;
	gBoneId[31] = SMS_LeftRing1;
	gBoneId[32] = SMS_LeftRing2;
	gBoneId[33] = SMS_LeftRing3;
	gBoneId[34] = SMS_LeftPinky1;
	gBoneId[35] = SMS_LeftPinky2;
	gBoneId[36] = SMS_LeftPinky3;

	gBoneId[37] = SMS_RightThumb1;
	gBoneId[38] = SMS_RightThumb2;
	gBoneId[39] = SMS_RightThumb3;
	gBoneId[40] = SMS_RightIndex1;
	gBoneId[41] = SMS_RightIndex2;
	gBoneId[42] = SMS_RightIndex3;
	gBoneId[43] = SMS_RightMiddle1;
	gBoneId[44] = SMS_RightMiddle2;
	gBoneId[45] = SMS_RightMiddle3;
	gBoneId[46] = SMS_RightRing1;
	gBoneId[47] = SMS_RightRing2;
	gBoneId[48] = SMS_RightRing3;
	gBoneId[49] = SMS_RightPinky1;
	gBoneId[50] = SMS_RightPinky2;
	gBoneId[51] = SMS_RightPinky3;
	for (int i = 0; i < 52; i++) {
		gBoneRow[gBoneId[i]] = i;
	}

	
	IK_Sqlite * avatarDb = new IK_Sqlite(TAvatarDatabase3);
	int NumOfRows;
	int Index;
	for (int dbNo = PF_TPOSE; dbNo <= PF_PRONNING; dbNo++) {

		NumOfRows = avatarDb->readTableDb(TableNameProfile[dbNo]);
		if (NumOfRows == 0 || NumOfRows != 37) {
			delete avatarDb;
			return false;
		}
	
		for (int i = 0; i < NumOfRows; i++) {
			Index = avatarDb->getInt(i, 0);
			if (NumOfRows > 100)
				continue;
			boneRot2[dbNo][Index].x = avatarDb->getFloat(i, 2);
			boneRot2[dbNo][Index].y = avatarDb->getFloat(i, 3);
			boneRot2[dbNo][Index].z = avatarDb->getFloat(i, 4);
		}
		for (int i = 20; i < NumOfRows; i++) {
			Index = avatarDb->getInt(i, 0);
			switch (Index) {
			case 101:
				jointRot2[ROT_R_ARM][dbNo][AIMMING].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_R_ARM][dbNo][AIMMING].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_R_ARM][dbNo][AIMMING].z = avatarDb->getFloat(i, 4);
				break;
			case 102:
				jointRot2[ROT_R_ELBOW][dbNo][AIMMING].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_R_ELBOW][dbNo][AIMMING].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_R_ELBOW][dbNo][AIMMING].z = avatarDb->getFloat(i, 4);
				break;

			case 103:
				jointRot2[ROT_R_HAND][dbNo][AIMMING].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_R_HAND][dbNo][AIMMING].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_R_HAND][dbNo][AIMMING].z = avatarDb->getFloat(i, 4);
				break;

			case 104:
				jointRot2[ROT_L_ARM][dbNo][AIMMING].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_L_ARM][dbNo][AIMMING].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_L_ARM][dbNo][AIMMING].z = avatarDb->getFloat(i, 4);
				break;

			case 105:
				jointRot2[ROT_L_ELBOW][dbNo][AIMMING].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_L_ELBOW][dbNo][AIMMING].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_L_ELBOW][dbNo][AIMMING].z = avatarDb->getFloat(i, 4);
				break;
			case 106:
				jointRot2[ROT_L_HAND][dbNo][AIMMING].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_L_HAND][dbNo][AIMMING].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_L_HAND][dbNo][AIMMING].z = avatarDb->getFloat(i, 4);
				break;

			case 201:
				jointRot2[ROT_R_ARM][dbNo][RELOAD].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_R_ARM][dbNo][RELOAD].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_R_ARM][dbNo][RELOAD].z = avatarDb->getFloat(i, 4);
				break;
			case 202:
				jointRot2[ROT_R_ELBOW][dbNo][RELOAD].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_R_ELBOW][dbNo][RELOAD].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_R_ELBOW][dbNo][RELOAD].z = avatarDb->getFloat(i, 4);
				break;

			case 203:
				jointRot2[ROT_R_HAND][dbNo][RELOAD].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_R_HAND][dbNo][RELOAD].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_R_HAND][dbNo][RELOAD].z = avatarDb->getFloat(i, 4);
				break;

			case 204:
				jointRot2[ROT_L_ARM][dbNo][RELOAD].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_L_ARM][dbNo][RELOAD].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_L_ARM][dbNo][RELOAD].z = avatarDb->getFloat(i, 4);
	
			case 205:
				jointRot2[ROT_L_ELBOW][dbNo][RELOAD].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_L_ELBOW][dbNo][RELOAD].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_L_ELBOW][dbNo][RELOAD].z = avatarDb->getFloat(i, 4);
				break;

			case 206:
				jointRot2[ROT_L_HAND][dbNo][RELOAD].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_L_HAND][dbNo][RELOAD].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_L_HAND][dbNo][RELOAD].z = avatarDb->getFloat(i, 4);
				break;

			case 301:
				jointRot2[ROT_L_ARM][dbNo][RELOAD2].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_L_ARM][dbNo][RELOAD2].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_L_ARM][dbNo][RELOAD2].z = avatarDb->getFloat(i, 4);
				break;

			case 302:
				jointRot2[ROT_L_ELBOW][dbNo][RELOAD2].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_L_ELBOW][dbNo][RELOAD2].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_L_ELBOW][dbNo][RELOAD2].z = avatarDb->getFloat(i, 4);
				break;
			case 303:
				jointRot2[ROT_L_HAND][dbNo][RELOAD2].x = avatarDb->getFloat(i, 2);
				jointRot2[ROT_L_HAND][dbNo][RELOAD2].y = avatarDb->getFloat(i, 3);
				jointRot2[ROT_L_HAND][dbNo][RELOAD2].z = avatarDb->getFloat(i, 4);
				break;
			}
		}

	}
	for (int dbNo = PF_DYING1; dbNo <= PF_DYING3; dbNo++) { // dying

		NumOfRows = avatarDb->readTableDb(TableNameProfile[dbNo]);
		if (NumOfRows == 0 || NumOfRows != 22) {
			delete avatarDb;
			return false;
		}

		for (int i = 0; i < NumOfRows; i++) {
			Index = avatarDb->getInt(i, 0);
			if (NumOfRows > 100)
				continue;
			boneRot2[dbNo][Index].x = avatarDb->getFloat(i, 2);
			boneRot2[dbNo][Index].y = avatarDb->getFloat(i, 3);
			boneRot2[dbNo][Index].z = avatarDb->getFloat(i, 4);
		}
	}

	delete avatarDb;
	return true;
}


