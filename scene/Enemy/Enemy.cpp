#include "Enemy.h"

#include<assert.h>
#include"UtilityFunction.h"
#include"Player/Player.h"

void Enemy::Initialize(Model* model)
{
	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	this->model_ = model;
	// テクスチャを読み込み
	textureHandle_ = TextureManager::Load("beta+a.png");

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
	velocity_ = { 0,0,0 };

	// 移動範囲限定
	const Vector3 kMoveLimit = { 35,19,40 };

	Vector3 playerPos = player_->GetWorldTransform().translation_;

	switch (phaze_)
	{
	case Enemy::Phaze::Default:
		MoveDefault(kMoveLimit);
		break;
	case Enemy::Phaze::Approach:
		MoveApproach(playerPos);
		break;
	case Enemy::Phaze::Leave:
		MoveLeave(kMoveLimit);
		break;
	default:
		break;
	}
	

	// 範囲限定処理
	worldTransform_.translation_.x = Clamp(worldTransform_.translation_.x, -kMoveLimit.x, kMoveLimit.x);
	worldTransform_.translation_.y = Clamp(worldTransform_.translation_.y, -kMoveLimit.y, kMoveLimit.y);
	worldTransform_.translation_.z = Clamp(worldTransform_.translation_.z, -kMoveLimit.z, kMoveLimit.z);

	Vector3 rotate = { 0.5f,45,0.5f };
	Rotate(velocity_, rotate);


	// 弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}

	MatSyntheticZXY(worldTransform_);
	worldTransform_.translation_ += velocity_;
	worldTransform_.TransferMatrix();

	// デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

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
	Vector3 rot = rotation / 10;

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

void Enemy::OnCollision()
{
}

void Enemy::Fire()
{
	// 弾速設定
	const float kBulletSpeed = 1;

	// 自キャラのワールド座標取得
	Vector3 pWorldPos = player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得
	Vector3 eWorldPos = GetWorldPosition();
	// 差分ベクトルを求める
	Vector3 velocity = pWorldPos - eWorldPos;
	velocity.normalize() *= kBulletSpeed;

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(this->model_, worldTransform_.translation_, velocity);

	// 弾を発射する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::MoveApproachInitialize()
{
	// 発射タイマーを初期化
	fireTimer = kFireInterVal;
}

void Enemy::MoveDefault(const Vector3& limit)
{
	velocity_ = { 0,0,0 };
	phaze_ = Phaze::Approach;

	//接近フェーズ初期化
	MoveApproachInitialize();
}

void Enemy::MoveApproach(const Vector3& limit)
{
	// 発射タイマーカウントダウン
	fireTimer--;
	// 指定時間に達した
	if (fireTimer < 0) {
		// 攻撃処理
		Fire();
		// 発射タイマー初期化
		fireTimer = kFireInterVal;
	}
	velocity_ = {
		0.5f * Sign(limit.x - worldTransform_.translation_.x),
		0.5f * Sign(limit.y - worldTransform_.translation_.y),
		0.5f * Sign(limit.z - worldTransform_.translation_.z),
	};

	velocity_ = { 0,0,0 };
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
	for (auto& bullet : bullets_) {
 		bullet->Draw(viewProjection);
	}
}



// ゲッター？
Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}