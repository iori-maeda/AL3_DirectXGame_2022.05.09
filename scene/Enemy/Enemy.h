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
	// 移動速度
	Vector3 velocity_;
	// 弾
	//std::list<std::unique_ptr<EnemyBullet>>bullets_;
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

public:
	// ゲッター
	WorldTransform GetWorldTransform() { return worldTransform_; }


public:
	// 行動フェーズ
	enum class Phaze {
		Default,		// 初期
		Approach,		// 接近
		Leave,			// 離脱
	};

	void MoveDefault(const Vector3& limit);
	void MoveApproach(const Vector3& limit);
	void MoveLeave(const Vector3& limit);
private:
	// フェーズ
	Phaze phaze_ = Phaze::Default;
};

