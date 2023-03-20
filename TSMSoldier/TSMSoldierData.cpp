#include "TSMSoldier.h"
#include "IK_Sqlite.h"

// using namespace Ikramatic;
// rrr
vec3 jointRot2[NO_OF_ROTS][PF_NUM_OF_PROFILES][4];
vec3 boneRot2[PF_NUM_OF_PROFILES][NUM_OF_BONES];

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


bool ReadSmartSoldierRotData() {
	
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


