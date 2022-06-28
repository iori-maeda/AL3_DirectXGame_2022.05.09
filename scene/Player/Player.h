#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include <input/Input.h>
#include <2d/DebugText.h>
#include <scene/PlayerBullet/PlayerBullet.h>
#include<memory>
#include<list>

class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model,uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 回転
	/// </summary>
	/// <param name="moveState"></param>
	/// <param name="rotate"></param>
	void Rotate(const Vector3& moveState, const Vector3& rotate);

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 入力処理するため
	Input* input_ = nullptr;
	// デバッグテキスト
	DebugText* debugText_ = nullptr;
	// 弾
	std::list<std::unique_ptr<PlayerBullet>>bullets_;
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

public:
	// ゲッター
	WorldTransform GetWorldTransform() { return worldTransform_; }
	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// セッター
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets_; }
};

