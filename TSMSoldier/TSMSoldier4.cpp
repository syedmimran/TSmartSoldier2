#include "TSMSoldier.h"
#include "Utils.h"


bool TSMSoldier::FindEnemy() {
	quat q = HeadBonePoint->getWorldRotation();
	Vec3 headPos = HeadBonePoint->getWorldPosition();
	//Visualizer::renderDirection(headPos, q.getNormal().normalize() * 5.0, vec4_blue, 0.025f, false, 0.0f, false);
	inView = 1;
	if (!find_the_enemy || inView == 0) {
			theEnemy = nullptr;
		return false;
	}

	float dis = 1000.0;
	mEnemyDistance = dis;
	theEnemy = nullptr;
	//Vec3 headPos = HeadBonePoint->getWorldPosition();
	HeadBoneTrajectory = headPos + HeadBone->getWorldTransform().getColumn3(2) * 5.0;// 1000 meter
	theEnemy = FindHumanEnemy();
	int ht = (int)human_type;
	if(!theEnemy && human_type == SM_BLACK_SOLDIER) // if not found other enemy, check player
	  theEnemy = FindCameraPlayer();

	if (theEnemy) {
		theEnemyPos = theEnemy->getWorldPosition();
		//Visualizer::renderLine3D(headPos, theEnemyPos + Vec3(0.0, 0.0, -1.0), vec4(255, 0, 0, 255));
		return true;

	}
	return false;
}

NodePtr TSMSoldier::FindCameraPlayer() {
	return nullptr;
	float dis = 1000.0;
	Vec3 headPos = HeadBonePoint->getWorldPosition();
	NodePtr enemy = Game::getPlayer();
	Vec3 ePos = enemy->getWorldPosition() + Vec3(0.0, 0.0, 0.0);
	//ObjectPtr obj = World::getIntersection(headPos, ePos, 1, wIntersection);

	Vec3 bb = ePos - headPos;
	dis = bb.length();
	mEnemyDistance = dis;

	Vec3 aa = HeadBoneTrajectory - headPos;
	float offsetangle = Math::acos(dot(bb, aa) / (aa.length() * bb.length())) * Consts::RAD2DEG;
	float viewdis = (mViewDistance+10) / (1 + offsetangle * 0.025);
	//lineDisplay(1, 17, String::format("offsetangle = %7.5f mViewAngle = %7.5f", offsetangle, mViewAngle));
	//lineDisplay(1, 18, String::format("viewdis = %7.5f mViewDistance = %7.5f", viewdis, mViewDistance));
	if (offsetangle < mViewAngle && dis < viewdis) {
		return enemy;  // find the nearest
		
	}
	else
		int probablybug = 0;

	return nullptr;
}

NodePtr TSMSoldier::FindHumanEnemy() {
	float dis = 1000.0;
	Vec3 headPos = HeadBonePoint->getWorldPosition();
	int size = myEnemies->size();
	for (int i = 0; i < size; i++) {
		NodePtr cEnemy = myEnemies->get(i);
		Vec3 ePos = cEnemy->getWorldPosition() + Vec3(0.0, 0.0, 1.0);
		//ObjectPtr obj = World::getIntersection(headPos, ePos, 1, wIntersection);
	
		Vec3 bb = ePos - headPos;
		dis = bb.length();
		if (dis > mViewDistance)
			continue;

		if (dis < 0.5) {
			mEnemyDistance = dis; // too close
			return nullptr;
		}

		Vec3 aa = HeadBoneTrajectory - headPos;
		float offsetangle = Math::acos(dot(bb, aa) / (aa.length() * bb.length())) * Consts::RAD2DEG;

		float viewdis = mViewDistance / (1 + offsetangle * 0.025);
		if (offsetangle < mViewAngle && dis < viewdis) {
			return cEnemy;  // find the nearest
		}
		else
			int probablybug = 0;
	}

	return nullptr;
}


void TSMSoldier::setWeaponAimming(bool aim) {
	rifleAimStatus = aim;
	if (!aim)
		reloadStatus = reload2Status = false;
	setWeaponOnTarget(rifleAimStatus);
}



bool TSMSoldier::CheckThreat() {
	if (action_Mode == AC_DEAD)
		return false;
	for (int i = 0; i < myEnemies->size(); i++) {
		PropertyPtr prop = myEnemies->get(i)->getProperty(0);
		PropertyParameterPtr param = prop->getParameterPtr("shotfired");
		int shotFired = param->getValueInt();
		if (shotFired == 1) {
			//param->setValueInt(0);
			return true;
		}
	}
	return false;
}



int TSMSoldier::getEnemyCurAction() {
	if (!theEnemy)
		return  -1;
	PropertyPtr objectProperty = theEnemy->getProperty();
	if (objectProperty) {
		PropertyParameterPtr param = objectProperty->getParameterPtr("mCurAction");
		return param->getValueInt();
	}
	return  -1;

}




