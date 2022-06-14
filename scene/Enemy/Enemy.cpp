#include "Enemy.h"

#include<assert.h>
#include"UtilityFunction.h"

void Enemy::Initialize(Model* model)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	// �����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	this->model_ = model;
	// �e�N�X�`����ǂݍ���
	textureHandle_ = TextureManager::Load("test_ball.png");

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,0 };
}

void Enemy::Update()
{
	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0.05f,-0.5f };

	// �ړ��͈͌���
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 19;
	const float kMoveLimitZ = 40;

	// �͈͌��菈��
	worldTransform_.translation_.x = Clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = Clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);
	worldTransform_.translation_.z = Clamp(worldTransform_.translation_.z, -kMoveLimitZ, kMoveLimitY);

	Vector3 rotate = { 0.5f,45,0.5f };
	Rotate(move, rotate);

	// �U������
	Attack();

	MatSyntheticZXY(worldTransform_);
	worldTransform_.translation_ += move;
	worldTransform_.TransferMatrix();

	// �L�����N�^�[�̍��W����ʕ\�����鏈��
	debugText_->SetPos(50, 200);
	debugText_->Printf(
		"enemyPos:(%f,%f,%f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Enemy::Rotate(const Vector3& moveState, const Vector3& rotate)
{
	Vector3 rotation = {
		ConvartToRadian(rotate.x),
		ConvartToRadian(rotate.y),
		ConvartToRadian(rotate.z)
	};
	Vector3 rot = {
		rotation.x / 10,
		rotation.y / 10,
		rotation.z / 10,
	};
	if (moveState.x > 0) {
		worldTransform_.rotation_.y -= rotation.y / 3;
	}
	else if (moveState.x < 0) {
		worldTransform_.rotation_.y += rotation.y / 3;
	}
	worldTransform_.rotation_.y = Clamp(worldTransform_.rotation_.y, -rotation.y, rotation.y);
	if (moveState.x == 0) {
		if (worldTransform_.rotation_.y > 0) {
			worldTransform_.rotation_.y -= rot.y;
			worldTransform_.rotation_.y = LowerLimit(worldTransform_.rotation_.y, 0);
		}
		else if (worldTransform_.rotation_.y < 0) {
			worldTransform_.rotation_.y += rot.y;
			worldTransform_.rotation_.y = UpperLimit(worldTransform_.rotation_.y, 0);
		}
	}
}

void Enemy::Attack()
{
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

