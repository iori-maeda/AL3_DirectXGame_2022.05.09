#include"UtilityFunction.h"
#include <math/MathUtility.h>
#include <cmath>
#include"PrimitiveDrawer.h"


// 関数群定義
// スケーリング行列
Matrix4 MatScaling(const Vector3& scale) {
	Matrix4 matScale;
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}
// X軸回転行列
Matrix4 MatRotX(const float& rotX) {
	// X軸回転行列を宣言
	// 単位行列を代入
	Matrix4 matRotX = MathUtility::Matrix4Identity();
	// X軸回転行列の各要素を設定
	matRotX.m[1][1] = cos(rotX);
	matRotX.m[1][2] = sin(rotX);
	matRotX.m[2][1] = -sin(rotX);
	matRotX.m[2][2] = cos(rotX);
	return matRotX;
}
// Y軸回転行列
Matrix4 MatRotY(const float& rotY) {
	// Y軸回転行列を宣言
	Matrix4 matRotY = MathUtility::Matrix4Identity();
	// Y軸回転行列の各要素を設定
	matRotY.m[0][0] = cos(rotY);
	matRotY.m[0][2] = -sin(rotY);
	matRotY.m[2][0] = sin(rotY);
	matRotY.m[2][2] = cos(rotY);
	return matRotY;
}
// Z軸回転行列
Matrix4 MatRotZ(const float& rotZ) {
	// Z軸回転行列を宣言
	Matrix4 matRotZ = MathUtility::Matrix4Identity();
	// Z軸回転行列の各要素を設定
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
	// 行列の合成
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= MatScaling(worldTransform_.scale_);
	worldTransform_.matWorld_ *= MatRotZ(worldTransform_.rotation_.z);
	worldTransform_.matWorld_ *= MatRotX(worldTransform_.rotation_.x);
	worldTransform_.matWorld_ *= MatRotY(worldTransform_.rotation_.y);
	worldTransform_.matWorld_ *= MatTrans(worldTransform_.translation_);
	// 行列の転送
	worldTransform_.TransferMatrix();
}

// ラジアンに変換
float ConvartToRadian(const float degree) {
	return (degree * PI) / 180;
}
// 度数変換
float ConvartToDegree(const float radian) {
	return radian * 180 / PI;
}

// 限界値設定関数
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

// 親から順に更新処理
void UpdateWorldTransform(WorldTransform& worldTransform) {
	MatSyntheticZXY(worldTransform);
	if (worldTransform.parent_ != nullptr) worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;
	worldTransform.TransferMatrix();
}

// ライン描画のラッピング関数
void DrawXYZLine3DRGB(const Vector3& startPos, const Vector3& endPosLen) {
	Vector3 endPosX = {
		startPos.x + endPosLen.x,
		startPos.y,
		startPos.z
	};
	Vector3 endPosY = {
		startPos.x,
		startPos.y + endPosLen.y,
		startPos.z
	};
	Vector3 endPosZ = {
		startPos.x,
		startPos.y,
		startPos.z + endPosLen.z
	};
	PrimitiveDrawer::GetInstance()->DrawLine3d(startPos, endPosX, { 0xff0000,0x000000,0x000000,0xffffff });
	PrimitiveDrawer::GetInstance()->DrawLine3d(startPos, endPosY, { 0x000000,0x00ff00,0x000000,0xffffff });
	PrimitiveDrawer::GetInstance()->DrawLine3d(startPos, endPosZ, { 0x000000,0x000000,0x0000ff,0xffffff });
}