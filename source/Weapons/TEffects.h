#pragma once

#include <Unigine.h>

#define NUM_OF_SOUND  8

using namespace Unigine;
using namespace Math;

class TEffects {
public:
	TEffects() {}
	~TEffects() {}
	static TEffects* get() {
		static TEffects instance;
		return &instance;
	}

	void setDistance(float dis) {
		distance = dis;
	}
	void setRifleOn(Vec3 pos, bool shot) {
		rifleOn = shot;
		soundPosition = pos;
	}
	void setPiringOn(Vec3 pos, bool shot) {
		piringOn = shot;
		soundPosition = pos;
	}
	void setRicochetOn(Vec3 pos, bool shot) {
		ricochetOn = shot;
		soundPosition = pos;
	}
	void setBulletSpark(Vec3 pos);
	void setBulletSplash(Vec3 pos);
	void setGunSpark(Vec3 pos);
	void setSplashDirection(NodePtr node);
	void setWindSpeed(float speed) {
		windSpeed = speed;
	}
	void setWindDirection(float dir) {
		windDirection = dir;
	}
	void init();
	void update();


private:
	NodePtr effects_param;
	Vector <String> soundfiles;
	Vector <NodePtr> bulletSparks;
	Vector <NodePtr> gunSparks;
	Vector <NodePtr> bulletSplashes;
	bool rifleOn = false;
	bool piringOn = false;
	bool ricochetOn = false;
	float distance = 200.0;

	void init_sound(const char* file_name, Unigine::SoundSourcePtr& sound_source, int is_loop = false);
	SoundSourcePtr sound_rifle[NUM_OF_SOUND];
	SoundSourcePtr sound_piring[NUM_OF_SOUND];
	SoundSourcePtr sound_ricochet[NUM_OF_SOUND];
	Vector <int> bulletSparkTimer;
	Vector <int> bulletSplashTimer;
	Vector <int> gunSparkTimer;
	Vec3 soundPosition;

	int numOfBulletSpark;
	int numOfBulletSplash;
	int numOfGunSpark;

	int rifleSoundIndex;
	int piringSoundIndex;
	int ricochetSoundIndex;
	int bulletSparkIndex;
	int gunSparkIndex;
	int bulletSplashIndex;
	float windDirection;
	float windSpeed;
};

