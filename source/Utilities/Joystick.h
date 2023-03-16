#pragma once

#include <UnigineControls.h>

using namespace Unigine;
//using namespace Math;

class TJoystickControl {
public:
	TJoystickControl();
	~TJoystickControl();
	void Update();
	bool getTrigger() { return trigger; }
	bool getPov(int i) { return Pov[i]; }
	bool getButton(int i) { return button[i]; }
	float getjValue(int i) { return jValue[i]; }

	static TJoystickControl* get() {
		static TJoystickControl instance;
		return &instance;
	}

private:
	Unigine::ControlsJoystickPtr Joytick;
	bool Pov[4];
	bool trigger;
	bool button[12];
	bool CreateOk;
	int NumOfAxis;
	float jValue[4];

	bool PovX[4];
	int NumX[4];
	float jValueX[4];
	bool isPress = false;

};




