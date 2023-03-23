#pragma once

#include <UnigineVector.h>
#include <UnigineApp.h>
#include <UnigineGame.h>


using namespace Unigine;
using namespace Math;
class Theatre;

class TPlayerManager {
public:
	 TPlayerManager() {}
	 ~TPlayerManager() {}
	 static TPlayerManager * get() {
		 static TPlayerManager instance;
		 return &instance;
	 }

	void createSoldierPatrol(int human_type, int group_type, int behaviour_type, int smartlevel, Vec3 initPos, NodePtr route, NodePtr platForm);
	void createSoldierAttack(int human_type, int group_type, int behaviour_type, int smartlevel, Vec3 initPos, int attack_id,int numofAttatkPerson, int attackSeparation);

	void deleteSoldier(int idx);
	void deleteAllSoldiers();
	void clearAll();
	void resetTeamList();
	void resetGroupList();
	void removeDeadInTeamList();
	void removeDeadInGroupList();
	void getKeyboardInput();
	void addToTeamList(ObjectPtr human, int listNo);
	void addToGroupList(ObjectPtr human, int listNo);
	void setGroupReadyToAttack(int groupNo);
	void resetNumOfSoldierReady() {
		NumOfSoldierReady = 0;
	}
	void oneSoldierIsReady() {
		NumOfSoldierReady++;
	}
	int getNumOfSoldierReady() {
		return NumOfSoldierReady;
	}
	bool checkHumanAlreadyExist(unsigned char type, int id) {
		if (soldiers[id])
			return true;
		return false;
	}

	NodePtr getHuman(int id) {
		return soldiers[id];
	}

	void MoveMainCameraUsingJoystick();

	

	Vector <ObjectPtr> * getHumanTeam(int humantype) {
		return &Team_Humans[humantype];
	}

	

	void init();
	void update();
	void shutdown();

private:

	int HumanIndex = 0;
	int vehicleId = 0;
	int SoldierDied = 0;
	int NumOfSoldierReady = 0;
	Map <int, ObjectPtr> soldiers;
	Vector <Vector <ObjectPtr>> Team_Humans;
	Vector <Vector <ObjectPtr>> Team_Groups;
	int infoType = 0;
	void checkRemovedData();
	float JoyPosPitch, JoyPosRoll, JoyPosYaw;
	NodePtr CameraBase;
	float camHeight = 0.0;
	Vec3 camRot	= Vec3_zero;


};


