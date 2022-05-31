#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

// 円周率
const float PI = 3.1415f;


// 関数群プロトタイプ宣言
// スケーリング行列
Matrix4 MatScaling(const Vector3& scale);
// X軸回転行列
Matrix4 MatRotX(const float& rotX);
// Y軸回転行列
Matrix4 MatRotY(const float& rotY);
// Z軸回転行列
Matrix4 MatRotZ(const float& rotZ);
// 平行移動行列
Matrix4 MatTrans(const Vector3& translation_);
// 行列のZXY合成
void MatSyntheticZXY(WorldTransform& worldTransform_);

// ラジアンに変換
float ConvartToRadian(const float degree) {
	return (degree * PI) / 180;
}
float ConvartToDegree(const float radian) {
	return radian * 180 / PI;
}

// 限界値設定
float UpperLimit(const float& num, const float& limit);
float LowerLimit(const float& num, const float& limit);
float Clamp(const float& num, const float& min, const float& max);


// 親から順に更新処理
void UpdateWorldTransform(WorldTransform& worldTransform);

GameScene::GameScene() {}

GameScene::~GameScene() {}

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

	// ワールドトランスフォームの初期化
	// キャラクター大元
	worldTransform_[PartID::kRoot].Initialize();
	// 脊椎
	worldTransform_[PartID::kSpine].Initialize();
	worldTransform_[PartID::kSpine].parent_ = &worldTransform_[PartID::kRoot];
	worldTransform_[PartID::kSpine].translation_ = { 0,0,0 };


	const float limbsWidth = 3;
	const float linmbsHeight = 3;
	// 上半身
	// 胴
	worldTransform_[PartID::kChest].Initialize();
	worldTransform_[PartID::kChest].parent_ = &worldTransform_[PartID::kSpine];
	worldTransform_[PartID::kChest].translation_ = { 0,0,0 };
	// 頭
	worldTransform_[PartID::kHead].Initialize();
	worldTransform_[PartID::kHead].parent_ = &worldTransform_[PartID::kChest];
	worldTransform_[PartID::kHead].translation_ = { 0,linmbsHeight,0 };
	// 左腕
	worldTransform_[PartID::kArmL].Initialize();
	worldTransform_[PartID::kArmL].parent_ = &worldTransform_[PartID::kChest];
	worldTransform_[PartID::kArmL].translation_ = { -limbsWidth,0,0 };
	// 右腕
	worldTransform_[PartID::kArmR].Initialize();
	worldTransform_[PartID::kArmR].parent_ = &worldTransform_[PartID::kChest];
	worldTransform_[PartID::kArmR].translation_ = { limbsWidth,0,0 };

	// 下半身
	// 尻
	worldTransform_[PartID::kHip].Initialize();
	worldTransform_[PartID::kHip].parent_ = &worldTransform_[PartID::kSpine];
	worldTransform_[PartID::kHip].translation_ = { 0,-linmbsHeight,0 };
	// 左足
	worldTransform_[PartID::kLegL].Initialize();
	worldTransform_[PartID::kLegL].parent_ = &worldTransform_[PartID::kHip];
	worldTransform_[PartID::kLegL].translation_ = { -limbsWidth,-linmbsHeight,0 };
	// 右足
	worldTransform_[PartID::kLegR].Initialize();
	worldTransform_[PartID::kLegR].parent_ = &worldTransform_[PartID::kHip];
	worldTransform_[PartID::kLegR].translation_ = { limbsWidth,-linmbsHeight,0 };
	
}

void GameScene::Update() {
	// キャラクター移動処理
	{
		// キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };
		
		// 左か右キーを押していたらmove(移動量)を変化
		if (input_->PushKey(DIK_LEFT))move.x -= 0.5f;
		else if (input_->PushKey(DIK_RIGHT))move.x += 0.5f;

		// 移動量を加算
		worldTransform_[PartID::kRoot].translation_ += move;
		
		// デバッグ用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf("translation:(%f,%f,%f)", worldTransform_[PartID::kRoot].translation_.x,
			worldTransform_[PartID::kRoot].translation_.y, worldTransform_[PartID::kRoot].translation_.z);

	}
	// 子の更新
	for (int i = 0; i < PartID::kNumPartId; i++) {
		UpdateWorldTransform(worldTransform_[i]);
	}
	// 上半身回転処理
	{
		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U))worldTransform_[PartID::kChest].rotation_.y -= ConvartToRadian(1);
		else if (input_->PushKey(DIK_I))worldTransform_[PartID::kChest].rotation_.y += ConvartToRadian(1);
	}
	// 下半身回転処理
	{
		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_J))worldTransform_[PartID::kHip].rotation_.y -= ConvartToRadian(1);
		else if (input_->PushKey(DIK_K))worldTransform_[PartID::kHip].rotation_.y += ConvartToRadian(1);
	}
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
	
	for (int i = PartID::kChest; i < PartID::kNumPartId; i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
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


// 関数群定義
// スケーリング行列
Matrix4 MatScaling(const Vector3& scale) {
	Matrix4 matScale;
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}
// X軸回転行列
Matrix4 MatRotX(const float& rotX) {
	// X軸回転行列を宣言
	// 単位行列を代入
	Matrix4 matRotX = MathUtility::Matrix4Identity();
	// X軸回転行列の各要素を設定
	matRotX.m[1][1] = cos(rotX);
	matRotX.m[1][2] = sin(rotX);
	matRotX.m[2][1] = -sin(rotX);
	matRotX.m[2][2] = cos(rotX);
	return matRotX;
}
// Y軸回転行列
Matrix4 MatRotY(const float& rotY) {
	// Y軸回転行列を宣言
	Matrix4 matRotY = MathUtility::Matrix4Identity();
	// Y軸回転行列の各要素を設定
	matRotY.m[0][0] = cos(rotY);
	matRotY.m[0][2] = -sin(rotY);
	matRotY.m[2][0] = sin(rotY);
	matRotY.m[2][2] = cos(rotY);
	return matRotY;
}
// Z軸回転行列
Matrix4 MatRotZ(const float& rotZ) {
	// Z軸回転行列を宣言
	Matrix4 matRotZ = MathUtility::Matrix4Identity();
	// Z軸回転行列の各要素を設定
	matRotZ.m[0][0] = cos(rotZ);
	matRotZ.m[0][1] = sin(rotZ);
	matRotZ.m[1][0] = -sin(rotZ);
	matRotZ.m[1][1] = cos(rotZ);
	return matRotZ;
}

Matrix4 MatTrans(const Vector3& translation_) {

	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = translation_.x;
	matTrans.m[3][1] = translation_.y;
	matTrans.m[3][2] = translation_.z;
	return matTrans;
}

void MatSyntheticZXY(WorldTransform& worldTransform_) {
	// 行列の合成
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	worldTransform_.matWorld_ *= MatScaling(worldTransform_.scale_);
	worldTransform_.matWorld_ *= MatRotZ(worldTransform_.rotation_.z);
	worldTransform_.matWorld_ *= MatRotX(worldTransform_.rotation_.x);
	worldTransform_.matWorld_ *= MatRotY(worldTransform_.rotation_.y);
	worldTransform_.matWorld_ *= MatTrans(worldTransform_.translation_);
	// 行列の転送
	worldTransform_.TransferMatrix();
}

// 限界値設定関数
float UpperLimit(const float& num, const float& limit) {
	if (num >= limit)return limit;
	return num;
}

float LowerLimit(const float& num, const float& limit) {
	if (num <= limit)return limit;
	return num;
}

float Clamp(const float& num, const float& min, const float& max) {
	if (num <= min) return min;
	else if (num >= max)return max;
	return num;
}

// 親から順に更新処理
void UpdateWorldTransform(WorldTransform& worldTransform) {
		MatSyntheticZXY(worldTransform);
		if (worldTransform.parent_ != nullptr) worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;
		worldTransform.TransferMatrix();
}