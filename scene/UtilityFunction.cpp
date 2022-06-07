#include"UtilityFunction.h"
#include <math/MathUtility.h>
#include <cmath>


// ŠÖ”ŒQ’è‹`
// ƒXƒP[ƒŠƒ“ƒOs—ñ
Matrix4 MatScaling(const Vector3& scale) {
	Matrix4 matScale;
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}
// XŽ²‰ñ“]s—ñ
Matrix4 MatRotX(const float& rotX) {
	// XŽ²‰ñ“]s—ñ‚ðéŒ¾
	// ’PˆÊs—ñ‚ð‘ã“ü
	Matrix4 matRotX = MathUtility::Matrix4Identity();
	// XŽ²‰ñ“]s—ñ‚ÌŠe—v‘f‚ðÝ’è
	matRotX.m[1][1] = cos(rotX);
	matRotX.m[1][2] = sin(rotX);
	matRotX.m[2][1] = -sin(rotX);
	matRotX.m[2][2] = cos(rotX);
	return matRotX;
}
// YŽ²‰ñ“]s—ñ
Matrix4 MatRotY(const float& rotY) {
	// YŽ²‰ñ“]s—ñ‚ðéŒ¾
	Matrix4 matRotY = MathUtility::Matrix4Identity();
	// YŽ²‰ñ“]s—ñ‚ÌŠe—v‘f‚ðÝ’è
	matRotY.m[0][0] = cos(rotY);
	matRotY.m[0][2] = -sin(rotY);
	matRotY.m[2][0] = sin(rotY);
	matRotY.m[2][2] = cos(rotY);
	return matRotY;
}
// ZŽ²‰ñ“]s—ñ
Matrix4 MatRotZ(const float& rotZ) {
	// ZŽ²‰ñ“]s—ñ‚ðéŒ¾
	Matrix4 matRotZ = MathUtility::Matrix4Identity();
	// ZŽ²‰ñ“]s—ñ‚ÌŠe—v‘f‚ðÝ’è
	matRotZ.m[0][0] = cos(rotZ);
	matRotZ.m[0][1] = sin(rotZ);
	matRotZ.m[1][0] = -sin(rotZ);
	matRotZ.m[1][1] = cos(rotZ);
	return matRotZ;
}

Matrix4 MatTrans(const Vector3& translation_) {

	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = translation_.x;
	matTrans.m[3][1] = translation_.y;
	matTrans.m[3][2] = translation_.z;
	return matTrans;
}

void MatSyntheticZXY(WorldTransform& worldTransform_) {
	// s—ñ‚Ì‡¬
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= MatScaling(worldTransform_.scale_);
	worldTransform_.matWorld_ *= MatRotZ(worldTransform_.rotation_.z);
	worldTransform_.matWorld_ *= MatRotX(worldTransform_.rotation_.x);
	worldTransform_.matWorld_ *= MatRotY(worldTransform_.rotation_.y);
	worldTransform_.matWorld_ *= MatTrans(worldTransform_.translation_);
	// s—ñ‚Ì“]‘—
	worldTransform_.TransferMatrix();
}

// ƒ‰ƒWƒAƒ“‚É•ÏŠ·
float ConvartToRadian(const float degree) {
	return (degree * PI) / 180;
}
// “x”•ÏŠ·
float ConvartToDegree(const float radian) {
	return radian * 180 / PI;
}

// ŒÀŠE’lÝ’èŠÖ”
float UpperLimit(const float& num, const float& limit) {
	if (num >= limit)return limit;
	return num;
}

float LowerLimit(const float& num, const float& limit) {
	if (num <= limit)return limit;
	return num;
}

float Clamp(const float& num, const float& min, const float& max) {
	if (num <= min) return min;
	else if (num >= max)return max;
	return num;
}

// e‚©‚ç‡‚ÉXVˆ—
void UpdateWorldTransform(WorldTransform& worldTransform) {
	MatSyntheticZXY(worldTransform);
	if (worldTransform.parent_ != nullptr) worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;
	worldTransform.TransferMatrix();
}