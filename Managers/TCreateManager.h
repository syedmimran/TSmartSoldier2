#pragma once

#include <UnigineComponentSystem.h>
#include <UnigineObjects.h>
#include <UnigineVector.h>
#include <Unigine.h>

#define MAIN_CAMERA_DISTANCE 5.42f

using namespace Unigine;
using namespace Math;

class TCreateManager  {
public:
	TCreateManager() {}
	~TCreateManager() {}
	static TCreateManager* get() {
		static TCreateManager instance;
		return &instance;
	}

	void init();
	void update();
	void shutdown();
	void placeObject();

private:
	int numOfPerson = 0;
	int numOfPlate = 0;
	int numOfTargetBoard = 0;
	ObjectTextPtr objtext;
	int intersectionMask;
	Vec3 markerPos, AimPos = Vec3_zero;
	NodePtr cross_marker;
	void moveMarker();
	WorldIntersectionPtr wIntersection;
	PlayerPtr projector;
	float rotateValue = 0.0;
	int startCounter = 0;
	Vec3 cameraPos, cam_trajectory;
	Vec3 DropPos;
	float moveSpeed2 = 0.005;
	bool isSyncker = false;
	bool MousePressed = false;
	bool offloadPassenger = false;
	int num_of_Attack_persons = 0;
	int attackSeparation = 0;
	float offload_timer = 0.0;
	int passangerCount = 0;
	int pageNo = 0;
	void moveMouseAim();
	void triggerMouse();
	bool getIntersection();
	void OffloadPassenger();
	void setOffLoadPassengers(int humantype, int group_type, Vec3 attackPos, int attack_id);


};

