#include "TSMSoldier.h"
#include "SynckerInterface.h"
#include "Utils.h"


void TSMSoldier::setupPatrolLogics() {

}

void TSMSoldier::setupPatrolRoute() {

}


void TSMSoldier::changeAction(int newAction) {
	if (prev_action_Mode == newAction || inTransition)
		return;
	mfirstModeState[newAction] = true;
	modeTimer[newAction] = 0.0;
	find_the_enemy = true;
	isDead = 0;
	switch (newAction) {
	case AC_TESTING:
		setFindEnemy(false);
		break;
	case AC_PATROL:
		break;
	case AC_SPOT:
		break;
	case AC_AIM:
		break;
	case AC_SHOOT:
		break;
	case AC_ATTACK:
		break;
	case AC_DEAD:
		isDead = 1;
		break;
	}

	prev_action_Mode = action_Mode;
	action_Mode = newAction;
}


void TSMSoldier::updateTransition() {
	if (newProfileQueue.empty() || inTransition)
		return;
	transitionTimer += Game::getIFps();
	if (transitionTimer > 1.0) {
		nextProfile = newProfileQueue.first();
		transitionTimer = 0.0;
	}
	else 
		return;

	int x = getProfileType(nextProfile);
	if (x == DEAD_TYPE) { // dead
		mPrevProfile = mProfile;
		mProfile = nextProfile;
		newProfileQueue.clear();
		return;
	}
	int y = getProfileType(mProfile);
	int c = Math::abs(y - x);
	if (c > 1) {
		newProfileQueue.append(0, PF_CROUCHING);
		transitionTimer = 2.0;
		return;
	}
	mPrevProfile = mProfile;
	mProfile = nextProfile;
	newProfileQueue.removeFirst();
}


void TSMSoldier::changeProfile(int newProfile) {
	transitionTimer = 2.0;
	if (newProfileQueue.empty()) {
		newProfileQueue.append(newProfile);
		return;
	}
	if (newProfileQueue.last() == newProfile)
		return;
	newProfileQueue.append(newProfile);
}

void TSMSoldier::changeProfile(int newProfile, float speed) {
	changeProfile(newProfile);
	tMovingSpeed = speed;
}

void TSMSoldier::turnXTo_G() {
	mTargetHeading = getDirection(mCurWorldPos, mTargetPos) + findPath() + avoidObstacle();
	mCurHeading = mCurWorldRot.getAngle(vec3(0.0, 0.0, 1.0));
	mHeadingDiff = HeadingDifference(mCurHeading, mTargetHeading);
	clampValue(mHeadingDiff, -45.0, 45.0);
	float actualTurnRate = mHeadingDiff * mTurnRate * Game::getIFps();
	node->setWorldRotation(quat(rotateZ(mCurHeading + actualTurnRate)));
	Vec3 v = mCurWorldPos - mTargetPos;
	mTargetDistance = sqrt(v.x * v.x + v.y * v.y);
	mTargetArrived =  mTargetDistance < 0.25;
	if (mTargetDistance > 0.25)
		mTargetArrivedDone = false;
}





float TSMSoldier::getDirection(Vec3 from, Vec3 to) {
	return -Math::atan2(to.x - from.x, to.y - from.y) * Consts::RAD2DEG;
}


bool TSMSoldier::checkPatrolLogicSequenceIsOk(int next_nextSequence) {

	return true;
}













