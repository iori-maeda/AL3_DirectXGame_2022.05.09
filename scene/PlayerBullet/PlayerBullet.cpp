#include "PlayerBullet.h"

#include<assert.h>
#include"UtilityFunction.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	this->model_ = model;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("block+p.png");
	
	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	this->worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {
	MatSyntheticZXY(worldTransform_);
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}