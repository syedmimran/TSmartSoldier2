#include "TPlayerManager.h"
#include <UnigineApp.h>
#include "SynckerInterface.h"
#include <plugins/UnigineSyncker.h>
#include "Utils.h"
#include "TSMSoldier.h"


//---------------------------------------------------------------------------
void TPlayerManager::createSoldierPatrol(int human_type, int group_type, int behaviour_type, int smartlevel, Vec3 initPos, NodePtr route, NodePtr platForm) {
	String nodeStr;
	int gender = GN_MALE;
	switch (human_type) {
	case 0: nodeStr = "Humans/Fatimah_Resize_Test_c.node"; //"Humans/Fatimah.node";
		    gender = GN_FEMALE;
		    break;
	case 1: nodeStr = "Humans/Aidil_Resize_Test.node";//"Humans/AidilAddBone.node";
		    break;
	case 2: nodeStr = "Humans/Fatimah.node";
			gender = GN_FEMALE;
			break;
	}

	HumanIndex++;
	ObjectPtr soldier = SynckerInterface::get()->LoadObjectNode(nodeStr);
	PropertyParameterPtr  param = soldier->getProperty()->getParameterPtr("human_type");
	param->setValueInt(human_type);
	param = soldier->getProperty()->getParameterPtr("group_type");
	param->setValueInt(group_type);
	param = soldier->getProperty()->getParameterPtr("behaviour_type");
	param->setValueInt(behaviour_type);
	param = soldier->getProperty()->getParameterPtr("smartLevel");
	param->setValueInt(smartlevel);
	param = soldier->getProperty()->getParameterPtr("id");
	param->setValueInt(HumanIndex);
	param = soldier->getProperty()->getParameterPtr("gender");
	param->setValueInt(gender);
	param = soldier->getProperty()->getParameterPtr("initPos");
	param->setValueDVec3(initPos);
	param = soldier->getProperty()->getParameterPtr("patrolRoute");
	param->setValueNode(route);
	param = soldier->getProperty()->getParameterPtr("platform");
	param->setValueNode(platForm);
	soldiers[HumanIndex] = soldier;
	//if (HumanIndex == 1)
	//	CameraBase->setParent(soldier);
}

void TPlayerManager::createSoldierAttack(int human_type, int group_type, int behaviour_type, int smartlevel, Vec3 initPos, int attack_id, int numofAttatkPerson, int attackSeparation) {
	String nodeStr;
	int gender = GN_MALE;
	switch (human_type) {
	case 0: nodeStr = "Humans/Fatimah_Resize_Test_c.node"; //"Humans/Fatimah.node";
		    gender = GN_FEMALE;
		    break;
	case 1: nodeStr = "Humans/Aidil_Resize_Test.node";//"Humans/AidilAddBone.node";
		    break;
	case 2: nodeStr = "Humans/Fatimah.node";
			gender = GN_FEMALE;
		break;
	}

	HumanIndex++;
	ObjectPtr soldier = SynckerInterface::get()->LoadObjectNode(nodeStr);
	PropertyParameterPtr  param = soldier->getProperty()->getParameterPtr("human_type");
	param->setValueInt(human_type);
	param = soldier->getProperty()->getParameterPtr("group_type");
	param->setValueInt(group_type);
	param = soldier->getProperty()->getParameterPtr("behaviour_type");
	param->setValueInt(behaviour_type);
	param = soldier->getProperty()->getParameterPtr("smartLevel");
	param->setValueInt(smartlevel);
	param = soldier->getProperty()->getParameterPtr("id");
	param->setValueInt(HumanIndex);
	param = soldier->getProperty()->getParameterPtr("gender");
	param->setValueInt(gender);
	param = soldier->getProperty()->getParameterPtr("initPos");
	param->setValueDVec3(initPos);
	param = soldier->getProperty()->getParameterPtr("attack_id");
	param->setValueInt(attack_id);
	param = soldier->getProperty()->getParameterPtr("numOfAttackPersons");
	param->setValueInt(numofAttatkPerson);
	param = soldier->getProperty()->getParameterPtr("attackSeparation");
	param->setValueInt(attackSeparation);
	soldiers[HumanIndex] = soldier;
	addToGroupList(soldier, group_type);
}


void TPlayerManager::deleteSoldier(int idx) {
	if (!soldiers[idx])
		return;
	PropertyParameterPtr param = soldiers[idx]->getProperty()->getParameterPtr("human_type");
	int human_type = param->getValueInt();
	Team_Humans[human_type].removeOne(soldiers[idx]);

	if (soldiers[idx])
		SynckerInterface::get()->deleteRecSyncNode(soldiers[idx]);
	soldiers[idx].clear();
}

void TPlayerManager::deleteAllSoldiers() {
	resetTeamList();
	resetGroupList();
	Map <int, ObjectPtr>::iterator it = soldiers.begin();
	while (it != soldiers.end()) {
		if (it->data)
			SynckerInterface::get()->deleteRecSyncNode(it->data);
		it++;
	}
	soldiers.clear();
	HumanIndex = 0;
}





