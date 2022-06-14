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
	velocity_ = { 0,0,0 };

	// �ړ��͈͌���
	const Vector3 kMoveLimit = { 35,19,40 };

	switch (phaze_)
	{
	case Enemy::Phaze::Default:
		MoveDefault(kMoveLimit);
		break;
	case Enemy::Phaze::Approach:
		MoveApproach({ 0,0,0 });
		break;
	case Enemy::Phaze::Leave:
		MoveLeave(kMoveLimit);
		break;
	default:
		break;
	}
	

	// �͈͌��菈��
	worldTransform_.translation_.x = Clamp(worldTransform_.translation_.x, -kMoveLimit.x, kMoveLimit.x);
	worldTransform_.translation_.y = Clamp(worldTransform_.translation_.y, -kMoveLimit.y, kMoveLimit.y);
	worldTransform_.translation_.z = Clamp(worldTransform_.translation_.z, -kMoveLimit.z, kMoveLimit.z);

	Vector3 rotate = { 0.5f,45,0.5f };
	Rotate(velocity_, rotate);

	// �U������
	Attack();

	MatSyntheticZXY(worldTransform_);
	worldTransform_.translation_ += velocity_;
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

void Enemy::MoveDefault(const Vector3& limit)
{
	velocity_ = { 0,0,0 };
	phaze_ = Phaze::Approach;
}

void Enemy::MoveApproach(const Vector3& limit)
{
	velocity_ = {
		0.5f * Sign(0 - worldTransform_.translation_.x),
		0.5f * Sign(0 - worldTransform_.translation_.y),
		0.5f * Sign(0 - worldTransform_.translation_.z),
	};
	if (worldTransform_.translation_.x <= limit.x) {
		velocity_.x = 0;
	}
	if (worldTransform_.translation_.y <= limit.y) {
		velocity_.y = 0;
	}
	if (worldTransform_.translation_.z <= limit.z) {
		velocity_.z = 0;
	}
	if (worldTransform_.translation_.x <= limit.x
		&& worldTransform_.translation_.y <= limit.y
		&& worldTransform_.translation_.z <= limit.z) {
		phaze_ = Phaze::Leave;
	}
}

void Enemy::MoveLeave(const Vector3& limit)
{
	velocity_ = { 0.5f,0.5f,0.5f };
	if (worldTransform_.translation_.x >= limit.x) {
		velocity_.x = 0;
	}
	if (worldTransform_.translation_.y >= limit.y) {
		velocity_.y = 0;
	}
	if (worldTransform_.translation_.z >= limit.z) {
		velocity_.z = 0;
	}
	if (worldTransform_.translation_.x >= limit.x
		&& worldTransform_.translation_.y >= limit.y
		&& worldTransform_.translation_.z >= limit.z) {
		phaze_ = Phaze::Default;
	}
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

