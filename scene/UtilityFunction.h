#pragma once
#include <math/Matrix4.h>
#include <math/Vector3.h>
#include <3d/WorldTransform.h>

// �~����
const float PI = 3.1415f;


// �֐��Q�v���g�^�C�v�錾
// �X�P�[�����O�s��
Matrix4 MatScaling(const Vector3& scale);
// X����]�s��
Matrix4 MatRotX(const float& rotX);
// Y����]�s��
Matrix4 MatRotY(const float& rotY);
// Z����]�s��
Matrix4 MatRotZ(const float& rotZ);
// ���s�ړ��s��
Matrix4 MatTrans(const Vector3& translation_);
// �s���ZXY����
void MatSyntheticZXY(WorldTransform& worldTransform_);

// ���W�A���ɕϊ�
float ConvartToRadian(const float degree);

float ConvartToDegree(const float radian);

// ���E�l�ݒ�
float UpperLimit(const float& num, const float& limit);
float LowerLimit(const float& num, const float& limit);
float Clamp(const float& num, const float& min, const float& max);


// �e���珇�ɍX�V����
void UpdateWorldTransform(WorldTransform& worldTransform);
