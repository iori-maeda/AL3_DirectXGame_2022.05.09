#pragma once
#include <math/Matrix4.h>
#include <math/Vector3.h>
#include <3d/WorldTransform.h>

// namespace �C�ӂ̖��́@�Ƃ���
// {} �ň͂����Ƃ�
// std�̂悤�Ɂ@�C�ӂ̖���::�֐��Ƃ��ā@�Ăяo�����Ƃ��o����
// ���������̏ꍇ�@usingnamespace �ŔC�ӂ̖��̂��ȗ����邩
// ����@�C�ӂ̖���:: �Ƃ���K�v������


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
// �s��ƃx�N�g���̊|���Z
Vector3 MatMulti(const Vector3& vec, const Matrix4& mat);

// ���W�A���ɕϊ�
float ConvartToRadian(const float degree);

float ConvartToDegree(const float radian);

// ���E�l�ݒ�
float UpperLimit(const float& num, const float& limit);
float LowerLimit(const float& num, const float& limit);
float Clamp(const float& num, const float& min, const float& max);


// �e���珇�ɍX�V����
void UpdateWorldTransform(WorldTransform& worldTransform);

//�@���C���`��̃��b�s���O�֐�
void DrawXYZLine3DRGB(const Vector3& startPos, const Vector3& endPosLen);

// ���`�⊮
float Lerp(const float& start, const float& end, float time);