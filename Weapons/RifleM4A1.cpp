#include "RifleM4A1.h"
#include "Utils.h"
#include <UnigineApp.h>
#include <UnigineGame.h>
//#include "IsmatInterface.h"
#include "SynckerInterface.h"
#include "TEffects.h"
#include "TSMSoldier.h"

REGISTER_COMPONENT(RifleM4A1);
using namespace Unigine;
using namespace Math;

char sendbuff[64];

void RifleM4A1::init(){
	String nodeStr = "Nodes/Laser_Dot.node";
	laser_marker = SynckerInterface::get()->LoadNode("Nodes/Laser_Dot.node");

	aim_marker = SynckerInterface::get()->LoadNode("Nodes/Marker_Green.node");
	aim_marker->setEnabled(display_aim_marker);
	for (int i = 0; i < 8; i++) {
		bullet_marker[i] = SynckerInterface::get()->LoadNode("Nodes/BulletMark.node");
		bullet_marker[i]->getChild(1)->setEnabled(false);
		ObjectTextPtr objtext = checked_ptr_cast<ObjectText>(bullet_marker[i]->getChild(2)); 
		objtext->setFontSize(15);
		objtext->setPosition(Vec3(-0.015, 0.075, 0.0));
		objtext->setText(String::format("%d", (int)id));
		bullet_marker[i]->setEnabled(false);
		bullet_marker_timer[i] = 0.0;
	}
	//weaponData.Id = id;
	//weaponData.Attrib = 0;
	//weaponData.Type = 9;
	control_type = USE_MOUSE;
	active = true;

	SectorNo = Config::getInt("SectorNo");
	laneNo = -1;
	
	actualLaneNo = 0;
	laneNo = actualLaneNo % 4;

	initProjector();
	nozzleNode = NodeDummy::create();
	nozzleNode->setParent(node);
	nozzleNode->setPosition(Vec3(0.0f, 0.60f, 0.065f));

	wAimIntersection = WorldIntersection::create();
	wHitIntersection = WorldIntersection::create();

	/*AimData.id = (char)id;
	HitData.id = (char)id;
	ShotData.id = (char)id;
	BarrelData.id = (char)id;*/
	display_laser_marker = 1;
	shotTimer = 0.0;
	triggerIndex = 0;
	hitCounter = 0;
	shot_counter = 0;
	hit_counter = 0;
	targetCentre = Vec3_zero;
	timeInterval2 = -0.00001;
	AimingRadius = 0.0;
	prevActualLaneNo = -1;
	AimingCentre = Vec2_zero;
	for (int i=0; i< CIRCULARBUFFER_SIZE; i++)
	    markerPos[i] = Vec3_zero;
	MarkerIndex = aimMarkerIndex = shotMarkerIndex = laserMarkerIndex = 0;

	for (int i = 0; i < 8; i++) {
		bullet[i].flying = 0;
		bullet[i].triggerTimer = 0.0;
	}

	adjustment[0] = 0.0;// 504;
	adjustment[1] = 0.0;// 207;
	//flashLight = checked_ptr_cast<LightProj>(node->getChild(1));
	//flashLight->setIntensity(10000);
}




void RifleM4A1::initProjector() {
	intersectionMask = 0xFFFE;
	projector = Game::getPlayer();
}
//-------------------------------------------------------------------------
void RifleM4A1::shutdown() {
	if (!active) {
		//Theatre::get()->setShotCounter(laneNo, 0);
		//Theatre::get()->setHitCounter(laneNo, 0);
		aim_marker->setEnabled(false);
		laser_marker->setEnabled(false);
	}
}
//-----------------------------------------------------------
void RifleM4A1::update() {

		//UpdateWeaponLaneAssignment();
		//UpdateLaneMode();
		if (!active) {
			aim_marker->setEnabled(false);
			laser_marker->setEnabled(false);
			return;
		}
		updateCircularBuffer();
		processShooting();
		setDisplayMarker();
		display();
		//flashLight->setEnabled(flashlightOn);
	updateBulletMarker();
	//weaponData.Attrib = 0;
}

void RifleM4A1::updateBulletMarker() {
	for (int i = 0; i < 8; i++) {
		bullet_marker_timer[i] += Game::getIFps();
		if (bullet_marker_timer[i] > 1.0) 
			bullet_marker[i]->setEnabled(false);		
	}
}


void RifleM4A1::updateCircularBuffer() {
	float ifps = Game::getIFps();
	timeInterval += ifps;
	if (timeInterval > 0.016666)  //60hz
		timeInterval = -0.00001;
	timeInterval2 += ifps;
	if (timeInterval2 > 0.10)     // 10 hz
		timeInterval2 = -0.00001;

	if (timeInterval < 0.0) {
		MarkerIndex++;
		MarkerIndex %= CIRCULARBUFFER_SIZE;
		laserMarkerIndex = MarkerIndex;
		aimMarkerIndex = MarkerIndex;
		if (aimMarkerIndex < 0)
			aimMarkerIndex += CIRCULARBUFFER_SIZE;
		shotMarkerIndex = MarkerIndex;
		if (shotMarkerIndex < 0)
			shotMarkerIndex += CIRCULARBUFFER_SIZE;
	}

	if (resetShotCounter) {
		shotCounter = 0;
		shot_counter = 0;
		hitCounter = 0;
		hit_counter = 0;
		resetShotCounter = 0;
		//Theatre::get()->resetShotCounter(laneNo);
		//Theatre::get()->resetHitCounter(laneNo);
	}
}

void RifleM4A1::display() {
	int Id = (int)id;
	lineDisplay(1, Id + 1, String::format("M4A1-%d L%d", Id, laneNo));
}

void RifleM4A1::setDisplayMarker() {
	if (display_aim_marker) {
		aim_marker->setWorldPosition(markerPos[MarkerIndex]);
		aim_marker->setWorldRotation(projector->getWorldRotation());
		aim_marker->setEnabled(true);
	}
	else
		aim_marker->setEnabled(false);

	if (display_laser_marker) {
		laser_marker->setWorldPosition(markerPos[laserMarkerIndex]);
		laser_marker->setWorldRotation(projector->getWorldRotation());
	}
	else
		laser_marker->setEnabled(false);
}


//-------------------------------------------------------------------------
void RifleM4A1::screenAim() {

	//lineDisplay(20 + id, 2, String::format("adjustment[0]=%.3f adjustment[1]=%.3f", adjustment[0], adjustment[1]));
	processAimming();
}


void RifleM4A1::processAimming() { 
	Vec3 ScreenPos;
	cameraPos[MarkerIndex] = projector->getWorldPosition();
	if (control_type == USE_MOUSE) {
		moveMouseAim();
		setMouseTrigger();

		ScreenPos = AimPos + Vec3(vec2(aimingOffset).x, vec2(aimingOffset).y, 0.0);
		ScreenPos.z = -MAIN_CAMERA_DISTANCE;
		//lineDisplay(12, 2, String::format("pos.x=%f pos.y=%f pos.z=%f", ScreenPos.x, ScreenPos.y, ScreenPos.z));
	}
	else {
		Vec3 pos = Vec3_zero;// IsmatInterface::get()->getWeaponMarkerPos(laserId) - TCameraManager::get()->getCentreScreenPos();
		ScreenPos = pos + Vec3(vec2(aimingOffset).x, vec2(aimingOffset).y, 0.0);
		ScreenPos.z = -MAIN_CAMERA_DISTANCE;
	}
	markerPos[MarkerIndex] = projector->getWorldTransform() * ScreenPos;
	//lineDisplay(14, 2, String::format("pos.x=%f pos.y=%f pos.z=%f", markerPos[MarkerIndex].x, markerPos[MarkerIndex].y, markerPos[MarkerIndex].z));


	//Vec3 Pos = markerPos[laserMarkerIndex];
	//lineDisplay(8 + id*2 + 1, 2, String::format("Pos.x=%.3f Pos.y=%.3f Pos.z=%.3f", ScreenPos.x, ScreenPos.y, ScreenPos.z));

}



void RifleM4A1::setMouseTrigger() {
	if (Input::isMouseButtonPressed(Input::MOUSE_BUTTON_RIGHT)) {
		if (!MousePressed)
			trigger = 1;
		MousePressed = true;
	}
	else
		MousePressed = false;
	if (App::getKeyState('f') && (int)id == 1) {
		App::setKeyState('f', 0);
		trigger = 1;
		//IsmatInterface::get()->SetUpMarkerSets();
	}
	if (App::getKeyState('g') && (int)id == 2) {
		App::setKeyState('g', 0);
		trigger = 1;
	}
}

void RifleM4A1::moveMouseAim() {
	if (calibrationMode) {
		AimPos.y = 0.0;
		AimPos.z = 0.0;
		return;
	}

	ivec2 v2 = Input::getMouseCoord();
	//lineDisplay(20, 5 + id * 15, String::format("x= %d y= %d", v2.x, v2.y));
	AimPos.x = (v2.x - 2560.0 / 2) * 11.0 / 2560.0;
	AimPos.y = -(v2.y - 1440 / 2) * 6.0 / 1440.0;

	//AimPos.x =  (v2.x - 960.0) * 5.0 / 1920.0;
	//AimPos.y = -(v2.y - 600.0) * 3.125 / 1200.0;
}

void RifleM4A1::moveKeyboardAim() {
	if (calibrationMode) {
		AimPos.y = 0.0;
		AimPos.z = 0.0;
		return;
	}
	if (App::getKeyState(App::KEY_RIGHT))
		moveSpeedX += 0.0001;
	if (App::getKeyState(App::KEY_LEFT))
		moveSpeedX -= 0.0001;
	if (Input::isKeyUp(Input::KEY_LEFT))
		moveSpeedX = 0.0;
	if (Input::isKeyUp(Input::KEY_RIGHT))
		moveSpeedX = 0.0;

	if (App::getKeyState(App::KEY_UP))
		moveSpeedY += 0.001;
	else
		moveSpeedY = 0.0;
	if (App::getKeyState(App::KEY_DOWN))
		moveSpeedY -= 0.001;
	else
		moveSpeedY = 0.0;
	AimPos.x += moveSpeedX;
	AimPos.y += moveSpeedY;
}

//-------------------------------------------------------------------------



TargetObjectType RifleM4A1::getTargetAimAndTrigger() {
	float dis = 1000.0;
	OpcodeType opcode;
	//char outGoingData[2048];
	int pos = 0;

	Vec3 aim_cameraEndPos = markerPos[aimMarkerIndex] + (markerPos[aimMarkerIndex] - cameraPos[aimMarkerIndex]).normalize() * 500;
	ObjectPtr aim_object = World::getIntersection(cameraPos[aimMarkerIndex], aim_cameraEndPos, intersectionMask + 0x10000000, wAimIntersection);
	targetAimPoint = Vec3_zero;
	if (aim_object) {
		targetAimPoint = wAimIntersection->getPoint();
		dis = (targetAimPoint - cameraPos[aimMarkerIndex]).length();
	}
	//Vec3 hitMarkerPos = markerPos[shotMarkerIndex] + Vec3(0.0, Theatre::get()->getWindSpeedX(laneNo) * 0.001, 0.0);
	Vec3 hitMarkerPos = markerPos[shotMarkerIndex];
	Vec3 hit_cameraEndPos = hitMarkerPos + (hitMarkerPos - cameraPos[shotMarkerIndex]).normalize() * 500;
	ObjectPtr hit_object = World::getIntersection(cameraPos[shotMarkerIndex], hit_cameraEndPos, intersectionMask + 0xFF0000000, wHitIntersection);
	targetHitPoint = Vec3_zero;
	String s = "x";
	if (hit_object) {
		targetHitPoint = wHitIntersection->getPoint();
		dis = (targetHitPoint - cameraPos[shotMarkerIndex]).length();
		//targetHitPoint.z += TBulletTrajectry.getValue(dis) / 100.0;
		s = hit_object->getName();
		if (s.contains("TargetF"))
			lineDisplay(17, 2,s);
	}
	if (trigger) {
		triggerIndex++;
		triggerIndex %= 8;
		bullet[triggerIndex].cameraPos = cameraPos[shotMarkerIndex];
		bullet[triggerIndex].aim_cameraEndPos = aim_cameraEndPos;
		bullet[triggerIndex].hit_cameraEndPos = hit_cameraEndPos;

		bullet[triggerIndex].hit_targetAimPoint = targetAimPoint;
		bullet[triggerIndex].hit_targetHitPoint = targetHitPoint;
		bullet[triggerIndex].targetDistance = dis;

		bullet[triggerIndex].triggerTimer = 0.0;
		bullet[triggerIndex].flying = 1;
		shotfired = 1;
		shotCounter = ++shot_counter;
		shotTimer = 0.0;

		TEffects::get()->setDistance(0.0);
		if (control_type == USE_MOUSE) {
			TEffects::get()->setGunSpark(nozlePos);
			TEffects::get()->setRifleOn(cameraPos[shotMarkerIndex], true);
		}

		//ShotData.shotCounter = shotCounter;
		bullet[triggerIndex].hit_PitchCantAngle = -node->getRotation().getAngle(vec3_left);
		bullet[triggerIndex].hit_YawCantAngle = -node->getRotation().getAngle(vec3_up);
		bullet[triggerIndex].hit_RollCantAngle = -node->getRotation().getAngle(vec3_forward);

		calculateAimingRadius();

		bullet_marker[triggerIndex]->setWorldPosition(markerPos[shotMarkerIndex]);
		bullet_marker[triggerIndex]->setWorldRotation(projector->getWorldRotation());
		//weaponData.Attrib = 1;
		bool hit = false;
		if (hit_object) {
			String s = hit_object->getName();
			hit = s.contains("TargetB") || s.contains("TargetF");
		}
		bullet_marker[triggerIndex]->getChild(0)->setEnabled(hit);
		bullet_marker[triggerIndex]->getChild(1)->setEnabled(!hit);
		bullet_marker_timer[triggerIndex] = 0.0;
		bullet_marker[triggerIndex]->setEnabled(display_bullet_marker);
		//lineDisplay(15 + id, 10, String::format("AimingRadius=%.3f", AimingRadius));
	}
	trigger = 0;
	TargetObjectType to;
	to.aimObject = aim_object;
	to.hitObject = hit_object;
	return to;
}

void RifleM4A1::calculateAimingRadius() {
	float z = 0.0;
	float y = 0.0;
	for (int i = 0; i < CIRCULARBUFFER_SIZE; i++) {
		z += markerPos[i].z;
		y += markerPos[i].y;
	}
	float az = z / CIRCULARBUFFER_SIZE;
	float ay = y / CIRCULARBUFFER_SIZE;
	float d = 0.0;
	for (int i = 0; i < CIRCULARBUFFER_SIZE; i++) {
		z = markerPos[i].z - az;
		y = markerPos[i].y - ay;
		d += sqrt(z * z + y * y);
	}
	AimingRadius = ((d / CIRCULARBUFFER_SIZE)) * 5000.0;
	AimingCentre.x = ay;
	AimingCentre.y = az;
}


void RifleM4A1::calculateAftershotRadius() {
	float z = 0.0;
	float y = 0.0;
	for (int i = 0; i < CIRCULARBUFFER_HALFSIZE; i++) {
		z += markerPos[i].z;
		y += markerPos[i].y;
	}
	float az = z / CIRCULARBUFFER_HALFSIZE;
	float ay = y / CIRCULARBUFFER_HALFSIZE;
	float d = 0.0;
	for (int i = 0; i < CIRCULARBUFFER_HALFSIZE; i++) {
		z = markerPos[i].z - az;
		y = markerPos[i].y - ay;
		d += sqrt(z * z + y * y);
	}
	AftershotRadius = ((d / CIRCULARBUFFER_HALFSIZE)) * 5000.0;
}


void RifleM4A1::processBullets() {
	for (int i = 0; i < 8; i++) {
		if (bullet[i].flying) {
			float timeTravel  = bullet[i].targetDistance / muzzleSpeed;
			bullet[i].triggerTimer += Game::getIFps();
			if (bullet[i].triggerTimer > timeTravel) {
				bullet[i].hit_target = nullptr;
				bullet[i].terrain = nullptr;
				bullet[i].flying = 0;
				bullet[i].triggerTimer = 0.0;
				bullet[i].hit_target = World::getIntersection(bullet[i].cameraPos, bullet[i].hit_cameraEndPos, intersectionMask, wHitIntersection);
				if (bullet[i].hit_target) {
					//lineDisplay(6, 1 + id * 15, bullet[i].target->getName());
					collectProperty(bullet[i]);
					bullet[i].hit_targetHitPoint = wHitIntersection->getPoint();
					//bullet[i].hit_targetHitPoint.z += TBulletTrajectry.getValue(bullet[i].targetDistance) / 100.0;
					continue;
				}
				bullet[i].terrain = World::getIntersection(bullet[i].cameraPos, bullet[i].hit_cameraEndPos, 1 << 28, wHitIntersection);
				if (bullet[i].terrain) {
					collectTerrainProperty(bullet[i]);
					bullet[i].hit_terrainHitPoint = wHitIntersection->getPoint();
					//bullet[i].hit_terrainHitPoint.z += TBulletTrajectry.getValue(bullet[i].targetDistance) / 100.0;
				}
			}
		}
	}
}
//-------------------------------------------------------------------------
void RifleM4A1::processHits() {
	ObjectPtr target;
	String objectName;
	PropertyParameterPtr param;

	for (int i = 0; i < 8; i++) {
		target = bullet[i].terrain;
		if (target && bullet[i].terrain_prop) {
			param = bullet[i].terrain_prop->getParameterPtr("hitPos");
			param->setValueDVec3(bullet[i].hit_terrainHitPoint);
			param = bullet[i].terrain_prop->getParameterPtr("hit");
			param->setValueInt(1);
			param = bullet[i].terrain_prop->getParameterPtr("hitDistance");
			param->setValueFloat(bullet[i].targetDistance);
			bullet[i].terrain = nullptr;
		}

		target = bullet[i].hit_target;
		if (target) {
			//bool display = false;
			objectName = target->getName();
			if (bullet[i].hit_targetType == 2 || bullet[i].hit_targetType == 3) {
				param = bullet[i].hit_prop->getParameterPtr("hit");
				param->setValueInt(1);
				param = bullet[i].hit_prop->getParameterPtr("hitPos");
				param->setValueDVec3(bullet[i].hit_targetHitPoint);
				param = bullet[i].hit_prop->getParameterPtr("distance");
				param->setValueFloat(bullet[i].targetDistance);
				hitCounter = ++hit_counter;
			}
			if (bullet[i].hit_targetType == 4) {
				param = bullet[i].hit_prop->getParameterPtr("set_Action");
				param->setValueInt(AC_DEAD);
				param = bullet[i].hit_prop->getParameterPtr("hitDistance");
				param->setValueFloat(bullet[i].targetDistance);
				param = bullet[i].hit_prop->getParameterPtr("killer_id"); 
				param->setValueInt(id);
				hitCounter = ++hit_counter;
			}
			bullet[i].hit_target=nullptr;
		}
	}


}
//-------------------------------------------------------------------------
void RifleM4A1::collectTerrainProperty(BulletProjectileType& mbullet) {
	String objectName = mbullet.terrain->getName();
	if (objectName.contains("Terrain") || objectName.contains("terrain") || objectName.contains("water")) {
		mbullet.terrain_prop = mbullet.terrain->getProperty(0);
		//mbullet.hit_targetType = 0;
	}
}
//-------------------------------------------------------------------------
void  RifleM4A1::collectProperty(BulletProjectileType & mbullet) {
	String objectName = mbullet.hit_target->getName();
	mbullet.hit_targetType = 0;
		//lineDisplay(8, 1, objectName);
	if (objectName.contains("Human_")) {
		mbullet.hit_prop = mbullet.hit_target->getProperty(0);
		mbullet.hit_targetType = 4;
	}
	else if (objectName.contains("TargetF")) { // falling plate
		mbullet.hit_prop = mbullet.hit_target->getParent()->getProperty(0);
		mbullet.hit_targetType = 2;
	}
	else if (objectName.contains("supply_ship_0")) { // ship
		mbullet.hit_prop = mbullet.hit_target->getProperty(0);
		mbullet.hit_targetType = 3;
	}
}
//---------------------------------------------------------------------
void RifleM4A1::processShooting() {
	screenAim();
	if (shotfired) {
		shotTimer += Game::getIFps();
		if (shotTimer > 1.0 && !calculateAftershotDone) {
			calculateAftershotRadius();
			calculateAftershotDone = true;
		}

		if (shotTimer > 2.0) {
			shotfired = 0;
			shotTimer = 0.0;
			calculateAftershotDone = false;
		}
	}

	TargetObjectType targetobject = getTargetAimAndTrigger();
	processBullets();
	processHits();
	char targetType = identifyObjectType(targetobject.aimObject);
	if (display_laser_marker) {
		if (targetType)
			laser_marker->setEnabled(true);
		else
			laser_marker->setEnabled(false);
	}
	else
		laser_marker->setEnabled(false);


	//lineDisplay(2, id * 15, String::format("intervalCounter=%d", intervalCounter));
}

char RifleM4A1::identifyObjectType(ObjectPtr object) {
	char targetType = 0;
	if (object) {
		//Visualizer::renderLine3D(cameraPos, targetHitPoint, vec4::GREEN);
		String objectName = object->getName();
		//lineDisplay(8, 1, objectName);
		if (objectName.contains("Board_X"))
			targetType = 1;
		else if (objectName.contains("Target"))
			targetType = 2;
		else if (objectName.contains("Board_Y"))
			targetType = 3;
		if (objectName.contains("Human_"))
			targetType = 4;
		if (objectName.contains("ship_"))
			targetType = 5;
	}
	return targetType;
}




