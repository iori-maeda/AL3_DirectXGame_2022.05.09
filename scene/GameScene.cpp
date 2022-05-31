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

// 限界値設定
float UpperLimit(const float& y, const float& limit);
float LowerLimit(const float& y, const float& limit);

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
	// 3D生成
	model_ = Model::Create();
	// カメラ視点座標を設定
	//viewProjection_.eye = { 0,0,-10 };
	viewProjection_.eye = { 0,0,-50 };
	// カメラ注視点座標を設定
	viewProjection_.target = { 10,0,0 };
	viewProjection_.up = { cosf(ConvartToRadian(45)),sinf(ConvartToRadian(45)),0 };
	// カメラの垂直方向視野角を設定
	viewProjection_.fovAngleY = ConvartToRadian(10);
	// アスペクト比を設定
	viewProjection_.aspectRatio = 1;
	// ニアクリップ距離を設定
	viewProjection_.nearZ = 52;
	// ファークリップ距離を設定
	viewProjection_.farZ = 53;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向が参照するビュープロジェクションの指定
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// ライン描画が参照するビュープロジェクション指定
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	// 乱数シード生成器
	std::random_device seed_gen;
	// メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	// 乱数範囲の指定
	std::uniform_real_distribution<float> dist(-10, 10);

	for (WorldTransform& worldTransform : worldTransform_) {
		// ワールドトランスフォームの初期化
		worldTransform.Initialize();
		// X Y Z 方向のスケーリング設定
		worldTransform.scale_ = { 1,1,1 };
		// X Y Z 軸周りの平行移動を設定
		worldTransform.translation_ = { dist(engine),dist(engine),dist(engine) };
		// X Y Z 軸周りの回転角を設定
		std::uniform_real_distribution<float> dist(ConvartToRadian(0), ConvartToRadian(360));
		worldTransform.rotation_ = { dist(engine),dist(engine),dist(engine) };

		// 行列計算と合成
		MatSyntheticZXY(worldTransform);
	}
}

void GameScene::Update() {
	// 視点移動処理
	{
		// 視点の移動ベクトル
		Vector3 move = { 0,0,0 };
		// 視点の移動速度
		const float kEyeSpeed = 0.2f;
		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W))move.z += kEyeSpeed;
		else if (input_->PushKey(DIK_S))move.z -= kEyeSpeed;
		// 視点移動(ベクトルの加算)
		viewProjection_.eye += move;
		// 行列の再計算
		viewProjection_.UpdateMatrix();
		// デバッグ用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
			"eye:(%f,%f,%f)", viewProjection_.eye.x,
			viewProjection_.eye.y, viewProjection_.eye.z); 
	}
	// 注視点移動処理
	{
		// 注視点の移動ベクトル
		Vector3 move = { 0,0,0 };
		// 注視点の移動速度
		const float kTargetSpeed = 0.2f;
		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT))move.x -= kTargetSpeed;
		else if (input_->PushKey(DIK_RIGHT))move.x += kTargetSpeed;
		// 注視点移動(ベクトルの加算)
		viewProjection_.target += move;
		// 行列の再計算
		viewProjection_.UpdateMatrix();
		// デバッグ用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf(
			"target:(%f,%f,%f)", viewProjection_.target.x,
			viewProjection_.target.y, viewProjection_.target.z);
	}
	// 上方向回転処理
	{
		// 上方向の回転速度[ラジアン/frame]1
		const float kUpSpeed = 0.05f;
		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_SPACE)) {
			viewAngle += kUpSpeed;
			// 2πを超えたら0に戻す
			viewAngle = fmodf(viewAngle, PI * 2.0);
		}
		// 上方向ベクトルを計算(半径iの円周上の座標)
		viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0 };
		// 行列の再計算
		viewProjection_.UpdateMatrix();
		// デバッグ用表示
		debugText_->SetPos(50, 90);
		debugText_->Printf(
			"up:(%f,%f,%f)", viewProjection_.up.x,
			viewProjection_.up.y, viewProjection_.up.z);
	}
	// FoV変更処理
	{
		// 上キーで視野角が広がる
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.fovAngleY += ConvartToRadian(5);
			viewProjection_.fovAngleY = UpperLimit(viewProjection_.fovAngleY, ConvartToRadian(180));
		}
		else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.fovAngleY -= ConvartToRadian(5);
			viewProjection_.fovAngleY = LowerLimit(viewProjection_.fovAngleY, ConvartToRadian(0.01f));
		}
		// 行列の再計算
		viewProjection_.UpdateMatrix();
		// デバッグ用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf(
			"fovAngleY(Degree):%f", ConvartToDegree(viewProjection_.fovAngleY));
	}
	// クリップ距離変更処理
	{
		// 上下キーでニアクリップ距離を増減
		if (input_->PushKey(DIK_UP))viewProjection_.nearZ += 0.5f;
		else if (input_->PushKey(DIK_DOWN))viewProjection_.nearZ -= 0.5f;
		// 行列の再計算
		viewProjection_.UpdateMatrix();
		// デバッグ用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
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

	//3Dモデル描画
	for (WorldTransform& worldTransform : worldTransform_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}
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
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			{ 0,0,0 }, p2_[i], color_[i]);
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
float UpperLimit(const float& y, const float& limit) {
	if (y >= limit)return limit;
	return y;
}

float LowerLimit(const float& y, const float& limit) {
	if (y <= limit)return limit;
	return y;
}