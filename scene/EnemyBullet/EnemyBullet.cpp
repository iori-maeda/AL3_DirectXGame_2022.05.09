#include "EnemyBullet.h"

#include<assert.h>
#include"UtilityFunction.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	this->model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("test_ball.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	this->worldTransform_.translation_ = position;
	// 引数で受け取った速度を代入
	this->velocity_ = velocity;
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

void EnemyBullet::Update() {
	// 時間経過でデス
	if (--deathTime_ <= 0) {
		isDead_ = true;
	}
	// 座標を移動(１フレーム分の移動量を足す)
	worldTransform_.translation_ += velocity_;
	MatSyntheticZXY(worldTransform_);
	worldTransform_.TransferMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
