#include "SynckerInterface.h"
#include <plugins/UnigineSyncker.h>

using namespace Plugins;


void SynckerInterface::init() {		
	manager = Syncker::Manager::get();

	//if (manager) {
	//	manager->getMaster()->setSyncViewOffset(false);
	//	manager->getMaster()->setSyncRender(true);
	//}

}


void SynckerInterface::shutdown() {
}

void SynckerInterface::SendTestMessage(ObjectTextPtr objecttext, String msg) {
	if (manager) {
		BlobPtr message = Blob::create();
		message->writeInt(objecttext->getID());
		message->writeString(msg);
		Syncker::Manager::get()->getMaster()->sendMessage("ObjectTextMessage", message);
	}
}



void SynckerInterface::update() {
	if (App::getKeyState(App::KEY_RETURN)) {
		App::setKeyState(App::KEY_RETURN, 0);
	}
}


void SynckerInterface::addRecSyncNode(NodePtr n) {
	//auto name = n->getName();
	if (!manager)
		return;
	manager->getMaster()->addSyncNode(n, Syncker::Master::SYNC_MASK::NODE_FLAGS | Syncker::Master::SYNC_MASK::TRANSFORM | Syncker::Master::SYNC_MASK::DERIVED | Syncker::Master::SYNC_MASK::OBJECT | Syncker::Master::SYNC_MASK::OBJECT_SURFACE);
	//manager->getMaster()->addSyncNode(n, 255);
	for (int i = 0; i < n->getNumChildren(); i++)
	   addRecSyncNode(n->getChild(i));
}

void SynckerInterface::deleteRecSyncNode(NodePtr n) {
	//auto name = n->getName();
	if (manager) {
		for (int i = 0; i < n->getNumChildren(); i++)
			deleteRecSyncNode(n->getChild(i));
		manager->getMaster()->deleteNode(n);
	}
	else
		n->deleteLater();
}

NodePtr SynckerInterface::LoadNode(String nodeStr) {
	NodePtr n;
	if (manager && 	String::compare(nodeStr, "Nodes/Effects.node") !=0  ) {
		n = checked_ptr_cast<NodeDummy>(manager->getMaster()->loadNode(nodeStr));
		addRecSyncNode(n);
	}
	else 
		n = checked_ptr_cast<NodeDummy>(NodeReference::create(nodeStr)->getReference());
	return n;
}

ObjectPtr SynckerInterface::LoadObjectNode(String nodeStr) {
	ObjectPtr obj;
	if (manager) {
		obj = checked_ptr_cast<Object>(manager->getMaster()->loadNode(nodeStr));
		addRecSyncNode(obj);
	}
	else 
		obj = checked_ptr_cast<Object>(NodeReference::create(nodeStr)->getReference());
	
	return obj;
}


void SynckerInterface::CreateNode(NodePtr node) {
	if (manager) 
		manager->getMaster()->createNode(node, 255);
}


void SynckerInterface::CreateMobi() {
	PlayerDummyPtr mobi = PlayerDummy::create();
	mobi->setProperty("bino");
	CreateNode(mobi);
	if (manager) 
		manager->getMaster()->setSlavePlayer("mobi", mobi);
}

