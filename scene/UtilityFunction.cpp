#include"UtilityFunction.h"
#include <math/MathUtility.h>
#include <cmath>
#include"PrimitiveDrawer.h"


// �֐��Q��`
// �X�P�[�����O�s��
Matrix4 MatScaling(const Vector3& scale) {
	Matrix4 matScale;
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}
// X����]�s��
Matrix4 MatRotX(const float& rotX) {
	// X����]�s���錾
	// �P�ʍs�����
	Matrix4 matRotX = MathUtility::Matrix4Identity();
	// X����]�s��̊e�v�f��ݒ�
	matRotX.m[1][1] = cos(rotX);
	matRotX.m[1][2] = sin(rotX);
	matRotX.m[2][1] = -sin(rotX);
	matRotX.m[2][2] = cos(rotX);
	return matRotX;
}
// Y����]�s��
Matrix4 MatRotY(const float& rotY) {
	// Y����]�s���錾
	Matrix4 matRotY = MathUtility::Matrix4Identity();
	// Y����]�s��̊e�v�f��ݒ�
	matRotY.m[0][0] = cos(rotY);
	matRotY.m[0][2] = -sin(rotY);
	matRotY.m[2][0] = sin(rotY);
	matRotY.m[2][2] = cos(rotY);
	return matRotY;
}
// Z����]�s��
Matrix4 MatRotZ(const float& rotZ) {
	// Z����]�s���錾
	Matrix4 matRotZ = MathUtility::Matrix4Identity();
	// Z����]�s��̊e�v�f��ݒ�
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
	// �s��̍���
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= MatScaling(worldTransform_.scale_);
	worldTransform_.matWorld_ *= MatRotZ(worldTransform_.rotation_.z);
	worldTransform_.matWorld_ *= MatRotX(worldTransform_.rotation_.x);
	worldTransform_.matWorld_ *= MatRotY(worldTransform_.rotation_.y);
	worldTransform_.matWorld_ *= MatTrans(worldTransform_.translation_);
	// �s��̓]��
	worldTransform_.TransferMatrix();
}

// ���W�A���ɕϊ�
float ConvartToRadian(const float degree) {
	return (degree * PI) / 180;
}
// �x���ϊ�
float ConvartToDegree(const float radian) {
	return radian * 180 / PI;
}

// ���E�l�ݒ�֐�
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

// �e���珇�ɍX�V����
void UpdateWorldTransform(WorldTransform& worldTransform) {
	MatSyntheticZXY(worldTransform);
	if (worldTransform.parent_ != nullptr) worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;
	worldTransform.TransferMatrix();
}

// ���C���`��̃��b�s���O�֐�
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