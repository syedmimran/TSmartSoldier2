#pragma once
//#include"TCommon.h"
#include <UnigineWorld.h>
#include "Utils.h"


#define JUNGLE_SCENE 0
#define OBUA_SCENE   1
#define PALM_SCENE   2
#define BUNGLOW_SCENE   3

#define VR          0
#define SERVER      1
#define REMOTE_VIEW 2
#define AAR         3
#define EDITOR      4


using namespace Unigine;
using namespace Math;


typedef struct {
	float hiFlightTime, lowFlightTime;
	float hiAngle, lowAngle;
	bool valid;
} ProjectileDataType;


typedef struct _markerset {
	int numOfMarkerset;
	Vector<Vec3> pos;
	Vector<int> id;
} MarkerSets;

class NatNetParser;
class TNetwork;
class DatagramSocket;

class IsmatInterface {
public:
	IsmatInterface();
	~IsmatInterface();

	static IsmatInterface*get() {
		static IsmatInterface instance;
		return &instance;
	}
	void init();
	void shutdown();
	int getAppType() {
		return mApppType; 
	}

	void MapMarkerData(sFrameOfMocapData* data);
	void SetUpMarkerSets();
	void ClearUpMarkerSets();
	void sendToIOS(const char* msg, int msgsize);
	void sendToTablet(const char* msg, int msgsize);
	void sendData(OpcodeType opcode, void* data, int port);

	OptiTrackData getCurProjector();
	double getWorldHeight(float x, float y, float hrange);
	void getUpdateFromIOS(IOSData);
	bool mIOSNewData;
	bool checkIONewData();
	IOSData getIOSData();

	MarkerSets getMarkerSet() { return mMarkerSets; }
	Unigine::Map<int,Vec3> GunMarker;
	Vec3 getWeaponMarkerPos(int id);

	void initWeather();
	void shutdownWeather();

	void DisplayRain(bool disp);
	void DisplayFog(bool disp);
	void sentHitData(int, int, int);
	void setSelectedTerrain(int terrainNo);
	void setSelectedTerrain(Unigine::String name);
    ProjectileDataType calculateFlightTime(float nozzleVelocity, float dis, float difHeight);


private:
	NatNetParser* mNatNetParser;
	TNetwork* mNetwork;
	DatagramSocket* mSocket[2];
	DatagramSocket* mDatagramSocket;
	NodePtr raineffects;
	NodePtr fogeffects;
	NodePtr lightningeffects;
	NodePtr rainparticleseffects;
	NodePtr puddleseffects;
	OptiTrackData mProjector;
	IOSData mIOSData;
	WorldIntersectionPtr wIntersection;
	MarkerSets mMarkerSets;
	int mApppType = SERVER;
	bool mMarkerNewData = false;
};

