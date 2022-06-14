#pragma once
#include <3d/Model.h>
#include <input/Input.h>
#include <2d/DebugText.h>
class Enemy
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model);

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
	//std::list<std::unique_ptr<EnemyBullet>>bullets_;
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

public:
	WorldTransform GetWorldTransform() { return worldTransform_; }
};

