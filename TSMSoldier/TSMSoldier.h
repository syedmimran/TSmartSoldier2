#pragma once

#include <UnigineComponentSystem.h>
#include <UnigineEditor.h>
#include <UnigineGame.h>

using namespace Unigine;
using namespace Math;

#define PF_TPOSE           0
#define PF_STANDING1       1
#define PF_STANDING2       2
#define PF_CROUCHING       3
#define PF_PRONNING        4
#define PF_DYING1          5
#define PF_DYING2          6
#define PF_DYING3          7
#define PF_NUM_OF_PROFILES 8

#define AC_TESTING		   0
#define AC_PATROL	       1
#define AC_SPOT	           2
#define AC_AIM	    	   3
#define AC_SHOOT           4
#define AC_ATTACK          5
#define AC_TAKECOVER       6
#define AC_RETREAT         7
#define AC_THROW_GRENADE   8
#define AC_DEAD            9
#define AC_NUM_OF_ACTIONS 10

#define ROT_R_ARM     0
#define ROT_R_ELBOW   1
#define ROT_R_HAND    2

#define ROT_L_ARM     3
#define ROT_L_ELBOW   4
#define ROT_L_HAND    5
#define NO_OF_ROTS    6


#define AIMMING     0
#define RELOAD      1
#define RELOAD2     2


#define DEAD_TYPE         0
#define STANDING_TYPE     1
#define CROUCHING_TYPE    2
#define PRONNING_TYPE     3

#define NUM_OF_BONES     56

#define SM_BLACK_SOLDIER 0
#define SM_GREEN_SOLDIER 1

#define GN_MALE   0
#define GN_FEMALE 1


enum SMS_Bones {
	SMS_Hips   = 0,
	SMS_Spine  = 1,
	SMS_Spine1 = 2,
	SMS_Spine2 = 3,
	SMS_Neck   = 4,
	//SMS_Neck1 = 5,
	SMS_Head = 6,

	//SMS_REye = 7,
	//SMS_LEye = 8,
	//SMS_Jaw  = 9,

	SMS_LeftShoulder = 10,
	SMS_LeftArm      = 11,
	SMS_LeftElbow    = 12,
	SMS_LeftHand     = 13,

	SMS_LeftThumb1  = 14,
	SMS_LeftThumb2  = 15,
	SMS_LeftThumb3  = 16,
	SMS_LeftIndex1  = 17,
	SMS_LeftIndex2  = 18,
	SMS_LeftIndex3  = 19,
	SMS_LeftMiddle1 = 20,
	SMS_LeftMiddle2 = 21,
	SMS_LeftMiddle3 = 22,
	SMS_LeftRing1   = 23,
	SMS_LeftRing2   = 24,
	SMS_LeftRing3   = 25,
	SMS_LeftPinky1  = 26,
	SMS_LeftPinky2  = 27,
	SMS_LeftPinky3  = 28,

	SMS_RightShoulder = 29,
	SMS_RightArm      = 30,
	SMS_RightElbow    = 31,
	SMS_RightHand     = 32,

	SMS_RightThumb1  = 33,
	SMS_RightThumb2  = 34,
	SMS_RightThumb3  = 35,
	SMS_RightIndex1  = 36,
	SMS_RightIndex2  = 37,
	SMS_RightIndex3  = 38,
	SMS_RightMiddle1 = 39,
	SMS_RightMiddle2 = 40,
	SMS_RightMiddle3 = 41,
	SMS_RightRing1   = 42,
	SMS_RightRing2   = 43,
	SMS_RightRing3   = 44,
	SMS_RightPinky1  = 45,
	SMS_RightPinky2  = 46,
	SMS_RightPinky3  = 47,

	SMS_LeftHip  = 48,
	SMS_LeftKnee = 49,
	SMS_LeftFoot = 50,
	SMS_LeftToe  = 51,

	SMS_RightHip  = 52,
	SMS_RightKnee = 53,
	SMS_RightFoot = 54,
	SMS_RightToe  = 55

};



typedef struct {
	vec3 Pos;
	quat Rot;
	vec3 spineRotV;
	vec3 headRotV;
	vec3 aimingDirection;
	char mProfile;
	char mPrevProfile;
	char mActionMode;
	float tMovingSpeed;
	unsigned short Attribute;

} SendDataType;

typedef struct {
	vec3 pos;
	char profile;
	char action;
	float time;
	short attribute;
} WayPointType;

extern vec3 jointRot2[NO_OF_ROTS][PF_NUM_OF_PROFILES][4];
extern vec3 boneRot2[PF_NUM_OF_PROFILES][NUM_OF_BONES];
extern int  gBoneId[];
extern int  gBoneRow[];
extern int  gNumOfActiveBones;
extern int gNumOfFingerBones;
extern String gBoneNames[];

extern String ActionStr[];
extern String ProfileStr[];
bool extern gTesting;

class TSoldierState;

class TSMSoldier : public ComponentBase {
public:
	COMPONENT(TSMSoldier, ComponentBase);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);
	PROP_NAME("TSMSoldier");
	PROP_PARAM(Int, human_type, 0);
	PROP_PARAM(Int, group_type, 0);
	PROP_PARAM(Int, behaviour_type, 0);
	PROP_PARAM(Int, gender, 0);
	PROP_PARAM(Int, id, 0);
	PROP_PARAM(Node, rifleRef, 0);
	PROP_PARAM(Int, mCurAction)
	PROP_PARAM(Int, set_Action, 0);
	PROP_PARAM(Int, isDead, 0);
	PROP_PARAM(Int, smartLevel, 0);
	PROP_PARAM(String, Name, 0);
	PROP_PARAM(Vec3, initPos, 0);
	PROP_PARAM(Node, patrolRoute, 0);
	PROP_PARAM(Int, attack_id, 0);
	PROP_PARAM(Int, groupInstruction, 0);
	PROP_PARAM(Float, hitDistance, 0.0f);
	PROP_PARAM(Int, killer_id, -1);
	PROP_PARAM(Int, numOfAttackPersons, 0);
	PROP_PARAM(Int, attackSeparation, 20);
	PROP_PARAM(Int, showText, 0);
	PROP_PARAM(Int, playback_mode, 0);
	PROP_PARAM(Node, platform, 0);


protected:
	// world main loop
	void init();
	void update();
	void shutdown();

public:
	void reRoutePatrol();

private:

	Vector<WayPointType> wayPointList;
	Vector<int> newProfileQueue;
	String Ai_Name;
	float speedVariation = 0.0;
	int action_Mode = 0;
	int prev_action_Mode = -1;
	bool testMode = false;
	String dispaytext;
	ObjectTextPtr objtext;
	float SlowDown;
	//Vector<AiRoute_Point*> PatrolRoutePoints;
	float cameraViewDistance = 10.0;
	int wpIndex = 0;
	//AiRoute_Point route4[4];
	bool  inView = false;
	quat  mCurWorldRot;
	Vec3  mCurWorldPos;
	quat  mCurLocalRot;
	Vec3  mCurLocalPos;
	Vec3 JoyPos;
	Vec3 retreatPointLeft = Vec3_zero;
	Vec3 retreatPointRight = Vec3_zero;
	Vec3 retreatPointMiddle = Vec3_zero;

	Vec3 mPatrolInitPos;
	bool inTransition = false;
	float cValue = 0.0;
	float pValue = 0.0;
	float mLeftFingerAngle  = 0.0;
	float mRightFingerAngle = 0.0;
	float cLeftFingerAngle = 0.0;
	float cRightFingerAngle = 0.0;

	bool inAimingOn = false;
	bool inAimingOff = false;
	bool AimingInTransition = false;

	float caOnValue = 0.0;
	float paOnValue = 0.0;
	float caOffValue = 0.0;
	float paOffValue = 0.0;

	bool inReloadOn = false;
	float crOnValue = 0.0;
	float prOnValue = 0.0;
	bool inReloadOff = false;
	float crOffValue = 0.0;
	float prOffValue = 0.0;
	bool ReloadInTransition = false;
	float mAnimationSpeed = 30.0;

	bool inReload2On = false;
	float cr2OnValue = 0.0;
	float pr2OnValue = 0.0;
	bool inReload2Off = false;
	float cr2OffValue = 0.0;
	float pr2OffValue = 0.0;
	bool Reload2InTransition = false;
	quat rifleRot;
	int numOfFingerBonesIncluded = 0;

	float mTargetHeading;
	float mCurHeading = 0.0;
	int   mBulletCount;
	float mAvoidAngle;
	float mHeadingDiff;
	bool drawline;
	Vec3 RearWayPoint = Vec3_zero;
	float mShootingErrorThetha = 0.0;

	PropertyParameterPtr WeaponTriggerProp;
	PropertyParameterPtr WeaponAttackModeProp;
	PropertyParameterPtr WeaponHitCounterProp;
	ObjectMeshSkinnedPtr mAvatar;
	ObjectMeshStaticPtr  mRifle;
	ObjectPtr mMarker;

	void getKeyboardInput();
	void showInfo();
	void setupText1(int in_func);

	void displayInfo();
	bool FindEnemy();
	NodePtr FindHumanEnemy();
	NodePtr FindCameraPlayer();
	float avoidObstacle();
	float findPath();
	float getDirection(Vec3 from, Vec3 to);
	void setupObstacleSensors();

	bool CheckThreat();
	int  ObstacleIndex;

	bool dontAvoidObstacle;
	float mTerrainHeight;
	float mTerrainLocalHeight = 0.0;
	Vec3 targetMove = Vec3_zero;
	float mTurnRate = 0;
	Vec3 mTargetPos = Vec3_zero;
	float mTargetDistance = 0.0;
	bool mTargetArrived = true;
	bool mTargetArrivedDone = false;

	int   mMagazineCount;
	int   magazineCapacity;
	float mCurSpeed;
	float mCurStateTimer;
	float dead_timer = 0.0;
	float nobullet_timer = 0.0;
	float retreat_timer = 0.0;
	float aim_timer = 0.0;

	NodePtr theEnemy;
	bool theEnemyFound = false;
	Vec3 theEnemyPos;
	PlayerPtr eyeSpectator;
	PlayerPtr bodySpectator;
	WorldIntersectionPtr wIntersection;

	NodePtr HeadBone;
	NodePtr HeadBonePoint;
	Vec3 HeadBoneTrajectory;
	NodePtr ChestBone;
	NodePtr ChestBonePoint[5];
	Vec3 outerChestBoneTrajectory[2];
	Vec3 innerChestBoneTrajectory[2];

	float forwardSpeed = 0.0;

	int pointAttackIndex = 0;
	int pointPatrolIndex = 0;
	int numOfWaypoints = 0;


	void processRecording();
	void processPlayback();
	

	void updateTestingLogicSequence();
	void updatePatrolLogicSequence();
	void updatePatrolLogicSequence1();
	void updatePatrolLogicSequence2();
	void updatePatrolLogicSequence3();
	void updatePatrolLogicSequence4();
	void updatePatrolLogicSequence5();
	void updateSpotLogicSequence();
	void updateAimLogicSequence();
	void updateShootLogicSequence();
	void updateRetreatLogicSequence();
	void updateThrowGrenadeLogicSequence();
	void updateDeadLogicSequence();
	void updateAttackLogicSequence();
	void updateTakeCoverLogicSequence();
	void updateSmartLevel();

	void setupPointOfAttack();

	void turnXTo_G();

	bool checkPatrolLogicSequenceIsOk(int next_nextSequence);
	int  getEnemyCurAction();
	void setWeaponAimming(bool aim);
	bool setThisTeamIdle(int teamtype);


	void InitEyePoint();
	void InitBonesTransform();
	void InitActiveBones();
	void InitBonesRot();
	void InitBonesAndAll();

	void getInputRot();

	Vec3 mRot1 = Vec3_zero;
	Vec3 mRot2 = Vec3_zero;
	Vec3 mRot3 = Vec3_zero;


	Vec3 textPos;
	int mPatrolLogicSequence;
	int mPatrolFunctionSequence;

	bool mFirstAction[AC_NUM_OF_ACTIONS];
	bool isShooting = false;

	Vec3 AttackStartPoint;
	bool newAttackModeDone = false;
	bool startTheAttack = false;
	bool arrivedAtStartPoint = false;
	float attack_start_timer = 0.0;
	float wait_attack_timer = 0.0;
	int prev_attack_start_timer = 0;
	float retreat_expired_timer = 0.0;
	float takeCover_timer = 0.0;
	float takeCover_expired_timer = 0.0;
	float takeCover_start_timer = 0.0;
	float attack_expired_timer = 0.0;

	float aim_expired_timer = 0.0;
	float currFrame = 0.0;
	float initFrameTime = 0.0;
	float stopFrameTime = 0.0;
	float mFrameTime;

	float mTime;
	float mEnemyDistance;
	bool find_the_enemy = false;
	ObjectMeshStaticPtr padDoorOpener;

	Vector <ObjectPtr>* myEnemies;
	
	NodePtr ForwardPoint;
	NodePtr CentrePoint;
	Vec3 PathForwardPoint;
	NodePtr RearPoint[3];
	Vec3 PathRearPoint[3];
	float mViewDistance = 20.0;
	float mShootingDistance = 10.0;
	float mViewAngle = 20.0;
	float mShootingError = 0.5;

	float sinValue = 0.0;
	float sinValueTrigger = 0.0;
	float triggerCycle = 0.0;
	float s_05, s_1, s_2;
	float sp_1, sp_2;
	float x10;
	float x20;
	float x30;
	float x40;
	float x50;
	float x75;
	float x125;
	float s1_d = 0.0;
	float stepAngle;
	float stepAngle_off;
	int oddX = 0;
	float sinValueAlertZ = 0.0;
	float sinValueAlertX = 0.0;
	float sinValueDead = 0.0;


	Vector<int>  BoneId;
	Vector<quat> initBoneRot;
	Vector<vec3> initBonePos;
	Vector<vec3> initScale;

	int numOfBones = 0;
	NodePtr eyePoint;

	float nodAngle = 0.0;
	quat headRot = quat_identity;
	vec3 headRotV = vec3_zero;
	vec3 spineRotV = vec3_zero;
	quat spineRot = quat_identity;
	float rotateRate = 0.0;

	Vec3 movingInc = Vec3_zero;
	float tMovingSpeed = 0.0;
	float mMovingSpeed = 0.0;
	int prevSmartLevel = -1;

	float forwardMove = 0;
	bool move_status = true;
	float tempMovingSpeed = 0.0;

	vec3 tPos;
	quat tRot;

	quat jointRot[NO_OF_ROTS][PF_NUM_OF_PROFILES][4];
	vec3 jointRot1[NO_OF_ROTS][PF_NUM_OF_PROFILES][4];
	vec3 boneRot[NUM_OF_BONES];

	int mProfile = PF_TPOSE;
	int mPrevProfile = PF_PRONNING;
	int  nextProfile = 0;
	char tempProfile = PF_PRONNING;
	bool deadTransition = false;
	bool shootingOk = false;

	float boneHeight;
	bool jsButtonPress[12];
	bool rifleAimStatus = false;
	bool reloadStatus = false;
	bool reload2Status = false;
	bool triggerPull = false;

	float reloadTimer = 0.0;
	float reloadWaitTime = 0.5;
	float transitionTimer = 5.0;
	bool reloadWait = false;
	int reloadSeq = 0;

	float modeTimer[AC_NUM_OF_ACTIONS];
	float shootTimer2 = 0.0;
	vec3  aimingDirection = vec3_zero;
	bool  alertOn = false;
	Vec3  mAttackPoint;
	bool weaponOnTarget = false;
	bool find_the_path = false;
	bool avoid_the_obstacles = false;
	bool mFreeze = false;



	void setNumOfMagazines(int num) {
		mMagazineCount = num;
	}
	void setMagazineCapacity(int num) {
		magazineCapacity = num;
	}


	void setFindEnemy(bool on) {
		find_the_enemy = on;
	}
	void setFindPath(bool on) {
		find_the_path = on;
	}
	void setAvoidObstacle(bool on) {
		avoid_the_obstacles = on;
	}

	void setWeaponOnTarget(bool on) {
		weaponOnTarget = on;
	}

	void setMovingSpeed(float speed) {
		tMovingSpeed = speed;
    }

	void setupRetreatPoint();

	void changeAction(int newAction);
	void setTurnRate(float rate) {
		mTurnRate = rate;
	}
	void setTargetPos(Vec3 pos) {
		mTargetPos = pos;
	}

	void setMoveStatus(bool status) {
		move_status = status;
	}
	void setSmartLevel(int sl) {
		smartLevel = sl;
	}	

	void changeProfile(int newProfile);
	void changeProfile(int newProfile, float speed);
	void showMarker(Vec3 pos, int id);
	void disposeMe() {
		shutdown();
	}


	void setProfileStanding1(int profile, int bone_num);
	void setProfileStanding2(int profile, int bone_num);
	void setProfileCrouching(int profile, int bone_num);
	void setProfilePronning(int profile, int bone_num);
	void setProfileDying(int profile, int bone_num);
	void setFingers(int bone_num);

	void resetBones();
	void updateBones();
	
	void updateAvatarRot();
	void updatePosition();
	void updateShooting();
	void updateTransition();
	void updateWeaponOnTarget();
	void updateAvatarInput();
	void updateAction();
	void updateMovement();
	void updateFingerAngles();
	void updateMovingSpeed();
	void updateAnimation(int bone_num);

	void setHeadRotation(Vec3 rot);
	void setSpineRotation(Vec3 rot);
	void setSinWaves();
	void setSpineRotation(int bone_num);
	void setHeadRotation(int bone_num);
	void setUp(float& x, float target, float rate);
	void setRifle(int profile,bool aiming);
	void setReload(int profile,bool reload);
	void setReload2(int profile,bool reload2);
	void setAimming(int profile, int bone_num);
	void setReloading(int profile, int bone_num);
	void setReloading2(int profile, int bone_num);
	void setShooting(int bone_num);
	void generateSinWavesStanding1();
	void generateSinWavesStanding2();
	void generateSinWavesCrouching();
	void generateSinWavesPronning();
	void generateSinWavesDying();
	void processClient();
	void processClientTest();
	void sendData();
	void sendDataTest();

	void setAlertSpine(bool on, float speed);
	void setAlertHead(bool on, float speed);
	void setAlerts();
	int  getProfileType(int profile);

	void InitAnimFile();
	void playAnimFile();

};
