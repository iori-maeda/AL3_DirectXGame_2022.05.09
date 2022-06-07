#include "PlayerBullet.h"

#include<assert.h>
#include"UtilityFunction.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);

	this->model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("block+p.png");
	
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	this->worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {
	MatSyntheticZXY(worldTransform_);
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}