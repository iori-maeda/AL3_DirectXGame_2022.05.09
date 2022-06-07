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

	void Rotate(const Vector3& moveState, const Vector3& rotate);


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
	WorldTransform GetWorldTransform() { return worldTransform_; }
};

