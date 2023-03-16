#pragma once
#include <Unigine.h>
#include <UnigineObjects.h>
#include <plugins/UnigineSyncker.h>

using namespace Unigine;
using namespace Math;
using namespace Plugins;


class SynckerInterface {
public:
	SynckerInterface() {}
	~SynckerInterface() {}

	static SynckerInterface* get() {
		static SynckerInterface instance;
		return &instance;
	}
	bool isSyncker() {
		return manager != nullptr;
	}

	void init();
	void update();
	void shutdown();
	void deleteRecSyncNode(NodePtr node);
	//void createRecSyncNode(NodePtr node);
	void CreateNode(NodePtr node);
	NodePtr LoadNode(String nodeStr);
	ObjectPtr LoadObjectNode(String nodeStr);
	void SendTestMessage(ObjectTextPtr objecttext, String msg);
	void addRecSyncNode(NodePtr node);
	void CreateMobi();

private:

	Syncker::Manager* manager = nullptr;
	

};



