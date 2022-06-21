#include "Player.h"
#include<assert.h>
#include"UtilityFunction.h"


void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	this->model_ = model;
	this->textureHandle_ = textureHandle;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

}

void Player::Update() {
	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	if (input_->PushKey(DIK_A))move.x = -0.5f;
	else if(input_->PushKey(DIK_D))move.x = 0.5f;
	if (input_->PushKey(DIK_W))move.y = 0.5f;
	else if (input_->PushKey(DIK_S))move.y = -0.5f;

	// �ړ��͈͌���
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 19;

	// �͈͌��菈��
	worldTransform_.translation_.x = Clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = Clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

	Vector3 rotate = { 0.5f,45,0.5f };
	Rotate(move, rotate);

	// �U������
	Attack();

	// �e�̍X�V
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
	// �f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	
	MatSyntheticZXY(worldTransform_);
	worldTransform_.translation_ += move;
	worldTransform_.TransferMatrix();

	// �L�����N�^�[�̍��W����ʕ\�����鏈��
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"playerPos:(%f,%f,%f)",worldTransform_.translation_.x,
		worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void Player::Rotate(const Vector3& moveState, const Vector3& rotate) {
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

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE)) {
		// �e���ݒ�
		const float kBulletSpeed = 1;
		Vector3 velocity(0, kBulletSpeed / 2, kBulletSpeed);

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = MatMulti(velocity, worldTransform_.matWorld_);

		// �e�𐶐����A������
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(this->model_, worldTransform_.translation_, velocity);

		// �e�𔭎˂���
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// �e�̕`��
	for (auto& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}