#pragma once
#include <3d/Model.h>
#include <input/Input.h>
#include <2d/DebugText.h>
#include <scene/EnemyBullet/EnemyBullet.h>

class Player;

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
	// 移動速度
	Vector3 velocity_;
	// 弾
	std::list<std::unique_ptr<EnemyBullet>>bullets_;

	// 自キャラ
	Player* player_ = nullptr;

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

public:
	// 発射間隔
	static const int kFireInterVal = 10;
private:
	// 発射タイマー
	int32_t fireTimer = 0;

public:
	// ゲッター
	WorldTransform GetWorldTransform() { return worldTransform_; }
	// ワールド座標の取得
	Vector3 GetWorldPosition();
	// 弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets_; }

	// セッター
	void SetPlayer(Player* player) { player_ = player; }

public:
	// 行動フェーズ
	enum class Phaze {
		Default,		// 初期
		Approach,		// 接近
		Leave,			// 離脱
	};

	// 接近フェーズ初期化
	void MoveApproachInitialize();

	// フェーズごとの移動関数
	void MoveDefault(const Vector3& limit);
	void MoveApproach(const Vector3& limit);
	void MoveLeave(const Vector3& limit);
private:
	// フェーズ
	Phaze phaze_ = Phaze::Default;
};

