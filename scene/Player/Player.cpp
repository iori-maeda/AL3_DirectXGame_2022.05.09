#include "Player.h"
#include<assert.h>
#include"UtilityFunction.h"


void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	this->model_ = model;
	this->textureHandle_ = textureHandle;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

}

void Player::Update() {
	// キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	if (input_->PushKey(DIK_A))move.x = -0.5f;
	else if(input_->PushKey(DIK_D))move.x = 0.5f;
	if (input_->PushKey(DIK_W))move.y = 0.5f;
	else if (input_->PushKey(DIK_S))move.y = -0.5f;

	// 移動範囲限定
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 19;

	// 範囲限定処理
	worldTransform_.translation_.x = Clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = Clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);
	

	MatSyntheticZXY(worldTransform_);
	worldTransform_.translation_ += move;
	worldTransform_.TransferMatrix();

	// キャラクターの座標を画面表示する処理
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"playerPos:(%f,%f,%f)",worldTransform_.translation_.x,
		worldTransform_.translation_.y, worldTransform_.translation_.z);
}


void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}