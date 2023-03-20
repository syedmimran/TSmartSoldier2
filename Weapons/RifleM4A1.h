#pragma once

#include <UnigineComponentSystem.h>
#include <UniginePlayers.h>
#include <UnigineWorld.h>
#include <UnigineLights.h>
#include <UnigineLights.h>
#include "RifleM4A2.h"

using namespace Unigine;
using namespace Math;

#define CIRCULARBUFFER_SIZE 60
#define CIRCULARBUFFER_HALFSIZE 30

#define USE_OPTITRACK 0
#define USE_MOUSE     1
#define MAIN_CAMERA_DISTANCE  5.42f 

typedef struct {
	ObjectPtr aimObject;
	ObjectPtr hitObject;
} TargetObjectType;

typedef struct {
	unsigned char Id;
	unsigned int  OpcodeSize;
} OpcodeType;


//class DatagramSocket;
//class Theatre;

class RifleM4A1 : public ComponentBase {
public:
	COMPONENT(RifleM4A1, ComponentBase);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);
	COMPONENT_SHUTDOWN(shutdown);
	PROP_NAME("RifleM4A1");

	PROP_PARAM(Int, id, 0);
	PROP_PARAM(Int, trigger, 0);
	PROP_PARAM(Int, control_type, 0);
	PROP_PARAM(Int, shotfired, 0);
	PROP_PARAM(Int, display_aim_marker, 1);
	PROP_PARAM(Int, display_laser_marker, 1);
	PROP_PARAM(Int, display_bullet_marker, 0);
	PROP_PARAM(Int, hitCounter,0);
	PROP_PARAM(Int, laserId, -1);
	PROP_PARAM(Vec2, aimingOffset, 0);
	PROP_PARAM(Int, active, 0);
	PROP_PARAM(Int, calibrationMode, 0);
	PROP_PARAM(Int, shotCounter, 0);
	PROP_PARAM(Int, resetShotCounter, 0);
	PROP_PARAM(Int, flashlightOn, 0);
	PROP_PARAM(Int, playback_mode, 0);


protected:
	// world main loop
	void init();
	void update();
	void shutdown();

private:
	Vec3 nozlePos;
	Vec3 weapon_trajectory;
	NodeDummyPtr nozzleNode;
	bool MousePressed = false;

	Vec3 markerPos[CIRCULARBUFFER_SIZE];
	int  MarkerIndex;
	int  aimMarkerIndex;
	int  shotMarkerIndex;
	int  laserMarkerIndex = 0;
	float adjustment[2];
	int intersectionMask;
	Vec3 cameraPos[CIRCULARBUFFER_SIZE];
	Vec3 targetAimPoint;
	Vec3 targetHitPoint;
	Vec3 targetCentre;
	NodePtr laser_marker;
	NodePtr aim_marker;
	NodePtr bullet_marker[8];
	float   bullet_marker_timer[8];
	int  bullet_marker_index = 0;

	float moveSpeedX = 0.0;
	float moveSpeedY = 0.0;

	float AimingRadius;
	float AftershotRadius;
	Vec2 AimingCentre;
	bool calculateAftershotDone = false;

	Vec3 AimPos = Vec3_zero;
	PlayerPtr projector;
	int hit_counter;
	int shot_counter;
	float shotTimer;
	int laneNo, actualLaneNo, prevActualLaneNo;
	BulletProjectileType bullet[8];
	int triggerIndex;
	float timeInterval = 0.0;
	float timeInterval2 = 0.0;
	int intervalCounter = 0;
	WorldIntersectionPtr wAimIntersection;
	WorldIntersectionPtr wHitIntersection;

	//LightProjPtr flashLight;
	//AimDataType AimData;
	//HitDataType HitData;
	//ShotDataType ShotData;
	//BarrelDataType BarrelData;
	//WeaponDataType weaponData;

	int TriggerTimer;
	//int prevLaneMode = -1;
	//int laneMode = 0;

	float muzzleSpeed = 910.0;
	int SectorNo;

	void screenAim();
	void processAimming();
	TargetObjectType getTargetAimAndTrigger();
	void processBullets();
	void calculateAimingRadius();
	void calculateAftershotRadius();
	//void UpdateWeaponLaneAssignment();
	void initProjector();

	void processHits();
	void collectProperty(BulletProjectileType & bullet);
	void collectTerrainProperty(BulletProjectileType& bullet);
	void processShooting();
	void updateCircularBuffer();

	void moveKeyboardAim();
	void moveMouseAim();
	void setMouseTrigger();
	//void UpdateLaneMode();
	void setDisplayMarker();
	void display();
	//void processRecording();
	//void processPlayback();
	void updateBulletMarker();
	char identifyObjectType(ObjectPtr object);


};

