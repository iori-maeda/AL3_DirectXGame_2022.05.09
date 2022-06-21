#include "EnemyBullet.h"

#include<assert.h>
#include"UtilityFunction.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	this->model_ = model;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("test_ball.png");

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	this->worldTransform_.translation_ = position;
	// �����Ŏ󂯎�������x����
	this->velocity_ = velocity;
}

void EnemyBullet::Update() {
	// ���Ԍo�߂Ńf�X
	if (--deathTime_ <= 0) {
		isDead_ = true;
	}
	// ���W���ړ�(�P�t���[�����̈ړ��ʂ𑫂�)
	worldTransform_.translation_ += velocity_;
	MatSyntheticZXY(worldTransform_);
	worldTransform_.TransferMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}