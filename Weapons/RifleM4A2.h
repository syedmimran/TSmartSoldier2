#pragma once

#include <UnigineComponentSystem.h>
#include <UniginePlayers.h>
#include <UnigineWorld.h>
#include <UnigineLights.h>
using namespace Unigine;
using namespace Math;

#define CIRCULARBUFFER_SIZE 60
typedef struct {
	Vec3 cameraPos;
	Vec3 hit_cameraEndPos;
	Vec3 aim_cameraEndPos;
	float triggerTimer;
	int hit_targetType;
	char flying;
	PropertyPtr hit_prop;
	PropertyPtr terrain_prop;
	ObjectPtr hit_target;
	ObjectPtr terrain;
	float hit_PitchCantAngle;
	float hit_RollCantAngle;
	float hit_YawCantAngle;
	Vec3 hit_targetAimPoint;
	Vec3 hit_targetHitPoint;
	Vec3 hit_terrainHitPoint;
	float targetDistance;
} BulletProjectileType;

class RifleM4A2 : public ComponentBase {
public:
	COMPONENT(RifleM4A2, ComponentBase);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);
	COMPONENT_SHUTDOWN(shutdown);
	PROP_NAME("RifleM4A2");

	PROP_PARAM(Int, id, 0);
	PROP_PARAM(Int, control_type, 0);
	PROP_PARAM(Int, trigger, 0);
	PROP_PARAM(Int, shotfired, 0);
	PROP_PARAM(Int, hitCounter);
	PROP_PARAM(Int, shotCounter, 0);
	PROP_PARAM(Int, attack_mode, 0);
	PROP_PARAM(Int, resetShotCounter, 0);


protected:
	// world main loop
	void init();
	void update();
	void shutdown();

private:
	Vec3 nozlePos;
	Vec3 weapon_trajectory;
	NodeDummyPtr nozzleNode;
	//Vec3 markerPos[CIRCULARBUFFER_SIZE];
	int  MarkerIndex;
	int  aimMarkerIndex;
	int  shotMarkerIndex;
	int intersectionMask;
	float targetDistance;

	Vec3 AimPos = Vec3_zero;
	int hit_counter;
	int shot_counter;
	float shotTimer;
	float ifps;
	BulletProjectileType bullet[8];
	int triggerIndex;
	float timeInterval  = 0.0;
	float timeInterval2 = 0.0;
	int intervalCounter = 0;
	WorldIntersectionPtr wIntersection;
	
	int TriggerTimer;
	bool isSyncker = false;

	ObjectPtr getTargetAimAndTrigger();
	void processBullets();

	void processtrigger();

	void processHits();
	void collectProperty(BulletProjectileType& bullet);
	void collectTerrainProperty(BulletProjectileType& bullet);
};

