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
	

	MatSyntheticZXY(worldTransform_);
	worldTransform_.translation_ += move;
	worldTransform_.TransferMatrix();

	// �L�����N�^�[�̍��W����ʕ\�����鏈��
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"playerPos:(%f,%f,%f)",worldTransform_.translation_.x,
		worldTransform_.translation_.y, worldTransform_.translation_.z);
}


void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}