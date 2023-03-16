#include "IsmatInterface.h"
#include <UnigineEditor.h>
#include <UnigineConfig.h>

#include <plugins/UnigineSyncker.h>

#include "TNetwork.h"
#include "IsmatUtils.h"
#include "WeaponNetwork.h"
#include "WeaponTriggerNetwork.h"
#include "Mortar80TriggerNetwork.h"

#include "DBIsmat.h"
#include "TWeaponManager.h"
#include "TCameraManager.h"

using namespace Unigine;
using namespace Math;
using namespace Plugins;
extern int gWeaponControlType;

IsmatInterface::IsmatInterface() {
}


IsmatInterface::~IsmatInterface() {
}

//---------------------------------------------------------------------
void IsmatInterface::init() {

	//init = false;
	wIntersection = WorldIntersection::create();

	ServerToClientPlayersPacket serverClientPacket;
	memset(&serverClientPacket, 0, sizeof(ServerToClientPlayersPacket));
	serverClientPacket.Avatar.Rot = quat_identity;
	
	String LocalIpAddress = "192.168.1.30";// Config::get()->getString("LocalIpAddress");
	String ServerIpAddress = "192.168.1.1";// Config::get()->getString("MotiveIpAddress");
	mNatNetParser = new NatNetParser(LocalIpAddress, ServerIpAddress);

	App::setBackgroundUpdate(true);

	mNetwork = new TNetwork();
	mNetwork->init();
	mNetwork->run();

	WeaponNetwork::get()->init();
	WeaponNetwork::get()->run();
	WeaponTriggerNetwork::get()->init();
	WeaponTriggerNetwork::get()->run();
	Mortar80TriggerNetwork::get()->init();
	Mortar80TriggerNetwork::get()->run();
	DBIsmat::get()->init();
	mSocket[0] = new DatagramSocket(4000, "192.168.1.138");
	mSocket[1] = new DatagramSocket(4000, "192.168.1.139");
	mDatagramSocket = new DatagramSocket(2000);
}

void IsmatInterface::shutdown() {
	shutdownWeather();
	delete mNatNetParser;

	// Write here code to be called on engine shutdown.

	mNetwork->shutdown();
	delete mNetwork;
	WeaponNetwork::get()->shutdown();
	WeaponTriggerNetwork::get()->shutdown();
}



void IsmatInterface::sendToIOS(const char* msg, int msgsize) {
	mSocket[0]->send(msg, msgsize);
}

void IsmatInterface::sendToTablet(const char* msg, int msgsize) {
	mSocket[1]->send(msg, msgsize);
}

void IsmatInterface::sendData(OpcodeType opcode, void* data, int port) {
	char outGoingData[2048];
	int pos = 0;
	memcpy(outGoingData, &opcode, sizeof(OpcodeType));
	pos += sizeof(OpcodeType);
	memcpy(outGoingData + pos, data, opcode.OpcodeSize);
	pos += opcode.OpcodeSize;
	mDatagramSocket->sendTo(outGoingData, pos, "192.168.1.255", port);
}

void IsmatInterface::MapMarkerData(sFrameOfMocapData* data) {
	if (gWeaponControlType == USE_MOUSE)
		return;
	int numofMarkerSet = data->nLabeledMarkers;
	//numofMarkerSet = 0;
	if (numofMarkerSet > 0 && !mMarkerNewData) {
		mMarkerNewData = true;
	}
	else {
		mMarkerNewData = false;
		mMarkerSets.numOfMarkerset = numofMarkerSet;
		return;
	}
	//sMarker mdata;
	if (!mMarkerNewData)
		return;
	mMarkerSets.numOfMarkerset = numofMarkerSet;
	mMarkerSets.pos.clear();
	mMarkerSets.id.clear();
	Vec3 tmp;
	//int mm;
	for (int i = 0; i < numofMarkerSet; i++) {
		tmp.x = -data->LabeledMarkers[i].z;
		tmp.y = data->LabeledMarkers[i].y;
		tmp.z = data->LabeledMarkers[i].x;

		/*	tmp.x = -data->LabeledMarkers[i].x;
			tmp.y =  data->LabeledMarkers[i].z;
			tmp.z =  data->LabeledMarkers[i].y;*/

		mMarkerSets.pos.append(tmp);
		int id = data->LabeledMarkers[i].ID;
		if (id > 2632)
			continue;
		mMarkerSets.id.append(id);
		GunMarker[id] = tmp;
	}
	//mMarkerNewData = false;
}

Vec3 IsmatInterface::getWeaponMarkerPos(int id) {
	return GunMarker[id];
}

void IsmatInterface::SetUpMarkerSets() {
	mMarkerSets.numOfMarkerset = 1;
	mMarkerSets.pos.clear();
	Vec3 tmp;
	for (int i = 0; i < mMarkerSets.numOfMarkerset; i++) {
		tmp.x = 2.0;
		tmp.y = 1.0;
		tmp.z = -MAIN_CAMERA_DISTANCE;
		mMarkerSets.pos.append(tmp);
	}
}

void IsmatInterface::ClearUpMarkerSets() {
	mMarkerSets.numOfMarkerset = 0;
}


//--------------------------------------------------------------
OptiTrackData IsmatInterface::getCurProjector() {
	return mProjector;
}

IOSData IsmatInterface::getIOSData() {
	return mIOSData;
}

//----------------------------------------------------------------------------------
double IsmatInterface::getWorldHeight(float x, float y, float hrange) {
	Vec3 p0 = Vec3(x, y,  hrange);
	Vec3 p1 = Vec3(x, y, -hrange);
	//Visualizer::renderLine3D(p0, p1, vec4::BLUE);

	ObjectPtr object = World::getIntersection(p0, p1, 1 << 28, wIntersection);
	if (!object)
		return  0.0;
	//String name = object->getName();
	//int k = 0;
	//if (!name.contains("OLT_PULADA") && !name.contains("OLT_Forest")) {

	//	k = object->getIntersectionMask(0);
	//}
	return  wIntersection->getPoint().z;
}

//----------------------------------------------------------------------
void IsmatInterface::getUpdateFromIOS(IOSData val) {
	mIOSData = val;
	mIOSNewData = true;
}
////----------------------------------------------------------------------
bool IsmatInterface::checkIONewData() {
	bool result = false;
	if (mIOSNewData)
		result = mIOSNewData;

	mIOSNewData = false;

	return result;
}  


void IsmatInterface::initWeather() {
	NodeReferencePtr noderef = NodeReference::create("weather/fx/volume_fog/nodes/fog_dummy.node");
	if (fogeffects.isNull())
		fogeffects = checked_ptr_cast<NodeDummy>(noderef->getReference());

	noderef = NodeReference::create("weather/fx/rain_particles/nodes/rain_dummy.node");
	if (raineffects.isNull())
		raineffects = checked_ptr_cast<NodeDummy>(noderef->getReference());

	noderef = NodeReference::create("weather/fx/rain_particles/nodes/rain_particles_group.node");
	if (rainparticleseffects.isNull())
		rainparticleseffects = checked_ptr_cast<NodeDummy>(noderef->getReference());

	fogeffects->setEnabled(false);
	raineffects->setEnabled(false);
	rainparticleseffects->setEnabled(false);
}

void IsmatInterface::DisplayFog(bool disp) {
	fogeffects->setEnabled(disp);
}

void IsmatInterface::DisplayRain(bool disp) {
	//raineffects->setEnabled(disp);
	rainparticleseffects->setEnabled(disp);
}

void IsmatInterface::shutdownWeather() {
	if (fogeffects)
		fogeffects.deleteLater();
	if (raineffects)
		raineffects.deleteLater();
	if (rainparticleseffects)
		rainparticleseffects.deleteLater();

}

void IsmatInterface::sentHitData(int target_type, int target_id, int killer_id) {
	GenericHitDataType gHitData;
	OpcodeType opcode;
	char outGoingData[16];
	gHitData.TargetType = target_type;
	gHitData.TargetId = target_id;
	gHitData.WeaponId = killer_id;
	opcode.Id = Opcode::GenericHitCmd;

	memcpy(outGoingData, &gHitData, sizeof(GenericHitDataType));
	opcode.OpcodeSize = sizeof(GenericHitDataType);
	sendData(opcode, outGoingData, 4050);
}

void IsmatInterface::setSelectedTerrain(int terrainNo) {
	String str;
	int TerrainNo = terrainNo;

	switch (TerrainNo) {
	case GEMAS_0:
		str = "Worlds/Gemas_0";
		break;
	case FOX_HOLE:
		str = "Worlds/Scene_Foxhole";
		break;
	case GEMAS_UNIGINE:
		//str = "Worlds/TestWorld";
		str = "Worlds/Gemas_Unigine";
		break;
		//case JUNGLE_RANGE:
		///	str = "Worlds/Forest";
		//	break;
	case ASAHAN:
		str = "Worlds/Asahan";
		//str = "Worlds/Scene_Startup";
		break;
	case SHIP:
		str = "Worlds/Ship";
		//str = "Worlds/Scene_Startup";
		break;
	}

	auto syncker_manager = Syncker::Manager::get();

	if (syncker_manager) {
		if (syncker_manager->isMasterInitialized()) // only master load the database
			syncker_manager->getMaster()->loadWorld(str);
	}
	else {
		Console::run("world_load " + str);
	}
}

void IsmatInterface::setSelectedTerrain(String name)
{
	auto syncker_manager = Syncker::Manager::get();

	if (syncker_manager) {
		if (syncker_manager->isMasterInitialized()) // only master load the database
			syncker_manager->getMaster()->loadWorld(name);
	}
	else {
		Console::run("world_load " + name);
	}
}

ProjectileDataType IsmatInterface::calculateFlightTime(float nozzleVelocity, float dis, float difHeight) {
	float g = -9.81;
	float a = 2 * g * dis * dis;
	float b = 2 * dis * nozzleVelocity * nozzleVelocity;
	float c = g * dis * dis - 2 * difHeight * nozzleVelocity * nozzleVelocity;
	float z = b * b - 2 * a * c;
	ProjectileDataType pd;
	pd.valid = false;
	if (z < 0.0)
		return pd;
	z = (float)sqrt(z);
	float tanHi = (-b + z) / a;
	float tanLow  = (-b - z) / a;
	pd.hiAngle = Math::atan(tanHi);
	pd.hiFlightTime  = dis / (nozzleVelocity * Math::cos(pd.hiAngle));
	pd.lowAngle = Math::atan(tanLow);
	pd.lowFlightTime = dis / (nozzleVelocity * Math::cos(pd.lowAngle));
	pd.hiAngle  *= Consts::RAD2DEG;
	pd.lowAngle *= Consts::RAD2DEG;

	pd.valid = true;
	return pd;
}

