#include "TCreateManager.h"
#include "SynckerInterface.h"
#include <UnigineApp.h>
#include "TPlayerManager.h"
#include <plugins/UnigineSyncker.h>
#include "Utils.h"
#include "TSMSoldier.h"

using namespace Plugins;

void TCreateManager::init() {
	isSyncker = SynckerInterface::get()->isSyncker();
	wIntersection = WorldIntersection::create();
	projector = Game::getPlayer();


	String nodeStr = "Nodes/Marker_Yellow.node";
	cross_marker = SynckerInterface::get()->LoadNode(nodeStr);
	cross_marker->setEnabled(true);
	objtext = checked_ptr_cast<ObjectText>(cross_marker->getChild(1));
	objtext->setFontSize(20);
	objtext->setFontResolution(300);
	objtext->setPosition(Vec3(0.05, 0.05, 0.0));
	objtext->setTextColor(vec4_black);
	intersectionMask = (1 << 28) +(1 << 1) + (1 << 0);
	AimPos.x = 2.5;
	DropPos = Vec3(Vec3_zero);
	//TPlayerManager::get()->createHandalan2(0, BEEN_DRIVEN, 0, DropPos);
	//TPlayerManager::get()->createA_109(0, 1, DropPos);
}

void TCreateManager::shutdown() {
}

void TCreateManager::update() {
	moveMarker();
	if (getIntersection())
	    placeObject();
	OffloadPassenger();
	if (!dropDone) {
		TPlayerManager::get()->createSoldierPatrol(1, SM_BLACK_SOLDIER, 0, 0, Vec3(-1.0, 3.0, 0.0), nullptr, nullptr);
		TPlayerManager::get()->createSoldierPatrol(1, SM_BLACK_SOLDIER, 8, 0, Vec3(1.0, 3.0, 0.0), nullptr, nullptr);
		dropDone = true;
	}
	getKeyDown();
}

void TCreateManager::OffloadPassenger() {
	int humanType = 1;
	if (offloadPassenger) {
		offload_timer += Game::getIFps();
		if (offload_timer > 1.0) {
			Vec3 pos = Vec3(0.0, -110.0, 0.0);
			setOffLoadPassengers(humanType, SM_BLACK_SOLDIER, pos, passangerCount);
			offload_timer -= 1.0 + (rand() % 10) * 0.25;
			passangerCount++;
			if (passangerCount >= num_of_Attack_persons)
				offloadPassenger = false;
		}
	}
	else {
		offload_timer = 0.0;
	}
}



void TCreateManager::setOffLoadPassengers(int humantype, int group_type, Vec3 attackPos, int attack_id) {
	Vec3 pos0 = attackPos;
	Vec3 pos1 = Vec3(0.0, 50.0, 0.0);
	Vec3 p = pos1 - pos0;
	float len = p.length();
	Vec3 pos2 = p * (len - 6.0) / len + pos0;
	Vec3 DropPos = pos2 + Vec3((rand() % 10) * 0.25, (rand() % 10) * 0.25, 0.0);
	TPlayerManager::get()->createSoldierAttack(humantype, group_type, 6, 0, pos1, attack_id, num_of_Attack_persons, attackSeparation);
}



void TCreateManager::moveMarker() {
	moveMouseAim();
	triggerMouse();
	//PlayerPtr p = Game::getPlayer();
	//String s = p->getName();
	Vec3 ScreenPos = AimPos;
	ScreenPos.z = -MAIN_CAMERA_DISTANCE;
	//lineDisplay(13, 2, String::format("pos.x=%f pos.y=%f pos.z=%f", ScreenPos.x, ScreenPos.y, ScreenPos.z));

	cameraPos = projector->getWorldPosition();
	markerPos = projector->getWorldTransform() * ScreenPos;
	cam_trajectory = markerPos + (markerPos - cameraPos).normalize() * 2500;
	//lineDisplay(15, 2, String::format("pos.x=%f pos.y=%f pos.z=%f", markerPos.x, markerPos.y, markerPos.z));
	cross_marker->setWorldPosition(markerPos);
	cross_marker->setWorldRotation(projector->getWorldRotation());
	//main_projector = Theatre::get()->getMainProjector();
}

void TCreateManager::triggerMouse() {
	if (Input::isMouseButtonPressed(Input::MOUSE_BUTTON_LEFT)) {
		if (!MousePressed)
			pageNo = (pageNo == 0) ? 1 : 0;
		MousePressed = true;
	}
	else
		MousePressed = false;

}


void  TCreateManager::moveMouseAim() {
	ivec2 v2 = Input::getMouseCoord();
	AimPos.x = (v2.x - 2560.0/2) * 11.0 / 2560.0;
	AimPos.y = -(v2.y - 1440/2) * 6.0 / 1440.0;

	//lineDisplay(20, 5, String::format("x= %d y= %d", v2.x, v2.y));
	//lineDisplay(21, 5, String::format("camPos x= %.3f y= %.3f z= %.3f", cameraPos.x, cameraPos.y, cameraPos.z));
	//lineDisplay(22, 5, String::format("AimPos x= %.3f y= %.3f z= %.3f", AimPos.x, AimPos.y, AimPos.z));
	//lineDisplay(23, 5, String::format("ScreenPos x= %.3f y= %.3f z= %.3f", ScreenPos.x, ScreenPos.y, ScreenPos.z));
	//lineDisplay(24, 5, String::format("markerPos x= %.3f y= %.3f z= %.3f", markerPos.x, markerPos.y, markerPos.z));
}


bool TCreateManager::getIntersection() {
	ObjectPtr object = World::getIntersection(cameraPos, cam_trajectory, intersectionMask, wIntersection);
	DropPos = Vec3_zero;
	if (object) {
		DropPos = wIntersection->getPoint();
		float distance = (float)(cameraPos - cam_trajectory).length();
		String s = String::format("d=%d x=%3.3f y=%3.3f z=%3.3f ", (int)distance, DropPos.x, DropPos.y, DropPos.z) + object->getName();
		objtext->setText(s);
		SynckerInterface::get()->SendTestMessage(objtext, String(s));
		objtext->setEnabled(true);
		return true;
	}
	objtext->setEnabled(false);
	return false;
}

void TCreateManager::placeObject() {
	if (DropPos == Vec3_zero)
		return;
	String nodeStr;
	NodePtr routex;
	if (Input::isKeyDown(Input::KEY_F1)) {
		TPlayerManager::get()->createSoldierPatrol(0, SM_BLACK_SOLDIER, 0, 0, DropPos, nullptr, nullptr);
	}
	if (Input::isKeyDown(Input::KEY_F2)) {
		/*routex = checked_ptr_cast<NodeDummy>(World::getNodeByName("AiPathRoutes"));
		String s = routex->getName();
		routex = routex->getChild(1);
		s = routex->getName();
		TPlayerManager::get()->createSoldierPatrol(1, SM_BLACK_SOLDIER, 1, 0, DropPos, routex, nullptr);*/
		TPlayerManager::get()->createSoldierPatrol(1, SM_BLACK_SOLDIER, 0, 0, DropPos, nullptr, nullptr);
	}
	if (Input::isKeyDown(Input::KEY_F3)) {
		/*routex = checked_ptr_cast<NodeDummy>(World::getNodeByName("AiPathRoutes"));
		String s = routex->getName();
		routex = routex->getChild(0);
		s = routex->getName();
		TPlayerManager::get()->createSoldierPatrol(0, SM_BLACK_SOLDIER, 1, 0, DropPos, routex, nullptr);
		TPlayerManager::get()->createSoldierPatrol(0, SM_BLACK_SOLDIER, 9, 0, DropPos, nullptr, nullptr);*/
		TPlayerManager::get()->createSoldierPatrol(2, SM_BLACK_SOLDIER, 0, 0, DropPos, nullptr, nullptr);
	}
}

void TCreateManager::getKeyDown() {

	if (Input::isKeyDown(Input::KEY_F4)) {
		num_of_Attack_persons = 7;
		attackSeparation = 3;
		offloadPassenger = true;
	}

	if (Input::isKeyDown(Input::KEY_F10)) {
		TPlayerManager::get()->deleteAllSoldiers();
	}
	if (Input::isKeyDown(Input::KEY_F11)) {
		dropDone = false;
	}

}


