#pragma once
#include <math/Matrix4.h>
#include <math/Vector3.h>
#include <3d/WorldTransform.h>

// ~üŚ
const float PI = 3.1415f;


// ÖQvg^Cvéž
// XP[Osń
Matrix4 MatScaling(const Vector3& scale);
// X˛ń]sń
Matrix4 MatRotX(const float& rotX);
// Y˛ń]sń
Matrix4 MatRotY(const float& rotY);
// Z˛ń]sń
Matrix4 MatRotZ(const float& rotZ);
// ˝sÚŽsń
Matrix4 MatTrans(const Vector3& translation_);
// sńĚZXYŹ
void MatSyntheticZXY(WorldTransform& worldTransform_);

// WAÉĎˇ
float ConvartToRadian(const float degree);

float ConvartToDegree(const float radian);

// ŔElÝč
float UpperLimit(const float& num, const float& limit);
float LowerLimit(const float& num, const float& limit);
float Clamp(const float& num, const float& min, const float& max);


// eŠçÉXV
void UpdateWorldTransform(WorldTransform& worldTransform);
