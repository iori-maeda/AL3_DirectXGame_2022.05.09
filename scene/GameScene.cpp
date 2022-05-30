#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"
#include<random>

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

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3D生成
	model_ = Model::Create();
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// デバッグカメラ生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向が参照するビュープロジェクションの指定
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	// ライン描画が参照するビュープロジェクション指定
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	// X Y Z 方向のスケーリング設定
	worldTransform_.scale_ = { 5,5,5 };
	
	// X Y Z 軸周りの回転角を設定
	worldTransform_.rotation_ = { ConvartToRadian(45),ConvartToRadian(45),ConvartToRadian(45) };
	// X Y Z 軸周りの平行移動を設定
	worldTransform_.translation_ = { 10,10,10 };

	// 行列計算と合成
	MatSyntheticZXY(worldTransform_);

	// 乱数シード生成器
	std::random_device seed_gen;
	// メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engin(seed_gen());
	// 乱数範囲の指定
	std::uniform_real_distribution<float> dist(10, -10);
}

void GameScene::Update() {
	debugCamera_->Update(); 
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

	//3Dモデル描画
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// x y z ライン描画
	const int MAX_LINE = 3;
	Vector3 p2_[MAX_LINE] = {
		{50,0,0},
		{0,50,0},
		{0,0,50},
	};
	Vector4 color_[MAX_LINE] = {
		{255,0,0,255},
		{0,255,0,255},
		{0,0,255,255},
	};

	for (int i = 0; i < MAX_LINE; i++) {
		p2_[i] += worldTransform_.translation_;
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			worldTransform_.translation_,
			p2_[i], color_[i]);
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