// Any utility or global functions goes here
#pragma once

#include <Unigine.h>

using namespace Unigine::Math;
using namespace Unigine;
using namespace std;

extern vec4 vec4_yellow, vec4_cyan, vec4_magenta;

char clampValue(float& Value, float LowerLimit, float UpperLimit);
char ClampValue(float& Value, float LowerLimit, float UpperLimit);
void ReduceToZero(float& Value, float rate);
float HeadingDifference(float curr, float target);
float getBearing(Vec3 diff);
double getWorldHeight(float x, float y, float hrange);
bool GetToValue(float& actualPos, float targetValue, float rate);
void NormaliseAngle(float& angle);


void lineDisplay(int posy, int posx, String str, vec4 color = vec4(0.0, 1.0, 0.0, 1.0));
void Visualizer_renderAxis(NodePtr n);
void Visualizer_renderAxis(Mat4 m);

