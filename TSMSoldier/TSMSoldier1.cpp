#include "TSMSoldier.h"
#include "TPlayerManager.h"
#include "SynckerInterface.h"
#include "Utils.h"

bool gTesting = false; 


REGISTER_COMPONENT(TSMSoldier);


void TSMSoldier::init() {
	//int sl = (int)smartLevel;
	node->setEnabled(true);
	ObjectPtr human = checked_ptr_cast<Object>(node);
	human->setName(String::format("Human_%d_%d", (int)human_type, (int)id));
	mAvatar = checked_ptr_cast<ObjectMeshSkinned>(node);
	mRifle = checked_ptr_cast<ObjectMeshStatic>(rifleRef->getChild(0));
	rifleRot = mRifle->getRotation();
	objtext = checked_ptr_cast<ObjectText>(node->getChild(0));
	dispaytext = "X";
	if (objtext)
		textPos = objtext->getPosition();

	mMarker = checked_ptr_cast<Object>(NodeReference::create("Nodes/Cuboid.node")->getReference());
	mMarker->setEnabled(false);
	speedVariation = 5 + ((rand() % 20) - 10) * 0.01;

	setupObstacleSensors();
	wpIndex = 0;

	if (platform) {
		node->setParent(platform);
		Vec3 pos = (Vec3)initPos - platform->getWorldPosition();
		pos.z = 0.0;
		node->setPosition(pos);
	}
	else
	    node->setWorldPosition((Vec3)initPos); 

	wIntersection = WorldIntersection::create();
	mAttackPoint = Game::getPlayer()->getWorldPosition();
	// rrr
	//mAttackPoint = Vec3(0.0, -1000.0, 0.0);

	mBulletCount = magazineCapacity = 8;
	mMagazineCount = 5;
	isShooting = false;

	mHeadingDiff = 10.0;
	drawline = false;

	mAvoidAngle = 0.0;
	SlowDown = 1.0;

	mTerrainHeight = 0.0;

	mCurSpeed = 0.0;
	mTargetHeading = 0.0;

	mCurStateTimer = 0.0;

	for (int i = 0; i < 12; i++) {
		jsButtonPress[i] = false;
	}

	wayPointList.resize(3);
	wayPointList[0].pos = vec3(5.0, 0.0, 0.0);
	wayPointList[0].profile = PF_STANDING1;
	wayPointList[0].action = AC_AIM;
	wayPointList[0].attribute = 1;
	wayPointList[1].pos = vec3(0.0, 10.0, 0.0);
	wayPointList[1].attribute = 0;
	wayPointList[1].profile = PF_CROUCHING;
	wayPointList[1].action = AC_AIM;
	wayPointList[2].pos = vec3(-5.0, 0.0, 0.0);
	wayPointList[2].profile = PF_PRONNING;
	wayPointList[2].action = AC_AIM;
	wayPointList[2].attribute = 0;

	//mAlertLogicSequence = 0;
	//mAlertFunctionSequence = 0;
	mTime = 0.0;
	Name = "SYED";
	theEnemy = nullptr;
	TPlayerManager::get()->addToTeamList(human, human_type);
	myEnemies = TPlayerManager::get()->getHumanTeam(human_type == SM_BLACK_SOLDIER ? SM_GREEN_SOLDIER : SM_BLACK_SOLDIER);
	padDoorOpener = checked_ptr_cast<ObjectMeshStatic>(node->getChild(5));
	if (padDoorOpener) {
		String name = padDoorOpener->getName();
	}

	InitBonesAndAll();
	setSmartLevel(0);

	switch (behaviour_type) {
	case 0:
		changeAction(AC_TESTING);
		break;
	case 1:
		changeAction(AC_PATROL);
		break;
	case 6:
		changeAction(AC_ATTACK);
		break;
	default:
		break;
	}
	InitAnimFile();
}


void TSMSoldier::update() {
	switch (behaviour_type) {
	case 0 : // test
		gTesting = true; 
		getKeyboardInput();
		updatePosition();
		updateAction();
		updateSmartLevel();
		updateMovement();
		//sendData();
		displayInfo();
		showInfo();
		break;
	case 1: case 2: case 3: case 4:case 5: // patrol
	case 6: // attack
	case 7: 
		updatePosition();
		updateAction();
		updateSmartLevel();
		updateMovement();
		//sendData();
		showInfo();
		break;
	case 8: // animation
		getKeyboardInput();
		playAnimFile();
		//sendDataTest();
		displayInfo();
		showInfo();
		break;
	case 10:
		processClient();
		objtext->setEnabled(0);
		break;
	case 11:
		processClientTest();
		objtext->setEnabled(0);
		break;
	}
}



int TSMSoldier::getProfileType(int profile) {
	switch (profile) {
	case PF_TPOSE : case PF_STANDING1: case PF_STANDING2:
		return STANDING_TYPE;
	case PF_CROUCHING:
		return CROUCHING_TYPE;
	case PF_PRONNING:
		return PRONNING_TYPE;
	case PF_DYING1:
	case PF_DYING2:
	case PF_DYING3:
		return DEAD_TYPE;
	default: return DEAD_TYPE;

	}
}



void TSMSoldier::updateSmartLevel() {
	if (prevSmartLevel == smartLevel)
		return;
	prevSmartLevel = smartLevel;
	switch ((int)smartLevel) {
	case 0 :
		mViewDistance = 20.0;
		mViewAngle = 90.0;
		mShootingDistance = 18.0;
		mShootingError = 0.5;
		aim_expired_timer = 1.0 + (rand() % 20) / 20.0;
		retreat_expired_timer = 1.0 + rand() % 2;
		break;
	case 1:
		mViewDistance = 12.0;
		mViewAngle = 10.0;
		mShootingDistance = 10.0;
		mShootingError = 0.2;
		aim_expired_timer = 1.0 + (rand() % 20) / 20.0;
		retreat_expired_timer = 1.0 + rand() % 2;
		break;
	case 2:
		mViewDistance = 17.0;
		mViewAngle = 15.0;
		mShootingDistance = 15.0;
		mShootingError = 0.15;
		aim_expired_timer = 1.0 + (rand() % 20) / 20.0;
		retreat_expired_timer = 1.0 + rand() % 2;
		break;
	case 3:
		mViewDistance = 22.0;
		mViewAngle = 20.0;
		mShootingDistance = 20.0;
		mShootingError = 0.1;
		aim_expired_timer = 1.0 + (rand() % 20) / 20.0;
		retreat_expired_timer = 1.0 + rand() % 2;
		break;
	case 4:
		mViewDistance = 27.0;
		mViewAngle = 25.0;
		mShootingDistance = 25.0;
		mShootingError = 0.05;
		aim_expired_timer = 1.0 + (rand() % 20) / 20.0;
		retreat_expired_timer = 1.0 + rand() % 2;
		break;
	case 5:
		mViewDistance = 50.0;
		mViewAngle = 30.0;
		mShootingDistance = 100.0;
		mShootingError = 0.0;
		aim_expired_timer = 0.0;// 1.0 + (rand() % 20) / 20.0;
		break;
	default:
		mViewDistance = 32.0;
		mViewAngle = 30.0;
		mShootingDistance = 30.0;
		mShootingError = 0.0;
		aim_expired_timer = 1.0 + (rand() % 20) / 20.0;
		retreat_expired_timer = 1.0 + rand() % 2;
		break;
	}
}


void TSMSoldier::shutdown() {
	mMarker->deleteLater();
	SynckerInterface::get()->deleteRecSyncNode(node);
}


bool TSMSoldier::setThisTeamIdle(int teamtype) {
	if (human_type != teamtype)
		return false;
	if (action_Mode == AC_DEAD)
		return true;

	changeAction(AC_PATROL);
	return true;
}


void TSMSoldier::updateAction() {
	mRifle->setEnabled(true); // remove rifle

	isShooting = false;

	updateWeaponOnTarget();

	if (set_Action==AC_DEAD && !isDead) {
		prev_action_Mode = AC_TESTING;
		changeAction(AC_DEAD);
		set_Action = 0;
	}
	if (set_Action == AC_RETREAT && !isDead) {
		prev_action_Mode = AC_TESTING;
		changeAction(AC_RETREAT);
		set_Action = 0;
	}

	//setAlerts();

	switch ((int)action_Mode) {
	case AC_TESTING:
		updateTestingLogicSequence();
		break;
	case AC_PATROL:
		updatePatrolLogicSequence();
		break;
	case AC_SPOT:
		updateSpotLogicSequence();
		break;
	case AC_AIM:
		updateAimLogicSequence();
		break;
	case AC_SHOOT:
		updateShootLogicSequence();
		break;
	case AC_ATTACK:
		updateAttackLogicSequence();
		break;
	case AC_TAKECOVER:
		updateTakeCoverLogicSequence();
		break;

	case AC_RETREAT:
		updateRetreatLogicSequence();
		break;

	case AC_THROW_GRENADE:
		updateThrowGrenadeLogicSequence();
		break;
	
	case AC_DEAD:
		updateDeadLogicSequence();
		break;
	}

	theEnemyFound = FindEnemy();
	//movingSpeed = 1.0;
	//if (theEnemyFound)
	//	movingSpeed = 0.0;
}




void TSMSoldier::processRecording() {
}


void TSMSoldier::processPlayback() {

}



void TSMSoldier::showInfo() {
	PlayerPtr player = Game::getPlayer();
	Vec3 pos = player->getWorldPosition();
	Vec3 pos2 = node->getWorldPosition();
	cameraViewDistance = (pos2 - pos).length();
	objtext->setWorldRotation(player->getWorldRotation());
	setupText1(1);
	objtext->setEnabled(1);
	objtext->setText(dispaytext);
	Vec3 testPos = pos + (pos2 +Vec3(0.0,0.0, 2.0+cameraViewDistance*0.01) - pos) * 5.42 / cameraViewDistance;
	objtext->setWorldPosition(testPos);
	objtext->setFontSize(20);
	objtext->setTextColor(vec4_black);
	SynckerInterface::get()->SendTestMessage(objtext, String(dispaytext));

}

void TSMSoldier::setupText1(int in_func) {
	//dispaytext = ActionStr[(int)action_Mode]+":"+ ProfileStr[(int)mProfile];
	//dispaytext = ProfileStr[(int)mProfile];
	//dispaytext = String::format("%5.3f:%d", attack_start_timer, prev_attack_start_timer);
	dispaytext = String::format("%d", (int)id);
}

void TSMSoldier::getKeyboardInput() {
	updateAvatarInput();

	if (Input::isKeyDown(Input::KEY_F)) {
		mFreeze = !mFreeze;
	}

	/*if (App::getKeyState('t')) {
		App::setKeyState('t', 0);
		testMode = !testMode;
	}*/
	if (App::getKeyState('h')) {
		//App::setKeyState('t', 0);
		//testMode = !testMode;

	}
	if (App::getKeyState('j')) {
		//App::setKeyState('u', 0);
		//testMode = !testMode;

	}
	if (App::getKeyState('k')) {
		//App::setKeyState('o', 0);
		//testMode = !testMode;
	}

}


