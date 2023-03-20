#include "Joystick.h"
#include "utils.h"

int gJosytickMode = 0;


TJoystickControl::TJoystickControl() {
	Joytick = Unigine::ControlsJoystick::create(0);
	CreateOk = false;
	if (Joytick)
		CreateOk = true;
	NumOfAxis = Joytick->getNumAxes();
	NumX[0] = NumOfAxis;
	NumX[1] = Joytick->getNumAxes();
	for (int i=0; i<4; i++)
	  Pov[i] =false;
	trigger = false;

}

TJoystickControl::~TJoystickControl() {

}

void TJoystickControl::Update() {
	if (!Joytick->isAvailable())
		return;
	Joytick->updateEvents();
	for (int i = 0; i < 4; i++)
		Pov[i] = false;
	//return;// xxx
	int x =  Joytick->getPov(0);
	switch (x) {
	   case 0:     Pov[0] = true;
		   break;
	   case 18000: Pov[1] = true;
		   break;
	   case 9000:  Pov[2] = true;
		   break;
	   case 27000: Pov[3] = true;
		   break;
	   case 4500:  Pov[0] = Pov[2] = true;
		   break;
	   case 13500: Pov[1] = Pov[2] = true;
		   break;
	   case 22500: Pov[1] = Pov[3] = true;
		   break;
	   case 31500: Pov[0] = Pov[3] = true;
		   break;

	}
	for (int i = 0; i < 4; i++) 
		PovX[i] = true;// Pov[i];
	NumX[0] = x;

	PovX[1] = Joytick->isAvailable();
	for (int i = 0; i < NumOfAxis; i++) {
		jValue[i] = Joytick->getAxis(i);
		if (jValue[i] < -0.1)
			jValue[i] += 0.1;
		else if (jValue[i] > 0.1)  // play
			jValue[i] -= 0.1;
		else
			jValue[i] = 0.0;
		jValueX[i] = jValue[i];
	}
	trigger = Joytick->getButton(0);
	//int z = Joytick->getNumButtons();
	for (int i = 1; i < Joytick->getNumButtons(); i++) {
		button[i] = Joytick->getButton(i);
		//lineDisplay(30, 10 + i, String::format("i=%d : %d", i, button[i]));
	}
	PovX[2] = trigger;
	if (button[6]) {
		if (!isPress) {
			gJosytickMode = gJosytickMode == 1 ? 0 : 1;
			isPress = true;
		}
	}
	else
		isPress = false;


}