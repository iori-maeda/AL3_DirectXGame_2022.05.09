#include "Enemy.h"

#include<assert.h>
#include"UtilityFunction.h"

void Enemy::Initialize(Model* model)
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	this->model_ = model;
	// テクスチャを読み込み
	textureHandle_ = TextureManager::Load("test_ball.png");

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,0 };
}

void Enemy::Update()
{
	// キャラクターの移動ベクトル
	Vector3 move = { 0,0.05f,-0.5f };

	// 移動範囲限定
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 19;
	const float kMoveLimitZ = 40;

	// 範囲限定処理
	worldTransform_.translation_.x = Clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y = Clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);
	worldTransform_.translation_.z = Clamp(worldTransform_.translation_.z, -kMoveLimitZ, kMoveLimitY);

	Vector3 rotate = { 0.5f,45,0.5f };
	Rotate(move, rotate);

	// 攻撃処理
	Attack();

	MatSyntheticZXY(worldTransform_);
	worldTransform_.translation_ += move;
	worldTransform_.TransferMatrix();

	// キャラクターの座標を画面表示する処理
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

