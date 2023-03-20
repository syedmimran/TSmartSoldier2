#include "TPlayerManager.h"
#include "Utils.h"
#include "TSMSoldier.h"
#include <UnigineApp.h>
#include "SynckerInterface.h"
#include "Joystick.h"

bool ReadSmartSoldierRotData();

void TPlayerManager::init() {
	ReadSmartSoldierRotData();
	Team_Humans.resize(2);
	Team_Groups.resize(10);
	resetTeamList();
	resetGroupList();
	CameraBase = NodeDummy::create();
	CameraBase->setPosition(Vec3(0.0, 0.0, 0.0));
	PlayerPtr main_camera = Game::getPlayer();
	main_camera->setDirection(vec3(0.0, 1.0, 0.0), vec3_up);
	main_camera->setPosition(Vec3(0.0, 0.0, 1.0));
	main_camera->setParent(CameraBase);

}

void TPlayerManager::shutdown() {
}

void TPlayerManager::resetTeamList() {
	Team_Humans[SM_BLACK_SOLDIER].clear();
	Team_Humans[SM_GREEN_SOLDIER].clear();
	//Team_Humans[SM_GREEN_SOLDIER].append(checked_ptr_cast<Object>(CameraBase));
}


void TPlayerManager::resetGroupList() {
	for (int i = 0; i < 10; i++)
		Team_Groups[i].clear();
}


void TPlayerManager::addToTeamList(ObjectPtr human, int listNo) {
	if (listNo > 1)
		return;
	Team_Humans[listNo].append(human);
}


void TPlayerManager::addToGroupList(ObjectPtr human, int listNo) {
	if (listNo > 9)
		return;
	Team_Groups[listNo].append(human);
	int size = Team_Groups[listNo].size();
}

void TPlayerManager::setGroupReadyToAttack(int groupNo) {
	PropertyPtr prop;
	PropertyParameterPtr param;
	int size = Team_Groups[groupNo].size();
	for (int i = 0; i < size; i++) {
		prop = Team_Groups[groupNo][i]->getProperty();
		if (prop) {
			param = prop->getParameterPtr("groupInstruction");
			param->setValueInt(1);
		}
	}
}

void TPlayerManager::checkRemovedData() {

}


//-----------------------------------------------------------0
void TPlayerManager::update() {
	//checkRemovedData();
	removeDeadInTeamList();
	removeDeadInGroupList();
	lineDisplay(30, 60, String::format("BLACK: %d ", Team_Humans[SM_BLACK_SOLDIER].size()));
	lineDisplay(36, 60, String::format("GREEN: %d ", Team_Humans[SM_GREEN_SOLDIER].size()));
	getKeyboardInput();
	MoveMainCameraUsingJoystick();
	if (Input::isKeyDown(Input::KEY_R)) {
		ReadSmartSoldierRotData();
	}
}

void TPlayerManager::removeDeadInTeamList() {
	PropertyPtr prop;
	PropertyParameterPtr param;
	for (int i = 0; i < Team_Humans[SM_BLACK_SOLDIER].size(); i++) {
		if (!Team_Humans[SM_BLACK_SOLDIER][i])
			continue;
		prop  = Team_Humans[SM_BLACK_SOLDIER][i]->getProperty();
		if (prop) {
			param = prop->getParameterPtr("isDead");
			if (param->getValueInt() == 1)
				Team_Humans[SM_BLACK_SOLDIER].removeOne(Team_Humans[SM_BLACK_SOLDIER][i]);
		}
	}
	for (int i = 0; i < Team_Humans[SM_GREEN_SOLDIER].size(); i++) {
		if (!Team_Humans[SM_GREEN_SOLDIER][i])
			continue;
		prop = Team_Humans[SM_GREEN_SOLDIER][i]->getProperty();
		if (prop) {
			param = prop->getParameterPtr("isDead");
			if (param->getValueInt() == 1)
				Team_Humans[SM_GREEN_SOLDIER].removeOne(Team_Humans[SM_GREEN_SOLDIER][i]);
		}
	}
}


void TPlayerManager::removeDeadInGroupList() {
	PropertyPtr prop;
	PropertyParameterPtr param;
	for (int x = 0; x < 10; x++) {
		for (int i = 0; i < Team_Groups[x].size(); i++) {
			if (!Team_Groups[x][i])
				continue;
			prop = Team_Groups[x][i]->getProperty();
			if (prop) {
				param = prop->getParameterPtr("isDead");
				if (!param)
					continue;
				if (param->getValueInt() == 1)
					Team_Groups[x].removeOne(Team_Groups[x][i]);
			}
		}
	}
}

void TPlayerManager::clearAll() {
	deleteAllSoldiers();
}




void TPlayerManager::getKeyboardInput() {
	bool change = false;

	//if (App::getKeyState('i')) {
	//	App::setKeyState('i', 0);
	//	change = true;
	//	infoType++;
	//	infoType %= 6;
	//}
	if (change) {
		PropertyPtr prop;
		PropertyParameterPtr param;
		for (int i = 0; i < Team_Humans[SM_BLACK_SOLDIER].size(); i++) {
			prop = Team_Humans[SM_BLACK_SOLDIER][i]->getProperty();
			if (prop) {
				param = prop->getParameterPtr("showText");
				if (param)
					param->setValueInt(infoType);
			}
		}
		for (int i = 0; i < Team_Humans[SM_GREEN_SOLDIER].size(); i++) {
			prop = Team_Humans[SM_GREEN_SOLDIER][i]->getProperty();
			if (prop) {
				param = prop->getParameterPtr("showText");
				if (param)
					param->setValueInt(infoType);
			}
		}
	}
}


void TPlayerManager::MoveMainCameraUsingJoystick() {
	TJoystickControl::get()->Update();
	JoyPosPitch = TJoystickControl::get()->getjValue(0);
	JoyPosRoll = TJoystickControl::get()->getjValue(1);
	JoyPosYaw = TJoystickControl::get()->getjValue(2);
	//lineDisplay(3 , 3, String::format("TJoystickControl p=%.2f, r=%.2f, y=%.2f", JoyPosPitch, JoyPosRoll, JoyPosYaw));

	float cleerence = 0.25;
	float yawvalue = 0.0;
	if (JoyPosYaw > cleerence)
		yawvalue = JoyPosYaw - cleerence;
	else if (JoyPosYaw < -cleerence)
		yawvalue = JoyPosYaw + cleerence;

	if (TJoystickControl::get()->getButton(11))
		camHeight += 0.01;
	if (TJoystickControl::get()->getButton(10))
		camHeight -= 0.01;
	camRot = Vec3_zero;
	//if (TJoystickControl::get()->getPov(0))
	//	camRot.z = 0.1;
	//if (TJoystickControl::get()->getPov(1))
	//	camRot.z = -0.1;
	//if (TJoystickControl::get()->getPov(3))
	//	camRot.x = -0.1;
	//if (TJoystickControl::get()->getPov(2))
	//	camRot.x = 0.1;

	quat camera_Rot = CameraBase->getRotation() * quat(rotateZ(-yawvalue * 2.0) * rotateY(camRot.z) * *rotateX(camRot.x));
	CameraBase->setRotation(camera_Rot);
	Vec3 Pos = CameraBase->getPosition();
	Pos.z = getWorldHeight(Pos.x, Pos.y, 1000.0) + camHeight;
	CameraBase->setPosition(Pos + camera_Rot * Vec3(JoyPosRoll * 0.05, -JoyPosPitch * 0.1, 0.0));
	float fx = 0.0025;
	//main_camera->setDirection(vec3(1.0, rotInc[1], rotInc[0]), vec3_up); // xxx
	//rotInc[0] /= 1.025;
	//rotInc[1] /= 1.025;
	float angle = camera_Rot.getAngle(vec3(0.0, 0.0, 1.0));
	float angle2 = 90.0 - angle;
	if (angle2 < 0)
		angle2 += 360;
}







