#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"


#include <scene/Player/Player.h>
#include <scene/Enemy/Enemy.h>
#include <3d/DebugCamera.h>
#include <scene/Skydome/Skydome.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void CheckAllCollisions();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデル
	Model* model_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	// デバッグカメラの有効判定
	bool isDebugCameraActive_ = true;

	// 自キャラ
	std::unique_ptr<Player>player_ = std::make_unique<Player>();

	// 敵キャラ
	std::unique_ptr<Enemy>enemy_ = std::make_unique<Enemy>();

	// 天球
	std::unique_ptr<Skydome>skydome = std::make_unique<Skydome>();
	// 天球用3Dモデル
	Model* modelSkydome_ = nullptr;
};
