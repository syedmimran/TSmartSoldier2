#include "TSMSoldier.h"
#include <UnigineVisualizer.h>

float TSMSoldier::avoidObstacle() {
	if (!avoid_the_obstacles || action_Mode == AC_DEAD)
		return 0.0;
	if (fabs(mCurSpeed) < 0.001)
		return 0.0;
	float dis[2] = { 100.0f,100.0f };
	Vec3 pos = ForwardPoint->getWorldPosition();
	PathForwardPoint = pos + (pos - CentrePoint->getWorldPosition()).normalize() * mCurSpeed;

	for (int i = 0; i < 2; i++) {
		PathRearPoint[i] = RearPoint[i]->getWorldPosition();
		//Visualizer::renderLine3D(PathForwardPoint, PathRearPoint[i], vec4(0, 255, 0, 255));
		ObjectPtr obj = World::getIntersection(PathForwardPoint, PathRearPoint[i], 2, wIntersection);
		if (obj) {
			//Visualizer::renderLine3D(PathRearPoint[i], wIntersection->getPoint(), vec4(255, 0, 0, 255));
			float d = (mCurWorldPos - wIntersection->getPoint()).length();
			dis[i] = d;
		}
	}
	float startValue = 100.0;
	ObstacleIndex = -1;
	for (int i = 0; i < 2; i++) {
		if (dis[i] < startValue) {
			startValue = dis[i];
			ObstacleIndex = i;
		}
	}

	if (ObstacleIndex != -1) {
		if (ObstacleIndex == 0)
			mAvoidAngle -= 100.0 * mCurSpeed;
		else
			mAvoidAngle += 100.0 * mCurSpeed;
	}
	mAvoidAngle /= 1.1;
	return mAvoidAngle;
}

void TSMSoldier::setupObstacleSensors() {
	HeadBone = node->getChild(2);
	HeadBonePoint = NodeDummy::create();
	HeadBonePoint->setParent(HeadBone);
	HeadBonePoint->setPosition(Vec3(0.0f, 0.0f, 0.5f));

	ChestBone = node->getChild(2);
	ChestBonePoint[0] = NodeDummy::create();
	ChestBonePoint[0]->setParent(HeadBone);
	ChestBonePoint[0]->setPosition(Vec3(0.0f, 0.0f, 0.0f));

	ChestBonePoint[1] = NodeDummy::create();
	ChestBonePoint[1]->setParent(HeadBone);
	ChestBonePoint[1]->setPosition(Vec3(-0.10f, 0.0f, 0.5f));

	ChestBonePoint[2] = NodeDummy::create();
	ChestBonePoint[2]->setParent(HeadBone);
	ChestBonePoint[2]->setPosition(Vec3(0.10f, 0.0f, 0.5f));

	ChestBonePoint[3] = NodeDummy::create();
	ChestBonePoint[3]->setParent(HeadBone);
	ChestBonePoint[3]->setPosition(Vec3(-0.25f, 0.0f, 0.5f));

	ChestBonePoint[4] = NodeDummy::create();
	ChestBonePoint[4]->setParent(HeadBone);
	ChestBonePoint[4]->setPosition(Vec3(0.25f, 0.0f, 0.5f));

	ForwardPoint = NodeDummy::create();
	ForwardPoint->setParent(node);
	ForwardPoint->setPosition(Vec3(0.0f, 1.0f, 0.4f));
	CentrePoint = NodeDummy::create();
	CentrePoint->setParent(node);
	CentrePoint->setPosition(Vec3(0.0f, 0.0f, 0.4f));

	for (int i = 0; i < 2; i++) {
		RearPoint[i] = NodeDummy::create();
		RearPoint[i]->setParent(node);
		RearPoint[i]->setPosition(Vec3(i == 0 ? -0.8 : 0.8, 0.5, 0.4f));
	}
}


float TSMSoldier::findPath() {
	if (!find_the_path || action_Mode == AC_DEAD)
		return 0.0;
	if(fabs(mCurSpeed) < 0.001)
		return 0.0;
	Vec3 nozlePos = HeadBonePoint->getWorldPosition();
	HeadBoneTrajectory = nozlePos + HeadBone->getWorldTransform().getColumn3(2) * 4.0;// 1000 meter
	//Visualizer::renderLine3D(nozlePos, HeadBoneTrajectory, vec4::BLUE);

	//Visualizer::renderLine3D(nozlePos, ChestBoneTrajectory, vec4::BLUE);
	//return;
	Vec3 chestBonePt[4];
	for (int i = 0; i < 4; i++)
		chestBonePt[i] = ChestBonePoint[i + 1]->getWorldPosition();
	nozlePos = ChestBonePoint[0]->getWorldPosition();
	float s = mCurSpeed * 3.0;
	innerChestBoneTrajectory[0] = chestBonePt[0] + (chestBonePt[0] - nozlePos).normalize() * s;
	innerChestBoneTrajectory[1] = chestBonePt[1] + (chestBonePt[1] - nozlePos).normalize() * s;
	outerChestBoneTrajectory[0] = chestBonePt[2] + (chestBonePt[2] - nozlePos).normalize() * s;
	outerChestBoneTrajectory[1] = chestBonePt[3] + (chestBonePt[3] - nozlePos).normalize() * s;
	drawline = false;
	if (drawline) {
	    //Visualizer::renderLine3D(nozlePos, chestBonePt[0], vec4(255, 255, 0, 255));
	    Visualizer::renderLine3D(chestBonePt[0], innerChestBoneTrajectory[0], vec4(255, 0, 0, 255));
		Visualizer::renderLine3D(chestBonePt[1], innerChestBoneTrajectory[1], vec4(255, 0, 0, 255));
		Visualizer::renderLine3D(chestBonePt[2], outerChestBoneTrajectory[0], vec4(0, 255, 255, 255));
		Visualizer::renderLine3D(chestBonePt[3], outerChestBoneTrajectory[1], vec4(0, 255, 255, 255));
	}

	if (dontAvoidObstacle)
		return 0.0;

	float dis[2] = { 100.0f,100.0f };
	float innerdis[2] = { 100.0f,100.0f };
	for (int i = 0; i < 2; i++) {
		ObjectPtr obj = World::getIntersection(chestBonePt[i], outerChestBoneTrajectory[i], 1, wIntersection);
		if (obj) {
			if (drawline)
				Visualizer::renderLine3D(chestBonePt[i], wIntersection->getPoint(), vec4(255, 0, 0, 255));
			dis[i] = (mCurWorldPos, wIntersection->getPoint()).length();
		}
		ObjectPtr innerobj = World::getIntersection(chestBonePt[i + 1], innerChestBoneTrajectory[i], 1, wIntersection);
		if (innerobj) {
			if (drawline)
				Visualizer::renderLine3D(chestBonePt[i + 2], wIntersection->getPoint(), vec4(255, 0, 0, 255));
			innerdis[i] = (mCurWorldPos, wIntersection->getPoint()).length();
		}
	}
	float startValue = 100.0;
	ObstacleIndex = -1;
	for (int i = 0; i < 2; i++) {
		if (dis[i] < startValue) {
			startValue = dis[i];
			ObstacleIndex = i;
		}
	}

	if (ObstacleIndex != -1) {
		if (ObstacleIndex == 0)
			mAvoidAngle -= 3.0 * forwardSpeed;
		else
			mAvoidAngle += 3.0 * forwardSpeed;
	}

	startValue = 100.0;
	ObstacleIndex = -1;
	for (int i = 0; i < 2; i++) {
		if (innerdis[i] < startValue) {
			startValue = innerdis[i];
			ObstacleIndex = i;
		}
	}
	if (ObstacleIndex != -1) {
		if (ObstacleIndex == 0)
			mAvoidAngle -= 6.0 * forwardSpeed;
		else
			mAvoidAngle += 6.0 * forwardSpeed;
	}
	mAvoidAngle /= 1.1;
	return mAvoidAngle;
}







