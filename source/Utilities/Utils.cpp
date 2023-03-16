#include "Utils.h"

void lineDisplay(int posx, int posy, String str, vec4 color) {
	Visualizer::renderMessage2D(vec3(posx * 0.015, (float)(posy * 0.015), 0.0), vec3(1.0, 0.0, 0.0), str, color, 1, 16);
}

void Visualizer_renderAxis(NodePtr n) {
	quat q1 = n->getWorldRotation();
	Vec3 xpos1 = n->getWorldPosition();
	Visualizer::renderDirection(xpos1, q1.getNormal().normalize() * 0.15, vec4_blue, 0.3f, false, 0.f, false);
	Visualizer::renderDirection(xpos1, q1.getTangent().normalize() * 0.15, vec4_red, 0.3f, false, 0.f, false);
	Visualizer::renderDirection(xpos1, q1 * quat(90.0, 0.0, 0.0).getNormal().normalize() * 0.15, vec4_green, 0.3f, false, 0.f, false);
}
void Visualizer_renderAxis(Mat4 m) {
	quat q1 = m.getRotate();
	Vec3 xpos1 = m.getColumn3(3);
	Visualizer::renderDirection(xpos1, q1.getNormal().normalize() * 0.15, vec4_blue, 0.3f, false, 0.f, false);
	Visualizer::renderDirection(xpos1, q1.getTangent().normalize() * 0.15, vec4_red, 0.3f, false, 0.f, false);
	Visualizer::renderDirection(xpos1, q1 * quat(90.0, 0.0, 0.0).getNormal().normalize() * 0.15, vec4_green, 0.3f, false, 0.f, false);
}


char clampValue(float& Value, float LowerLimit, float UpperLimit) {
	if (Value > UpperLimit) {
		Value = UpperLimit;
		return 1;
	}
	if (Value < LowerLimit) {
		Value = LowerLimit;
		return -1;
	}
	return 0;
}

char ClampValue(float& Value, float LowerLimit, float UpperLimit) {
	if (Value > UpperLimit) {
		Value = UpperLimit;
		return 1;
	}
	if (Value < LowerLimit) {
		Value = LowerLimit;
		return -1;
	}
	return 0;
}

void ReduceToZero(float& Value, float rate) {
	if (Value > rate)
		Value -= rate;
	else if (Value < -rate)
		Value += rate;
	else 
		Value = 0.0;
}

float getBearingAngle(float angle2) {
	if (angle2 < 0)
		angle2 += 360;
	return 90.0 - angle2;
}

float getBearing(Vec3 diff) {
	float  tbearing = Math::atan2(diff.x, diff.y);
	if (tbearing < 0)
		tbearing += Consts::PI2;// UNIGINE_PI2;
	return tbearing;
}

bool GetToValue(float& actualPos, float targetValue, float rate) {
	if (actualPos > targetValue + rate)
		actualPos -= rate;
	if (actualPos < targetValue - rate)
		actualPos += rate;
	return fabs(actualPos - targetValue) < rate*2;
}

float HeadingDifference(float curr, float target) {
	float headingdiff = target - curr;
	if (headingdiff > 180.0)
		headingdiff -= 360.0;
	else if (headingdiff < -180.0)
		headingdiff += 360;
	return headingdiff;
}

double getWorldHeight(float x, float y, float hrange) {
	Vec3 p0 = Vec3(x, y, hrange);
	Vec3 p1 = Vec3(x, y, -hrange);
	//Visualizer::renderLine3D(p0, p1, vec4::BLUE);
	WorldIntersectionPtr wIntersection;

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

