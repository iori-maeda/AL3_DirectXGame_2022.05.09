#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <3d/PrimitiveDrawer.h>
#include <scene/UtilityFunction.h>


GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデル生成
	model_ = Model::Create();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// デバッグカメラの生成
	const int WIN_WIDTH = 1280;
	const int WIN_HEIGHT = 720;
	debugCamera_ = new DebugCamera(WIN_WIDTH, WIN_HEIGHT);
	// ライン描画が参照するビュープロジェクション(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 自キャラ初期化
	player_->Initialize(model_, textureHandle_);
	// 敵キャラ初期化
	enemy_->Initialize(model_);
	enemy_->SetPlayer(player_.get());

}

void GameScene::Update() {
	// 自キャラ更新
	player_->Update();
	// 敵キャラ更新
	enemy_->Update();

	// 衝突判定チェック
	CheckAllCollisions();

#ifndef デバッグカメラ
	if (input_->PushKey(DIK_LSHIFT) || input_->PushKey(DIK_RSHIFT)) {
		if (input_->TriggerKey(DIK_D)) {
			if (isDebugCameraActive_ == false) {
				isDebugCameraActive_ = true;
			}
			else {
				isDebugCameraActive_ = false;
			}
		}
	}
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	else {
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
	}
#endif
}

void GameScene::CheckAllCollisions()
{
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自キャラ弾のリスト取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullet();
	// 敵キャラ弾のリスト取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullet();

#pragma region 自キャラと敵キャラ弾の当たり判定
	// 自キャラワールド座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵キャラ弾の当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		// 球のワールド座標を取得
		posB = bullet->GetWorldPosition();
		// キャラの半径
		const float radius = 0.5f;
		float totalRadius = (radius + radius) * (radius + radius);

		// 距離計算
		Vector3 distance = {
			(posB.x - posA.x) * (posB.x - posA.x),
			(posB.y - posA.y) * (posB.y - posA.y),
			(posB.z - posA.z) * (posB.z - posA.z)
		};
		float totalDistance = distance.x + distance.y + distance.z;
		// 球と球の交差判定
		if (totalDistance <= totalRadius) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵キャラ弾の衝突時コールバック関数を呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自キャラ弾と敵キャラの当たり判定
	// 敵キャラワールド座標
	posA = enemy_->GetWorldPosition();

	// 敵キャラと自キャラ弾の当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		// 球のワールド座標を取得
		posB = bullet->GetWorldPosition();
		// キャラの半径
		const float radius = 0.5f;
		float totalRadius = (radius + radius) * (radius + radius);

		// 距離計算
		Vector3 distance = {
			(posB.x - posA.x) * (posB.x - posA.x),
			(posB.y - posA.y) * (posB.y - posA.y),
			(posB.z - posA.z) * (posB.z - posA.z)
		};
		float totalDistance = distance.x + distance.y + distance.z;
		// 球と球の交差判定
		if (totalDistance <= totalRadius) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自キャラ弾の衝突時コールバック関数を呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自キャラ弾と敵キャラ弾の当たり判定
	// 自キャラ弾と敵キャラ弾の当たり判定
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBullets) {
		// 自キャラ球のワールド座標を取得
		posA = playerBullet->GetWorldPosition();
		// キャラの半径
		const float radius = 0.5f;
		float totalRadius = (radius + radius) * (radius + radius);

		for (const std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets) {
			// 敵キャラ球のワールド座標を取得
			posB = enemyBullet->GetWorldPosition();

			// 距離計算
			Vector3 distance = {
				(posB.x - posA.x) * (posB.x - posA.x),
				(posB.y - posA.y) * (posB.y - posA.y),
				(posB.z - posA.z) * (posB.z - posA.z)
			};
			float totalDistance = distance.x + distance.y + distance.z;
			// 球と球の交差判定
			if (totalDistance <= totalRadius) {
				// 自キャラ弾の衝突時コールバック関数を呼び出す
				playerBullet->OnCollision();
				// 敵キャラ弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	if (isDebugCameraActive_) {
		player_->Draw(debugCamera_->GetViewProjection());
		enemy_->Draw(debugCamera_->GetViewProjection());
		Vector3 startPos = {
			player_->GetWorldTransform().translation_.x,
			player_->GetWorldTransform().translation_.y,
			player_->GetWorldTransform().translation_.z,
		};
		Vector3 endPos = {
			startPos.x + 50,
			startPos.y + 50,
			startPos.z + 50,
		};
		DrawXYZLine3DRGB(startPos, endPos);
	}
	else {
		player_->Draw(viewProjection_);
		enemy_->Draw(viewProjection_);
	}


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}