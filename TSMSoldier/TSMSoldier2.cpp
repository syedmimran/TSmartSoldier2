#include "TSMSoldier.h"
#include "TPlayerManager.h"
#include "Joystick.h"
#include "Utils.h"
//#include "IsmatInterface.h" // rrr


void TSMSoldier::updateTestingLogicSequence() {
	if (mFirstAction[AC_TESTING]) {
		mFirstAction[AC_TESTING] = false;
		tempProfile = PF_PRONNING;
		changeProfile(tempProfile);
		setMovingSpeed(0.20);
		tempMovingSpeed = 0.0;
		setMoveStatus(false);
		setTurnRate(2.5);
		setTargetPos(Vec3(4000.0, 4.0, 0.0));
		showMarker(mTargetPos, 2);
		//node->setWorldRotation(quat(0.0, 0.0, -90));
		//Game::setPlayer(checked_ptr_cast<PlayerDummy>(node->getChild(6)));
		return;
	}
	if (TJoystickControl::get()->getButton(2) && !jsButtonPress[2]) {
		jsButtonPress[2] = true;
		if (tempProfile < PF_PRONNING) {
			tempProfile++;
		//	//tempProfile++;
		}
		//tempProfile = PF_STANDING1 + rand() % 4;
		changeProfile(tempProfile);

		for (int i = ROT_R_ARM; i < NO_OF_ROTS; i++)
			jointRot[i][mProfile][AIMMING] = quat_identity;
		rifleAimStatus = 0;
	}
	if (!TJoystickControl::get()->getButton(2))
		jsButtonPress[2] = false;

	if (TJoystickControl::get()->getButton(4) && !jsButtonPress[4]) {
		jsButtonPress[4] = true;
		if (tempProfile > PF_STANDING1) {
			tempProfile--;
	//		tempProfile--;
		}
		changeProfile(tempProfile);
		for (int i = ROT_R_ARM; i < NO_OF_ROTS; i++)
			jointRot[i][mProfile][AIMMING] = quat_identity;
		rifleAimStatus = 0;
	}
	if (!TJoystickControl::get()->getButton(4))
		jsButtonPress[4] = false;


	if (TJoystickControl::get()->getButton(3)) {
		jsButtonPress[3] = true;
		if (tempMovingSpeed > -1.0)
			tempMovingSpeed -= 0.005;

		//mLeftFingerAngle -= 0.1;
		setMovingSpeed(tempMovingSpeed);
	}

	if (TJoystickControl::get()->getButton(5)) {
		jsButtonPress[5] = true;
		if (tempMovingSpeed < 4.5)
			tempMovingSpeed += 0.005;
		//mLeftFingerAngle += 0.1;
		setMovingSpeed(tempMovingSpeed);
	}


	if (TJoystickControl::get()->getButton(1) && !jsButtonPress[1] && reloadSeq == 0) {
		jsButtonPress[1] = true;
		if (inTransition)
			return;
		rifleAimStatus = !rifleAimStatus;
	}

	if (!TJoystickControl::get()->getButton(1))
		jsButtonPress[1] = false;


	if (TJoystickControl::get()->getButton(6) && !jsButtonPress[6]) {
		jsButtonPress[6] = true;
		tempMovingSpeed = 0.0;
		setMovingSpeed(tempMovingSpeed);
	}

	if (!TJoystickControl::get()->getButton(6))
		jsButtonPress[6] = false;

	if (TJoystickControl::get()->getButton(8) && !jsButtonPress[8]) {
		jsButtonPress[8] = true;
		move_status = !move_status;
		if (!move_status)
			node->setWorldPosition(Vec3(-1.0, 3.0, 0.0));
	}

	if (!TJoystickControl::get()->getButton(8))
		jsButtonPress[8] = false;

	
	triggerPull = TJoystickControl::get()->getTrigger();
	if (Input::isKeyDown(Input::KEY_M)) {
		prev_action_Mode = AC_TESTING;
		changeAction(AC_DEAD);
	}
}


void TSMSoldier::showMarker(Vec3 pos, int id) {
	mMarker->setEnabled(true);
	mMarker->setWorldPosition(pos);
	ObjectTextPtr objtext = checked_ptr_cast<ObjectText>(mMarker->getChild(0));
	objtext->setEnabled(1);
	PlayerPtr camera = Game::getPlayer();

	objtext->setWorldRotation(camera->getWorldRotation());
	objtext->setText(String::format("%d", id));
	objtext->setFontSize(50);
	objtext->setTextColor(vec4_black);
	objtext->setPosition(Vec3(0.0, 0.0, 0.7));
}


void TSMSoldier::setupPointOfAttack() {
	Vec3 attackPos = mAttackPoint;
	Vec3 lorryPos = (Vec3)initPos; // lorry
	float h = lorryPos.z;
	attackPos.z = 0.0;
	lorryPos.z = 0.0;
	Vec3 pn = lorryPos - attackPos;
	float r = -Math::atan2(pn.x, pn.y) * Consts::RAD2DEG;
	Vec3 pos1 = pn * quat(rotateZ(r));
	float len = pos1.length();
	Vec3 pos2 = pos1 * (len - 15.0) / len;
	int a_id = (int)attack_id;
	if ((a_id % 2) == 0)
		pointAttackIndex = a_id / 2;
	else
		pointAttackIndex = numOfAttackPersons - a_id * 0.5  - 0.5;
	int offset = pointAttackIndex - numOfAttackPersons / 2;
	int x = (int)numOfAttackPersons;
	Vec3 pos4 = pos2;
	if (x % 2)
		pos4 += Vec3((float)attackSeparation * offset, 0.0, 0.0);
	else
	    pos4 += Vec3((float)attackSeparation * offset + attackSeparation/2.0, 0.0, 0.0);
	pos4 = pos4 * quat(rotateZ(-r));
	AttackStartPoint = pos4 + attackPos;
	// rrr
	AttackStartPoint.z = getWorldHeight(AttackStartPoint.x, AttackStartPoint.y, 1000.0);
	//AttackStartPoint.z = IsmatInterface::get()->getWorldHeight(AttackStartPoint.x, AttackStartPoint.y, 1000.0);
	
	showMarker(AttackStartPoint, a_id);

	pn = AttackStartPoint - (Vec3)initPos;
	r = -Math::atan2(pn.x, pn.y) * Consts::RAD2DEG;
	if (platform)
		node->setRotation(quat(rotateZ(r)));
	else
		node->setWorldRotation(quat(rotateZ(r)));
}


void TSMSoldier::updateAttackLogicSequence() {
	if (numOfAttackPersons == 0)
		return;
	if (mFirstAction[AC_ATTACK]) {
		setupPointOfAttack();
		changeProfile(PF_STANDING1 + rand() % 2, 2.5);
		mFirstAction[AC_ATTACK] = false;
		setTargetPos(AttackStartPoint);
		setTurnRate(2.5);
		groupInstruction = 0;
		oddX = 0;
		return;
	}
	if (groupInstruction == 1) {
		startTheAttack = true;
		groupInstruction = 0;
	}

	if (!arrivedAtStartPoint) {
		arrivedAtStartPoint = mTargetArrived;
		if (arrivedAtStartPoint) {
			wait_attack_timer = 0.0;
			TPlayerManager::get()->oneSoldierIsReady();
		}
	}

	if (arrivedAtStartPoint && !startTheAttack) {
		wait_attack_timer += Game::getIFps();
		changeProfile(PF_CROUCHING, 0.0);
		setTargetPos(mAttackPoint);
		int x = (int)attack_id;
		int y = (int)numOfAttackPersons;
		int z = TPlayerManager::get()->getNumOfSoldierReady();
		if (TPlayerManager::get()->getNumOfSoldierReady() == numOfAttackPersons && wait_attack_timer > 5.0) { // last person
			startTheAttack = true;
			attack_start_timer = 0.0;
			TPlayerManager::get()->setGroupReadyToAttack((int)group_type);
		}
		return;
	}


	if (startTheAttack) {
		int x = (int)attack_id;
		setFindEnemy(true);
		//setWeaponAimming(true);
		//if ((AttackPoint- mCurWorldPos).length() < 20.0)
		attack_start_timer += Game::getIFps();
		if (prev_attack_start_timer % 15 == 0) { 
			if (!newAttackModeDone) {
				targetMove = Vec3(rand() % 3 - 1, rand() % 3 - 1, 0.0);
				oddX++;
				if (oddX % 2) {
					setWeaponAimming(true);
					if (pointAttackIndex % 2)
						changeProfile(PF_STANDING1, 2.0);
					else
						changeProfile(PF_PRONNING, 0.0);

				}
				else {
					setWeaponAimming(false);
					if (pointAttackIndex % 2)
						changeProfile(PF_PRONNING, 0.0);
					else
						changeProfile(PF_STANDING1, 2.0);
				}
				newAttackModeDone = true;
			}
		}
		else
			newAttackModeDone = false;
		//setTargetPos(AttackPoint + targetMove);
	
		if (theEnemyFound) {
			attack_start_timer = 0.0;
			changeAction(AC_SPOT);
		}

		prev_attack_start_timer = (int)attack_start_timer;
	}
	else
		prev_attack_start_timer = 0;
}

void TSMSoldier::updatePatrolLogicSequence() {
	switch (behaviour_type) {
	case 1:
		updatePatrolLogicSequence1();
		break;
	case 2:
		updatePatrolLogicSequence2();
	case 3:
		updatePatrolLogicSequence3();  // local
		break;
	case 4:
		updatePatrolLogicSequence4();
		break;
	case 5:
		updatePatrolLogicSequence5();
		break;
	}
}


void TSMSoldier::updatePatrolLogicSequence1() {
	NodePtr n;

	if (mFirstAction[AC_PATROL]) {
		mFirstAction[AC_PATROL] = false;
		changeProfile(PF_STANDING1, 1.0);
		pointPatrolIndex = 0;
		if (patrolRoute) {
			numOfWaypoints = patrolRoute->getNumChildren();
			n = patrolRoute->getChild(pointPatrolIndex);
			setTargetPos(n->getWorldPosition());
			//showMarker(n->getWorldPosition(), pointPatrolIndex);
		}
		else
			setMovingSpeed(0.0);

		setTurnRate(2.0);
		setFindEnemy(true);
		return;
	}
	modeTimer[AC_PATROL] += Game::getIFps();
	if (modeTimer[AC_PATROL] > 10.0) {
		changeProfile(PF_STANDING1 + rand() % 2, 0.8 + (rand() % 5) * 0.25);
		modeTimer[AC_PATROL] = 0.0;
	}
	if (theEnemyFound) {
		setTargetPos(theEnemy->getWorldPosition());
		changeAction(AC_SPOT);
	}
	setWeaponAimming(false);

	if (!patrolRoute) 
		return;

	String s = patrolRoute->getName();
	triggerPull = false;

	if (mTargetArrived && !mTargetArrivedDone) {
		mTargetArrivedDone = true;
		pointPatrolIndex++;
		pointPatrolIndex %= numOfWaypoints;
		n = patrolRoute->getChild(pointPatrolIndex);
		setTargetPos(n->getWorldPosition());
		//showMarker(n->getWorldPosition(), pointPatrolIndex);
	}
}



void TSMSoldier::updatePatrolLogicSequence2() {

}


void TSMSoldier::updatePatrolLogicSequence3() {

}


void TSMSoldier::updatePatrolLogicSequence4() {

}

void TSMSoldier::updatePatrolLogicSequence5() {

}


void TSMSoldier::updateSpotLogicSequence() {
	if (mFirstAction[AC_SPOT]) {
		//changeProfile(PF_CROUCHING, 0.0);
		changeProfile(PF_STANDING1, 0.0);
		mFirstAction[AC_SPOT] = false;
		return;
	}
	modeTimer[AC_SPOT] += Game::getIFps();
	triggerPull = false;

	if (!theEnemyFound)
		changeAction(AC_PATROL);
	else
		setTargetPos(theEnemy->getWorldPosition());
	setTurnRate(5.0);
	if (modeTimer[AC_SPOT] > 5.0) 
		setMovingSpeed(1.0);	
	if (mTargetDistance < 20.0)
		changeAction(AC_AIM);
	setWeaponAimming(false);
	modeTimer[AC_AIM] = 0.0;
}


void TSMSoldier::updateAimLogicSequence() {
	if (mFirstAction[AC_AIM]) {
		//changeProfile(PF_CROUCHING, 0.0);
		changeProfile(PF_STANDING1, 0.0);
		mFirstAction[AC_AIM] = false;
		return;
	}
	modeTimer[AC_AIM] += Game::getIFps();
	triggerPull = false;
	setTurnRate(2.0);
	if (modeTimer[AC_AIM] > 5.0 ) {
		setMovingSpeed(1.0);
	}

	setWeaponAimming(true);
	if (modeTimer[AC_AIM] > 10.0 && mEnemyDistance < 50.0)
		changeAction(AC_SHOOT);
	if (!theEnemyFound)
		changeAction(AC_PATROL);
	else
		setTargetPos(theEnemy->getWorldPosition());
}


void TSMSoldier::updateShootLogicSequence() {
	if (isDead || inTransition || inReloadOn)
		return;
	if (mFirstAction[AC_SHOOT]) {
		mFirstAction[AC_SHOOT] = false;
		return;
	}
	modeTimer[AC_SHOOT] += Game::getIFps();
	shootTimer2 += Game::getIFps();
	setTurnRate(2.0);
	if (modeTimer[AC_SHOOT] < 0.0001)
		shootTimer2 = 0.0;
	
	if (shootTimer2 > 0.25) {
		triggerPull = true;
	}
	else
		triggerPull = false;
	if (shootTimer2 > 0.5 || !inAimingOn || !inReloadOff)
		shootTimer2 = 0.0;
		
	if (modeTimer[AC_SHOOT] < 10.0 || mEnemyDistance < 10.0) {
		//changeProfile(PF_CROUCHING, 0.0);
		changeProfile(PF_STANDING1, 0.0);
	}
	else {
		changeProfile(PF_STANDING1,1.0);
	}

	setWeaponAimming(true);
	if (!theEnemyFound)
		changeAction(AC_PATROL);
	else
		setTargetPos(theEnemy->getWorldPosition());
}

void TSMSoldier::setupRetreatPoint() {
	Vec3 p1 = Game::getPlayer()->getWorldPosition();
	Vec3 p2 = mCurWorldPos;
	p1.z = p2.z = 0.0;

	Vec3 pn = p2 - p1;
	float len = pn.length() + 0.5;
	retreatPointMiddle = mCurWorldPos + pn * 100.0 / len;
	float r = -Math::atan2(pn.x, pn.y) * Consts::RAD2DEG;
	retreatPointLeft  = retreatPointMiddle + Vec3(-100.0, 0.0, 0.0) * quat(rotateZ(r));
	retreatPointRight = retreatPointMiddle + Vec3(100.0, 0.0, 0.0) * quat(rotateZ(r));
	//ObjectPtr c = checked_ptr_cast<Object>(NodeReference::create("Nodes/Cuboid")->getReference());
	//c->setWorldPosition(retreatPointMiddle);
	//c = checked_ptr_cast<Object>(NodeReference::create("Nodes/Cuboid")->getReference());
	//c->setWorldPosition(retreatPointLeft);
	//c = checked_ptr_cast<Object>(NodeReference::create("Nodes/Cuboid")->getReference());
	//c->setWorldPosition(retreatPointRight);
}

void TSMSoldier::updateRetreatLogicSequence() {
	if (isDead || inTransition)
		return;
	if (mFirstAction[AC_RETREAT]) {
		changeProfile(PF_STANDING1, 3.0);
		mFirstAction[AC_RETREAT] = false;
		modeTimer[AC_RETREAT] = 0.0;
		//setMoveStatus(false);
		setTurnRate(5.0);
		setupRetreatPoint();
		setTargetPos(retreatPointLeft);
		setWeaponAimming(false);
		oddX = 0;
		return;
	}
	modeTimer[AC_RETREAT] += Game::getIFps();
	if (modeTimer[AC_RETREAT] > 5) {
		modeTimer[AC_RETREAT] = 0.0;
		oddX++;
		if (oddX % 2)
		  setTargetPos(retreatPointRight);
		else
		  setTargetPos(retreatPointLeft);
	}
	if ((retreatPointMiddle - mCurWorldPos).length() < 10.0)
		disposeMe();

}

void TSMSoldier::updateTakeCoverLogicSequence() {
	if (isDead || inTransition || inReloadOn)
		return;

}


void TSMSoldier::updateThrowGrenadeLogicSequence() {
	if (isDead || inTransition || inReloadOn)
		return;

}



void TSMSoldier::updateDeadLogicSequence() {
	if (mFirstAction[AC_DEAD]) {
		newProfileQueue.clear();
		//changeAction(AC_DEAD);
		mFirstAction[AC_DEAD] = false;
		deadTransition = true;
		changeProfile(PF_DYING1 + rand() % 3, 0.0);
		modeTimer[AC_DEAD] = 0.0;
		return;
	}
	modeTimer[AC_DEAD] += Game::getIFps();

	mMovingSpeed /= 1.01;
	if (modeTimer[AC_DEAD] > 10.0)
		 disposeMe();
	if (Input::isKeyDown(Input::KEY_N)) {
		int x = prev_action_Mode;
		prev_action_Mode = AC_PATROL;
		changeAction(x);
		mPrevProfile = PF_STANDING2;
		mProfile = PF_PRONNING;
		isDead = false;
		deadTransition = false; 
		inTransition = true;
	}
}

void TSMSoldier::reRoutePatrol() {

}








