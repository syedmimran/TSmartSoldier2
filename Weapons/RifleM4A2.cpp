#include "RifleM4A2.h"
#include "Utils.h"
//#include "FTables.h"
#include <UnigineApp.h>
#include <UnigineGame.h>
#include <plugins/UnigineSyncker.h>
#include "TEffects.h"
#include "TSMSoldier.h"

using namespace Plugins;
using namespace Unigine;
using namespace Math;

REGISTER_COMPONENT(RifleM4A2);

//extern TTable1 TBulletTrajectry;

void RifleM4A2::init(){
	isSyncker = false;
	nozzleNode = NodeDummy::create();
	nozzleNode->setParent(node);
	nozzleNode->setPosition(Vec3(0.0f, 0.80f, 0.065f));
	wIntersection = WorldIntersection::create();
	shotTimer = 0.0;
	triggerIndex = 0;
	hitCounter = 0;
	shot_counter = 0;
	hit_counter = 0;
	timeInterval2 = -0.00001;
	MarkerIndex = aimMarkerIndex = 0;

	for (int i = 0; i < 8; i++) {
		bullet[i].flying = 0;
		bullet[i].triggerTimer = 0.0;
	}
	targetDistance = 0.0;
	shotMarkerIndex = 0;
}


void RifleM4A2::shutdown() {
}
//-----------------------------------------------------------
void RifleM4A2::update() {

	intersectionMask = 0xFFFE;
		
	processtrigger();
	
	if (resetShotCounter) {
		shotCounter = 0;
		shot_counter = 0;
		hitCounter = 0;
		hit_counter = 0;
		resetShotCounter = 0;
	}

	shotMarkerIndex = MarkerIndex - 5;
	if (shotMarkerIndex < 0)
		shotMarkerIndex += CIRCULARBUFFER_SIZE;
	aimMarkerIndex = MarkerIndex - 10;
	if (aimMarkerIndex < 0)
		aimMarkerIndex += CIRCULARBUFFER_SIZE;

	// xxxx
	if (timeInterval < 0.0)
	   MarkerIndex++;
	MarkerIndex %= CIRCULARBUFFER_SIZE;
}

//-------------------------------------------
ObjectPtr RifleM4A2::getTargetAimAndTrigger() {
	nozlePos = nozzleNode->getWorldPosition();
	weapon_trajectory = nozlePos + node->getWorldTransform().getColumn3(1) * 1000;// 1000 meter
	weapon_trajectory = nozlePos + node->getWorldTransform().getColumn3(1) * 1000;// 1000 meter
	//if (attack_mode)
    Visualizer::renderLine3D(nozlePos, weapon_trajectory, vec4_green);
	if (trigger) {
		triggerIndex++;
		triggerIndex %= 8;
		TEffects::get()->setGunSpark(nozlePos);
		bullet[triggerIndex].cameraPos        = nozlePos;
		bullet[triggerIndex].aim_cameraEndPos = weapon_trajectory;
		ObjectPtr p = World::getIntersection(nozlePos, bullet[triggerIndex].aim_cameraEndPos, intersectionMask + 0x10000000, wIntersection);
		if (p) {
			Vec3 hitPoint = wIntersection->getPoint();
			bullet[triggerIndex].hit_targetAimPoint = hitPoint;
			bullet[triggerIndex].targetDistance = (nozlePos-hitPoint).length();
		}
		else {
			bullet[triggerIndex].hit_targetAimPoint = Vec3_zero;
			bullet[triggerIndex].targetDistance = 10000.0;
		}
		bullet[triggerIndex].hit_cameraEndPos = weapon_trajectory;
		//bullet[triggerIndex].triggerTimer = 0.0;
		bullet[triggerIndex].flying = 1;
		shotfired = 1;
		shotCounter = ++shot_counter;

		TEffects::get()->setDistance(0.0);
		TEffects::get()->setRifleOn(nozlePos, true);

		bullet[triggerIndex].hit_PitchCantAngle = -node->getRotation().getAngle(vec3_left);
		bullet[triggerIndex].hit_YawCantAngle = -node->getRotation().getAngle(vec3_up);
		bullet[triggerIndex].hit_RollCantAngle = -node->getRotation().getAngle(vec3_forward);
	}
	//lineDisplay(15 + id, 10, String::format("AimingRadius=%.3f", AimingRadius));
	ObjectPtr p = World::getIntersection(nozlePos, weapon_trajectory, intersectionMask, wIntersection);
	//ObjectPtr p = World::getIntersection(cameraPos, cameraEndPos, 1 << 28, wIntersection);
	if (p) {
		String s = p->getName();
	}
	trigger = 0;
	return p;
}


void RifleM4A2::processBullets() {
	for (int i = 0; i < 8; i++) {
		if (bullet[i].flying) {
			float timeTravel  = bullet[i].targetDistance / 500.0f;
			float timeTravel2 = (bullet[i].targetDistance + 100) / 500.0f;
			bullet[i].triggerTimer += ifps;
			if (bullet[i].triggerTimer > timeTravel2) {
				bullet[i].triggerTimer = 0.0;
				bullet[i].flying = 0;
				bullet[i].hit_target = nullptr;
				bullet[i].terrain = nullptr;
				continue;
			}
			if (bullet[i].triggerTimer > timeTravel) {
				bullet[i].hit_target = World::getIntersection(bullet[i].cameraPos, bullet[i].hit_cameraEndPos, intersectionMask, wIntersection);
				if (bullet[i].hit_target) {
					//lineDisplay(6, 1 + id * 15, bullet[i].target->getName());
					collectProperty(bullet[i]);
					bullet[i].flying = 0;
					bullet[i].triggerTimer = 0.0;
					bullet[i].hit_targetHitPoint = wIntersection->getPoint();
					//bullet[i].hit_targetHitPoint.z += TBulletTrajectry.getValue(bullet[i].targetDistance) / 100.0;
					continue;
				}
				bullet[i].terrain = World::getIntersection(bullet[i].cameraPos, bullet[i].hit_cameraEndPos, 1 << 28, wIntersection);
				if (bullet[i].terrain) {
					collectTerrainProperty(bullet[i]);
					bullet[i].flying = 0;
					bullet[i].triggerTimer = 0.0;
					bullet[i].hit_terrainHitPoint = wIntersection->getPoint();
					//bullet[i].hit_terrainHitPoint.z += TBulletTrajectry.getValue(bullet[i].targetDistance) / 100.0;
				}
			}
		}
	}
}
//-------------------------------------------------------------------------
void  RifleM4A2::processHits() {
	ObjectPtr target;
	String objectName;
	PropertyParameterPtr param;
	//WeaponSensorDataType mWeaponSensorData;

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

			if (bullet[i].hit_targetType == 4) {
				param = bullet[i].hit_prop->getParameterPtr("set_Action");
				param->setValueInt(AC_DEAD);
				param = bullet[i].hit_prop->getParameterPtr("hitDistance");
				param->setValueFloat(bullet[i].targetDistance);
				hitCounter = ++hit_counter;
			}
			bullet[i].hit_target = nullptr;
		}
	}


}
//-------------------------------------------------------------------------
void  RifleM4A2::collectTerrainProperty(BulletProjectileType& mbullet) {
	String objectName = mbullet.terrain->getName();
	if (objectName.contains("terrain")) {
		mbullet.terrain_prop = mbullet.terrain->getProperty(0);
		//mbullet.hit_targetType = 0;
	}
}
//-------------------------------------------------------------------------
void  RifleM4A2::collectProperty(BulletProjectileType & mbullet) {
	String objectName = mbullet.hit_target->getName();
	mbullet.hit_targetType = 0;
		//lineDisplay(8, 1, objectName);
	if (objectName.contains("Human_")) {
		mbullet.hit_prop = mbullet.hit_target->getProperty(0);
		mbullet.hit_targetType = 4;
	}
}
//---------------------------------------------------------------------
void RifleM4A2::processtrigger() {
	ifps = Game::getIFps();
	timeInterval += ifps;
	if (timeInterval > 0.016666)  //60hz
		timeInterval = -0.00001; 
	timeInterval2 += ifps;
	if (timeInterval2 > 0.10)     // 10 hz
		timeInterval2 = -0.00001;

	ObjectPtr object = getTargetAimAndTrigger();

	String objectName;
	PropertyPtr prop;
	if (object) {
		unsigned char targetType = 100;
		//Visualizer::renderLine3D(cameraPos, targetHitPoint, vec4::GREEN);
		objectName = object->getName();
		//lineDisplay(8, 1, objectName);
		if (objectName.contains("Human_")) {
			targetType = 6;
		}
	}
	processBullets();
	processHits();
	//lineDisplay(2, id * 15, String::format("intervalCounter=%d", intervalCounter));
}

