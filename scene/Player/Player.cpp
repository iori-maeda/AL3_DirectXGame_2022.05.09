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

	Vector3 rotate = { 0.5f,45,0.5f };
	Rotate(move, rotate);

	// 攻撃処理
	Attack();

	// 弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
	// デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	
	MatSyntheticZXY(worldTransform_);
	worldTransform_.translation_ += move;
	worldTransform_.TransferMatrix();

	// キャラクターの座標を画面表示する処理
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
		// 弾速設定
		const float kBulletSpeed = 1;
		Vector3 velocity(0, kBulletSpeed / 2, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = MatMulti(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(this->model_, worldTransform_.translation_, velocity);

		// 弾を発射する
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾の描画
	for (auto& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}