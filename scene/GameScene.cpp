#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>


#include"PrimitiveDrawer.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete debugCamera_;
	delete player_;
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
	// ライン描画が参照するビュープロジェクションの指定(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

#ifdef _DEBUG
	if (input_->PushKey(DIK_LSHIFT) || input_->PushKey(DIK_RSHIFT)) {
		if (input_->TriggerKey(DIK_D)) {
			if (isDubugCameraActive_ == false) {
				isDubugCameraActive_ = true;
			}
			else {
				isDubugCameraActive_ = false;
			}
		}
	}
	// カメラの処理
	if (isDubugCameraActive_) {
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
	
	// 自キャラの描画 
	if (isDubugCameraActive_) {
		player_->Draw(debugCamera_->GetViewProjection());
		Vector3 startPos = {
			player_->GetWorldTransform().translation_.x,
			player_->GetWorldTransform().translation_.y,
			player_->GetWorldTransform().translation_.z };
		Vector3 endPosX = {
			player_->GetWorldTransform().translation_.x + 50,
			player_->GetWorldTransform().translation_.y,
			player_->GetWorldTransform().translation_.z };
		Vector3 endPosY = {
			player_->GetWorldTransform().translation_.x,
			player_->GetWorldTransform().translation_.y + 50,
			player_->GetWorldTransform().translation_.z };
		Vector3 endPosZ = {
			player_->GetWorldTransform().translation_.x,
			player_->GetWorldTransform().translation_.y,
			player_->GetWorldTransform().translation_.z + 50 };
		
		PrimitiveDrawer::GetInstance()->DrawLine3d(startPos, endPosX, { 0xff0000,0x000000,0x000000,0xffffff });
		PrimitiveDrawer::GetInstance()->DrawLine3d(startPos, endPosY, { 0x000000,0x00ff00,0x000000,0xffffff });
		PrimitiveDrawer::GetInstance()->DrawLine3d(startPos, endPosZ, { 0x000000,0x000000,0x0000ff,0xffffff });
	}
	else {
		player_->Draw(viewProjection_);
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
