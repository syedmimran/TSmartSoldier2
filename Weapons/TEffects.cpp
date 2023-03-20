#include "TEffects.h"

using namespace Unigine;
using namespace Math;

void TEffects::init() {
	effects_param = checked_ptr_cast<NodeDummy>(NodeReference::create("Nodes/Effects.node")->getReference());
	effects_param->setEnabled(true);
	soundPosition = Vec3_zero;
	PropertyParameterPtr param = effects_param->getProperty()->getParameterPtr("sound_file");
	int num = param->getArraySize();
	for (int j = 0; j < num; j++) {
		PropertyParameterPtr n = param->getChild(j);
		String s = n->getValueFile();
		soundfiles.append(n->getValueFile());
	}

	for (int i = 0; i < NUM_OF_SOUND; i++) {
		init_sound(soundfiles[0], sound_rifle[i], 0);
		sound_rifle[i]->stop();
		init_sound(soundfiles[1], sound_piring[i], 0);
		sound_piring[i]->stop();
		init_sound(soundfiles[2 + rand() % 4], sound_ricochet[i], 0);
		sound_ricochet[i]->stop();
	}

	NodePtr n = effects_param->findNode("BulletSparks");
	n->setEnabled(true);
	numOfBulletSpark = n->getNumChildren();
	bulletSparkTimer.resize(numOfBulletSpark);
	bulletSparks.resize(numOfBulletSpark);
	for (int j = 0; j < numOfBulletSpark; j++) {
		bulletSparks[j] = n->getChild(j);
		bulletSparks[j]->setEnabled(false);
		bulletSparkTimer[j] = 0;
	}
	n = effects_param->findNode("GunSparks");
	n->setEnabled(true);
	numOfGunSpark = n->getNumChildren();
	gunSparkTimer.resize(numOfGunSpark);
	gunSparks.resize(numOfGunSpark);
	for (int j = 0; j < numOfGunSpark; j++) {
		gunSparks[j] = n->getChild(j);
		gunSparks[j]->setEnabled(false);
		gunSparkTimer[j] = 0;
	}
	n = effects_param->findNode("BulletSplashes");
	n->setEnabled(true);
	numOfBulletSplash = n->getNumChildren();
	bulletSplashes.resize(numOfBulletSplash);
	bulletSplashTimer.resize(numOfBulletSplash);
	for (int j = 0; j < numOfBulletSplash; j++) {
		bulletSplashes[j] = n->getChild(j);
		bulletSplashes[j]->setEnabled(false);
		bulletSplashTimer[j] = 0;
	}

	rifleSoundIndex = 0;
	piringSoundIndex = 0;
	ricochetSoundIndex = 0;
	bulletSplashIndex = 0;
	bulletSparkIndex = 0;
	gunSparkIndex = 0;

	windDirection = 10.0;
	windSpeed = 2.0;
}

//-----------------------------------------------------------
void TEffects::update() {
	float s_volume = 1.0 / (0.1 + sqrt(distance));
	if (s_volume > 1.0)
		s_volume = 1.0;
	if (rifleOn) {
		rifleOn = false;
		sound_rifle[rifleSoundIndex]->setWorldPosition(soundPosition);
		sound_rifle[rifleSoundIndex]->setGain(s_volume);
		sound_rifle[rifleSoundIndex]->play();
		rifleSoundIndex++;
		rifleSoundIndex %= NUM_OF_SOUND;
	}
	if (piringOn) {
		piringOn = false;
		sound_rifle[piringSoundIndex]->setWorldPosition(soundPosition);
		sound_piring[piringSoundIndex]->setGain(s_volume);
		sound_piring[piringSoundIndex]->play();
		piringSoundIndex++;
		piringSoundIndex %= NUM_OF_SOUND;
	}
	if (ricochetOn) {
		ricochetOn = false;
		sound_rifle[ricochetSoundIndex]->setWorldPosition(soundPosition);
		sound_ricochet[ricochetSoundIndex]->setGain(s_volume);
		sound_ricochet[ricochetSoundIndex]->play();
		ricochetSoundIndex++;
		ricochetSoundIndex %= NUM_OF_SOUND;
	}
	for (int i = 0; i < numOfBulletSpark; i++) {
		if (bulletSparkTimer[i] > 0)
			bulletSparkTimer[i]--;
		else  if (bulletSparks[i]->isEnabled())
			bulletSparks[i]->setEnabled(false);
	}

	for (int i = 0; i < numOfBulletSplash; i++) {
		if (bulletSplashTimer[i] > 0)
			bulletSplashTimer[i]--;
		else if (bulletSplashes[i]->isEnabled())
			bulletSplashes[i]->setEnabled(false);
	}

	for (int i = 0; i < numOfGunSpark; i++) {
		if (gunSparkTimer[i] > 0)
			gunSparkTimer[i]--;
		else  if (gunSparks[i]->isEnabled())
			gunSparks[i]->setEnabled(false);
	}
	//for (int i = 0; i < numOfBulletSplash; i++) 
	//	lineDisplay(20 + i, 15, String::format("= %d %d", bulletSplashTimer[i], bulletSplash_param[i]->isEnabled()));
	
}


void TEffects::setGunSpark(Vec3 pos) {
	gunSparkTimer[gunSparkIndex] = 30;
	gunSparks[gunSparkIndex]->setWorldPosition(pos);
	gunSparks[gunSparkIndex]->setEnabled(true);
	gunSparkIndex++;
	gunSparkIndex %= numOfGunSpark;
}

void TEffects::setBulletSpark(Vec3 pos) {
	bulletSparkTimer[bulletSparkIndex] = 30;
	bulletSparks[bulletSparkIndex]->setWorldPosition(pos);
	bulletSparks[bulletSparkIndex]->setEnabled(true);
	bulletSparkIndex++;
	bulletSparkIndex %= numOfBulletSpark;
}

void TEffects::setBulletSplash(Vec3 pos) {
	bulletSplashTimer[bulletSplashIndex] = 100;
	bulletSplashes[bulletSplashIndex]->setWorldPosition(pos);
	setSplashDirection(bulletSplashes[bulletSplashIndex]->getChild(2));
	bulletSplashes[bulletSplashIndex]->setEnabled(true);
	bulletSplashIndex++;
	bulletSplashIndex %= numOfBulletSplash;
}

void TEffects::setSplashDirection(NodePtr node) {
	ObjectParticlesPtr smoke = checked_ptr_cast<ObjectParticles>(node);
	String s = smoke->getName();
	auto smokegravity = smoke->getGravityOverTimeModifier();
	if (smokegravity.get()->getMode() == ParticleModifier::MODE_CONSTANT) {
		smokegravity.get()->setConstant(vec3_zero);
	}
	auto modvec = smoke->getDirectionOverTimeModifier();
	
	float dir = windDirection;
	float speed = windSpeed;

	vec3 mdir = vec3_up;
	mdir.y = 1.0 * dir * speed;
	if (modvec.get()->getMode() == ParticleModifier::MODE_RANDOM_BETWEEN_TWO_CONSTANTS) {
		modvec.get()->setConstantMin(mdir);
		modvec.get()->setConstantMax(mdir);
	}

	smoke->setSpawnRate(4.0);
	smoke->setLife(20, 0.5);
}

void TEffects::init_sound(const char* file_name, SoundSourcePtr& sound_source, int is_loop) {
	if (FileSystem::isFileExist(file_name)) {
		sound_source = SoundSource::create(file_name);
		sound_source->setLoop(is_loop);
		sound_source->setOcclusion(0);
		sound_source->setMinDistance(0.0f);
		sound_source->setMaxDistance(2000.0f);
		sound_source->setGain(1.0);
		sound_source->setPlayOnEnable(true);
		sound_source->setRestartOnEnable(true);
		sound_source->setWorldPosition(soundPosition);
	}
}